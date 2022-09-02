#include <stdio.h>
#include "uart.h"
#include <unistd.h>
#include "pid.h"
#include "gpio.h"

int menu();

int comando_usuario = 0;
int airfray_ligada = 0;
int airfray_em_uso = 0;
float temperatura_interna = 0;
float temperatura_referencia = 0;
int inicia = 0;
double sinal_controle = 0.0;
float tempo = 0;
float segundos = 0;

int main(){
    pid_configura_constantes(30.0, 0.2, 400.0);
    inicializa_GPIO();
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
            if(airfray_ligada){
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
                printf("TEMPO ATUAL: %f\n", tempo);
            }
            
            break;
        case 6:
            //Tempo-- AirFryer
            if(airfray_ligada && !airfray_em_uso){
                envia_valor_temporizador(-1);
                tempo--;
                printf("TEMPO ATUAL: %f\n", tempo);
            }
            break;
        
        default:
            break;
     }

     if(airfray_ligada == 1 && airfray_em_uso == 1){
        printf("Valor inicia: %d\n", inicia);
        if(!inicia){
            temperatura_referencia = solicita_temperatura_referencia();
            pid_atualiza_referencia(temperatura_referencia);
            printf("Entrou na solicitação de temperatura de referencia %f\n", temperatura_referencia);
            inicia = 1;
        }
        if(inicia){
            temperatura_interna = solicita_temperatura_interna();
            sinal_controle = pid_controle(temperatura_interna);
            printf("Sinal de controle: %f", sinal_controle);
            printf("Entrou na solicitação de temperatura %f\n", temperatura_interna);
            envia_sinal_controle(sinal_controle);
            if(sinal_controle >= 0){
                aquece(sinal_controle);
                
            }else if(sinal_controle <0){
                if(sinal_controle>=-40){
                    resfria(40);
                }
                else {
                    sinal_controle *=-1;
                    resfria(sinal_controle);
                }
            }

        }

     }else if(!airfray_ligada && !airfray_em_uso){
        para_aquecimento();
        inicia = 0;
     } else {
        para_aquecimento();
        airfray_em_uso = 0;
     }
     sleep(0.5);
     if(airfray_ligada && airfray_em_uso && temperatura_interna >= temperatura_referencia){
        segundos+=1.5;
        printf("Segundos %f -- Minutos %f\n", segundos, tempo);
        if(segundos >= 60){
            tempo--;
            envia_valor_temporizador(tempo);
            segundos = 0;
        }
     }
    }
   return 0;
}

int menu(){
  printf("-------------Opções-----------\n");
  printf("1. Solicitar Temperatura Interna\n");
  printf("2. Solicitar Temperatura Referencia\n");
  printf("3. Le comandos Usuario\n");
  printf("4. Enviar Sinal de Controle\n");
  printf("5. Enviar Sinal de Referencia\n");
  printf("6. Enviar Sinal de Estado do Sistema\n");
  printf("7. Enviar Sinal de Funcionamento do Sistema\n");
  printf("8. Enviar valor do Temporizador\n");
  printf("0. SAIR\n");
  printf("\n");
  printf("Digite o número da Opção desejada\n");
  int opcao;
  scanf("%d", &opcao);
  return opcao;
}