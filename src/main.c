#include <stdio.h>
#include "uart.h"

int menu();

int main(){
  int opcao;
    opcao = menu();
    while(opcao){
        switch(opcao){
        case 1:
            solicita_temperatura_interna();
            break;
        case 2:
            solicita_temperatura_referencia();
            break;
        case 3:
            le_comandos_usuario();
            break;
        case 4:
            //enviar_int();
            break;
        case 5:
            //enviar_float();
            break;
        case 6:
            //enviar_string();
            break;
        case 0:
            printf ("SAINDO\n");
            break;
        default :
            printf ("Valor invalido!\n");
        }
     opcao = menu();
    }
   return 0;
}

int menu(){
  printf("-------------Opções-----------\n");
  printf("1. Solicitar Temperatura Interna\n");
  printf("2. Solicitar Temperatura Referencia\n");
  printf("3. Le comandos Usuario\n");
  printf("4. Enviar um INT\n");
  printf("5. Enviar um FLOAT\n");
  printf("6. Enviar uma STRING\n");
  printf("0. SAIR\n");
  printf("\n");
  printf("Digite o número da Opção desejada\n");
  int opcao;
  scanf("%d", &opcao);
  return opcao;
}