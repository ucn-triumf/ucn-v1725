/*****************************************************************************/
/**
\file v1725CONET2.cxx

## Contents

This file contains the class implementation for the v1725 module driver.
 *****************************************************************************/

#include "v1725CONET2.hxx"
#include <bitset>

#define UNUSED(x) ((void)(x)) //!< Suppress compiler warnings

using namespace std;

//! Configuration string for this board. (ODB: /Equipment/[eq_name]/Settings/[board_name]/)
const char * v1725CONET2::config_str_board[] = {\
    "Acq mode = INT : 2",\
    "Software Trig Mode = INT : 0",\
    "Trig Input Mode = INT :0",\
    "Channel Mask = DWORD : 65535",\
    "Save waveforms = INT : 1", \
    "DPP TrigHoldoff = INT : 28",\
    "DPP Threshold = INT[16] :",\
    "[0] 200",\
    "[1] 200",\
    "[2] 200",\
    "[3] 200",\
    "[4] 200",\
    "[5] 200",\
    "[6] 200",\
    "[7] 200",\
    "[8] 200",\
    "[9] 200",\
    "[10] 200",\
    "[11] 200",\
    "[12] 200",\
    "[13] 200",\
    "[14] 200",\
    "[15] 200",\
    "DPP Num Sample Baseline = INT[16] :",\
    "[0] 0",\
    "[1] 0",\
    "[2] 0",\
    "[3] 0",\
    "[4] 0",\
    "[5] 0",\
    "[6] 0",\
    "[7] 0",\
    "[8] ",\
    "[9] ",\
    "[10] ",\
    "[11] ",\
    "[12] ",\
    "[13] ",\
    "[14] ",\
    "[15] ",\
    "DPP Long Gate = INT[16] :",\
    "[0] 50",\
    "[1] 50",\
    "[2] 50",\
    "[3] 50",\
    "[4] 50",\
    "[5] 50",\
    "[6] 50",\
    "[7] 50",\
    "[8] 50",\
    "[9] 50",\
    "[10] 50",\
    "[11] 50",\
    "[12] 50",\
    "[13] 50",\
    "[14] 50",\
    "[15] 50",\
    "DPP Short Gate = INT[16] :",\
    "[0] 10",\
    "[1] 10",\
    "[2] 10",\
    "[3] 10",\
    "[4] 10",\
    "[5] 10",\
    "[6] 10",\
    "[7] 10",\
    "[8] 10",\
    "[9] 10",\
    "[10] 10",\
    "[11] 10",\
    "[12] 10",\
    "[13] 10",\
    "[14] 10",\
    "[15] 10",\
    "DPP Gate Offset = INT[16] :",\
    "[0] 8",\
    "[1] 8",\
    "[2] 8",\
    "[3] 8",\
    "[4] 8",\
    "[5] 8",\
    "[6] 8",\
    "[7] 8",\
    "[8] 8",\
    "[9] 8",\
    "[10] 8",\
    "[11] 8",\
    "[12] 8",\
    "[13] 8",\
    "[14] 8",\
    "[15] 8",\
    "DPP SelfTrig= INT[16] :",\
    "[0] 1",\
    "[1] 1",\
    "[2] 1",\
    "[3] 1",\
    "[4] 1",\
    "[5] 1",\
    "[6] 1",\
    "[7] 1",\
    "[8] 1",\
    "[9] 1",\
    "[10] 1",\
    "[11] 1",\
    "[12] 1",\
    "[13] 1",\
    "[14] 1",\
    "[15] 1",\
    "DPP Trig Valid Window = INT[16] :",\
    "[0] 0",\
    "[1] 0",\
    "[2] 0",\
    "[3] 0",\
    "[4] 0",\
    "[5] 0",\
    "[6] 0",\
    "[7] 0",\
    "[8] 0",\
    "[9] 0",\
    "[10] 0",\
    "[11] 0",\
    "[12] 0",\
    "[13] 0",\
    "[14] 0",\
    "[15] 0",\
    "DPP Charge Sensitivity = INT[16] :",\
    "[0] 0",\
    "[1] 0",\
    "[2] 0",\
    "[3] 0",\
    "[4] 0",\
    "[5] 0",\
    "[6] 0",\
    "[7] 0",\
    "[8] 0",\
    "[9] 0",\
    "[10] 0",\
    "[11] 0",\
    "[12] 0",\
    "[13] 0",\
    "[14] 0",\
    "[15] 0",\
    "DC Offset = INT[16] :",\
    "[0] 32768",\
    "[1] 32768",\
    "[2] 32768",\
    "[3] 32768",\
    "[4] 32768",\
    "[5] 32768",\
    "[6] 32768",\
    "[7] 32768",\
    "[8] 32768",\
    "[9] 32768",\
    "[10] 32768",\
    "[11] 32768",\
    "[12] 32768",\
    "[13] 32768",\
    "[14] 32768",\
    "[15] 32768",\
    "Pre-Trigger Size = INT[16] :",\
    "[0] 16",\
    "[1] 16",\
    "[2] 16",\
    "[3] 16",\
    "[4] 16",\
    "[5] 16",\
    "[6] 16",\
    "[7] 16",\
    "[8] 16",\
    "[9] 16",\
    "[10] 16",\
    "[11] 16",\
    "[12] 16",\
    "[13] 16",\
    "[14] 16",\
    "[15] 16",\
    "DPP Record Length = INT[16] :",\
    "[0] 5",			\
    "[1] 5",\
    "[2] 5",\
    "[3] 5",\
    "[4] 5",\
    "[5] 5",\
    "[6] 5",\
    "[7] 5",\
    "[8] 5",\
    "[9] 5",\
    "[10] 5",\
    "[11] 5",\
    "[12] 5",\
    "[13] 5",\
    "[14] 5",\
    "[15] 5",\
    "Baseline = INT : 8192",	\
    "Force Trigger = INT : 0",	\
    NULL
};

const char v1725CONET2::history_settings[][NAME_LENGTH] = { "eStored", "busy" };

void DCOFFSETCALC(int sum1[8], int sum0[8], int moduleID, int NumEvents1[16]);

/**
 * \brief   Constructor for the module object
 *
 * Set the basic hardware parameters
 *
 * \param   [in]  feindex   Frontend index number
 * \param   [in]  link      Optical link number
 * \param   [in]  board     Board number on the optical link
 * \param   [in]  moduleID  Unique ID assigned to module
 */
v1725CONET2::v1725CONET2(int feindex, int link, int board, int moduleID, HNDLE hDB)
: _feindex(feindex), _link(link), _board(board), _moduleID(moduleID), _odb_handle(hDB)
{
  _device_handle = -1;
  _settings_handle = 0;
  verbose = 0;
  _settings_loaded=false;
  _settings_touched=false;
  _running=false;
  mZLE=false;
  mDataType = 0;
 
}
/**
 * \brief   Destructor for the module object
 *
 * Nothing to do.
 */
v1725CONET2::~v1725CONET2()
{
}

/**
 * \brief   Get short string identifying the module's index, link and board number
 *
 * \return  name string
 */
string v1725CONET2::GetName()
{
  stringstream txt;
  txt << "F" << _feindex << _link << _board;
  return txt.str();
}
/**
 * \brief   Get connected status
 *
 * \return  true if board is connected
 */
bool v1725CONET2::IsConnected()
{
  return (_device_handle >= 0);
}
/**
 * \brief   Get run status
 *
 * \return  true if run is started
 */
bool v1725CONET2::IsRunning()
{
  return _running;
}
/**
 * \brief   Connect the board through the optical link
 *
 * \return  ConnectErrorCode (see v1725CONET2.hxx)
 */
v1725CONET2::ConnectErrorCode v1725CONET2::Connect()
{
  memset(&DPPConfig.Params, 0, sizeof(DigitizerParams_t));
  memset(&DPPConfig.DPPParams, 0, sizeof(CAEN_DGTZ_DPP_PSD_Params_t));
  
  // Direct optical connection - for DPP
  DPPConfig.Params.LinkType = CAEN_DGTZ_PCI_OpticalLink;  // Link Type
  DPPConfig.Params.VMEBaseAddress = 0;                    // For direct CONET connection, 
                                                          // VMEBaseAddress must be 0
  return Connect(2, 10);                                  //reasonable default values
}

/**
 * \brief   Connect the board through the optical link
 *
 * \return  ConnectErrorCode (see v1725CONET2.hxx)
 */
v1725CONET2::ConnectErrorCode v1725CONET2::Connect(int connAttemptsMax, 
						   int secondsBeforeTimeout)
{
  if (verbose) cout << GetName() << "::Connect()\n";
  ConnectErrorCode returnCode;

  if (IsConnected()) {
    cout << "Error: trying to connect already connected board" << endl;
    returnCode = ConnectErrorAlreadyConnected;
  }


  // open the digitizer  
  CAEN_DGTZ_ErrorCode zCAEN;
  zCAEN = CAEN_DGTZ_OpenDigitizer( DPPConfig.Params.LinkType, _link, _board, 
				   DPPConfig.Params.VMEBaseAddress, 
				   &_device_handle );
  if (zCAEN) {
    _device_handle = -1;
    printf("Connect failed for link:%d board:%d\n",_link,_board);
    returnCode = ConnectErrorCaenComm;    
  } else {
    printf("Link#:%d Board#:%d Module_Handle[%d]:%d\n",
	   _link, _board, _moduleID, this->GetDeviceHandle());
    returnCode = ConnectSuccess;
  }

  // read in information about the digitizer board
  CAEN_DGTZ_BoardInfo_t           BoardInfo;
  zCAEN = CAEN_DGTZ_GetInfo(_device_handle, &BoardInfo);
  if (zCAEN) {
    printf("<v1725CONET2::Connect> Can't read board info\n");
    return ConnectErrorTimeout;
  }

  // print type of board
  printf("\n<v1725CONET2::Connect> Connected to CAEN Digitizer Model %s, recognized as board %d\n", 
	 BoardInfo.ModelName, _board);
  printf("<v1725CONET2::Connect> ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
  printf("<v1725CONET2::Connect> AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);
  
  // Check firmware revision (only DPP-PSD firmware can be used)                                 
  int MajorNumber;
  sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
  if (MajorNumber != 136 ) {
    printf("<v1725CONET2::Connect> This digitizer doesn't have a DPP-PSD firmware\n");
    return ConnectErrorCaenComm;
  }
  
  // Reset counters
  for(int i = 0; i < 16; i++){
    TriggerCounterCuts[i] = 0;
    EventCounter[i] = 0;
  }

  return returnCode;
}

/**
 * \brief   Disconnect the board through the optical link
 *
 * \return  ConnectErrorCode (see v1725CONET2.hxx)
 */
bool v1725CONET2::Disconnect()
{
  if (verbose) cout << GetName() << "::Disconnect()\n";

  // check if the board is already disconnected from frontend
  if (!IsConnected()) {
    cout << "Error: trying to disconnect already disconnected board" << endl;
    return false;
  }

  // check if the board is still taking data
  if (IsRunning()) {
    cout << "Error: trying to disconnect running board" << endl;
    return false;
  }

  if (verbose) cout << "Closing device (i,l,b) = (" << _feindex << "," 
		    << _link << "," << _board << ")" << endl;

  // close digitizer board
  CAENComm_ErrorCode sCAEN = CAENComm_ErrorCode(CAEN_DGTZ_CloseDigitizer(_device_handle));
  if(sCAEN == CAENComm_Success)
    _device_handle = -1;
  else
    return false;

  return true;
}
/**
 * \brief   Start data acquisition
 *
 * Write to Acquisition Control reg to put board in RUN mode. If ODB
 * settings have been changed, re-initialize the board with the new settings.
 * Set _running flag true.
 *
 * \return  true for success, false for fail
 */
bool v1725CONET2::StartRun()
{

  cout << GetName() << "::StartRun()\n";

  // check if board is already taking data
  if (IsRunning()) {
    cout << "Error: trying to start already started board" << endl;
    return false;
  }

  // check if board is connected to frontend
  if (!IsConnected()) {
    cout << "Error: trying to start disconnected board" << endl;
    return false;
  }

  // check if the ODB settings have been changed since the previous run
  // if so, reinitialize board with new settings
  if (_settings_touched) {
    cm_msg(MINFO, "feoV1725", "Note: settings on board %s touched. Re-initializing board.",
	   GetName().c_str());
    cout << "reinitializing" << endl;
    InitializeForAcq();
  }


  // Reset counters
  for(int i = 0; i < 16; i++){
    EventCounter[i] = 0;
    TriggerCounterCuts[i] = 0;
  }
  gettimeofday(&v1725LastTime, NULL);

  // save config settings to file


  // begin data taking
  CAEN_DGTZ_ErrorCode e = CAEN_DGTZ_SWStartAcquisition(_device_handle);
  if ( e== CAEN_DGTZ_Success) {
    printf("<v1725CONET2::StartRun> Acquisition Started for Link %d Board %d\n", _link, _board);
    _running=true;
  } else {
    printf("<v1725CONET2::StartRun> Acquisition Start FAILED for Link %d Board %d  Error %d\n", _link,  _board, e);
    return false;
  }

  return true;
}
/**
 * \brief   Stop data acquisition
 *
 * Write to Acquisition Control reg to put board in STOP mode.
 * Set _running flag false.
 *
 * \return  true for success, false for fail
 */
bool v1725CONET2::StopRun()
{
  if (verbose) cout << GetName() << "::StopRun()\n";

  // check if board is already stopped
  if (!IsRunning()) {
    cout << "Error: trying to stop already stopped board" << endl;
    return false;
  }

  // check if board is connected to frontend
  if (!IsConnected()) {
    cout << "Error: trying to stop disconnected board" << endl;
    return false;
  }

  // stop data taking
  CAEN_DGTZ_ErrorCode e = CAEN_DGTZ_SWStopAcquisition(_device_handle);
  if(e == CAEN_DGTZ_Success) {
    printf("<v1725CONET2::StopRun> Acquisition Successfully Stopped for Link %d, Board %d\n", _link, _board);
    _running = false;
  }
  else
    printf("<v1725CONET2::StopRun> Acquisition Stopped with Errors for Link %d, Board %d\n", _link, _board);
  return true;
}

/**
 * \brief   Control data acquisition
 *
 * Write to Acquisition Control reg
 *
 * \param   [in]  operation acquisition mode (see v1725.h)
 * \return  CAENComm Error Code (see CAENComm.h)
 */
CAENComm_ErrorCode v1725CONET2::AcqCtl(uint32_t operation)
{
  // read in acquisition control status into register
  uint32_t reg;
  CAEN_DGTZ_ErrorCode zCAEN;
  zCAEN = CAEN_DGTZ_ReadRegister(_device_handle, V1725_ACQUISITION_CONTROL, &reg);

  // send command to board
  switch (operation) {
  case V1725_RUN_START:               // begin data taking
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, (reg | 0x4));
    break;
  case V1725_RUN_STOP:                // stop data taking
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, (reg & ~( 0x4)));
    break;
  case V1725_REGISTER_RUN_MODE:       // set run mode
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, 0x100);
    break;
  case V1725_SIN_RUN_MODE:            // set software trigger in
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, 0x101);
    break;
  case V1725_SIN_GATE_RUN_MODE:       // set software trigger gate
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, 0x102);
    break;
  case V1725_MULTI_BOARD_SYNC_MODE:   // set sync 
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, 0x103);
    break;
  case V1725_COUNT_ACCEPTED_TRIGGER:  // omit pileup
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, (reg & ~( 0x8)));
    break;
  case V1725_COUNT_ALL_TRIGGER:       // count all events including pileup
    zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, V1725_ACQUISITION_CONTROL, (reg | 0x8));
    break;
  default:
    printf("operation %d not defined\n", operation);
    break;
  }
  return CAENComm_ErrorCode(zCAEN);
}


/**
 * \brief   Read 32-bit register
 *
 * \param   [in]  address  address of the register to read
 * \param   [out] val      value read from register
 * \return  CAENComm Error Code (see CAENComm.h)
 */
CAENComm_ErrorCode v1725CONET2::_ReadReg(DWORD address, DWORD *val)
{
  if (verbose >= 2) cout << GetName() << "::ReadReg(" << hex << address << ")" << endl;
  CAEN_DGTZ_ErrorCode zCAEN = CAEN_DGTZ_ReadRegister(_device_handle, address, val);
  if (zCAEN != CAEN_DGTZ_Success) 
    cout << "Error Reading register: " << zCAEN << endl;
  return CAENComm_ErrorCode(zCAEN);
}

/**
 * \brief   Write to 32-bit register
 *
 * \param   [in]  address  address of the register to write to
 * \param   [in]  val      value to write to the register
 * \return  CAENComm Error Code (see CAENComm.h)
 */
CAENComm_ErrorCode v1725CONET2::_WriteReg(DWORD address, DWORD val)
{
  if (verbose >= 2) cout << GetName() << "::WriteReg(" << hex << address << "," << val << ")" << endl;
  CAEN_DGTZ_ErrorCode zCAEN = CAEN_DGTZ_WriteRegister(_device_handle, address, val);
  if (zCAEN != CAEN_DGTZ_Success) 
    cout << "Error Writing register: " << zCAEN << endl;
  return CAENComm_ErrorCode(zCAEN);
}

bool v1725CONET2::ReadReg(DWORD address, DWORD *val)
{
  return (_ReadReg(address, val) == CAENComm_Success);
}

bool v1725CONET2::WriteReg(DWORD address, DWORD val)
{
  return (_WriteReg(address, val) == CAENComm_Success);
}

/**
 * \brief   Poll Event Stored register
 *
 * Check Event Stored register for any event stored
 *
 * \param   [out]  val     Number of events stored
 * \return  CAENComm Error Code (see CAENComm.h)
 */
bool v1725CONET2::Poll(DWORD *val)
{
  CAENComm_ErrorCode sCAEN = CAENComm_ErrorCode(CAEN_DGTZ_ReadRegister(_device_handle, V1725_EVENT_STORED, val));
  return (sCAEN == CAENComm_Success);
}

// function will return vector with number of triggers per channel for data packet
// std::pair has triggers before and after cuts
std::vector<std::vector<int> > GetNumberEvents(int bklen, DWORD *pdata)
{
  
  std::vector<std::vector<int> > nevents;
  for(int i = 0; i < 16; i++) nevents.push_back(std::vector<int>(2,0));

  //printf("Start decoding!!! %i\n",sizeof(long int));

  // Do decoding.  Decoding is complicated by the fact that there can be 
  // multiple events in the same bank.  So need to find and save multiple
  // event headers, trailers.
  
  // Do some sanity checking.  
  // Make sure first word has right identifier
  if( (pdata[0] & 0xf0000000) != 0xa0000000) 
    std::cerr << "First word has wrong identifier; first word = 0x" 
	      << std::hex << pdata[0] << std::dec << std::endl;
  
  int counter = 4;  
  // Loop over dual channel data
  for(int ch = 0; ch < 8; ch++){    
    if((1<<ch) & (pdata[1] & 0xff)){      
      uint32_t header0 = pdata[counter];
      counter++;
      uint32_t header1 = pdata[counter];
      counter++;
      uint32_t header2 = pdata[counter];
      counter++;
      uint32_t size = header0 & 0x7fff;

      // skip over the samples.
      int nsamples = size - 5; // calculate number of samples.
      int min_sample = 99999;
      for(int i = 0; i < nsamples; i++){
	int ii = i + counter;
	int samp1 = (pdata[ii] & 0x3fff);
	if(samp1 < min_sample){ 
	  min_sample = samp1;
	}
	int samp2 = ((pdata[ii] & 0x3fff0000)>>16);
	if(samp2 < min_sample){
	  min_sample = samp2;
	}
      }
      int ph = 14718 - min_sample;

      counter += nsamples;

      uint32_t extras = pdata[counter];
      counter++;
      uint32_t qword = pdata[counter];
      counter++;
      
      if(nsamples*2 != (0xffff & header1 )*8)
	std::cout << "V1725 Check2: " << nsamples << " " 
		  << (0xffff & header1 )*8 
		  << " whoops, mistake in decoding, sample size not as expected. " 
		  << std::endl;

      // increment the counter
      unsigned int chan = ch*2 + ((header2 & 0x80000000) >> 31);
      if(chan < 0 || chan > 16){
	printf("V1725 decode; Bad channel number %i\n",
				       chan);
      }else{
	nevents[chan][0]++;
	
	// PSD cut for Li-6
	if(chan < 9){
	  double ql = (double)((qword & 0xffff0000) >> 16);
	  double qs = (double)((qword & 0x7fff));
	  double psd = 0;
	  if(ql != 0) psd = (ql - qs)/ql;    
	  if(0)std::cout << "QL/QS/PSD " 
		    << ql << " " 
		    << qs << " " 
		    << psd << " " 
		    << std::endl;
	  if(ql > 6000.0 && psd > 0.3) nevents[chan][1]++;
	  
	}else if(chan == 12 || chan == 13){
	  if(0)std::cout << "PH check: " << ph << " " << min_sample << std::endl;
	  // Pulse height cut for He-3...
	  if(ph > 1500) nevents[chan][1]++;

	}else{
	  nevents[chan][1]++;
	}

      }      
    }
  }
  if((unsigned int)counter != (0xfffffff & pdata[0])) 
    std::cout << "V1725 Check:  " << counter << " " 
	      << (0xfffffff & pdata[0]) 
	      << "Whoops, multi event readout, decoding is wrong@! " 
	      << std::endl;
  


  return nevents;
}



//! Maximum size of data to read using BLT (32-bit) cycle
//#define MAX_BLT_READ_SIZE_BYTES 10000
#define MAX_BLT_READ_SIZE_BYTES 100000
/**
 * \brief   Read event buffer and create Midas bank
 *
 * Read the event buffer for this module using BLT (32-bit) cycles.
 * This function reads nothing if EVENT_SIZE register was zero.
 *
 * \param   [out]  data         Where to write content of event buffer
 * \param   [out]  dwords_read  Number of DWORDs read from the buffer
 * \return  CAENComm Error Code (see CAENComm.h)
 */
bool v1725CONET2::FillEventBank(char * pevent)
{
  //printf("* ");
  // check if board is connected
  if (! this->IsConnected()) {
    cout << "Error: trying to ReadEvent disconnected board" << endl;
    return false;
  }
  
  // grab data stream
  DWORD *pdata = (DWORD *)pevent;
  int ch,ret=0;
  char bankName[5];
  DWORD NTotal =0;
  
  sprintf(bankName, "W5%02d", this->_moduleID);
  bk_create(pevent, bankName, TID_DWORD, (void**)&pdata);
  DWORD *idata = pdata;
  //  *pdata++ = NTotal;

  DWORD size_remaining_dwords, to_read_dwords;//, *pdata = (DWORD *)wp;
  int dwords_read_total = 0, dwords_read = 0;
  
  bool sCAEN = ReadReg(0x814C, &size_remaining_dwords);
  bool loud = false;  

  while ((size_remaining_dwords > 0) && (sCAEN)) {
    
    //calculate amount of data to be read in this iteration
    to_read_dwords = (size_remaining_dwords > MAX_BLT_READ_SIZE_BYTES/sizeof(DWORD)) ?
      MAX_BLT_READ_SIZE_BYTES/sizeof(DWORD) : size_remaining_dwords;
    sCAEN = CAENComm_BLTRead(_device_handle, 0, (DWORD *)pdata, to_read_dwords, &dwords_read);
    if(loud)
    std::cout << sCAEN << " = BLTRead(handle=" << _device_handle
                                 << ", addr=" << 0
                                 << ", pdata=" << pdata
	      << ", dwords=" << size_remaining_dwords
                                 << ", to_read_dwords=" << to_read_dwords
                                 << ", dwords_read returned " << dwords_read << ");" << std::endl;

    for(int i = 0; i < 6; i++) if(loud)printf("0x%x\n",pdata[i]);   
    
    //increment pointers/counters
    dwords_read_total += dwords_read;
    size_remaining_dwords -= dwords_read;
    pdata += dwords_read;
  }
  
  // Calculate the number of events per channel in bank and save for rate calculation.
  std::vector<std::vector<int> > nevents = GetNumberEvents(dwords_read_total,idata);
  for(int i = 0; i < 16; i++){
    EventCounter[i] += nevents[i][0];
    TriggerCounterCuts[i] += nevents[i][1];
  }

  bk_close(pevent, pdata);


  return true;
}

/**
 * \brief   Send a software trigger to the board
 *
 * Send a software trigger to the board.  May require
 * software triggers to be enabled in register 0x810C.
 *
 * \return  CAENComm Error Code (see CAENComm.h)
 */
bool v1725CONET2::SendTrigger()
{
  if (verbose) cout << GetName() << "::SendTrigger()" << endl;
  if (!IsConnected()) {
    cout << "Error: trying to SendTrigger disconnected board" << endl;
    return false;
  }

  if (verbose) cout << "Sending Trigger (l,b) = (" << _link << "," << _board << ")" << endl;

  return (WriteReg(V1725_SW_TRIGGER, 0x1) == CAENComm_Success);
}
/**
 * \brief   Set the ODB record for this board
 *
 * Create a record for the board with settings from the configuration
 * string (v1725CONET2::config_str_board) if it doesn't exist or merge with
 * existing record. Create hotlink with callback function for when the
 * record is updated.  Get the handle to the record.
 *
 * Ex: For a frontend with index number 2 and board number 0, this
 * record will be created/merged:
 *
 * /Equipment/UCN_Detector/Settings/Module0
 *
 * \param   [in]  h        main ODB handle
 * \param   [in]  cb_func  Callback function to call when record is updated
 * \return  ODB Error Code (see midas.h)
 */
int v1725CONET2::SetBoardRecord(HNDLE h, void(*cb_func)(INT,INT,void*))
{
  char set_str[500];

  // if _feindex == -1, no index supplied; assume 1 frontend for all boards
  // else set up the number of modules with the _feindex supplies
  if(_feindex == -1)
    sprintf(set_str, "/Equipment/UCN_Detector/Settings/Module%d", _moduleID);
  else
    sprintf(set_str, "/Equipment/UCN_Detector%02d/Settings/Module%d", _feindex, _moduleID);

  if (verbose) cout << GetName() << "::SetBoardRecord(" << h << "," << set_str << ",...)" << endl;
  int status,size;

  


  //create record if doesn't exist and find key
  status = db_find_key(h, 0, set_str, &_settings_handle);
  if ( status != DB_SUCCESS ){
    status = db_create_record(h, 0, set_str, strcomb(config_str_board));
    if (verbose) cout<<"    "<<set_str<<endl;
    status = db_find_key(h, 0, set_str, &_settings_handle);
  }
  if (status != DB_SUCCESS) cm_msg(MINFO,"FE","Key %s not found", set_str);

  //hotlink
  size = sizeof(V1725_CONFIG_SETTINGS);
  printf("size: %d\n", size);
  status = db_open_record(h, _settings_handle, &config, size, MODE_READ, cb_func, NULL);

  //get actual record
  status = db_get_record(h, _settings_handle, &config, &size, 0);
  //printf("get config %d\n",status);
  if (status == DB_SUCCESS) 
    _settings_loaded = true;
  _settings_touched = true;

  return status; //== DB_SUCCESS for success
}
/**
 * \brief   Set the ODB record for history variable names
 *
 * \param   [in]  h        main ODB handle
 * \param   [in]  cb_func  Callback function to call when record is updated
 * \return  ODB Error Code (see midas.h)
 */
int v1725CONET2::SetHistoryRecord(HNDLE h, void(*cb_func)(INT,INT,void*))
{
  char settings_path[200] = "/Equipment/BUFLVL/Settings/";

  if(_feindex == -1)
    sprintf(settings_path, "/Equipment/BUFLVL/Settings/");
  else
    sprintf(settings_path, "/Equipment/BUFLVL%02d/Settings/", _feindex);

  if (verbose) cout << GetName() << "::SetHistoryRecord(" << h << "," << settings_path << ",...)" << endl;
  int status;

  HNDLE settings_key;
  status = db_find_key(h, 0, settings_path, &settings_key);

  if(status == DB_NO_KEY){
    db_create_key(h, 0, settings_path, TID_KEY);
    db_find_key(h, 0, settings_path, &settings_key);
  }

  char tmp[11];
  sprintf(tmp, "Names BL%02d", this->_moduleID);

  char names_path[100];
  strcpy(names_path, settings_path);
  strcat(names_path, tmp);

  db_create_key(h, 0, names_path, TID_STRING);
  HNDLE path_key;
  status = db_find_key(h, 0, names_path, &path_key);

  db_set_data(h, path_key, history_settings, sizeof(history_settings),
      sizeof(history_settings)/NAME_LENGTH, TID_STRING);

  if (status != DB_SUCCESS) cm_msg(MINFO,"SetHistoryRecord","Key %s not found", names_path);
  return status;
}
/**
 * \brief   Initialize the hardware for data acquisition
 *
 * \return  0 on success, -1 on error
 */
int v1725CONET2::InitializeForAcq()
{
  CAEN_DGTZ_ErrorCode ret;
  
  uint32_t reg, reg3, reg4;

  // I think this part could be combined into a single command...
  ret = CAEN_DGTZ_ReadRegister(_device_handle, 0x8000, &reg);
  if(verbose){
    if ( ret != CAEN_DGTZ_Success ) {
      std::cout<<"CAEN_DGTZ_ReadRegister 0x8000 failed with "<<ret<<std::endl;
    } else {
      //std::cout<<"0x8000 read    to be "<<std::bitset<32>(reg)<<std::endl;
    }
  }
  ret = CAEN_DGTZ_WriteRegister(_device_handle, 0x8000, (reg | (1 << 17)));
  if(verbose){
    if ( ret != CAEN_DGTZ_Success ) {
      std::cout<<"CAEN_DGTZ_WriteRegister 0x8000 failed with "<<ret<<std::endl;
    } else {
      //std::cout<<"0x8000 written to be "<<std::bitset<32>( reg|(1<<17) )<<std::endl;
    }
  }
  // check enable extra info and extra time tag:
  ret = CAEN_DGTZ_ReadRegister(_device_handle, 0x8000, &reg3);
  if (verbose){
    if ( ret != CAEN_DGTZ_Success ) {
      std::cout<<"CAEN_DGTZ_ReadRegister 0x8000 failed with "<<ret<<std::endl;
    } else {
      std::cout<<"0x8000 read    to be "<<std::bitset<32>(reg3)<<std::endl;
    }
  }
 
  if (verbose) cout << GetName() << "::InitializeForAcq()" << endl;

  // check if the ODB settings are loaded
  if (!_settings_loaded) {
    cout << "Error: cannot call InitializeForAcq() without settings loaded properly" << endl;
    return -1;    }

  // check if the board is connected to the frontend
  if (!IsConnected())     {
    cout << "Error: trying to call InitializeForAcq() to unconnected board" << endl;
    return -1;    }

  // check if the board is already taking data
  if (IsRunning())    {
    cout << "Error: trying to call InitializeForAcq() to already running board" << endl;
    return -1;    }


  cm_msg(MINFO,"feoV1725","### In InitializeForAcq, _settings_loaded: %d, _settings_touched: %d",
	 _settings_loaded, _settings_touched);

  // don't do anything if settings haven't been changed
  if (_settings_loaded && !_settings_touched) {
    return 0;
  }

  /****************************\
  *      DPP parameters        *
  \****************************/
    DPPConfig.Params.IOlev = CAEN_DGTZ_IOLevel_NIM;
  //DPPConfig.Params.IOlev = CAEN_DGTZ_IOLevel_TTL; // July 2019; TL: use TTL levels.
  
  // Direct optical connection
  DPPConfig.Params.LinkType = CAEN_DGTZ_PCI_OpticalLink;   // Link Type
  DPPConfig.Params.VMEBaseAddress = 0;                     // For direct CONET connection, VMEBaseAddress must be 0
  DPPConfig.Params.AcqMode = (CAEN_DGTZ_DPP_AcqMode_t)config.acq_mode;              // 0== CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope
                                                           // 1==CAEN_DGTZ_DPP_ACQ_MODE_List or 
                                                           // 2== CAEN_DGTZ_DPP_ACQ_MODE_Mixed; 
                                                           // Warning: List mode doesn't save baseline info
  DPPConfig.Params.SaveWaveforms = config.savewaveforms;
  DPPConfig.Params.ChannelMask = config.channel_mask;      // Channel enable mask

  for(int ch=0; ch< 16; ch++) {
    DPPConfig.DPPParams.thr[ch] = config.DPPthresh[ch];    // Trigger Threshold
    /* The following parameter is used to specifiy the number of samples for the baseline averaging:
       0 -> absolute Bl
       1 -> 4samp
       2 -> 8samp
       3 -> 16samp
       4 -> 32samp
       5 -> 64samp
       6 -> 128samp */
    DPPConfig.DPPParams.nsbl[ch]  = config.DPPnsbl[ch];   
    DPPConfig.DPPParams.lgate[ch] = config.DPPLongGate[ch];    // Long Gate Width (N*4ns)
    DPPConfig.DPPParams.sgate[ch] = config.DPPShortGate[ch];   // Short Gate Width (N*4ns)
    DPPConfig.DPPParams.pgate[ch] = config.DPPPregateWidth[ch];// Pre Gate Width (N*4ns) also known as gate offset
    /* Self Trigger Mode:
       0 -> Disabled
       1 -> Enabled
    */
    DPPConfig.DPPParams.selft[ch] = config.DPPSelfTrig[ch];
    // Trigger configuration:
    // CAEN_DGTZ_DPP_TriggerConfig_Peak       -> trigger on peak. NOTE: Only for FW <= 13X.5
    // CAEN_DGTZ_DPP_TriggerConfig_Threshold  -> trigger on threshold */
    DPPConfig.DPPParams.trgc[ch] = CAEN_DGTZ_DPP_TriggerConfig_Threshold;
    /* Trigger Validation Acquisition Window */
    DPPConfig.DPPParams.tvaw[ch] = config.DPPtvw[ch];
    /* Charge sensitivity: 0->40fc/LSB; 1->160fc/LSB; 2->640fc/LSB; 3->2,5pc/LSB */
    DPPConfig.DPPParams.csens[ch] = config.DPPChargeSen[ch];

    //DPPConfig.DPPParams.dcoffset[ch] = config.DCoffset[ch];
    
  }
  // purity not yet usable
  DPPConfig.DPPParams.blthr = config.baseline;
  DPPConfig.DPPParams.purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
  DPPConfig.DPPParams.purgap = 100;    // Purity Gap  
  DPPConfig.DPPParams.bltmo = 100;     // Baseline Timeout
  DPPConfig.DPPParams.trgho = config.DPPTrigHoldoff;      // Trigger HoldOff 
  // end setting DPP Parameters
 
  // reset digitizer
  ret = CAEN_DGTZ_Reset(_device_handle);
  ret = CAEN_DGTZ_SetDPPAcquisitionMode(_device_handle, DPPConfig.Params.AcqMode, 
					CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);
  if (ret != CAEN_DGTZ_Success) {
    printf("<v1725CONET2::InitializeForAcq> Error in CAEN_DGTZ_SetDPPAcquisitionMode %d\n",ret);
    printf("%d\n",_device_handle);
    return (int)ret;
  }

  // Set the digitizer acquisition mode (CAEN_DGTZ_SW_CONTROLLED or CAEN_DGTZ_S_IN_CONTROLLED)
  ret = CAEN_DGTZ_SetAcquisitionMode(_device_handle, CAEN_DGTZ_SW_CONTROLLED);
  if(ret) {
    printf("Error setting Acquisition Mode\n");
    return (int)ret;
  }

  // Set the I/O level (CAEN_DGTZ_IOLevel_NIM or CAEN_DGTZ_IOLevel_TTL)
  ret = CAEN_DGTZ_SetIOLevel(_device_handle, DPPConfig.Params.IOlev);
  if(ret) {
    printf("Error setting IOLevel %d\n",ret);
    return (int)ret;
  }

  // Set the digitizer's behaviour when an external trigger arrives:
  // 0==CAEN_DGTZ_TRGMODE_DISABLED: do nothing
  // 2==CAEN_DGTZ_TRGMODE_EXTOUT_ONLY: generate the Trigger Output signal
  // 1==CAEN_DGTZ_TRGMODE_ACQ_ONLY = generate acquisition trigger X
  // 3==CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT = generate both Trigger Output and acquisition trigger
  // see CAENDigitizer user manual, chapter "Trigger configuration" for details 
  ret = CAEN_DGTZ_SetExtTriggerInputMode(_device_handle, (CAEN_DGTZ_TriggerMode_t)config.trig_input_mode);
  if (ret != CAEN_DGTZ_Success) {
    printf("<v1725CONET2::InitializeForAcq> Error in CAEN_DGTZ_SetExtTriggerInputMode %d\n",ret);
    return (int)ret;
  }
  
  ret = CAEN_DGTZ_SetSWTriggerMode(_device_handle, (CAEN_DGTZ_TriggerMode_t)config.sw_trig_mode );
  if (ret != CAEN_DGTZ_Success) {
    printf("<v1725CONET2::InitializeForAcq> Error in CAEN_DGTZ_SetSWTriggerMode %d\n",ret);
    return (int)ret;
  }



  // Set the enabled channels
  ret = CAEN_DGTZ_SetChannelEnableMask(_device_handle, DPPConfig.Params.ChannelMask);  
  std::cout << " V1725 enable mask : " << DPPConfig.Params.ChannelMask << " " << _link << std::endl;
  if (ret != CAEN_DGTZ_Success) {
    printf("<v1725CONET2::InitializeForAcq> Error in CAEN_DGTZ_SetChannelEnableMask %d\n",ret);
    return (int)ret;
  }
    
  // Set the mode used to syncronize the acquisition between different boards.
  // In this example the sync is disabled 
  // CAEN_DGTZ_RUN_SYNC_Disabled - only one board; no need to syncronize
  ret = CAEN_DGTZ_SetRunSynchronizationMode(_device_handle, CAEN_DGTZ_RUN_SYNC_Disabled);
    
  // Set the DPP specific parameters for the channels in the given channelMask
  // Note that this sets all of the settings in DPPConfig.DPPParams!
  ret = CAEN_DGTZ_SetDPPParameters(_device_handle, config.channel_mask, &DPPConfig.DPPParams);
  if (ret != CAEN_DGTZ_Success) {
    printf("<v1725CONET2::InitializeForAcq> Error in CAEN_DGTZ_SetDPPParameters %d\n",ret);
    return (int)ret;
  }


  // Set DPP parameters for individual channels
  int retval=0;
  int addr;
  bool retbool;
  for(int i=0; i< 16; i++) {

    if (DPPConfig.Params.ChannelMask & (1<<i)) {

      // Set the number of samples for each waveform (you can set differently for different pairs of channels)    
      addr = V1725_RECORD_LENGTH | (i << 8);
      retbool = WriteReg(addr, config.recordLen[i]);
      
      // Set a DC offset to the input signal to adapt it to digitizer's dynamic range                              
      addr = V1725_CHANNEL_DAC | (i << 8);
      retbool = WriteReg(addr, config.DCoffset[i]);  // pre-trigger samples is register value * 4
      
      // Set the Pre-Trigger size (in samples)                                                                    
      addr = V1725_PRE_TRIGGER | (i << 8);
      retbool = WriteReg(addr, config.PreTriggerSize[i]/4);  // pre-trigger samples is register value * 4
      
      // Set the negative polarity, charge sensitivity, self-trigger configuration etc               
      addr = V1725_DPP_ALGORITHM_CONTROL1 | (i << 8);
      unsigned int value = 0x30000;
      if(!config.DPPSelfTrig[i]) value = 0x1030000;
      value += (0x7 & config.DPPChargeSen[i]);  // charge sensitivity
      retbool = WriteReg(addr,value );

    }
  } // end for

  ReadReg(0x810C, &reg);
  printf("0x810C 0x%x\n",reg);
  ReadReg(0x1064, &reg);
  printf("0x1064 0x%x\n",reg);
  ReadReg(0x1080, &reg);
  printf("0x1080 0x%x\n",reg);
  ReadReg(0x1084, &reg);
  printf("0x1084 0x%x\n",reg);
  ReadReg(0x811C, &reg);
  printf("0x811C 0x%x\n",reg);


  // read and write all 0x1n80 registers
  // set bit 7 to 1 to allow for extended time stamp
  for (int ich=0; ich< 16; ++ich){
    unsigned addrn = 0x1080 + ich*0x100;
    unsigned regn;
    ret = CAEN_DGTZ_ReadRegister(_device_handle, addrn, &regn);
    if (verbose){
      if ( ret != CAEN_DGTZ_Success ) {
	std::cout<<"CAEN_DGTZ_ReadRegister "<<std::hex<<addrn<< " failed with "<<ret<<std::endl;
      }
    }
    
    ret = CAEN_DGTZ_WriteRegister(_device_handle, addrn, ( regn | (1<<7) ) );
    if(verbose){
      if ( ret != CAEN_DGTZ_Success ) {
	std::cout<<"CAEN_DGTZ_WriteRegister "<<std::hex<<addrn<<" failed with "<<ret<<std::endl;
      }
    }
    ret = CAEN_DGTZ_ReadRegister(_device_handle, addrn, &regn);
    if (verbose){      
      if ( ret != CAEN_DGTZ_Success ) {
	std::cout<<"CAEN_DGTZ_ReadRegister "<<std::hex<<addrn<< " failed with "<<ret<<std::endl;
      }
    }
  }

  unsigned int regnn, regnn1, regnn2;
  //// set number of aggregates in the v1725 memory (see multi-event memory organization in 1725 manual)
  WriteReg(0x800C, 0x8);
  ReadReg(0x800C,&regnn1);
  //// set number of events per aggregate
  WriteReg(0x8034, 0x1);
  ReadReg(0x1034,&regnn2);
  printf("Buffer org (0x800C)=0x%x, number aggregates (0x8034)=0x%x\n",regnn1,regnn2);

  // Wait for 200ms after channing DAC offsets, before starting calibration. 
  usleep(800000);
  
  // Start the ADC calibration
  WriteReg(V1725_ADC_CALIBRATION , 1);
  // Now we check to see when the calibration has finished.
  // by checking register 0x1n88.
  DWORD temp;
  for (int i=0;i<16;i++) {
    addr = V1725_CHANNEL_STATUS | (i << 8);
    ReadReg(addr,&temp);
    if((temp & 0x4) == 0x4){
      printf("waiting for ADC calibration to finish...\n");
      int j;
      for(j =0; j < 20; i++){
	sleep(1);
	ReadReg(addr,&temp);
	if((temp & 0x4) == 0x0){
	  break;
	}
      }
      if(j < 19){
	ReadReg(addr,&temp);	
	printf("Took %i seconds to finish calibration. calibration status: %x\n",j+1,(temp & 0x8));
      }else{
	cm_msg(MINFO, "InitializeForAcq", "ADC Calibration did not finish!");
      }					
    }
  }
  
  printf("Module[...] : ADC calibration finished already\n");


  _settings_touched = false;

  // Final reads for 0x8000 and 0x1n80 registers
  if (verbose){
    ret = CAEN_DGTZ_ReadRegister(_device_handle, 0x8000, &reg3);
    if ( ret != CAEN_DGTZ_Success ) {
      std::cout<<"CAEN_DGTZ_ReadRegister 0x8000 failed with "<<ret<<std::endl;
    } else {
      std::cout<<"0x8000 read to be "<<std::bitset<32>(reg3)<<std::endl;
    }
    
    for (int ich=0; ich< 16; ++ich){
      //std::cout << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" " << ich <<" "<<std::endl;
      unsigned regn;
      unsigned addrn = 0x1080 + ich*0x100;
      ret = CAEN_DGTZ_ReadRegister(_device_handle, addrn, &regn);
      if ( ret != CAEN_DGTZ_Success ) {
	std::cout<<"CAEN_DGTZ_ReadRegister "<<std::hex<<addrn<< " failed with "<<ret<<std::endl;
      } else {
	std::cout<<std::hex<<addrn<<" read  to be "<<std::bitset<32>(regn)<<" == "<<std::hex<<regn<<std::dec<<std::endl;
      }
    }
  }

  /// Check if bits[1:0] of register 0x8100 so see what acquisition mode is used. See v1725
  /// user manual pg 25.
  if (verbose){
    ret = CAEN_DGTZ_ReadRegister(_device_handle, 0x8100, &reg4);
    if ( ret != CAEN_DGTZ_Success ) {
      std::cout<<"CAEN_DGTZ_ReadRegister 0x8000 failed with "<<ret<<std::endl;
    } else {
      //std::cout<<"0x8100 read to be "<<std::bitset<32>(reg4)<<std::endl;
    }
    ret = CAEN_DGTZ_ReadRegister(_device_handle, 0x800C, &regnn);
    if ( ret != CAEN_DGTZ_Success ) {
      std::cout<<"CAEN_DGTZ_ReadRegister 0x800C failed with "<<ret<<std::endl;
    } else {
      std::cout<<"0x800C"<<" read  to be "<<std::bitset<32>(regnn)<<" == "<<std::hex<<regnn<<std::dec<<std::endl;
    }
  }

  ret = CAEN_DGTZ_ReadRegister(_device_handle, 0x8000, &reg3);
  std::cout<<"0x8000 read    to be "<<std::bitset<32>(reg3)<<std::endl;

  ret = CAEN_DGTZ_ReadRegister(_device_handle, 0x1020, &regnn);
  printf("Record length final: %i %i\n",ret,regnn);

  return 0;
}
/**
 * \brief   Get data type and ZLE configuration
 *
 * Takes the channel configuration (0x8000) as parameter and checks
 * against the fields for data type (pack 2 or pack 2.5) and for ZLE
 * (Zero-length encoding).  Puts the results in fields mDataType and
 * mZLE.
 *
 * \param   [in]  aChannelConfig  Channel configuration (32-bit)
 */
std::string v1725CONET2::GetChannelConfig(DWORD aChannelConfig){
	
  // Set Device, data type and packing for QT calculation later
  int dataType = ((aChannelConfig >> 11) & 0x1);
  if(((aChannelConfig >> 16) & 0xF) == 0) {
    if(dataType == 1) {
      // 2.5 pack, full data
      mDataType = 1;
      mZLE=0;
      return std::string("Raw Data 2.5 Packing");
    } else {
      // 2 pack, full data
      mZLE=0;
      mDataType = 0;
      return std::string("Raw Data");
    }
  } else if(((aChannelConfig >> 16) & 0xF) == 2) {
    if(dataType == 1) {
      // 2.5 pack, ZLE data
      mDataType = 3;
      mZLE=1;
      return std::string("ZLE Data 2.5 Packing");
    } else {
      // 2 pack, ZLE data
      mDataType = 2;
      mZLE=1;
      return std::string("ZLE Data");
    } 
  } else
    return std::string("V1725 Data format Unrecognised");
}

/**
 * \brief   Get ZLE setting
 *
 * Get the current ZLE setting from the channel configuration.
 *
 * \return  true if data is ZLE
 */
BOOL v1725CONET2::IsZLEData(){
  return mZLE;
}


void v1725CONET2::PrintSettings(){

  CAEN_DGTZ_ErrorCode retval;

  printf("v1725CONET2::PrintSettings vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
  CAEN_DGTZ_AcqMode_t amode;
  retval = CAEN_DGTZ_GetAcquisitionMode(_device_handle,&amode);
  if (retval != CAEN_DGTZ_Success) 
    printf("<v1725CONET2::Print> Error in GetAcquisitionMode %d\n",retval);
  printf("acq_mode           Set = %08d Read = %08d\n",config.acq_mode,amode);     
  uint32_t achanmask;
  retval = CAEN_DGTZ_GetChannelEnableMask(_device_handle, &achanmask);  
  if (retval != CAEN_DGTZ_Success) 
    printf("<v1725CONET2::Print> Error in GetChannelEnableMask %d\n",retval);
  printf("channel_mask       Set = %08d Read = %08d\n",config.channel_mask, achanmask);
  CAEN_DGTZ_TriggerMode_t aswtrigmode;
  retval = CAEN_DGTZ_GetSWTriggerMode(_device_handle, &aswtrigmode );
  if (retval != CAEN_DGTZ_Success) 
    printf("<v1725CONET2::Print> Error in GetSWTriggerMode %d\n",retval);
  printf("sw_trig_mode       Set = %08d Read = %08d\n",config.sw_trig_mode, aswtrigmode);
  CAEN_DGTZ_TriggerMode_t atriginputmode;
  retval = CAEN_DGTZ_GetExtTriggerInputMode(_device_handle, &atriginputmode);
  if (retval != CAEN_DGTZ_Success) 
    printf("<v1725CONET2::Print> Error in GetExtTriggerInputMode %d\n",retval);
  printf("trig_input_mode    Set = %08d Read = %08d\n",config.trig_input_mode, aswtrigmode);



  // No way to read back these settings:  Just print what I think we set
  //  ret = CAEN_DGTZ_SetDPPParameters(_device_handle, config.channel_mask, &DPPConfig.DPPParams);

  printf("DPPTrigHoldoff     Set = %08d\n",config.DPPTrigHoldoff);
  printf("Baseline threshold Set = %08d\n",config.baseline);
  for (int i=0; i< 16; i++){
    printf("Channel %d Settings:\n",i);
    printf("  threshold          = %08d\n", config.DPPthresh[i]);
    printf("  nsbl               = %08d\n", config.DPPnsbl[i]);
    printf("  long gate          = %08d\n", config.DPPLongGate[i]);
    printf("  short gate         = %08d\n", config.DPPShortGate[i]);
    printf("  pre gate           = %08d\n", config.DPPPregateWidth[i]);
    printf("  self trig          = %08d\n", config.DPPSelfTrig[i]);
    printf("  trig valid acq win = %08d\n", config.DPPtvw[i]);
    printf("  charge sens        = %08d\n", config.DPPChargeSen[i]);
    uint32_t adcoffset;
    retval = CAEN_DGTZ_GetChannelDCOffset(_device_handle, i, &adcoffset);
    if (retval != CAEN_DGTZ_Success) {
      printf("<v1725CONET2::Print> Error in GetChannelDCOffset %d\n",retval);
    }
    printf("  dcoffset       Set = %08d Read = %08d\n",config.DCoffset[i], adcoffset);
    uint32_t pretrigsize;
    retval = CAEN_DGTZ_GetDPPPreTriggerSize(_device_handle, i, &pretrigsize); 
    printf("  pre trig size  Set = %08d Read = %08d\n",config.PreTriggerSize[i], pretrigsize);

  }

  printf("v1725CONET2::PrintSettings ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  return;

}

void v1725CONET2::SaveSettings(){

  string output = "";
  char *ptr;

  CAEN_DGTZ_ErrorCode retval;

  output += "v1725CONET2::SaveSettings vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n";
  CAEN_DGTZ_AcqMode_t amode;
  retval = CAEN_DGTZ_GetAcquisitionMode(_device_handle,&amode);
  if (retval != CAEN_DGTZ_Success) {
    sprintf(ptr,"<v1725CONET2::Print> Error in GetAcquisitionMode %d\n",retval);
    output += std::string(ptr);
  }
  sprintf(ptr,"acq_mode           Set = %08d Read = %08d\n",config.acq_mode,amode );  
  output += std::string(ptr);
 
  uint32_t achanmask;
  retval = CAEN_DGTZ_GetChannelEnableMask(_device_handle, &achanmask);  
  if (retval != CAEN_DGTZ_Success) {
    sprintf(ptr,"<v1725CONET2::Print> Error in GetChannelEnableMask %d\n",retval);
    output += std::string(ptr);
  }
  sprintf(ptr,"channel_mask       Set = %08d Read = %08d\n",config.channel_mask, achanmask); 
  output += std::string(ptr);
 
  CAEN_DGTZ_TriggerMode_t aswtrigmode;
  retval = CAEN_DGTZ_GetSWTriggerMode(_device_handle, &aswtrigmode );
  if (retval != CAEN_DGTZ_Success) { 
    sprintf(ptr,"<v1725CONET2::Print> Error in GetSWTriggerMode %d\n",retval);
    output += std::string(ptr); 
  }
  sprintf(ptr,"sw_trig_mode       Set = %08d Read = %08d\n",config.sw_trig_mode, aswtrigmode);
  output += std::string(ptr);

  output += std::string(ptr);
  CAEN_DGTZ_TriggerMode_t atriginputmode;
  retval = CAEN_DGTZ_GetExtTriggerInputMode(_device_handle, &atriginputmode);
  if (retval != CAEN_DGTZ_Success) { 
    sprintf(ptr,"<v1725CONET2::Print> Error in GetExtTriggerInputMode %d\n",retval);
    output += std::string(ptr);
  }  
  sprintf(ptr,"trig_input_mode    Set = %08d Read = %08d\n",config.trig_input_mode, aswtrigmode);
  output += std::string(ptr);

  output += std::string(ptr);

  // No way to read back these settings:  Just print what I think we set
  //  ret = CAEN_DGTZ_SetDPPParameters(_device_handle, config.channel_mask, &DPPConfig.DPPParams);

  sprintf(ptr,"DPPTrigHoldoff     Set = %08d\n",config.DPPTrigHoldoff);
  output += std::string(ptr);
  sprintf(ptr,"Baseline threshold Set = %08d\n",config.baseline);
  output += std::string(ptr);
  for (int i=0; i< 16; i++){
    sprintf(ptr,"Channel %d Settings:\n",i);
    output += std::string(ptr);
    sprintf(ptr,"  threshold          = %08d\n", config.DPPthresh[i]);
    output += std::string(ptr);
    sprintf(ptr,"  nsbl               = %08d\n", config.DPPnsbl[i]);
    output += std::string(ptr);
    sprintf(ptr,"  long gate          = %08d\n", config.DPPLongGate[i]);
    output += std::string(ptr);
    sprintf(ptr,"  short gate         = %08d\n", config.DPPShortGate[i]);
    output += std::string(ptr);
    sprintf(ptr,"  pre gate           = %08d\n", config.DPPPregateWidth[i]);
    output += std::string(ptr);
    sprintf(ptr,"  self trig          = %08d\n", config.DPPSelfTrig[i]);
    output += std::string(ptr);
    sprintf(ptr,"  trig valid acq win = %08d\n", config.DPPtvw[i]);
    output += std::string(ptr);
    sprintf(ptr,"  charge sens        = %08d\n", config.DPPChargeSen[i]);
    output += std::string(ptr);

    uint32_t adcoffset;
    retval = CAEN_DGTZ_GetChannelDCOffset(_device_handle, i, &adcoffset);
    if (retval != CAEN_DGTZ_Success) {
      sprintf(ptr,"<v1725CONET2::Print> Error in GetChannelDCOffset %d\n",retval);
      output += std::string(ptr);
    }
    sprintf(ptr,"  dcoffset       Set = %08d Read = %08d\n",config.DCoffset[i], adcoffset);
    output += std::string(ptr);

    uint32_t pretrigsize;
    retval = CAEN_DGTZ_GetDPPPreTriggerSize(_device_handle, i, &pretrigsize); 
    sprintf(ptr,"  pre trig size  Set = %08d Read = %08d\n",config.PreTriggerSize[i], pretrigsize);
    output += std::string(ptr);
  }

  sprintf(ptr,"v1725CONET2::PrintSettings ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  output += std::string(ptr);

  printf("%s",output.c_str());
  
  return;

}


bool v1725CONET2::FillBufferLevelBank(char * pevent)
{
  if (! this->IsConnected()) {
    cm_msg(MERROR,"FillBufferLevelBank","Board %d disconnected", this->GetModuleID());
    return false;
  }

  float *pdata, *pdata2, *pdata3;
  DWORD eStored, almostFull, nagg,nepa,status;
  char statBankName[5];

  snprintf(statBankName, sizeof(statBankName), "BL5%01d", this->GetModuleID());
  bk_create(pevent, statBankName, TID_FLOAT, (void **)&pdata);

  //Get v1725 buffer level
  CAEN_DGTZ_ReadRegister(_device_handle,V1725_EVENT_STORED, &eStored);
  CAEN_DGTZ_ReadRegister(_device_handle,V1725_ALMOST_FULL_LEVEL, &almostFull);
  CAEN_DGTZ_ReadRegister(_device_handle,0x800C, &nagg);
  CAEN_DGTZ_ReadRegister(_device_handle,0x8034, &nepa);
  CAEN_DGTZ_ReadRegister(_device_handle,0x8104, &status);



  // save if there are events ready
  *pdata++ = (status & 0x4) >> 2;
  // save if any buffers are full
  *pdata++ = (status & 0x8) >> 3;

  if(verbose)  printf("For board=%i estored,almostfull,busy,n_aggregates= %i, %i, %i  %x %x %x %x\n",_link,eStored,almostFull, nagg,V1725_EVENT_STORED, V1725_ALMOST_FULL_LEVEL, nepa, status);

  bk_close(pevent, pdata);

  // Make second bank with the rates for each channel (before and after cuts).
  snprintf(statBankName, sizeof(statBankName), "VT5%01d", this->GetModuleID());
  bk_create(pevent, statBankName, TID_FLOAT, (void **)&pdata2);

  struct timeval nowTime;  
  gettimeofday(&nowTime, NULL);
  
  double dtime = nowTime.tv_sec - v1725LastTime.tv_sec + (nowTime.tv_usec - v1725LastTime.tv_usec)/1000000.0;
  if(verbose) printf("Rates: ");
  float total_rate = 0;
  for(int i = 0; i < 16; i++){
    double rate = 0;
    if (dtime !=0) rate = (float)EventCounter[i]/(dtime);    
    *pdata2++ = rate;

    if(i < 9)
      total_rate += rate;
    if(verbose) printf(" %f",rate);
    EventCounter[i] = 0;
  }
  *pdata2++ = total_rate; // save the total rate for the first 9 channels as well.
  if(verbose) printf(" %f \n",total_rate);

  bk_close(pevent, pdata2);
  
  snprintf(statBankName, sizeof(statBankName), "CT5%01d", this->GetModuleID());
  bk_create(pevent, statBankName, TID_FLOAT, (void **)&pdata3);

  if(verbose) printf("Rates (after cuts): ");
  float total_rate_cut = 0;
  for(int i = 0; i < 16; i++){
    double rate = 0;
    if (dtime !=0) rate = (float)TriggerCounterCuts[i]/(dtime);   
    *pdata3++ = rate;

    if(i < 9)
      total_rate_cut += rate;
    if(verbose) printf(" %f",rate);
    TriggerCounterCuts[i] = 0;
  }
  *pdata3++ = total_rate_cut; // save the total rate (after cuts) for the first 9 channels as well.
  if(verbose) printf(" %f \n",total_rate_cut);
  gettimeofday(&v1725LastTime, NULL);

  bk_close(pevent, pdata3);




  // Make third bank with ADC temmperatures.
  DWORD *pdata4;
  DWORD temp;
  int addr;
  char bankName[5];
  sprintf(bankName,"TM5%01d", GetModuleID());
  bk_create(pevent, bankName, TID_DWORD, (void **)&pdata4);
  for (int i=0;i<16;i++) {
    addr = V1725_CHANNEL_TEMPERATURE | (i << 8);
    ReadReg(addr, &temp);
    *pdata4++ =  temp;
    
  }
  bk_close(pevent,pdata4);

  // Force a trigger on each channel, if so configured.
  for(int i = 0; i < 16; i++){
    //usleep(10);
    int thisbit = 1 << i;
    if(config.forcetrigger & thisbit){    
      int addr = V1725_FORCE_TRIGGER | (i << 8);
       WriteReg(addr,1);
    }
  }

  return bk_size(pevent);

}

