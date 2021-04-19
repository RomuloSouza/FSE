#include <server.h>
#include <gpio.h>

int servidorSocket;


void TrataClienteTCP(int socketCliente) {
	char buffer[BUFFER_SIZE];
	int received_length;

    /*
    Characters and responsabilities

    E -> ask for enviroment temperature and humidity
    I -> ask for initial state of the system
    S -> ask for state of switches (lamps and airs)
    */

	if((received_length = recv(socketCliente, buffer, BUFFER_SIZE, 0)) < 0){
		printf("Error in recv()\n");
        return;
    }

    printf("o que eu recebi no buffer: %s\n", buffer);

    switch(buffer[0]){
        case 'E':
            printf("Showing temperature vrau\n");
            break;
        case 'I':
            printf("Showing initial state vrau\n");
            break;
        case 'S':
            printf("Showing state of switches vrau\n");
            break;
        default:
            printf("Error when mapping buffer to responsabilty\n");
            return;
    }

    char response[BUFFER_SIZE];
    int size = serialize_states(response);
    printf("size returned = %d\n", size);
    printf("sizeof(response) = %d\n", sizeof(response));
    if(send(socketCliente, response, sizeof(response), 0) != sizeof(response))
        printf("Erro no envio - send()\n");

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
