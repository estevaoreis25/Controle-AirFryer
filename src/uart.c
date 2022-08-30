#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h> 
#include <string.h>   
#include "crc16.h" 

int uart0_filestream = -1;
unsigned char tx_buffer[20];
unsigned char *p_tx_buffer;
unsigned char rx_buffer[256];
int rx_length;

int inicia_UART(){
  uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
  if (uart0_filestream == -1)
  {
      printf("Erro - Não foi possível iniciar a UART.\n");
      return 0;
  }
  else
  {
      printf("UART inicializada!\n");
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
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
            sleep(1);
            return 0;
        }
        else
        {
            printf("escrito.\n");
            sleep(1);
        }
    }
    return 1;
}

int le_da_UART(){
  // Read up to 255 characters from the port if they are there
    rx_length = read(uart0_filestream, (void*)rx_buffer, sizeof(rx_buffer));      //Filestream, buffer to store in, number of bytes to read (max)
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
}

int verifica_CRC(){
  short crc_enviado;
  memcpy(&crc_enviado, &rx_buffer[7], sizeof(short));
  short crc_calculado = calcula_CRC(rx_buffer, 7);
  if(crc_enviado == crc_calculado){
      printf("CRC Validado\n CRC Enviado: %i\n CRC Calculado: %i\n", crc_enviado, crc_calculado);
      return 1;
  }else{
      printf("CRC Invalido\n CRC Enviado: %i\n CRC Calculado: %i\n", crc_enviado, crc_calculado);
      return 0;
  }
}

void fecha_conexao_UART(){
  close(uart0_filestream);
}

float solicita_temperatura_interna(){
  if(inicia_UART() == 0){
    printf("Falha na Conexão da UART\n");
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

  printf("Buffers de memória criados!\n");

  if(escreve_na_UART()==0){
    printf("Falha na escrita da UART\n");
    return 0;
  }
  if(le_da_UART()==0){
    printf("Falha na leitura da UART\n");
    return 0;
  } else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      float result;
      memcpy(&result, &rx_buffer[3], sizeof(result));
      printf("%i Bytes lidos : %f\n", rx_length, result);
      
      if(verifica_CRC == 0){
        printf("CRC Inválido");
        // Voltar a um loop e refazer toda a requisição
      }

    }
    fecha_conexao_UART();
}