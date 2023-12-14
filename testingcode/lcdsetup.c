#include "lcdutils.h"
#include "msp430.h"

u_char _orientation = 0;

/** LCD pin definitions*/
/** SCLK & MOSI*/
#define LCD_SPI_OUT	P1OUT
#define LCD_SPI_DIR	P1DIR
#define LCD_SPI_SEL	P1SEL
#define LCD_SPI_SEL2	P1SEL2
#define LCD_SCLK_PIN	BIT5
#define LCD_MOSI_PIN	BIT7

/** Chip select */
#define LCD_CS_PIN	BIT0
#define LCD_CS_DIR	P1DIR
#define LCD_CS_OUT	P1OUT

/** CS convenience defines */
#define LCD_SELECT() LCD_CS_OUT &= ~LCD_CS_PIN
#define LCD_DESELECT()

/** Data/command */
#define LCD_DC_PIN	BIT4
#define LCD_DC_DIR	P1DIR
#define LCD_DC_OUT	P1OUT

/** D/C convenience defines */
#define LCD_DC_LO() LCD_DC_OUT &= ~LCD_DC_PIN
#define LCD_DC_HI() LCD_DC_OUT |= LCD_DC_PIN

/** LCD driver IC specific defines */
#define SWRESET	0x01
#define SLEEPOUT	0x11
#define DISPON		0x29
#define CASETP		0x2A
#define PASETP		0x2B
#define RAMWRP		0x2C
#define MADCTL		0x36
#define COLMOD		0x3A
#define GMCTRP1	0xE0
#define GMCTRN1	0xE1
