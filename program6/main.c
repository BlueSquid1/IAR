
#include "msp430x22x4.h"

void configureClocks();

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  configureClocks();
  
  //set P3.0 and P3.4 to second multiplexing action
  //P3.1 is assumed to already be for USCI_BO
  P3SEL |= BIT0 + BIT4;
  
  //set up the USCI (Universal Serial Communication Interface) with SPI
  //Select the most signficant bit first
  //Make this USCI module a master
  // in synchronous mode
  //defaults to 3 wire communication
  //use 3-pin interface
  UCB0CTL0 |= UCMSB + UCMST + UCSYNC + UCCKPH;
  
  //Source from SMCLK
  UCB0CTL1 |= UCSSEL_2;
  
  //bit rate control register
  UCB0BR0 = 0x02;
  
  UCB0BR1 = 0;
  
  UCA0MCTL = 0;
  
  //enable SPI
  UCB0CTL1 &= ~UCSWRST;
  
  unsigned char data = 0x0F;
  volatile unsigned int i;
  
  while(1)
  {
    data++;
    while(!(IFG2 & UCA0TXIFG))
    {
    }
    
    UCA0TXBUF = data;
    
    //delay
    for(i = 0; i < 0xFFFF; i++)
    {
    }
  }
  
  //disable SPI
  P3OUT |= 0x01;
  
  return 0;
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