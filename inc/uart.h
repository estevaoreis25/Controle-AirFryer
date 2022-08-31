#ifndef UART_H_
#define UART_H_

int inicia_UART();
int escreve_na_UART();
int le_da_UART();
int verifica_CRC();
void reinicializa_variaveis();
void fecha_conexao_UART();
float solicita_temperatura_interna();
float solicita_temperatura_referencia();
int le_comandos_usuario();

#endif /* UART_H_ */
