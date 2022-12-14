#ifndef UART_H_
#define UART_H_

#define TAMANHO_TX 20
#define TAMANHO_RX 256

int inicia_UART();
int escreve_na_UART();
int le_da_UART();
int verifica_CRC();
void reinicializa_variaveis();
void fecha_conexao_UART();
float solicita_temperatura_interna();
float solicita_temperatura_referencia();
int le_comandos_usuario();
void envia_sinal_controle(int sinal_controle);
void envia_sinal_referencia(float sinal_referencia);
int envia_estado_sistema(int sinal_usuario);
int envia_estado_funcionamento(int sinal_usuario);
int envia_valor_temporizador(int tempo);

#endif /* UART_H_ */
