#include "msp430x22x4.h"

//volatile to prevent optimization
volatile unsigned int i;
volatile unsigned int count;

void configureClocks();


void main( void )
{
  // Stop watchdog timer to prevent idle time out
  WDTCTL = WDTPW + WDTHOLD;
  //set the clock speed
  configureClocks();
  
  //set P1.0 to output direction
  P1DIR ^= BIT0 + BIT1;
  P1OUT = 0x00;
  P1OUT |= BIT0;
  
  for(;;)
  { 
    //turn on the pin
    P1OUT ^= BIT0 + BIT1;
    
    //delay
    count = 20;
    do
    {
      count--;
      i = 50000;
      do
      {
        i--;
      }
      while(i != 0);
    }
    while(count != 0);
  }
}


void configureClocks()
{
  //set Digitally Controlled Oscillator (DCO)
  //to 8MHz (this is sets the CPU clock speed to 8MHz)
  DCOCTL = CALDCO_8MHZ;
  
  //set ACLK to 8MHz
  BCSCTL1 = CALBC1_8MHZ;
  
  // Set LFXT1 (BCSCTL3) to internal Very Low Frequency Clock (VLOCLK) @ 12kHz
  BCSCTL3 |= LFXT1S_2;
}
