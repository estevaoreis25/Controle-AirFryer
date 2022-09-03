# Controle-AirFryer
Este repositório, consiste no trabalho 2 de Fundamentos de Sistemas Embarcados, onde é simulado o controle de uma Airfryer.

## Como Executar a aplicação
Para executar a aplicação é necessário que tenha instalado a o compilador `gcc` e o `make` na placa que for utilizada.

* Faça o download deste repositório em uma das Raspberrys (43 ou 44)
* Execute o arquivo `run.sh` na raiz do projeto

      $ sh run.sh

* Alternativamente voce pode executar os seguintes comandos nessa ordem:
    
      $ make clean
      $ make
      $ make run

* Após esses comandos, o programa estará em funcionamento
* Voce pode interagir com o programa atraves de uma das dashboards, dependendo de qual Raspberry esteja utilizando

  * [Dashboard Rasp43](http://164.41.98.25:443/dashboard/657fba30-2706-11ed-be92-e3a443145aec?publicId=ba042a80-0322-11ed-9f25-414fbaf2b065)
  * [Dashboard Rasp44](http://164.41.98.25:443/dashboard/a4b10e40-1d8b-11ed-a520-7b07ee36c1c6?publicId=ba042a80-0322-11ed-9f25-414fbaf2b065)

## Como Utilizar a aplicação
Existe as seguintes funcionalidades na aplicação:
* Ligar Airfryer
* Desligar Airfryer
* Iniciar Cozimento
* Para Cozimento
* Incrementar tempo de 1 em 1 minuto
* Decrementar tempo de 1 em 1 minuto
* Incrementar temperatura de 5 em 5 graus
* Decrementar temperatura de 5 em 5 graus
* Menu de presets de cozimentos de alimentos

Algumas regras de uso:
* Antes de qualquer coisa é preciso ligar a Airfrey. Se ela nao estiver ligada, os outros botões estarão inativos
* Para selecionar o alimento no modo preset é preciso ir clicando no botão `Menu` até encontrar o alimento que deseja. As informações de cada alimento será mostrada no display. Quando escolher o alimento desejado clique em `Iniciar`. Caso não queira nenhum dos alimentos continue clicando até voltar ao estado inicial da airfrey
* O presets de alimentos posui 7 configurações: `FRANGO`, `PEIXE`, `CARNE`, `ARROZ`, `PIPOCA`, `VEGETAIS` e `MENU INICIAL`

## Testes e Gráficos
Antes de iniciar os é importante mostrar o modo de menu de alimentos. Para os 3 testes será usado a configuração para aquecer um `FRANGO`. Como apresentado na imagem, a configuração consiste basicamente no tempo que é de 2 minutos.

![1](/imgs/01-%20adicionando_modo.png)
### Primeiro Teste - PRE-AQUECIMENTO
No primeiro teste vemos o comportamento da aplicação no momento de `PRE-AQUECIMENTO`.

![2](/imgs/02-%20Iniciando%20cozimento.png)

A temperatura interna está subindo até chegar à temperatura de referência selecionada. Enquanto isso o tempo permanece parado.

![3](/imgs/03%20-%20Grafico%20de%20cozimento%20-%20Iniciando.png)

## Segundo Teste - COZIMENTO
Após o a temperatura interna alcançar a temperatura de referência, o tempo começa a ser contado. 
![4](/imgs/04-%20Inicio%20de%20cozimento.png)

É possível ver que a temperatura interna está se mantendo alinhada com a temperatura de referência.

![5](/imgs/05-%20Estabilizando.png)

O tempo também está diminuindo

![6](/imgs/06-%20Tempo%20finalizando.png)

### Terceiro Teste - RESFRIAMENTO

Após o tempo de cozimento ser encerrado começa o momento de `RESFRIAMENTO`. É possível ver no gráfico que a temperatura está diminuindo.

![7](/imgs/07-%20Resfriando.png)

Nesse momento a temperatura de referência se torna a temperatura ambiente. A temperatura vai resfriando até chegar no momento em que as temperaturas se aproximem suficientemente para que a Airfrey possa ser desligada.

![8](/imgs/08-%20Resfriando.png)

### Quarto Teste - AIRFRYER PARADA

Nas imagens abaixo é demonstrado o comportamento da aplicação quando o botão `PARAR` é clicado. Tanto o processo de aquecimento quando de resfriamento são interrompidos.

![9](/imgs/09-%20Parando%20Airfryer.png)

![10](/imgs/10-%20Parando%20Airfryer%20LCD.png)


