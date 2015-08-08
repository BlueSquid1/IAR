#include "msp430x22x4.h"

void configureClocks();

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  configureClocks();
  
  P1DIR |= BIT0;
  P1DIR |= BIT1;
  
  P1OUT = 0x00;
  
  //Capture/compare interrupt enable
  TACCTL0 = CCIE;
  
  //count to 6000
  TACCR0 = 6000;
  
  //use SMCLK and count up to 5000
  TACTL = TASSEL_1 + ID_0 + MC_1;
  
  //enable interrupts (also can be done with "__enable_interrupt()")
  //also put CPU to sleep (only to be waken by an interrupt)
  __bis_SR_register(LPM3_bits + GIE);
  
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

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  P1OUT ^= BIT0;
  
  TACCR0+= 200;
  //wake up the CPU from LPM3
  //__bic_SR_register_on_exit(LPM3_bits);
}