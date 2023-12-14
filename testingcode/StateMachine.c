#include <msp430.h>
#include "StateMachine.h"
#include "switch.h"
#include "light.h"
#include "buzzer.h"
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

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
      if(playerX != 0){
        playerX -= 16;
        fillRectangle(playerX, playerY, 15, 15, COLOR_ORANGE); 
        if(playerX!=LastplayerX || playerY!=LastplayerY){
          fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        }
        LastplayerX -= 16;
      }


      state = 2;
      break;
    case 2:
      //lights_off();
      //buzzer_set_period(0);
      //blinking = 0;
      if(playerY != 0){
        playerY -= 16;
        fillRectangle(playerX, playerY, 15, 15, COLOR_ORANGE); 
        if(playerX!=LastplayerX || playerY!=LastplayerY){
          fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        } 
        LastplayerY -= 16;
      }
      state = 3;
      break;
    case 3:
      //lights_on();
      //blinking = 0;
      if(playerY < 144){
        playerY += 16;
        fillRectangle(playerX, playerY, 15, 15, COLOR_ORANGE); 
        if(playerX!=LastplayerX || playerY!=LastplayerY){
          fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        }
        LastplayerY += 16;
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
      if(playerX < 111){
        playerX += 16;
        fillRectangle(playerX, playerY, 15, 15, COLOR_ORANGE); 
        if(playerX!=LastplayerX || playerY!=LastplayerY){
          fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
        }
        LastplayerX += 16;
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

void state_change(int new_state){
    state = new_state;
    The_State_Machine();
}






void
update_shape()
{
    fillRectangle(LastplayerX, LastplayerY, 15, 15, COLOR_BLACK); 
    /* draw new shape */
    fillRectangle(playerX, playerY, 15, 15, COLOR_ORANGE); /* draw new shape */
    /* remember color & pos for next redraw */
}