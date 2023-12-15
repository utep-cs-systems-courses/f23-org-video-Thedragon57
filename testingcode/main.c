#include <msp430.h>
#include "buzzer.h"
#include "StateMachine.h"
#include "switch.h"
#include "light.h"
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define NUM_POSITIONS 5

unsigned short sqColors[] = {COLOR_RED, COLOR_GREEN, COLOR_ORANGE, COLOR_BLUE};
#define NUM_SQCOLORS 4
#define BG_COLOR COLOR_BLACK

char current_position = 0, current_color = 0;
  
// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SWITCHES 15

//int redrawScreen = 1;

int main(void) {
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  P1DIR &= ~(BIT3);//set the negation to have the direction of the button to 0
  P1OUT |= (BIT3);//sets the out dirction to 1
  P1REN |= (BIT3);//enables the pull resitors for ports 0-3
  P1IE |= (BIT3);//enable interupt
  P1IES |= (BIT3);//enable interupt edge select
  P1IFG &= ~(BIT3);//sets the interupt flag to 0
  P2DIR &= ~(BIT0 | BIT1 | BIT2| BIT3);//set the negation to have the direction of the button to 0
  P2OUT |= (BIT0 | BIT1 | BIT2 | BIT3);//sets the out dirction to 1
  P2REN |= (BIT0 | BIT1 | BIT2 | BIT3);//enables the pull resitors for ports 0-3
  P2IE |= (BIT0 | BIT1 | BIT2 | BIT3);//enable interupt
  P2IES |= (BIT0 | BIT1 | BIT2 | BIT3);//enable interupt edge select
  P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3);//sets the interupt flag to 0
  timerAUpmode();
  P2DIR = BIT6;		/* enable output to speaker (P2.6) */
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;

  lcd_init();

  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLACK);

  for(int i = 0; i <= width+1; i+=16){
    if(i!=0){
      fillRectangle(i-1, 0, 1, 160, COLOR_WHITE);
    }
  }
  for(int i = 0; i <= height+1; i+=16){
    if(i!=0){
      fillRectangle(0, i-1, 128, 1, COLOR_WHITE);
    }
  fillRectangle(64, 80, 15, 15, COLOR_BLUE);
  fillRectangle(0, 0, 15, 15, COLOR_ORANGE);
  /*
  fillRectangle(112, 16, 15, 15, COLOR_ORANGE);
  fillRectangle(96, 0, 15, 15, COLOR_ORANGE);
  fillRectangle(80, 0, 15, 15, COLOR_ORANGE);
  fillRectangle(0, 144, 15, 15, COLOR_ORANGE);
  */
  }


  __bis_SR_register((LPM0_bits | GIE)); //LPM 4 disables watchdog timer and LMP 0 disables the cpu
}


void configureClocks(){
  WDTCTL = WDTPW + WDTHOLD;//Disable Watchdog Timer
  BCSCTL1 = CALBC1_16MHZ;  // Set DCO to 16 Mhz
  DCOCTL = CALDCO_16MHZ;

  BCSCTL2 &= ~(SELS);     // SMCLK source = DCO
  BCSCTL2 |= DIVS_3;      // SMCLK = DCO / 8
}


// enable watchdog timer periodic interrupt
// period = SMCLOCK/32k
void enableWDTInterrupts()
{
  WDTCTL = WDTPW |	   // passwd req'd.  Otherwise device resets
    WDTTMSEL |		     // watchdog interval mode
    WDTCNTCL |		     // clear watchdog count
    1;			     // divide SMCLK by 8192
  IE1 |= WDTIE;		   // Enable watchdog interval timer interrupt
}


void timerAUpmode()
{
  TA0CCR0 = 0;
  TA0CCR1 = 0;
  TA0CCTL1 = OUTMOD_3;		/* Toggle p1.6 when timer=count1 */

  // Timer A control:
  //  Timer clock source 2: system clock (SMCLK)
  //  Mode Control 1: continuously 0...CCR0
  TACTL = TASSEL_2 + MC_1;
}

void wdt_c_handler()
{
  static int sec2Count = 0;
  static int sec1Count = 0;
  if (sec2Count++ >= 125) {		/* 2/sec */
    sec2Count = 0;
    current_color = (current_color+1) % NUM_SQCOLORS;
    redrawScreen = 1;
  }
  if (sec1Count++ >= 250) {		/* 1/sec */
    sec1Count = 0;
    current_position = (current_position+1) % NUM_POSITIONS;
    redrawScreen = 1;
  }
}

void __interrupt_vec(WDT_VECTOR) WDT(){
  static char blink_count = 0;
  if (++blink_count == (rate/2) && blinking == 1) {
    light_toggle();
    blink_count = 0;
  }
}
