# Projeto 2 - Fundamentos de Sistemas Embarcados

## Dados do Aluno:

* **Nome:** Rômulo Vinícius de Souza
* **Matrícula:** 15/0147601

## Introdução

Este trabalho tem por objetivo a criação de um sistema distribuído de automação residencial para monitoramento e acionamento de sensores e dispositivos. O sistema deve ser desenvolvido para funcionar em um conjunto de placas Raspberry Pi, com um servidor central responsável pelo controle e interface com o usuário e servidores distribuídos para leitura e acionamento dos dispositivos.

## Dependências

### Servidor Distribuído

* [WiringPi](http://wiringpi.com/download-and-install/)

### Servidor Central

* Asyncio
* Prompt-toolkit

## Executando o projeto

### Servidor Central (Primeiro a ser inicializado)

```bash
# Clone o repositório
$ git clone https://github.com/RomuloSouza/FSE

# Vá para o diretório do servidor central do projeto
$ cd FSE/projeto_2/servidor_central

# Instale as dependências (recomenda-se o uso de um ambiente virtual)
$ pip3 install -r requirements.txt

# Execute
$ python3 main.py
```


### Servidor Distribuído

```bash
# Com o repositório clonad, vá para o diretório do servidor distribuído do projeto
$ cd FSE/projeto_2/servidor_distribuido

# Faça o build o projeto
$ make

# Execute
$ make run
```

## Parando a Execução de Forma segura

* Pressione **q** no terminal do servidor central para fechar o menu e mantenha a aplicação
* Pare a execução no servidor distribuído (ctrl+c)

## Funcionamento do Menu

foto aqui

### Legendas

* Verde: dispositivo conectado/ativo
* Vermelho: dispositivo desconectado/inativo

### Comandos

* **q**: fechar o menu
* **setas para cima/baixo**: selecionar dispositivo no menu
* **enter**: selecionar dispositivos a terem o estado alterado
* **s**: envia os dispositivos que devem ter seus estados alterados para o servidor distribuido (é possível ter mais de um dispositivo selecionado)