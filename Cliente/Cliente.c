#include<stdio.h>
#include<winsock2.h>
#include<conio.h>

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

void textcolor(int color)
{
	static int __BACKGROUND;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


	GetConsoleScreenBufferInfo(h, &csbiInfo);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		color + (__BACKGROUND << 4));
}

//menu
void menuDisplay() {
	textcolor(14);
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	cprintf("\t|"); textcolor(9); cprintf("               Escolha uma opcao do menu :                "); textcolor(14); cprintf("|\n");
	puts("\t| -------------------------------------------------------- |");
	cprintf("\t|"); textcolor(13); cprintf(" -> 1 - Obter uma chave do euromilhoes.                   "); textcolor(14); cprintf("|\n");
	cprintf("\t|"); textcolor(13); cprintf(" -> 2 - Obter mais que uma chave do euromilhoes.          "); textcolor(14); cprintf("|\n");
	cprintf("\t|"); textcolor(13); cprintf(" -> 3 - Obter chaves do Euromilhoes ja atribuidas.        "); textcolor(14); cprintf("|\n");
	cprintf("\t|"); textcolor(13); cprintf(" -> 4 - Ajuda.                                            "); textcolor(14); cprintf("|\n");
	cprintf("\t|"); textcolor(13); cprintf(" -> 5 - Sair do Programa.                                 "); textcolor(14); cprintf("|\n");
	textcolor(14);
	puts("\t|__________________________________________________________|\n");
	//Op��es de admin
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	cprintf("\t|"); textcolor(9); cprintf("                    Opcoes de Admin :                     "); textcolor(14); cprintf("|\n");
	puts("\t| -------------------------------------------------------- |");
	cprintf("\t|"); textcolor(13); cprintf(" -> 6 - Encerrar a App e o Server                         "); textcolor(14); cprintf("|\n");
	cprintf("\t|"); textcolor(13); cprintf(" -> 7 - Apagar historico de chaves                        "); textcolor(14); cprintf("|\n");
	puts("\t|__________________________________________________________|\n");
	textcolor(15);
}

void helpFunc() {
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	puts("\t|             Opcoes de comandos disponiveis:              |");
	puts("\t| -------------------------------------------------------- |");
	puts("\t| -> 1 - Obter uma chave do euromilhoes.                   |");
	puts("\t| -> 2 - Obter mais que uma chave do euromilhoes.          |");
	puts("\t| -> 3 - Obter chaves do Euromilh�es j� atribuidas.        |");
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
			char chave[12] = "chave_";
			char opt[2] = "";
			puts("Quantas chaves quer? (2-99)");
			scanf("%s", opt);
			strcat(chave, opt);
			strcpy(message, chave);
		}
		else if (opcao == 3)
		{
			strcpy(message, "hist");
			puts("\nHistorico de chaves por ordem de sa�da=> 5 Numeros + 2 Estrelas:\n");
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
		printf("N�o foi poss�vel criar o Socket: %d", WSAGetLastError());
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
		puts("Erro de Conex�o!");
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
