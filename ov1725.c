/*********************************************************************

  Name:         ov1720.c

  $Id$
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "ov1725drv.h"

#define LARGE_NUMBER 10000000
// Buffer organization map for number of samples
uint32_t V1725_NSAMPLES_MODE[11] = { (1<<20), (1<<19), (1<<18), (1<<17), (1<<16), (1<<15)
              ,(1<<14), (1<<13), (1<<12), (1<<11), (1<<10)};

/*****************************************************************/
CAENComm_ErrorCode ov1725_ChannelSet(int handle, uint32_t channel, uint32_t what, uint32_t that)
{
  uint32_t reg, mask;

  if (what == V1725_CHANNEL_THRESHOLD)   mask = 0x0FFF;
  if (what == V1725_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
  if (what == V1725_CHANNEL_DAC)         mask = 0xFFFF;
  reg = what | (channel << 8);
  return CAENComm_Write32(handle, reg, (that & 0xFFF));
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_ChannelGet(int handle, uint32_t channel, uint32_t what, uint32_t *data)
{
  uint32_t reg, mask;

  if (what == V1725_CHANNEL_THRESHOLD)   mask = 0x0FFF;
  if (what == V1725_CHANNEL_OUTHRESHOLD) mask = 0x0FFF;
  if (what == V1725_CHANNEL_DAC)         mask = 0xFFFF;
  reg = what | (channel << 8);
  return CAENComm_Read32(handle, reg, data);
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_ChannelThresholdSet(int handle, uint32_t channel, uint32_t threshold)
{
  uint32_t reg;
  reg = V1725_CHANNEL_THRESHOLD | (channel << 8);
  printf("reg:0x%x, threshold:%x\n", reg, threshold);
  return CAENComm_Write32(handle, reg,(threshold & 0xFFF));
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_ChannelOUThresholdSet(int handle, uint32_t channel, uint32_t threshold)
{
  uint32_t reg;
  reg = V1725_CHANNEL_OUTHRESHOLD | (channel << 8);
  printf("reg:0x%x, outhreshold:%x\n", reg, threshold);
  return CAENComm_Write32(handle, reg, (threshold & 0xFFF));
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_ChannelDACSet(int handle, uint32_t channel, uint32_t dac)
{
  uint32_t reg, status, ncount;
  
  if ((channel >= 0) && (channel < 8)) {
    reg = V1725_CHANNEL_STATUS | (channel << 8);
    ncount = 10000;
    do {
      CAENComm_Read32(handle, reg, &status);
    } while ((status & 0x04) && (ncount--));
    if (ncount == 0) return -1;
    reg = V1725_CHANNEL_DAC | (channel << 8);
    printf("reg:0x%x, DAC:%x\n", reg, dac);
    return CAENComm_Write32(handle, reg, (dac & 0xFFFF));
  }
  return -1;
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_ChannelDACGet(int handle, uint32_t channel, uint32_t *dac)
{
  uint32_t reg;
  CAENComm_ErrorCode sCAEN = -1;

  if ((channel >= 0) && (channel < 8)) {
    reg = V1725_CHANNEL_DAC | (channel << 8);
    sCAEN = CAENComm_Read32(handle, reg, dac);
  }
  return sCAEN;
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_AcqCtl(int handle, uint32_t operation)
{
  uint32_t reg;
  CAENComm_ErrorCode sCAEN;
  
  sCAEN = CAENComm_Read32(handle, V1725_ACQUISITION_CONTROL, &reg);
  //  printf("sCAEN:%d ACQ Acq Control:0x%x\n", sCAEN, reg);

  switch (operation) {
  case V1725_RUN_START:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, (reg | 0x4));
    break;
  case V1725_RUN_STOP:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, (reg & ~( 0x4)));
    break;
  case V1725_REGISTER_RUN_MODE:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, 0x0);
    break;
  case V1725_SIN_RUN_MODE:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, 0x1);
    break;
  case V1725_SIN_GATE_RUN_MODE:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, 0x2);
    break;
  case V1725_MULTI_BOARD_SYNC_MODE:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, 0x3);
    break;
  case V1725_COUNT_ACCEPTED_TRIGGER:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, (reg & ~( 0x8)));
    break;
  case V1725_COUNT_ALL_TRIGGER:
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL, (reg | 0x8));
    break;
  default:
    printf("operation not defined\n");
    break;
  }
  return sCAEN;
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_ChannelConfig(int handle, uint32_t operation)
{
  CAENComm_ErrorCode sCAEN;
  uint32_t reg, cfg;
  
  sCAEN = CAENComm_Read32(handle, V1725_CHANNEL_CONFIG, &reg);  
  sCAEN = CAENComm_Read32(handle, V1725_CHANNEL_CONFIG, &cfg);  
  //  printf("Channel_config1: 0x%x\n", cfg);  

  switch (operation) {
  case V1725_TRIGGER_UNDERTH:
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_CFG_BIT_SET, 0x40);
    break;
  case V1725_TRIGGER_OVERTH:
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_CFG_BIT_CLR, 0x40);
    break;
  case V1725_PACK25_ENABLE:
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_CONFIG, (reg | 0x800));
    break;
  case V1725_PACK25_DISABLE:
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_CONFIG, (reg & ~(0x800)));
    break;
  case V1725_NO_ZERO_SUPPRESSION:
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_CONFIG, (reg & ~(0xF000)));
    break;
  case V1725_ZLE:
    reg &= ~(0xF000);
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_CONFIG, (reg | 0x2000));
    break;
  case V1725_ZS_AMP:
    reg &= ~(0xF000);
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_CONFIG, (reg | 0x3000));
    break;
  default:
    break;
  }
  sCAEN = CAENComm_Read32(handle, V1725_CHANNEL_CONFIG, &cfg);  
  //  printf("Channel_config2: 0x%x\n", cfg);
  return sCAEN;
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_info(int handle, int *nchannels, uint32_t *data)
{
  CAENComm_ErrorCode sCAEN;
  int i, chanmask;
  uint32_t reg;

  // Evaluate the event size
  // Number of samples per channels
  sCAEN = CAENComm_Read32(handle, V1725_BUFFER_ORGANIZATION, &reg);  
  *data = V1725_NSAMPLES_MODE[reg];

  // times the number of active channels
  sCAEN = CAENComm_Read32(handle, V1725_CHANNEL_EN_MASK, &reg);  
  chanmask = 0xff & reg; 
  *nchannels = 0;
  for (i=0;i< 16;i++) {
    if (chanmask & (1<<i))
      *nchannels += 1;
  }

  *data *= *nchannels;
  *data /= 2;   // 2 samples per 32bit word
  *data += 4;   // Headers
  return sCAEN;
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_BufferOccupancy(int handle, uint32_t channel, uint32_t *data)
{
  uint32_t reg;

  reg = V1725_BUFFER_OCCUPANCY + (channel<<16);
  return  CAENComm_Read32(handle, reg, data);  
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_BufferFree(int handle, int nbuffer, uint32_t *mode)
{
  CAENComm_ErrorCode sCAEN;

  sCAEN = CAENComm_Read32(handle, V1725_BUFFER_ORGANIZATION, mode);  
  if (nbuffer <= (1 << *mode) ) {
    sCAEN = CAENComm_Write32(handle,V1725_BUFFER_FREE, nbuffer);
    return sCAEN;
  } else
    return sCAEN;
}

/*****************************************************************/
CAENComm_ErrorCode ov1725_Status(int handle)
{
  CAENComm_ErrorCode sCAEN;
  uint32_t reg;

  printf("================================================\n");
  sCAEN = CAENComm_Read32(handle, V1725_BOARD_ID, &reg);  
  printf("Board ID             : 0x%x\n", reg);
  sCAEN = CAENComm_Read32(handle, V1725_BOARD_INFO, &reg);  
  printf("Board Info           : 0x%x\n", reg);
  sCAEN = CAENComm_Read32(handle, V1725_ACQUISITION_CONTROL, &reg);  
  printf("Acquisition control  : 0x%8.8x\n", reg);
  sCAEN = CAENComm_Read32(handle, V1725_ACQUISITION_STATUS, &reg);  
  printf("Acquisition status         : 0x%8.8x\n", reg);
  sCAEN = CAENComm_Read32(handle, V1725_CHANNEL_CONFIG, &reg);  
  printf("Channel Configuration      : 0x%5.5x\n", reg);
  sCAEN = CAENComm_Read32(handle, V1725_TRIG_SRCE_EN_MASK, &reg);  
  printf("Trigger Source Enable Mask : 0x%8.8x\n", reg);
  sCAEN = CAENComm_Read32(handle, V1725_VME_STATUS, &reg);  
  printf("VME Status                 : 0x%x\n", reg);
  sCAEN = CAENComm_Read32(handle, V1725_EVENT_STORED, &reg);  
  printf("Event Stored               : 0x%8.8x\n", reg);
  printf("================================================\n");
  return sCAEN;
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
CAENComm_ErrorCode  ov1725_Setup(int handle, int mode)
{
  CAENComm_ErrorCode sCAEN;
  switch (mode) {
  case 0x0:
    printf("--------------------------------------------\n");
    printf("Setup Skip\n");
    printf("--------------------------------------------\n");
  case 0x1:
    printf("--------------------------------------------\n");
    printf("Trigger from FP, 8ch, 1Ks, postTrigger 800\n");
    printf("--------------------------------------------\n");
    sCAEN = CAENComm_Write32(handle, V1725_BUFFER_ORGANIZATION,   0x0A);    // 1K buffer
    sCAEN = CAENComm_Write32(handle, V1725_TRIG_SRCE_EN_MASK,     0x4000);  // External Trigger
    sCAEN = CAENComm_Write32(handle, V1725_CHANNEL_EN_MASK,       0xFF);    // 8ch enable
    sCAEN = CAENComm_Write32(handle, V1725_POST_TRIGGER_SETTING,  800);     // PreTrigger (1K-800)
    sCAEN = CAENComm_Write32(handle, V1725_ACQUISITION_CONTROL,   0x00);    // Reset Acq Control
    printf("\n");
    break;
  case 0x2:
    printf("--------------------------------------------\n");
    printf("Trigger from LEMO\n");
    printf("--------------------------------------------\n");
    sCAEN = CAENComm_Write32(handle, V1725_BUFFER_ORGANIZATION, 1);
    printf("\n");
    break;
  default:
    printf("Unknown setup mode\n");
    return -1;
  }
  return ov1725_Status(handle);
}


//end
