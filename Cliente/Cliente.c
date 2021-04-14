/*
	Simple winsock client
*/

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable : 4996)

void getIp(char* ip_addr, int* port)
{
	puts("Olá bem-vindo ! \n");
	puts("Indique o endereço IP do Servidor:");
	scanf("%s", ip_addr);
	puts("Indique a porta:");
	scanf("%d", port);

}
//menu
void menuDisplay(int*option) {
	puts("Escolha uma opção do menu :");
	puts("1 - Obter uma chave do euromilhões.");
	puts("2 - Obter mais que uma chave do euromilhões.");
	puts("3 - Obter chaves do Euromilhões já atribuidas.");
	puts("4 - Ajuda.");
	scanf("%d", option);
	
	//comandos de resetar servidor secreto!!
}

// dados a enviar para o server!!
void dataToSend(int opcao,char* message) {
	if (opcao == 1)
	{
		strcpy(message, "chave");
	}
	else {
		strcpy(message, "bye");
	}
	

}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char message[1000];
	char server_reply[4000];
	int recv_size;
	int ws_result;
	int port=0;
	char ip_addr[20];
	int option;

	getIp(ip_addr,&port);

	puts(ip_addr);
	printf("%d",port);

	

	// Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		return 1;
	}

	printf("Socket created.\n");

	// create the socket  address (ip address and port)
	server.sin_addr.s_addr = inet_addr(ip_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	ws_result = connect(s, (struct sockaddr*)&server, sizeof(server));
	if (ws_result < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Está conectado com o servidor!");
	 
	//quando conectados temos que receber mensagem do servidor antes de enviar...
	recv_size = recv(s, server_reply, 4000, 0);
	if (recv_size == SOCKET_ERROR)
	{
		puts("recv failed");
	}

	server_reply[recv_size - 1] = '\0';
	puts(server_reply);

	while (1)
	{	
	
		//Menu
		menuDisplay(&option);
		//Send some data
		dataToSend(option, message);
		
		ws_result = send(s, message, strlen(message), 0);
		
		if (ws_result < 0)
		{
			puts("Send failed");
			return 1;
		}
		puts("Data Sent\n");

		//Receive a reply from the server
		
		//verificar se tamanho de envio do servidor corresponde ao tamanho que o cliente recebe!!!
		recv_size = recv(s, server_reply, 2000, 0);
		if (recv_size == SOCKET_ERROR)
		{
			puts("recv failed");
		}

		puts("Reply received\n");

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size - 1] = '\0';
		puts(server_reply);


		if (strcmp(server_reply, "\nBye Client...\n") == 0)
			break;
		
	}


	// Close the socket
	closesocket(s);

	//Cleanup winsock
	WSACleanup();

	return 0;
}
