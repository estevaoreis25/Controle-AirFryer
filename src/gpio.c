#include <wiringPi.h>
#include <softPwm.h>
#include "gpio.h"

int RESISTOR_PINO = 4;
int VENTOINHA_PINO = 5;

void inicializa_GPIO(){
  wiringPiSetup();
  softPwmCreate(RESISTOR_PINO, 0, 100);
  softPwmCreate(VENTOINHA_PINO, 0, 100);
} 

void aquece(int intensidade){
  softPwmWrite(RESISTOR_PINO, intensidade);
}

void resfria(int intensidade){
  softPwmWrite(VENTOINHA_PINO, intensidade);
}

void para_aquecimento(){
  softPwmStop(VENTOINHA_PINO);
  softPwmStop(RESISTOR_PINO);
}