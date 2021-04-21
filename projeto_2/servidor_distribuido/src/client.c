#include <client.h>

int clienteSocket;

void open_client_socket(){
    const char HOST_CENTRAL[] = {"192.168.0.53"};
    const int PORT_CENTRAL = 10009;

	struct sockaddr_in servidorAddr;

    // Criar Socket
	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		printf("Erro creating socket!\n");
        return;
    }

    // Construir struct sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(HOST_CENTRAL);
	servidorAddr.sin_port = htons(PORT_CENTRAL);

    if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
		printf("Error connecting client socket!\n");
        return;
    }

    printf("Successfully connected on central server.\n");
}

void send_message(char *buffer){
    int buffer_size = strlen(buffer);

    if(send(clienteSocket, buffer, buffer_size, 0) != buffer_size){
		printf("Error sending message: number of bytes sent is different of the buffer message\n");
        return;
    }
}

void close_client_socket(){
    close(clienteSocket);
}
