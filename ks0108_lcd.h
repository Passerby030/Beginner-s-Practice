#ifndef KS0108_LCD_H
#define KS0108_LCD_H

#include <REGX52.H>

/* ===== Wiring used in this example =====
 * LCD DB0~DB7 -> P0.0~P0.7
 * LCD RS      -> P2.0
 * LCD RW      -> P2.1
 * LCD E       -> P2.2
 * LCD CS1     -> P2.3
 * LCD CS2     -> P2.4
 * LCD RST     -> +5V (hardware pull-up, no MCU control)
 * LCD VCC     -> +5V
 * LCD GND     -> GND
 * LCD VO      -> contrast input (recommended: 10k pot wiper)
 * LCD VOUT    -> one end of contrast pot
 */

#define GLCD_DATA_PORT P0

sbit GLCD_RS  = P2^0;
sbit GLCD_RW  = P2^1;
sbit GLCD_EN  = P2^2;
sbit GLCD_CS1 = P2^3;
sbit GLCD_CS2 = P2^4;

void DelayMs(unsigned int ms);
void GLCD_Init(void);
void GLCD_Clear(void);
void GLCD_SetPage(unsigned char page);
void GLCD_SetColumn(unsigned char column);
void GLCD_WriteData(unsigned char dat);

#endif
