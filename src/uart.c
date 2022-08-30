#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h> 
#include <string.h>   
#include "crc16.h" 

int uart0_filestream = -1;
unsigned char tx_buffer[20];
unsigned char *p_tx_buffer;

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
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);
}

float solicita_temperatura_interna(){
  if(inicia_UART() == 0){
    printf("Falha na Conexão da UART");
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
}

