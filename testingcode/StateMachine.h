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

extern short catX;
extern short catY;
extern short LastcatX;
extern short LastpcatY;

extern int ind;

extern int blockX[];
extern int blockY[];

extern int check[];

void update_shape();
void CatChange(int);
void catUpdate(int);

void The_State_Machine();
void state_change(int);

#endif // included
