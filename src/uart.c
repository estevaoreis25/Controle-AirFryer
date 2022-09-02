#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h> 
#include <string.h>   
#include "crc16.h" 
#include "uart.h"

int uart0_filestream = -1;
unsigned char tx_buffer[TAMANHO_TX];
unsigned char *p_tx_buffer;
unsigned char rx_buffer[TAMANHO_RX];
int rx_length;
int tempo_airfrey = 0;

int inicia_UART(){
  uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
  if (uart0_filestream == -1)
  {
      printf("Erro - Não foi possível iniciar a UART.\n");
      return 0;
  }
  else
  {
      //printf("UART inicializada!\n");
  }    
  struct termios options;
  tcgetattr(uart0_filestream, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_filestream, TCIFLUSH);
  tcsetattr(uart0_filestream, TCSANOW, &options);
  return 1;
}

int escreve_na_UART(){
    if (uart0_filestream != -1)
    {
        //printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
            sleep(1);
            return 0;
        }
        else
        {
            //printf("escrito.\n");
            sleep(1);
        }
    }
    return 1;
}



int le_da_UART(){
    rx_length = read(uart0_filestream, (void*)rx_buffer, sizeof(rx_buffer));      //Filestream, buffer to store in, number 
    if (rx_length < 0)
    {
        printf("Erro na leitura.\n");
        return 0; //An error occured (will occur if there are no bytes)
    }
    else if (rx_length == 0)
    {
        printf("Nenhum dado disponível.\n"); //No data waiting
        return 0;
    }
    return 1;
}

int verifica_CRC(){
  short crc_enviado;
  memcpy(&crc_enviado, &rx_buffer[7], sizeof(short));
  short crc_calculado = calcula_CRC(rx_buffer, 7);
  if(crc_enviado == crc_calculado){
      //printf("CRC Validado\n CRC Enviado: %i\n CRC Calculado: %i\n", crc_enviado, crc_calculado);
      return 1;
  }else{
      printf("CRC Invalido\n CRC Enviado: %i\n CRC Calculado: %i\n", crc_enviado, crc_calculado);
      return 0;
  }
}

void reinicializa_variaveis(){
  memset(tx_buffer, 0, TAMANHO_TX);
  memset(rx_buffer, 0, TAMANHO_RX);
}

void fecha_conexao_UART(){
  close(uart0_filestream);
  reinicializa_variaveis();
}

float solicita_temperatura_interna(){
  float result;
  if(inicia_UART() == 0){
    //printf("Falha na Conexão da UART\n");
    return 0;
  }

  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x23;
  *p_tx_buffer++ = 0xC1;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;
  short resposta = calcula_CRC(tx_buffer, 7);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);

  if(escreve_na_UART()==0){
    //printf("Falha na escrita da UART\n");
    return 0;
  }
  if(le_da_UART()==0){
    //printf("Falha na leitura da UART\n");
    return 0;
  } else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&result, &rx_buffer[3], sizeof(result));
      printf("Valor da Temperatura interna: %f\n", result);
      
      if(verifica_CRC() == 0){
        //printf("CRC Inválido");
        fecha_conexao_UART();
        return -1;

      }
    }
    fecha_conexao_UART();
    return result;
}

float solicita_temperatura_referencia(){
  float result;
  if(inicia_UART() == 0){
    //printf("Falha na Conexão da UART\n");
    return 0;
  }

  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x23;
  *p_tx_buffer++ = 0xC2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;

  short resposta = calcula_CRC(tx_buffer, 7);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);

  if(escreve_na_UART()==0){
    //printf("Falha na escrita da UART\n");
    return 0;
  }
  if(le_da_UART()==0){
    //printf("Falha na leitura da UART\n");
    return 0;
  } else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&result, &rx_buffer[3], sizeof(result));
      printf("Valor da Temperatura de Referencia: %f\n", result);
      
      if(verifica_CRC() == 0){
        printf("CRC Inválido");
        fecha_conexao_UART();
        return -1;

      }

    }
    printf("\n");
    fecha_conexao_UART();
    return result;
}

int le_comandos_usuario(){
  int result;
  if(inicia_UART() == 0){
    //printf("Falha na Conexão da UART\n");
    return 0;
  }

  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x23;
  *p_tx_buffer++ = 0xC3;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;
  short resposta = calcula_CRC(tx_buffer, 7);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);


  if(escreve_na_UART()==0){
    //printf("Falha na escrita da UART\n");
    return 0;
  }
  if(le_da_UART()==0){
    //printf("Falha na leitura da UART\n");
    return 0;
  } else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&result, &rx_buffer[3], sizeof(result));
      //printf("Comando do usuario: %d\n", result);
      
      if(verifica_CRC() == 0){
        //printf("CRC Inválido");
        fecha_conexao_UART();
        return -1;

      }

    }
    printf("\n");
    fecha_conexao_UART();
    return result;
}

void envia_sinal_controle(){
  if(inicia_UART() == 0){
    //printf("Falha na Conexão da UART\n");
  }
  int sinal_controle = -20;
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x16;
  *p_tx_buffer++ = 0xD1;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;

  memcpy(p_tx_buffer, &sinal_controle, sizeof(int));
  p_tx_buffer+=sizeof(int);

  short resposta = calcula_CRC(tx_buffer, 11);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);


  if(escreve_na_UART()==0){
    //printf("Falha na escrita da UART\n");
  }
}

void envia_sinal_referencia(){
  if(inicia_UART() == 0){
    //printf("Falha na Conexão da UART\n");
    fecha_conexao_UART();
  }
  float sinal_referencia;
  printf("Digite o numero da temperatura de referencia que deseja entre 0 a 90\n");
  scanf("%f", &sinal_referencia);
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x16;
  *p_tx_buffer++ = 0xD2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;

  memcpy(p_tx_buffer, &sinal_referencia, sizeof(sinal_referencia));
  p_tx_buffer+=sizeof(sinal_referencia);

  short resposta = calcula_CRC(tx_buffer, 11);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);


  if(escreve_na_UART()==0){
    printf("Falha na escrita da UART\n");
    fecha_conexao_UART();
  }
}

int envia_estado_sistema(int sinal_usuario){
  int result;
  
  if(inicia_UART() == 0){
    printf("Falha na Conexão da UART\n");
    fecha_conexao_UART();
    return 0;
  }

  unsigned char estado_sistema;

  estado_sistema = sinal_usuario;

  printf("Estado: %d\n", sinal_usuario);

  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x16;
  *p_tx_buffer++ = 0xD3;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;

  memcpy(p_tx_buffer, &estado_sistema, sizeof(estado_sistema));
  p_tx_buffer+=sizeof(estado_sistema);

  short resposta = calcula_CRC(tx_buffer, 8);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);


  if(escreve_na_UART()==0){
    //printf("Falha na escrita da UART\n");
    fecha_conexao_UART();
    return 0;
  }
  if(le_da_UART()==0){
    //printf("Falha na leitura da UART\n");
    fecha_conexao_UART();
    return 0;
  } else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&result, &rx_buffer[3], sizeof(result));
      //printf("Estado do Sistema: %d\n", result);
      
      if(verifica_CRC() == 0){
        printf("CRC Inválido");
        fecha_conexao_UART();
        return -1;
      }
    }

    printf("\n");
    fecha_conexao_UART();
    return result;
}

int envia_estado_funcionamento(int sinal_usuario){
  float result;
  if(inicia_UART() == 0){
    //printf("Falha na Conexão da UART\n");
    fecha_conexao_UART();
    return 0;
  }

  unsigned char estado_funcionamento;

  estado_funcionamento = sinal_usuario;

  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x16;
  *p_tx_buffer++ = 0xD5;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;

  memcpy(p_tx_buffer, &estado_funcionamento, sizeof(estado_funcionamento));
  p_tx_buffer+=sizeof(estado_funcionamento);

  short resposta = calcula_CRC(tx_buffer, 8);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);

  if(escreve_na_UART()==0){
    //printf("Falha na escrita da UART\n");
    fecha_conexao_UART();
    return 0;
  }
  if(le_da_UART()==0){
    //printf("Falha na leitura da UART\n");
    fecha_conexao_UART();
    return 0;
  } else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&result, &rx_buffer[3], sizeof(result));
      //printf("Comando do usuario: %f\n", result);
      
      if(verifica_CRC() == 0){
        printf("CRC Inválido");
        fecha_conexao_UART();
        return -1;

      }

    }
    printf("\n");
    fecha_conexao_UART();
    return result;
}

int envia_valor_temporizador(int tempo){
  int result;
  if(inicia_UART() == 0){
    //printf("Falha na Conexão da UART\n");
    fecha_conexao_UART();
    return 0;
  }

  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 0x01;
  *p_tx_buffer++ = 0x16;
  *p_tx_buffer++ = 0xD6;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 6;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 6;

  if(tempo == -1 && tempo_airfrey <= 0){
    tempo_airfrey = 0;
  }else {
    tempo_airfrey+=tempo;
  }

  memcpy(p_tx_buffer, &tempo_airfrey, sizeof(tempo_airfrey));
  p_tx_buffer+=sizeof(tempo_airfrey);

  short resposta = calcula_CRC(tx_buffer, 11);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);

  if(escreve_na_UART()==0){
    //printf("Falha na escrita da UART\n");
    fecha_conexao_UART();
    return 0;
  }
  if(le_da_UART()==0){
    //printf("Falha na leitura da UART\n");
    fecha_conexao_UART();
    return 0;
  } else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&result, &rx_buffer[3], sizeof(result));
      printf("Valor do temporizador: %d\n", result);
      
      if(verifica_CRC() == 0){
        printf("CRC Inválido");
        fecha_conexao_UART();
        return -1;

      }

    }
    printf("\n");
    fecha_conexao_UART();
    return result;
}