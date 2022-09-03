#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include <unistd.h>

#define I2C_ADDR   0x27 // I2C device address

#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINHA1  0x80 // 1st line
#define LINHA2  0xC0 // 2nd line

#define LCD_LIGADO  0x08  // On
#define LCD_DESLIGADO = 0x00  //Off

#define ENABLE  0b00000100 // Enable bit


void typeChar(char val);
int fd;  // seen by all subroutines

// float to string
void typeFloat(float myFloat)   {
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}

// int to string
void typeInt(int i)   {
  char array1[20];
  sprintf(array1, "%d",  i);
  typeln(array1);
}

// clr lcd go home loc 0x80
void ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// this allows use of any size string
void typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_LIGADO ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_LIGADO ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(100);
}


void lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}

void liga_lcd(float temperatura_interna, float temperatura_referencia){
  printf("ENTROU NO LCD\n");
  if (wiringPiSetup () == -1) exit (1);
  fd = wiringPiI2CSetup(I2C_ADDR);
  lcd_init(); // setup LCD
  mostra_ti_tr(temperatura_interna, temperatura_referencia);
  mostra_status(0);
}

void mostra_ti_tr(float temperatura_interna, float temperatura_referencia){
    ClrLcd(); 
    lcdLoc(LINHA1);
    typeln("TI:");
    typeFloat(temperatura_interna);
    typeln(" ");
    typeln("TR:");
    typeFloat(temperatura_referencia);
}

void mostra_status(int status){
  switch (status){
    case 0:
      // Airfri Iniciada
      lcdLoc(LINHA2);
      //typeln("            ");
      typeln("PARADO");
      break;
    case 1:
      // PRE-AQUECENDO
      lcdLoc(LINHA2);
      //typeln("            ");
      typeln("PRE-AQUECENDO");
      break;
    case 2:
      // RESFRIANDO
      lcdLoc(LINHA2);
      //typeln("            ");
      typeln("RESFRIANDO");
      break;
  default:
    break;
  }
}

void mostra_tempo(float tempo){
  lcdLoc(LINHA2);
  typeln("TEMPO ");
  typeFloat(tempo);
}

void mostra_menu(int opcao_menu){
    switch (opcao_menu){
    case 1:
        ClrLcd(); 
        lcdLoc(LINHA1);
        typeln("TEMPO:");
        typeFloat(2.0);
        lcdLoc(LINHA2);
        typeln("FRANGO");
    break;
    case 2:
        ClrLcd(); 
        lcdLoc(LINHA1);
        typeln("TEMPO:");
        typeFloat(3.0);
        lcdLoc(LINHA2);
        typeln("PEIXE");
    break;
    case 3:
        ClrLcd(); 
        lcdLoc(LINHA1);
        typeln("TEMPO:");
        typeFloat(4.0);
        lcdLoc(LINHA2);
        typeln("CARNE");
    break;
    case 4:
        ClrLcd(); 
        lcdLoc(LINHA1);
        typeln("TEMPO:");
        typeFloat(5.0);
        lcdLoc(LINHA2);
        typeln("ARROZ");
    break;
    case 5:
        ClrLcd(); 
        lcdLoc(LINHA1);
        typeln("TEMPO:");
        typeFloat(3.0);
        lcdLoc(LINHA2);
        typeln("PIPOCA");
    break;
    case 6:
        ClrLcd(); 
        lcdLoc(LINHA1);
        typeln("TEMPO:");
        typeFloat(4.0);
        lcdLoc(LINHA2);
        typeln("VEGETAIS");
    break;
    default:
        break;
    }
}