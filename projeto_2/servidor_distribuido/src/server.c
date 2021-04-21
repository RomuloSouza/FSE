#include <server.h>
#include <client.h>
#include <gpio.h>

int servidorSocket;
int socketCliente;

void TrataClienteTCP() {
	char buffer[MAX_BUFFER_SIZE];
	int received_length;

    /*
    Characters and responsabilities

    I -> ask for initial state of the system
	T -> toggle switch
    */

	if((received_length = recv(socketCliente, buffer, MAX_BUFFER_SIZE-1, 0)) < 0){
		printf("Error in recv()\n");
        return;
    }

    printf("Received message from central server: %s\n", buffer);

    switch(buffer[0]){
	case 'I':
		printf("Sending initial state!\n");
		break;
	case 'T':
		switch (buffer[1]){
		case '1':
			printf("Toogle lamp_1\n");
			toggle_switch(LAMP_1);
			break;
		case '2':
			printf("Toogle lamp_2\n");
			toggle_switch(LAMP_2);
			break;
		case '3':
			printf("Toogle lamp_3\n");
			toggle_switch(LAMP_3);
			break;
		case '4':
			printf("Toogle lamp_4\n");
			toggle_switch(LAMP_4);
			break;
		case '5':
			printf("Toogle air_1\n");
			toggle_switch(AIR_1);
			break;
		case '6':
			printf("Toogle air_2\n");
			toggle_switch(AIR_2);
			break;

		default:
			printf("Error when mapping TOGGLE to responsabilty\n");
		}
		break;
	default:
		printf("Error when mapping BUFFER to responsabilty\n");
		return;
    }

    char response[MAX_BUFFER_SIZE];
    serialize_states(response);
	printf("Sent: %s\n", response);

	send_message(response);
}

void create_server(){
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
		
		TrataClienteTCP();
		close(socketCliente);
	}
}

void stop_server(){
	close(socketCliente);
	close(servidorSocket);
}
