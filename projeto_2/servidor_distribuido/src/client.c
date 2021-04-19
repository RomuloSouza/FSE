#include <client.h>

int clienteSocket;

void open_socket(){
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
    int bytes_recieved;

    if(send(clienteSocket, buffer, buffer_size, 0) != buffer_size){
		printf("Error sending message: number of bytes sent is different of the buffer message\n");
        return;
    }

    char response[MAX_BUFFER_SIZE];
    if((bytes_recieved = recv(clienteSocket, response, MAX_BUFFER_SIZE-1, 0)) <= 0)
        printf("Did not receive the bytes sent\n");

    response[bytes_recieved] = '\n';
    printf("Response from central server: %s\n", response);
}

void close_socket(){
    close(clienteSocket);
}

// int main() {
// 	int clienteSocket;
// 	struct sockaddr_in servidorAddr;
// 	unsigned short servidorPorta;
// 	char *IP_Servidor;
// 	char *mensagem;
// 	char buffer[16];
// 	unsigned int tamanhoMensagem;

// 	int bytesRecebidos;
// 	int totalBytesRecebidos;

// 	IP_Servidor = argv[1];
// 	servidorPorta = atoi(argv[2]);
// 	mensagem = argv[3];

// 	// Criar Socket
// 	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
// 		printf("Erro no socket()\n");

// 	// Construir struct sockaddr_in
// 	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
// 	servidorAddr.sin_family = AF_INET;
// 	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
// 	servidorAddr.sin_port = htons(servidorPorta);

// 	// Connect
// 	if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, 
// 							sizeof(servidorAddr)) < 0)
// 		printf("Erro no connect()\n");

// 	tamanhoMensagem = strlen(mensagem);

// 	if(send(clienteSocket, mensagem, tamanhoMensagem, 0) != tamanhoMensagem)
// 		printf("Erro no envio: numero de bytes enviados diferente do esperado\n");

// 	totalBytesRecebidos = 0;
// 	while(totalBytesRecebidos < tamanhoMensagem) {
// 		if((bytesRecebidos = recv(clienteSocket, buffer, 16-1, 0)) <= 0)
// 			printf("Não recebeu o total de bytes enviados\n");
// 		totalBytesRecebidos += bytesRecebidos;
// 		buffer[bytesRecebidos] = '\0';
// 		printf("%s\n", buffer);
// 	}
// 	close(clienteSocket);
// 	exit(0);
// }
