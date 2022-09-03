#ifndef LCD_H_
#define LCD_H_
void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);

void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); //move cursor
void ClrLcd(void); // clr LCD return home
void typeln(const char *s);
void typeChar(char val);
void liga_lcd(float temperatura_interna, float temperatura_referencia);
void mostra_ti_tr(float temperatura_interna, float temperatura_referencia);
void mostra_status(int status);
void mostra_tempo(float tempo);
#endif /* LCD_H_ */
