#include <stdio.h>
#include "uart.h"
#include <unistd.h>

int menu();

int comando_usuario;

int main(){
    while(1){

     comando_usuario = le_comandos_usuario();
     switch (comando_usuario){
     case 1:
        //Liga AirFryer
        envia_estado_sistema(1);

        break;
    case 2:
        //Desliga AirFryer
        envia_estado_sistema(0);
        
        break;
    case 3:
        //Inicia AirFryer
        envia_estado_funcionamento(1);
        
        break;
    case 4:
        //Para AirFryer
        envia_estado_funcionamento(0);

        break;
    case 5:
        //Tempo++ AirFryer
        envia_valor_temporizador(1);
        
        break;
    case 6:
        //Tempo-- AirFryer
        envia_valor_temporizador(-1);
        break;
     
     default:
        break;
     }
     sleep(0.5);
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