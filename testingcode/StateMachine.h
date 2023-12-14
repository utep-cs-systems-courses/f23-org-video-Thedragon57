#ifndef StateMachine_included
#define StateMachine_included



extern int state;
extern int toggle_led;
extern short rate;
extern char blinking;
extern short frz;
extern short playerX;
extern short playerY;
extern short LastplayerX;
extern short LastplayerY;

extern short current_positionX, current_positionY;

extern int redrawScreen;

void update_shape();


void The_State_Machine();
void state_change(int);

#endif // included
