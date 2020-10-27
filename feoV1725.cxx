/*****************************************************************************/
/**
\file feoV1725.cxx

\mainpage

\section contents Contents

Standard Midas Frontend for Optical access to the CAEN v1725 using the A3818 CONET2 driver

Meant for use with DPP-PSD firmware.


 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sched.h>
#include <sys/resource.h>
#include <string>
#include <cerrno>
#include <vector>
using  std::vector;

#include "midas.h"
#include "mfe.h"

extern "C" {
#include "CAENComm.h"
#include "ov1725drv.h"
}

#include "v1725CONET2.hxx"

// __________________________________________________________________
// --- General feov1725 parameters

#ifndef NBLINKSPERA3818
#define NBLINKSPERA3818   1   //!< Number of optical links used per A3818
#define NBLINKSPERFE      1   //!< Number of optical links controlled by each frontend
#define NB1725PERLINK     1   //!< Number of daisy-chained v1725s per optical link
#define NBV1725TOTAL      1   //!< Number of v1725 boards in total
#endif

#define  EQ_EVID   1                //!< Event ID
#define  EQ_TRGMSK 0                //!< Trigger mask

#define  FE_NAME   "feov1725I"       //!< Frontend name

#define SLEEP_TIME_BETWEEN_CONNECTS 50 // in milliseconds

#define UNUSED(x) ((void)(x)) //!< Suppress compiler warnings

// __________________________________________________________________
// --- MIDAS global variables
extern HNDLE hDB;   //!< main ODB handle
extern BOOL debug;  //!< debug printouts

/* make frontend functions callable from the C framework */

/*-- Globals -------------------------------------------------------*/

//! The frontend name (client name) as seen by other MIDAS clients
const char *frontend_name = (char*)FE_NAME;
//! The frontend file name, don't change it
const char *frontend_file_name = (char*)__FILE__;
//! frontend_loop is called periodically if this variable is TRUE
BOOL frontend_call_loop = FALSE;
//! a frontend status page is displayed with this frequency in ms
INT display_period = 000;
//! maximum event size produced by this frontend
//INT max_event_size = 100000000;
INT max_event_size = 4194304;
//! maximum event size for fragmented events (EQ_FRAGMENTED)
INT max_event_size_frag = 200 * 1024 * 1024;
//5 * 409600;//5 * 1024 * 1024;
//! buffer size to hold events
INT event_buffer_size = 10 * max_event_size;

bool runInProgress = false; //!< run is in progress
bool runOver = false; //!< run is over
bool runStopRequested = false; //!< stop run requested


// __________________________________________________________________
/*-- MIDAS Function declarations -----------------------------------------*/
INT frontend_init();
INT frontend_exit();
INT begin_of_run(INT run_number, char *error);
INT end_of_run(INT run_number, char *error);
INT pause_run(INT run_number, char *error);
INT resume_run(INT run_number, char *error);
INT frontend_loop();
extern void interrupt_routine(void);  //!< Interrupt Service Routine

INT read_trigger_event(char *pevent, INT off);
INT read_buffer_level(char *pevent, INT off);

// __________________________________________________________________
/*-- Equipment list ------------------------------------------------*/
#undef USE_INT
//! Main structure for midas equipment
EQUIPMENT equipment[] =
{
  {
    "UCN_Detector",            /* equipment name */
    {
      EQ_EVID, EQ_TRGMSK,     /* event ID, trigger mask */
      //      //Use this to make different frontends (indexes) write
      //      //to different buffers
      "SYSTEM",               /* event buffer */
//      "SYSTEM",               /* event buffer */

# ifdef USE_INT
      EQ_INTERRUPT,           /* equipment type */
# else
      EQ_POLLED | EQ_EB,      /* equipment type */
# endif //USE_INT

      LAM_SOURCE(0, 0x0),     /* event source crate 0, all stations */
      "MIDAS",                /* format */
      TRUE,                   /* enabled */
      RO_RUNNING,             /* read only when running */
      500,                    /* poll for 500ms */
      0,                      /* stop run after this event limit */
      0,                      /* number of sub events */
      0,                      /* don't log history */
      "", "", ""
    },
    read_trigger_event,       /* readout routine */
  },

  {
    "V1725_Slow",                /* equipment name */
    {
      2, 0,                   /* event ID, trigger mask */
      "SYSTEM",               /* event buffer */
      EQ_PERIODIC,   /* equipment type */
      0,                      /* event source */
      "MIDAS",                /* format */
      TRUE,                   /* enabled */
      RO_RUNNING | RO_TRANSITIONS |   /* read when running and on transitions */
      RO_ODB,                 /* and update ODB */
      1000,                  /* read every 1 sec */
      0,                      /* stop run after this event limit */
      0,                      /* number of sub events */
      1,                      /* log history */
      "", "", ""
    },
    read_buffer_level,       /* readout routine */
  },

  {""}
};


vector<v1725CONET2> ov1725; //!< objects for the v1725 modules controlled by this frontend
vector<v1725CONET2>::iterator itv1725;  //!< iterator


/********************************************************************/
/********************************************************************/
/********************************************************************/

/**
 * \brief   Sequencer callback info
 *
 * Function which gets called when record is updated
 *
 * \param   [in]  hDB main ODB handle
 * \param   [in]  hseq Handle for key where search starts in ODB, zero for root.
 * \param   [in]  info Record descriptor additional info
 */
void seq_callback(INT hDB, INT hseq, void *info){
  KEY key;

  for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725) {
    if (hseq == itv1725->GetSettingsHandle()){
      db_get_key(hDB, hseq, &key);
      itv1725->SetSettingsTouched(true);
      printf("Settings %s touched. Changes will take effect at start of next run.\n", key.name);
    }
  }
}


int wait_counter = 0;
// Check how many events we have in the ring buffer
BOOL wait_buffer_empty(int transition, BOOL first)
 {

   if(first){
     printf("\nDeferred transition.  First call of wait_buffer_empty. Stopping run\n");
     wait_counter = 0;
     return FALSE;
   }

   // Stop the deferred transition after 100 checks.  
   // If not finished, will never finish.
   wait_counter++;
   if(wait_counter > 100) return TRUE;

   DWORD vmeStat;
   bool haveEventsInBuffer = false;
   for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725){
     itv1725->ReadReg(V1725_VME_STATUS, &vmeStat);
     if( vmeStat & 0x1 ){
       haveEventsInBuffer = true;
     }     
   }	

   // Stay in deferred transition till all events are cleared
   if(haveEventsInBuffer){
     printf("Deferred transition: still have events\n");
     return FALSE;
   }

   printf("Deferred transition: cleared all events\n");
   return TRUE;
 }


/**
 * \brief   Frontend initialization
 *
 * Runs once at application startup.  We initialize the hardware and optical
 * interfaces and set the equipment status in ODB.  We also lock the frontend
 *  to once physical cpu core.
 *
 * \return  Midas status code
 */
INT frontend_init(){

  int feIndex = get_frontend_index();

  set_equipment_status(equipment[0].name, "Initializing...", "#FFFF00");

  // --- Suppress watchdog for PICe for now
  cm_set_watchdog_params(FALSE, 0);

  int nExpected = 0; //Number of v1725 boards we expect to activate
  int nActive = 0;   //Number of v1725 boards activated at the end of frontend_init
  std::vector<std::pair<int,int> > errBoards;  //v1725 boards which we couldn't connect to

  // If no index was supplied on the command-line, assume 1 frontend
  // to control all the links and boards
  if(feIndex == -1){
    nExpected = NB1725PERLINK*NBLINKSPERA3818;

    printf("<<< No index supplied! Assuming only 1 frontend only and starting all boards on all links\n");
    printf("    nExpected=%d  nPerLink=%d  nPer3818=%d\n",nExpected,NB1725PERLINK, NBLINKSPERA3818);
    for (int iLink=0; iLink < NBLINKSPERA3818; iLink++)
    {
      for (int iBoard=0; iBoard < NB1725PERLINK; iBoard++)
      {
        printf("<<< Begin of Init \n link:%i, board:%i\n", iLink, iBoard);

        // Compose unique module ID
        int moduleID = iLink*NB1725PERLINK + iBoard;

        // Create module objects
        ov1725.emplace_back(v1725CONET2(feIndex, iLink+1, iBoard, moduleID, hDB));  /// For the moment, while using second link.

	ov1725.back().SetBoardRecord(hDB,seq_callback);

 #if 1 //PAA
        // Open Optical interface
        printf("Opening optical interface Link %d, Board %d\n", iLink, iBoard);
        switch(ov1725.back().Connect()){
        case v1725CONET2::ConnectSuccess:
          nActive++;
        printf("InitializeForAcq Link %d, Board %d\n", iLink, iBoard);
	  ov1725.back().InitializeForAcq();
          break;
        case v1725CONET2::ConnectErrorCaenComm:
        case v1725CONET2::ConnectErrorTimeout:
          errBoards.push_back(std::pair<int,int>(iLink,iBoard));
          break;
        case v1725CONET2::ConnectErrorAlreadyConnected:
          //do nothing
          break;
        default:
          //Can't happen
          break;
        }

        if(!((iLink == (NBLINKSPERA3818-1)) && (iBoard == (NB1725PERLINK-1)))) {
          printf("Sleeping for %d milliseconds before next board\n", SLEEP_TIME_BETWEEN_CONNECTS);
          ss_sleep(SLEEP_TIME_BETWEEN_CONNECTS);
        }
#endif

      }
    }
  } else {  //index supplied

    nExpected = NB1725PERLINK*NBLINKSPERFE;

    if((NBV1725TOTAL % (NB1725PERLINK*NBLINKSPERFE)) != 0){
      printf("Incorrect setup: the number of boards controlled by each frontend"
          " is not a fraction of the total number of boards.");
    }

    int maxIndex = (NBV1725TOTAL/NB1725PERLINK)/NBLINKSPERFE - 1;
    if(feIndex < 0 || feIndex > maxIndex){
      printf("Front end index must be between 0 and %d\n", maxIndex);
      exit(1);
    }

    int firstLink = (feIndex % (NBLINKSPERA3818 / NBLINKSPERFE)) * NBLINKSPERFE;
    int lastLink = firstLink + NBLINKSPERFE - 1;
    for (int iLink=firstLink; iLink <= lastLink; iLink++)
    {
      for (int iBoard=0; iBoard < NB1725PERLINK; iBoard++)
      {

        printf("<<< Begin of Init \n feIndex:%i, link:%i, board:%i\n",
            feIndex, iLink, iBoard);

        // Compose unique module ID
        int moduleID = feIndex*NBLINKSPERFE*NB1725PERLINK + (iLink-firstLink)*NB1725PERLINK + iBoard;

        // Create module objects
        ov1725.push_back(v1725CONET2(feIndex, iLink, iBoard, moduleID, hDB));
        ov1725.back().verbose = 1;

        // Setup ODB record (create if necessary)
        ov1725.back().SetBoardRecord(hDB,seq_callback);
        // Set history ODB record (create if necessary)
        ov1725.back().SetHistoryRecord(hDB,seq_callback);

        // Open Optical interface
        printf("Opening optical interface Link %d, Board %d\n", iLink, iBoard);
        switch(ov1725.back().Connect()){
        case v1725CONET2::ConnectSuccess:
          nActive++;
          ov1725.back().InitializeForAcq();
	  //ov1725.back().SaveSettings();
          break;
        case v1725CONET2::ConnectErrorCaenComm:
        case v1725CONET2::ConnectErrorTimeout:
          errBoards.push_back(std::pair<int,int>(iLink,iBoard));
          break;
        case v1725CONET2::ConnectErrorAlreadyConnected:
          //do nothing
          break;
        default:
          //Can't happen
          break;
        }
      }
    }
  }

  // Setup a deferred transition to wait till the V1725 buffer is empty.
  //  cm_register_deferred_transition(TR_STOP, wait_buffer_empty);

  printf(">>> End of Init. %d active v1725. Expected %d\n\n", nActive, nExpected);
   
  if(nActive == nExpected){
    set_equipment_status(equipment[0].name, "Initialized", "#00ff00");
  }
  else{

    return FE_ERR_HW;
  }


  // Lock to one core on the processor. Need to be moved to TFeCommon
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(1, &mask);  // arbitrarily chose 1
  if( sched_setaffinity(0, sizeof(mask), &mask) < 0 ){
    printf("ERROR setting cpu affinity: %s\n", strerror(errno));
  }

  return SUCCESS;
}




/**
 * \brief   Frontend exit
 *
 * Runs at frontend shutdown.  Disconnect hardware and set equipment status in ODB
 *
 * \return  Midas status code
 */
INT frontend_exit(){

  set_equipment_status(equipment[0].name, "Exiting...", "#FFFF00");

  for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725) {
    if (itv1725->IsConnected()){
      itv1725->Disconnect();
    }
  }
  set_equipment_status(equipment[0].name, "Exited", "#00ff00");
  return SUCCESS;
}

/**
 * \brief   Begin of Run
 *
 * Called every run start transition.  Set equipment status in ODB,
 * start acquisition on the modules.
 *
 * \param   [in]  run_number Number of the run being started
 * \param   [out] error Can be used to write a message string to midas.log
 */
INT begin_of_run(INT run_number, char *error){

  set_equipment_status(equipment[0].name, "Starting run...", "#FFFF00");

  printf("<<< Begin of begin_of_run\n");

  // Reset and Start v1725s
  for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725) {
    if (! itv1725->IsConnected()) continue;   // Skip unconnected board
    // Start run then wait for trigger
    itv1725->StartRun();
  }

  runInProgress = true;

  set_equipment_status(equipment[0].name, "Started run", "#00ff00");
  printf(">>> End of begin_of_run\n\n");

  return SUCCESS;
}

/**
 * \brief   End of Run
 *
 * Called every stop run transition. Set equipment status in ODB,
 * stop acquisition on the modules.
 *
 * \param   [in]  run_number Number of the run being ended
 * \param   [out] error Can be used to write a message string to midas.log
 */
INT end_of_run(INT run_number, char *error){

  DWORD eStored;
  bool result;

  set_equipment_status(equipment[0].name, "Ending run...", "#FFFF00");

  cm_msg(MINFO,"EOR", "Beginning of end_of_run");
  printf("<<< Beginning of end_of_run \n");

  // Stop run
  for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725) {
    if (itv1725->IsConnected()) {  // Skip unconnected board
      result = itv1725->StopRun();

      if(!result)
        cm_msg(MERROR, "feov1725:EOR",
            "Could not stop the run for module %d\n", itv1725->GetModuleID());
    }
  }

  runOver = false;
  runStopRequested = false;
  runInProgress = false;
  result = ov1725[0].Poll(&eStored);
  if(eStored != 0x0) {
    cm_msg(MINFO, "feov1725:EOR", "Events left in the v1725: %d",eStored);
  }

  printf(">>> End Of end_of_run\n\n");
  set_equipment_status(equipment[0].name, "Ended run", "#00ff00");

  return SUCCESS;

}

/**
 * \brief   Pause Run
 *
 * Called every pause run transition.
 *
 * \param   [in]  run_number Number of the run being ended
 * \param   [out] error Can be used to write a message string to midas.log
 *
 * \return  Midas status code
 */
INT pause_run(INT run_number, char *error)
{
  runInProgress = false;
  return SUCCESS;
}

/**
 * \brief   Resume Run
 *
 * Called every resume run transition.
 *
 * \param   [in]  run_number Number of the run being ended
 * \param   [out] error Can be used to write a message string to midas.log
 *
 * \return  Midas status code
 */
INT resume_run(INT run_number, char *error)
{
  runInProgress = true;
  return SUCCESS;
}


DWORD sn=0;
/**
 * \brief   Frontend loop
 *
 * If frontend_call_loop is true, this routine gets called when
 * the frontend is idle or once between every event.
 *
 * \return  Midas status code
 */
INT frontend_loop()
{
  char str[128];
  static DWORD evlimit;

  if (runStopRequested && !runOver) {
    db_set_value(hDB,0,"/logger/channels/0/Settings/Event limit", &evlimit, sizeof(evlimit), 1, TID_DWORD);
    if (cm_transition(TR_STOP, 0, str, sizeof(str), BM_NO_WAIT, FALSE) != CM_SUCCESS) {
      cm_msg(MERROR, "feov1725", "cannot stop run: %s", str);
    }
    runInProgress = false;
    runOver = true;
    cm_msg(MERROR, "feov1725","feov1725 Stop requested");
  }
  return SUCCESS;
}

/*------------------------------------------------------------------*/
/********************************************************************\
  Readout routines for different events
\********************************************************************/
DWORD acqStat; //!< ACQUISITION STATUS reg, must be global because read by poll_event, accessed by read_trigger_event
// ___________________________________________________________________
/*-- Trigger event routines ----------------------------------------*/
/**
 * \brief   Polling routine for events.
 *
 * \param   [in]  source Event source (LAM/IRQ)
 * \param   [in]  count Loop count for event polling timeout
 * \param   [in]  test flag used to time the polling
 * \return  1 if event is available, 0 if done polling (no event).
 * If test equals TRUE, don't return.
 */
INT poll_event(INT source, INT count, BOOL test) {

 
    DWORD vmeStat;
    DWORD acqStat;
    register int i;


    //printf("poll_event source=%d count=%d test=%d\n",source, count, test);
    for (i = 0; i < count; i++) {
      
      //read the vme status register and check if event ready
      bool evtReady = false;
      //bool evtReady = true;
      int j = 0;
      for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725){
	usleep(20);
	j++;
	itv1725->ReadReg(V1725_VME_STATUS, &vmeStat);
	
	//if (vmeStat !=0x8) printf(" vmeStat=%d\n",vmeStat);
        if((!test) && (vmeStat != 0x8)) 
          fflush(stdout);
	
        //if( !(vmeStat & 0x1) )
	//  evtReady = false;
	if( vmeStat & 0x1 ){
	  evtReady = true;
	}

      }	
      
      //If event not ready or we're in test phase, keep looping     
      if (evtReady==true && !test)
        return 1;
    }
    return 0;

}

/**
 * \brief   Interrupt configuration (not implemented)
 *
 * Routine for interrupt configuration if equipment is set in EQ_INTERRUPT
 * mode.  Not implemented right now, returns SUCCESS.
 *
 * \param   [in]  cmd Command for interrupt events (see midas.h)
 * \param   [in]  source Equipment index number
 * \param   [in]  adr Interrupt routine (see mfe.c)
 *
 * \return  Midas status code
 */
INT interrupt_configure(INT cmd, INT source, POINTER_T adr)
{
  switch (cmd) {
  case CMD_INTERRUPT_ENABLE:
    break;
  case CMD_INTERRUPT_DISABLE:
    break;
  case CMD_INTERRUPT_ATTACH:
    break;
  case CMD_INTERRUPT_DETACH:
    break;
  }
  return SUCCESS;
}

/**
 * \brief   Event readout
 *
 * Event readout routine.  This is called by the polling or interrupt routines.
 * (see mfe.c).  For each module, read the event buffer into a midas data bank.
 * If ZLE data exists, create another bank for it.  Finally, create a statistical
 * bank for data throughput analysis.
 *
 * \param   [in]  pevent Pointer to event buffer
 * \param   [in]  off Caller info (unused here), see mfe.c
 *
 * \return  Size of the event
 */
INT read_trigger_event(char *pevent, INT off) {

  if (!runInProgress) return 0;

  sn = SERIAL_NUMBER(pevent);
  
  bk_init32(pevent);

  for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725) {
 
    if (! itv1725->IsConnected()) continue;   // Skip unconnected board

    // >>> Fill Event bank
    itv1725->FillEventBank(pevent);

  }

  //primitive progress bar
  if (sn % 1000 == 0) printf(".");

  return bk_size(pevent);
}

// do nothing here
INT read_buffer_level(char *pevent, INT off) {

  bk_init32(pevent);
  for (itv1725 = ov1725.begin(); itv1725 != ov1725.end(); ++itv1725){

    itv1725->FillBufferLevelBank(pevent);
  }
  
  return bk_size(pevent);

  return 0;
}

