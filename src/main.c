#include <stdio.h>
#include "uart.h"
#include <unistd.h>
#include "pid.h"
#include "gpio.h"
#include <math.h>
#include <stdlib.h>

void conta_tempo();
void aquece_airfrey();

int comando_usuario = 0;
int airfray_ligada = 0;
int airfray_em_uso = 0;
float temperatura_interna = 0;
float temperatura_referencia = 0;
int inicia = 0;
double sinal_controle = 0.0;
float tempo = 0;
float segundos = 0;
int inicia_cozimento = 0;
int resfriado = 1;

int main(){
    pid_configura_constantes(30.0, 0.2, 400.0);
    inicializa_GPIO();
    aquece(0);
    resfria(0);
    envia_estado_sistema(0);
    envia_estado_funcionamento(0);
    envia_valor_temporizador(0);

    while(1){
     comando_usuario = le_comandos_usuario();
     switch (comando_usuario){
        case 1:
            //Liga AirFryer
            envia_estado_sistema(1);
            airfray_ligada = 1;

            break;
        case 2:
            //Desliga AirFryer
            envia_estado_sistema(0);
            envia_estado_funcionamento(0);
            envia_valor_temporizador(0);
            airfray_ligada = 0;
            
            break;
        case 3:
            //Inicia AirFryer
            if(airfray_ligada && tempo >0){
                envia_estado_funcionamento(1);
                airfray_em_uso = 1;
            }
            
            break;
        case 4:
            //Para AirFryer
            envia_estado_funcionamento(0);
            airfray_em_uso = 0;

            break;
        case 5:
            //Tempo++ AirFryer
            if(airfray_ligada && !airfray_em_uso){
                envia_valor_temporizador(1);
                tempo++;
                //printf("TEMPO ATUAL: %f\n", tempo);
            }
            
            break;
        case 6:
            //Tempo-- AirFryer
            if(airfray_ligada && !airfray_em_uso){
                envia_valor_temporizador(-1);
                tempo--;
                //printf("TEMPO ATUAL: %f\n", tempo);
            }
            break;
        
        default:
            break;
     }

     if(airfray_ligada && airfray_em_uso){
        aquece_airfrey();

     }else if(!airfray_em_uso && airfray_ligada) {
        //para_aquecimento();
     }
     if(!inicia_cozimento && airfray_em_uso && !resfriado && tempo==0){
        // Resfriamento da airfrey apos o cozimento
        //printf("Entrou no resfriamento\n");
        pid_atualiza_referencia(26.0f);
        envia_sinal_referencia(26.0f);
        temperatura_referencia = 26.0;
        if((temperatura_interna - 0.7) <= temperatura_referencia){
            // ser for completamente resfriada setta os paramentos de inicialização
            aquece(0);
            resfria(0);
            para_aquecimento();
            envia_estado_funcionamento(0);
            envia_estado_sistema(0);
            airfray_em_uso = 0;
            airfray_ligada = 0;
            inicia = 0;
            envia_valor_temporizador(0);
            tempo = 0;
            segundos = 0;
            inicia_cozimento = 0;
            resfriado = 1;
        }
     }
     conta_tempo();
     sleep(0.5);
    }
   return 0;
}

void conta_tempo(){
    if(inicia_cozimento){
        segundos+=3.0f;
        if(segundos >= 60){
            tempo--;
            envia_valor_temporizador(-1);
            segundos = 0;
        }
        if(tempo <= 0.0){
            //printf("ACABOU O TEMPO\n");
            inicia_cozimento = 0;
            segundos = 0;
        }
        //printf("Segundos %.1f -- Minutos %.1f\n", segundos, tempo);
    }
}

void aquece_airfrey(){
    resfriado = 0;
    if(!inicia){
        temperatura_referencia = solicita_temperatura_referencia();
        pid_atualiza_referencia(temperatura_referencia);
        //printf("Entrou na solicitação de temperatura de referencia %f\n", temperatura_referencia);
        inicia = 1;
    }
    if(inicia){
        temperatura_interna = solicita_temperatura_interna();
        sinal_controle = pid_controle(temperatura_interna);
        //printf("Sinal de controle: %f\n", sinal_controle);
        envia_sinal_controle((int)sinal_controle);
        if(sinal_controle >= 0){
            aquece((int)sinal_controle);
            resfria(0);
            
        }else if(sinal_controle <0){
            if(sinal_controle>=-40) resfria(40);
            else resfria(abs((int)sinal_controle));
            aquece(0);
        }
        if((temperatura_interna + 0.5) >= temperatura_referencia && tempo>0){
            inicia_cozimento = 1;
        }
    }
}