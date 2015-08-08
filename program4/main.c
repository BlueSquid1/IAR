#include "msp430x22x4.h"

void Initalize();
void configureClocks();

void Initalize()
{
  // Stop watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  //set clock speed
  configureClocks();
}

void main(void)
{ 
  Initalize();
  
  //set p1.1 to output
  P1DIR = BIT1;
  //and p1.2 to input
  P1DIR &= ~BIT2;
  
  //enable pullup/down for p1.2
  P1REN |= BIT2;
  
  //set p1.2 to a pullup
  P1OUT = BIT2;
  P1OUT &= ~BIT1;
  
  //enable interrupt on p1.2
  P1IE |= BIT2;
  //interrupt when high to low for p1.2
  P1IES |= BIT2;
  
  //p1.2 Interrupt Flag (IFG) cleared (just in case)
  P1IFG &= ~BIT2;
  
  _EINT();
  
  //activates low power mode (LPM) 4
  //with General Interrupt Enable (GIE)
  __bis_SR_register(LPM4_bits + GIE);
  
  //this code is only ran after an interupt
  while(1)
  {
  __no_operation();
  }
}

void configureClocks()
{
  //set Digitally Controlled Oscillator (DCO)
  //to 8MHz (this is sets the CPU clock speed to 8MHz)
  
  if (CALBC1_8MHZ==0xFF)
  {		
    P1DIR = BIT0;
    P1OUT = BIT0;
    while(1);	
  }
  
  DCOCTL = CALDCO_8MHZ;
  
  //set ACLK (BCSCTL1) to 8MHz
  BCSCTL1 = CALBC1_8MHZ;
  
  // Set LFXT1 (BCSCTL3) to internal Very Low Frequency Clock (VLOCLK) @ 12kHz
  BCSCTL3 |= LFXT1S_2;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  //toggle LED
  P1OUT ^= BIT1;
  
  //clear IFG
  P1IFG &= ~BIT2;
}