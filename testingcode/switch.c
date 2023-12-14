#include <msp430.h>
#include "switch.h"
#include "light.h"
#include "StateMachine.h"
#include "buzzer.h"

void __interrupt_vec(PORT2_VECTOR) Port_2(){ //telling the computer that when an intrupt happens to use this funtion
  if(P2IFG & BIT0){
    //lights_off();
    state_change(1);
    P2IFG &= ~(BIT0);
  }
  if(P2IFG & BIT1){
    state_change(2);
    //lights_on();
    P2IFG &= ~(BIT1);
  }

  if(P2IFG & BIT2){
    state_change(3);
    P2IFG &= ~(BIT2);
  }

  if(P2IFG & BIT3){
    state_change(4);
    P2IFG &= ~(BIT3);
  }
  __delay_cycles(20000);

}
