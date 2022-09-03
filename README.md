# Controle-AirFryer
Este repositório, consiste no trabalho 2 de Fundamentos de Sistemas Embarcados, onde é simulado o controle de uma Airfryer.

## Como Executar a aplicação
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
* Antes de qualquer coisa é preciso ligar a Airfrey. Se ela nao estiver ligada, os outros botões estaram inativos
* Para selecionar o alimento no modo preset é preciso ir clicando no botão `Menu` até encontrar o alimento que deseja. As informações de cada alimento será mostrada no display. Quando escolher o alimento desejado clique em `Iniciar`. Caso não queira nenhum dos alimentos continue clicando até voltar ao estado inicial da airfrey
* O presets de alimentos posui 7 configurações: `FRANGO`, `PEIXE`, `CARNE`, `ARROZ`, `PIPOCA`, `VEGETAIS` e `MENU INICIAL`