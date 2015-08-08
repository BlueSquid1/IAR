#include "msp430x22x4.h"

void Initalize();
void configureClocks();
void ClearFlashSegment(char * segment);
void WriteToFlash(char * flashPointer, const char * source, unsigned int size);

#define INFO_SEG_A (0x1000u)
#define INFO_SEG_B (0x1040u)
#define INFO_SEG_C (0x1080u)
#define INFO_SEG_D (0x10C0u)
#define INFO_SEG_END (0x1100u)

void Initalize()
{
  // Stop watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  //set clock speed
  configureClocks();
  
  FCTL2 = FWKEY + FSSEL0 + FN1;
}

void main(void)
{ 
  Initalize();
  
  char *Flash_ptr = (char *) INFO_SEG_B; // point to flash (INFO segment B)
  
  char infMem[64];
  infMem[0] = 5;
  infMem[1] = 4;
  infMem[2] = 2;
  infMem[3] = 1;
  
  WriteToFlash(Flash_ptr, infMem, 4);
  
  while(1)
  {
  __no_operation();
  }
}

void configureClocks()
{
  //set Digitally Controlled Oscillator (DCO)
  //to 8MHz (this is sets the CPU clock speed to 1MHz)
  
  if (CALBC1_1MHZ==0xFF)
  {		
    P1DIR = BIT0;
    P1OUT = BIT0;
    while(1);	
  }
  
  DCOCTL = CALDCO_1MHZ;
  
  //set ACLK (BCSCTL1) to 1MHz
  BCSCTL1 = CALBC1_1MHZ;
  
  // Set LFXT1 (BCSCTL3) to internal Very Low Frequency Clock (VLOCLK) @ 12kHz
  BCSCTL3 |= LFXT1S_2;
}

void ClearFlashSegment(char * segment)
{
  FCTL3 = FWKEY; // Clear LOCK
  FCTL1 = FWKEY + ERASE; // Set register to erase
  
  //wipes the whole flash segment 
  *segment = 0; //dummy write to erase Flash seg
  
  //clear erase bit
  FCTL1 = FWKEY;
  
  //set LOCK bit
  FCTL3 = FWKEY + LOCK;
}

void WriteToFlash(char * flashPointer, const char * source, unsigned int size)
{
  //first clear the memory
  ClearFlashSegment(flashPointer);
  
  //clear lock bit
  FCTL3 = FWKEY;
  
  //Set WRT bit for write operation
  FCTL1 = FWKEY + WRT;
  
  int i;
  for(i = 0; i < size; i++)
  {
    flashPointer[i] = source[i];
  }
  
  //clear WRT bit
  FCTL1 = FWKEY;
  //set LOCK bit
  FCTL3 = FWKEY + LOCK;
}