#include <msp430.h>
#include "buzzer.h"
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

int state = 2;
int toggle_led = 0;
short rate = 0x100;
char blinking = 0;
short frz = 0;
short playerX = 0;
short playerY = 0;
short LastplayerX = 0;
short LastplayerY = 0;

int redrawScreen = 1;


int ind = 0;
unsigned char move = 1;

int snake[5][3];
int snake_length = 0;



//-----------------------------------------------------------------------------------------------------
void __interrupt_vec(PORT2_VECTOR) Port_2(){ //telling the computer that when an intrupt happens to use this funtion

  if(P2IFG & BIT0){
    //lights_off();
    //state_change(1);
    move = 3;
    P2IFG &= ~(BIT0);
  }
  if(P2IFG & BIT1){
    //state_change(2);
    move = 2;
    //lights_on();
    P2IFG &= ~(BIT1);
  }

  if(P2IFG & BIT2){
    //state_change(3);
    move = 0;
    P2IFG &= ~(BIT2);
  }

  if(P2IFG & BIT3){
    //state_change(4);
    move = 1;
    P2IFG &= ~(BIT3);
  }
  
  __delay_cycles(20000);

}

void state_change(int new_state){
    state = new_state;
    The_State_Machine();
}



//-----------------------------------------------------------------------------------------------------


void The_State_Machine(){
  switch(state){
    case 1:
    /*
      blinking = 1;
      if (frz < 1900){
        frz += 100;
        buzzer_set_period(frz);
      }
      if(rate > 0x10)
      rate = rate >> 1;
      */
      if(snake[0][1] != -1){
        fillRectangle(snake[0][1], snake[0][2], 15, 15, COLOR_ORANGE); 
        if(playerX!=LastplayerX || playerY!=LastplayerY){
          fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        }
        LastplayerX = snake[0][1];
      }


      state = 2;
      break;
    case 2:
      //lights_off();
      //buzzer_set_period(0);
      //blinking = 0;
      if(snake[0][2] != -1){
        fillRectangle(snake[0][1], snake[0][2], 15, 15, COLOR_ORANGE); 
        if(playerX!=LastplayerX || playerY!=LastplayerY){
          fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        } 
        LastplayerY = snake[0][2];
      }
      state = 3;
      break;
    case 3:
      //lights_on();
      //blinking = 0;
      if(snake[0][2] != 143){
        fillRectangle(snake[0][1], snake[0][2], 15, 15, COLOR_ORANGE); 
        
        fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        
        LastplayerY = snake[0][2];
      }
      state = 4;
      break;
    case 4:
      //blinking = 1;
      /*
      if (frz > 100){
        frz -= 100;
        buzzer_set_period(frz);
      }
      
      if(rate < 0x400);
      rate = rate << 1;
      */
      if(snake[0][1] != 111){
        fillRectangle(snake[0][1], snake[0][2], 15, 15, COLOR_ORANGE); 
        
        fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        
        LastplayerX = snake[0][1];
      }


      state = 1;
      break;
    default:
      buzzer_set_period(0);
      playerX = 0;
      playerY = 0;
      LastplayerX = playerX;
      LastplayerY = playerY;
      state = 2;
      break;
  }
}




//-----------------------------------------------------------------------------------------------------



void
update_shape()
{
    fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
    /* draw new shape */
    fillRectangle(playerX, playerY, 15, 15, COLOR_ORANGE); /* draw new shape */
    /* remember color & pos for next redraw */
}

//-----------------------------------------------------------------------------------------------------


void configureClocks(){
  WDTCTL = WDTPW + WDTHOLD;//Disable Watchdog Timer
  BCSCTL1 = CALBC1_16MHZ;  // Set DCO to 16 Mhz
  DCOCTL = CALDCO_16MHZ;

  BCSCTL2 &= ~(SELS);     // SMCLK source = DCO
  BCSCTL2 |= DIVS_3;      // SMCLK = DCO / 8
}

//-----------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
void __interrupt_vec(WDT_VECTOR) WDT(){
  static char blink_count = 0;
  if (++blink_count == (rate/2) && blinking == 1) {
    light_toggle();
    blink_count = 0;
  }

  static int secCount = 0;
  secCount++;
  if (secCount++ >= 250) {		/* 1/sec */
    secCount = 0;
    if(move == 3 && snake[0][1] != 0){
      snake[0][1] -= 16;
      state_change(1);
    }
    if(move == 2 && snake[0][2] < 144){
      snake[0][2] += 16;
      state_change(3);
    }
    if(move == 0 && snake[0][2] != 0){
      snake[0][2] -= 16;
      state_change(2);
    }
    if(move == 1 && snake[0][1] < 111){
      snake[0][1] += 16;
      state_change(4);
    }
  }
}

//-----------------------------------------------------------------------------------------------------
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
  }
  snake[0][0] = 1;
  snake[0][1] = 0; //x value
  snake[0][2] = 0; //y value

  __bis_SR_register((LPM0_bits | GIE)); //LPM 4 disables watchdog timer and LMP 0 disables the cpu

  while (1) {			/* forever */
    P1OUT &= ~BIT0;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= BIT0;	/* led on */
  }


}
//-----------------------------------------------------------------------------------------------------