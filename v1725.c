/*********************************************************************

  Name:         v1725.c
  
  $Id$
*********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "v1725drv.h"
#include "mvmestd.h"

// Buffer organization map for number of samples
uint32_t V1725_NSAMPLES_MODE[11] = { (1<<20), (1<<19), (1<<18), (1<<17), (1<<16), (1<<15)
			       ,(1<<14), (1<<13), (1<<12), (1<<11), (1<<10)};

/*****************************************************************/
/*
Read V1725 register value
*/
static uint32_t regRead(MVME_INTERFACE *mvme, uint32_t base, int offset)
{
  mvme_set_am(mvme, MVME_AM_A32);
  mvme_set_dmode(mvme, MVME_DMODE_D32);
  return mvme_read_value(mvme, base + offset);
}

/*****************************************************************/
/*
Write V1725 register value
*/
static void regWrite(MVME_INTERFACE *mvme, uint32_t base, int offset, uint32_t value)
{
  mvme_set_am(mvme, MVME_AM_A32);
  mvme_set_dmode(mvme, MVME_DMODE_D32);
  mvme_write_value(mvme, base + offset, value);
}

/*****************************************************************/
uint32_t v1725_RegisterRead(MVME_INTERFACE *mvme, uint32_t base, int offset)
{
  return regRead(mvme, base, offset);
}

/*****************************************************************/
void v1725_RegisterWrite(MVME_INTERFACE *mvme, uint32_t base, int offset, uint32_t value)
{
  regWrite(mvme, base, offset, value);
}

/*****************************************************************/
void v1725_Reset(MVME_INTERFACE *mvme, uint32_t base)
{
  regWrite(mvme, base, V1725_SW_RESET, 0);
}

/*****************************************************************/
void v1725_TrgCtl(MVME_INTERFACE *mvme, uint32_t base, uint32_t reg, uint32_t mask)
{
  regWrite(mvme, base, reg, mask);
}

/*****************************************************************/
void v1725_ChannelCtl(MVME_INTERFACE *mvme, uint32_t base, uint32_t reg, uint32_t mask)
{
  regWrite(mvme, base, reg, mask);
}

/*****************************************************************/
void v1725_ChannelSet(MVME_INTERFACE *mvme, uint32_t base, uint32_t channel, uint32_t what, uint32_t that)
{
  uint32_t reg, mask;

  if (what == V1725_CHANNEL_THRESHOLD)   mask = 0x0FFF;
  if (what == V1725_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
  if (what == V1725_CHANNEL_DAC)         mask = 0xFFFF;
  if (what == V1725_ZS_THRESHOLD)         mask = 0xFFFFFFFF;
  if (what == V1725_ZS_NSAMP)         mask = 0xFFFFFFFF;
  reg = what | (channel << 8);
  printf("base:0x%x reg:0x%x, this:%x\n", base, reg, (that & mask));
  regWrite(mvme, base, reg, (that & mask));
}

/*****************************************************************/
uint32_t v1725_ChannelGet(MVME_INTERFACE *mvme, uint32_t base, uint32_t channel, uint32_t what)
{
  uint32_t reg;

  //  if (what == V1725_CHANNEL_THRESHOLD)   mask = 0x0FFF;
  //  if (what == V1725_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
  //  if (what == V1725_CHANNEL_DAC)         mask = 0xFFFF;
  reg = what | (channel << 8);
  return regRead(mvme, base, reg);
}

/*****************************************************************/
void v1725_ChannelThresholdSet(MVME_INTERFACE *mvme, uint32_t base, uint32_t channel, uint32_t threshold)
{
  uint32_t reg;
  reg = V1725_CHANNEL_THRESHOLD | (channel << 8);
  printf("base:0x%x reg:0x%x, threshold:%x\n", base, reg, threshold);
  regWrite(mvme, base, reg, (threshold & 0xFFF));
}

/*****************************************************************/
void v1725_ChannelOUThresholdSet(MVME_INTERFACE *mvme, uint32_t base, uint32_t channel, uint32_t threshold)
{
  uint32_t reg;
  reg = V1725_CHANNEL_OUTHRESHOLD | (channel << 8);
  printf("base:0x%x reg:0x%x, outhreshold:%x\n", base, reg, threshold);
  regWrite(mvme, base, reg, (threshold & 0xFFF));
}

/*****************************************************************/
void v1725_ChannelDACSet(MVME_INTERFACE *mvme, uint32_t base, uint32_t channel, uint32_t dac)
{
  uint32_t reg;

  reg = V1725_CHANNEL_DAC | (channel << 8);
  printf("base:0x%x reg:0x%x, DAC:%x\n", base, reg, dac);
  regWrite(mvme, base, reg, (dac & 0xFFFF));
}

/*****************************************************************/
int v1725_ChannelDACGet(MVME_INTERFACE *mvme, uint32_t base, uint32_t channel, uint32_t *dac)
{
  uint32_t reg;
  int   status;

  reg = V1725_CHANNEL_DAC | (channel << 8);
  *dac = regRead(mvme, base, reg);
  reg = V1725_CHANNEL_STATUS | (channel << 8);
  status = regRead(mvme, base, reg);
  return status;
}

/*****************************************************************/
void v1725_Align64Set(MVME_INTERFACE *mvme, uint32_t base)
{
  regWrite(mvme, base, V1725_VME_CONTROL, V1725_ALIGN64);
}

/*****************************************************************/
void v1725_AcqCtl(MVME_INTERFACE *mvme, uint32_t base, uint32_t operation)
{
  uint32_t reg;
  
  reg = regRead(mvme, base, V1725_ACQUISITION_CONTROL);  
  switch (operation) {
  case V1725_RUN_START:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg | 0x4));
    break;
  case V1725_RUN_STOP:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg & ~(0x4)));
    break;
  case V1725_REGISTER_RUN_MODE:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg & ~(0x3)));
    break;
  case V1725_SIN_RUN_MODE:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg | 0x01));
    break;
  case V1725_SIN_GATE_RUN_MODE:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg | 0x02));
    break;
  case V1725_MULTI_BOARD_SYNC_MODE:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg | 0x03));
    break;
  case V1725_COUNT_ACCEPTED_TRIGGER:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg | 0x08));
    break;
  case V1725_COUNT_ALL_TRIGGER:
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL, (reg & ~(0x08)));
    break;
  default:
    break;
  }
}

/*****************************************************************/
void v1725_ChannelConfig(MVME_INTERFACE *mvme, uint32_t base, uint32_t operation)
{
  uint32_t reg;
  
//  regWrite(mvme, base, V1725_CHANNEL_CONFIG, 0x10);
  reg = regRead(mvme, base, V1725_CHANNEL_CONFIG);  
  printf("Channel_config1: 0x%x\n", regRead(mvme, base, V1725_CHANNEL_CONFIG));  
  switch (operation) {
  case V1725_TRIGGER_UNDERTH:
    regWrite(mvme, base, V1725_CHANNEL_CONFIG, (reg | 0x40));
    break;
  case V1725_TRIGGER_OVERTH:
    regWrite(mvme, base, V1725_CHANNEL_CONFIG, (reg & ~(0x40)));
    break;
  default:
    break;
  }
  printf("Channel_config2: 0x%x\n", regRead(mvme, base, V1725_CHANNEL_CONFIG));  
}

/*****************************************************************/
void v1725_info(MVME_INTERFACE *mvme, uint32_t base, int *nchannels, uint32_t *n32word)
{
  int i, chanmask;

  // Evaluate the event size
  // Number of samples per channels
  *n32word = V1725_NSAMPLES_MODE[regRead(mvme, base, V1725_BUFFER_ORGANIZATION)];

  // times the number of active channels
  chanmask = 0xff & regRead(mvme, base, V1725_CHANNEL_EN_MASK); 
  *nchannels = 0;
  for (i=0;i<16;i++) {
    if (chanmask & (1<<i))
      *nchannels += 1;
  }

  *n32word *= *nchannels;
  *n32word /= 2;   // 2 samples per 32bit word
  *n32word += 4;   // Headers
}

/*****************************************************************/
uint32_t v1725_BufferOccupancy(MVME_INTERFACE *mvme, uint32_t base, uint32_t channel)
{
  uint32_t reg;
  reg = V1725_BUFFER_OCCUPANCY + (channel<<16);
  return regRead(mvme, base, reg);
}


/*****************************************************************/
uint32_t v1725_BufferFree(MVME_INTERFACE *mvme, uint32_t base, int nbuffer)
{
  int mode;

  mode = regRead(mvme, base, V1725_BUFFER_ORGANIZATION);
  if (nbuffer <= (1<< mode) ) {
    regWrite(mvme, base, V1725_BUFFER_FREE, nbuffer);
    return mode;
  } else
    return mode;
}

/*****************************************************************/
uint32_t v1725_BufferFreeRead(MVME_INTERFACE *mvme, uint32_t base)
{
  return regRead(mvme, base, V1725_BUFFER_FREE);
}

/*****************************************************************/
uint32_t v1725_DataRead(MVME_INTERFACE *mvme, uint32_t base, uint32_t *pdata, uint32_t n32w)
{
  uint32_t i;

  for (i=0;i<n32w;i++) {
    *pdata = regRead(mvme, base, V1725_EVENT_READOUT_BUFFER);
    //    printf ("pdata[%i]:%x\n", i, *pdata); 
//    if (*pdata != 0xffffffff)
    pdata++;
    //    else
      //      break;
  }
  return i;
}

/*-PAA- Contains the KO modifications for reading the V1740/42 
        Presently the block mode seem to fail on the last bytes, work around
        is to read the last bytes through normal PIO. Caen has been
        contacted about this problem.
 */
void v1725_DataBlockRead(MVME_INTERFACE* mvme, uint32_t base, uint32_t* pbuf32, int nwords32)
{
  int i, to_read32, status;
  uint32_t w;
  printf("--------------------- DataBlockRead() nwords32:%d\n", nwords32);
  if (nwords32 < 50) { // PIO
    for (i=0; i<nwords32; i++) {
      w = regRead(mvme, base, 0);
      //printf("word %d: 0x%08x\n", i, w);                                                    
      *pbuf32++ = w;
    }
  } else {
    mvme_set_dmode(mvme, MVME_DMODE_D32);
    //    mvme_set_blt(mvme, MVME_BLT_BLT32);                                                     
    //mvme_set_blt(mvme, MVME_BLT_MBLT64);                                                    
    //mvme_set_blt(mvme, MVME_BLT_2EVME);                                                     
    mvme_set_blt(mvme, MVME_BLT_2ESST);

    while (nwords32>0) {
      to_read32 = nwords32;
      to_read32 &= ~0x3;
      if (to_read32*4 >= 0xFF0)
        to_read32 = 0xFF0/4;
      else
        to_read32 = nwords32 - 8;
      to_read32 &= ~0x3;
      if (to_read32 <= 0)
        break;
      printf("going to read: read %d, total %d\n", to_read32*4, nwords32*4);                
      status=mvme_read(mvme, pbuf32, base, to_read32*4);
      printf("read %d, status %d, total %d\n", to_read32*4, status, nwords32*4);            
      nwords32 -= to_read32;
      pbuf32 += to_read32;
    }

    while (nwords32) {
      *pbuf32 = regRead(mvme, base, 0);
      pbuf32++;
      nwords32--;
    }
  }
}

#if 0
/********************************************************************/
/** v1725_DataBlockRead
Read N entries (32bit) 
@param mvme vme structure
@param base  base address
@param pdest Destination pointer
@return nentry
*/
uint32_t v1725_DataBlockRead(MVME_INTERFACE *mvme, uint32_t base, uint32_t *pdest, uint32_t *nentry)
{
  int status;

  mvme_set_am(  mvme, MVME_AM_A32);
  mvme_set_dmode(  mvme, MVME_DMODE_D32);
  mvme_set_blt(  mvme, MVME_BLT_MBLT64);
  //  mvme_set_blt(  mvme, MVME_BLT_BLT32);
  //mvme_set_blt(  mvme, 0);

  // Transfer in MBLT64 (8bytes), nentry is in 32bits(VF48)
  // *nentry * 8 / 2
  status = mvme_read(mvme, pdest, base+V1725_EVENT_READOUT_BUFFER, *nentry<<2);
  if (status != MVME_SUCCESS)
    return 0;

  return (*nentry);
}
#endif

/*****************************************************************/
void  v1725_Status(MVME_INTERFACE *mvme, uint32_t base)
{
  printf("================================================\n");
  printf("V1725 at A32 0x%x\n", (int)base);
  printf("Board ID             : 0x%x\n", regRead(mvme, base, V1725_BOARD_ID));
  printf("Board Info           : 0x%x\n", regRead(mvme, base, V1725_BOARD_INFO));
  printf("Acquisition status   : 0x%8.8x\n", regRead(mvme, base, V1725_ACQUISITION_STATUS));
  printf("================================================\n");
}

/*****************************************************************/
/**
Sets all the necessary paramters for a given configuration.
The configuration is provided by the mode argument.
Add your own configuration in the case statement. Let me know
your setting if you want to include it in the distribution.
- <b>Mode 1</b> : 

@param *mvme VME structure
@param  base Module base address
@param mode  Configuration mode number
@return 0: OK. -1: Bad
*/
int  v1725_Setup(MVME_INTERFACE *mvme, uint32_t base, int mode)
{
  switch (mode) {
  case 0x0:
    printf("--------------------------------------------\n");
    printf("Setup Skip\n");
    printf("--------------------------------------------\n");
  case 0x1:
    printf("--------------------------------------------\n");
    printf("Trigger from FP, 8ch, 1Ks, postTrigger 800\n");
    printf("--------------------------------------------\n");
    regWrite(mvme, base, V1725_BUFFER_ORGANIZATION,  0x0A);    // 1K buffer
    regWrite(mvme, base, V1725_TRIG_SRCE_EN_MASK,    0x4000);  // External Trigger
    regWrite(mvme, base, V1725_CHANNEL_EN_MASK,      0xFF);    // 8ch enable
    regWrite(mvme, base, V1725_POST_TRIGGER_SETTING, 800);     // PreTrigger (1K-800)
    regWrite(mvme, base, V1725_ACQUISITION_CONTROL,   0x00);   // Reset Acq Control
    printf("\n");
    break;
  case 0x2:
    printf("--------------------------------------------\n");
    printf("Trigger from LEMO\n");
    printf("--------------------------------------------\n");
    regWrite(mvme, base, V1725_BUFFER_ORGANIZATION, 1);
    printf("\n");
    break;
  default:
    printf("Unknown setup mode\n");
    return -1;
  }
  v1725_Status(mvme, base);
  return 0;
}

/*****************************************************************/
/*-PAA- For test purpose only */
#ifdef MAIN_ENABLE
int main (int argc, char* argv[]) {

  uint32_t V1725_BASE  = 0x32100000;

  MVME_INTERFACE *myvme;

  uint32_t data[100000], n32word, n32read;
  int status, channel, i, j, nchannels=0, chanmask;

  if (argc>1) {
    sscanf(argv[1],"%x", &V1725_BASE);
  }

  // Test under vmic
  status = mvme_open(&myvme, 0);

  v1725_Setup(myvme, V1725_BASE, 1);
  // Run control by register
  v1725_AcqCtl(myvme, V1725_BASE, V1725_REGISTER_RUN_MODE);
  // Soft or External trigger
  v1725_TrgCtl(myvme, V1725_BASE, V1725_TRIG_SRCE_EN_MASK     , V1725_SOFT_TRIGGER|V1725_EXTERNAL_TRIGGER);
  // Soft and External trigger output
  v1725_TrgCtl(myvme, V1725_BASE, V1725_FP_TRIGGER_OUT_EN_MASK, V1725_SOFT_TRIGGER|V1725_EXTERNAL_TRIGGER);
  // Enabled channels
  v1725_ChannelCtl(myvme, V1725_BASE, V1725_CHANNEL_EN_MASK, 0x3);
  //  sleep(1);

  channel = 0;
  // Start run then wait for trigger
  v1725_AcqCtl(myvme, V1725_BASE, V1725_RUN_START);
  sleep(1);

  //  regWrite(myvme, V1725_BASE, V1725_SW_TRIGGER, 1);

  // Evaluate the event size
  // Number of samples per channels
  n32word  =  1<<regRead(myvme, V1725_BASE, V1725_BUFFER_ORGANIZATION);
  // times the number of active channels
  chanmask = 0xff & regRead(myvme, V1725_BASE, V1725_CHANNEL_EN_MASK); 
  for (i=0;i<16;i++) 
    if (chanmask & (1<<i))
      nchannels++;
  printf("chanmask : %x , nchannels:  %d\n", chanmask, nchannels);
  n32word *= nchannels;
  n32word /= 2;   // 2 samples per 32bit word
  n32word += 4;   // Headers
  printf("n32word:%d\n", n32word);

  printf("Occupancy for channel %d = %d\n", channel, v1725_BufferOccupancy(myvme, V1725_BASE, channel)); 

  do {
    status = regRead(myvme, V1725_BASE, V1725_ACQUISITION_STATUS);
    printf("Acq Status1:0x%x\n", status);
  } while ((status & 0x8)==0);
  
  n32read = v1725_DataRead(myvme, V1725_BASE, &(data[0]), n32word);
  printf("n32read:%d\n", n32read);
  
  for (i=0; i<n32read;i+=4) {
    printf("[%d]:0x%x - [%d]:0x%x - [%d]:0x%x - [%d]:0x%x\n"
	   , i, data[i], i+1, data[i+1], i+2, data[i+2], i+3, data[i+3]);
  }
  
  do {
    status = regRead(myvme, V1725_BASE, V1725_ACQUISITION_STATUS);
    printf("Acq Status2:0x%x\n", status);
  } while ((status & 0x8)==0);
  
  n32read = v1725_DataRead(myvme, V1725_BASE, &(data[0]), n32word);
  printf("n32read:%d\n", n32read);
  
  for (i=0; i<n32read;i+=4) {
    printf("[%d]:0x%x - [%d]:0x%x - [%d]:0x%x - [%d]:0x%x\n"
	   , i, data[i], i+1, data[i+1], i+2, data[i+2], i+3, data[i+3]);
  }
  
  
  //  v1725_AcqCtl(myvme, V1725_BASE, RUN_STOP);
  
  regRead(myvme, V1725_BASE, V1725_ACQUISITION_CONTROL);
  status = mvme_close(myvme);

  return 1;

}
#endif

/* emacs
 * Local Variables:
 * mode:C
 * mode:font-lock
 * tab-width: 8
 * c-basic-offset: 2
 * End:
 */

//end
