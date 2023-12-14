#include <msp430.h>
#include "light.h"
#include "switch.h"

void enable_lights(){
    P1DIR |= (BIT0 | BIT6);
    P1OUT |= (BIT0 | BIT6);
}

void light_toggle(){
    P1OUT ^= (BIT0 | BIT6);
}

void lights_on(){
    P1OUT |= (BIT0 | BIT6);
}

void lights_off(){
    P1OUT &= ~(BIT0 | BIT6);
}
