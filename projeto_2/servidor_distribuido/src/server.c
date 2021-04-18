#include <server.h>

int servidorSocket;


void TrataClienteTCP(int socketCliente) {
	char buffer[16];
	int tamanhoRecebido;

	if((tamanhoRecebido = recv(socketCliente, buffer, 16, 0)) < 0)
		printf("Erro no recv()\n");

	while (tamanhoRecebido > 0) {
		if(send(socketCliente, buffer, tamanhoRecebido, 0) != tamanhoRecebido)
			printf("Erro no envio - send()\n");
		
		if((tamanhoRecebido = recv(socketCliente, buffer, 16, 0)) < 0)
			printf("Erro no recv()\n");
	}
}


void create_server(){
	int socketCliente;
	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned short servidorPorta;
	unsigned int clienteLength;

	servidorPorta = SERVER_PORT;

	// Abrir Socket
	if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("falha no socker do Servidor\n");

    // Montar a estrutura sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(servidorPorta);

	// Bind
	if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0)
		printf("Falha no Bind\n");

	// Listen
	if(listen(servidorSocket, 10) < 0)
		printf("Falha no Listen\n");		

	while(1) {
		clienteLength = sizeof(clienteAddr);

		if((socketCliente = accept(servidorSocket, 
			                      (struct sockaddr *) &clienteAddr, 
			                      &clienteLength), SOCK_CLOEXEC) < 0)
			printf("Falha no Accept\n");
		
		printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
		
		TrataClienteTCP(socketCliente);
		close(socketCliente);
	}
}

void stop_server(){
	close(servidorSocket);
}
