# makfile configuration
COMMON_OBJECTS  = main.o
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		=  -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c


all: main.elf

#additional rules for files
main.elf: ${COMMON_OBJECTS} main.o light.o StateMachine.o buzzer.o switch.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: main.elf
	msp430loader.sh main.elf

main.o: light.h StateMachine.h buzzer.h switch.h

light.o: StateMachine.h buzzer.h switch.h

StateMachine.o: light.h buzzer.h switch.h 

buzzer.o: light.h StateMachine.h switch.h

switch.o: light.h StateMachine.h switch.h


clean:
	rm -f *.o *.elf
