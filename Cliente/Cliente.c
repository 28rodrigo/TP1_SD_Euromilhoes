#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable : 4996)

void getIp(char* ip_addr, int* port)
{
	puts("Ola bem-vindo ! \n");
	puts("Indique o endereco IP do Servidor:");
	scanf("%s", ip_addr);
	puts("Indique a porta:");
	scanf("%d", port);
}
//menu
void menuDisplay() {
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	puts("\t|               Escolha uma opcao do menu :                |");
	puts("\t| -------------------------------------------------------- |");
	puts("\t| -> 1 - Obter uma chave do euromilhoes.                   |");
	puts("\t| -> 2 - Obter mais que uma chave do euromilhoes.          |");
	puts("\t| -> 3 - Obter chaves do Euromilhoes ja atribuidas.        |");
	puts("\t| -> 4 - Ajuda.                                            |");
	puts("\t| -> 5 - Sair do Programa.                                 |");
	puts("\t|__________________________________________________________|\n");
	//Opções de admin
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	puts("\t|                      Opcoes de Admin :                   |");
	puts("\t| -------------------------------------------------------- |");
	puts("\t| -> 6 - Encerrar a App e o Server                         |");
	puts("\t| -> 7 - Apagar historico de chaves                        |");
	puts("\t|__________________________________________________________|\n");
}

void helpFunc() {
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	puts("\t|             Opcoes de comandos disponiveis:              |");
	puts("\t| -------------------------------------------------------- |");
	puts("\t| -> 1 - Obter uma chave do euromilhoes.                   |");
	puts("\t| -> 2 - Obter mais que uma chave do euromilhoes.          |");
	puts("\t| -> 3 - Obter chaves do Euromilhões já atribuidas.        |");
	puts("\t| -> 4 - Ajuda.                                            |");
	puts("\t| -> 5 - Sair do Programa.                                 |");
	puts("\t| -> 6 - Encerrar a App e o Server                         |");
	puts("\t| -> 7 - Apagar historico de chaves                        |");
	puts("\t|__________________________________________________________|\n");
}

//scan repetitivo
void menuOption(char*option) {
	scanf("%c", option);
}

// dados a enviar para o server!!
void dataToSend(char option,char* message) {
	int opcao = atoi(&option);
	int teste = 0;
	for (int i = 1; i < 8; i++)
	{ 
		if (opcao == i) 
		{
			teste = 1;
		} 
	}
	if (teste == 1)
	{
		if (opcao == 1)
		{
			strcpy(message, "chave");
		}
		else if (opcao == 2)
		{
			char chave[12] = "chave";
			char opt[2] = "";
			puts("Quantas chaves quer? (2-99)");
			scanf("%s", opt);
			strcat(chave, opt);
			strcpy(message, chave);
		}
		else if (opcao == 3)
		{
			strcpy(message, "hist");
			puts("\nHistorico de chaves por ordem de saída=> 5 Numeros + 2 Estrelas:\n");
		}
		else if (opcao == 4)
		{
			helpFunc();
			strcpy(message, " ");
		}
		else if (opcao == 5)
		{
			strcpy(message, "quit");
		}
		else if (opcao == 6)
		{
			strcpy(message, "quits");
		}
		else if (opcao == 7)
		{
			strcpy(message, "delete");
		}
		else {
			puts("Opcao invalida!");
			menuDisplay();
		}
	}
}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char message[1000]=" ";
	char server_reply[4000];
	int recv_size;
	int ws_result;
	int port=0;
	char ip_addr[20];
	char option;

	getIp(ip_addr,&port);

	//puts(ip_addr);
	//printf("%d",port);

	

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
		printf("Não foi possível criar o Socket: %d", WSAGetLastError());
		return 1;
	}

	//printf("Socket created.\n");

	// create the socket  address (ip address and port)
	server.sin_addr.s_addr = inet_addr(ip_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	ws_result = connect(s, (struct sockaddr*)&server, sizeof(server));
	if (ws_result < 0)
	{
		puts("Erro de Conexão!");
		return 1;
	}

	puts("Conexao com o servidor efetuada com sucesso!");
	 
	//quando conectados temos que receber mensagem do servidor antes de enviar...
	recv_size = recv(s, server_reply, 4000, 0);
	if (recv_size == SOCKET_ERROR)
	{
		puts("recv failed");
	}

	server_reply[recv_size - 1] = '\0';
	puts(server_reply);
	//Menu
	menuDisplay();
	while (1)
	{	
		if (strcmp(message, "quit") == 0)
		{
			break;
		}
		else if (strcmp(message, "quits") == 0)
		{
			break;
		}
		menuOption(&option);
		//Send some data
		dataToSend(option, message);
		
		ws_result = send(s, message, strlen(message), 0);
		
		if (ws_result < 0)
		{
			puts("Send failed");
			return 1;
		}
		//puts("Data Sent\n");

		//Receive a reply from the server
		
		//verificar se tamanho de envio do servidor corresponde ao tamanho que o cliente recebe!!!
		recv_size = recv(s, server_reply, 2000, 0);
		if (recv_size == SOCKET_ERROR)
		{
			puts("recv failed");
		}

		//puts("Reply received\n");

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size - 1] = '\0';
		puts(server_reply);
		if (strcmp(server_reply, "\nBye Client...\n") == 0)
		{
			break;
		}	
	}

	// Close the socket
	closesocket(s);

	//Cleanup winsock
	WSACleanup();

	return 0;
}
