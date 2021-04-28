#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h> //for text color
#include <windows.h>// for audio

#define MAX_SIZE 16100

#pragma comment(lib,"winmm.lib") //for audio

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable : 4996)

//Função que pede ao user o IP e porta para aceder ao servidor
void getIp(char* ip_addr, int* port)
{
	puts("Ola bem-vindo ! \n");
	puts("Indique o endereco IP do Servidor:");
	scanf("%s", ip_addr);
	puts("Indique a porta:");
	scanf("%d", port);
}

//Serve apenas para dar cor aos textos
void textcolor(int color)
{
	static int __BACKGROUND;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


	GetConsoleScreenBufferInfo(h, &csbiInfo);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		color + (__BACKGROUND << 4));
}

//Apresenta as opções do Menu
void menuDisplay() {
	Sleep(1000);
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
	//Opções de admin
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	cprintf("\t|"); textcolor(9); cprintf("                    Opcoes de Admin :                     "); textcolor(14); cprintf("|\n");
	puts("\t| -------------------------------------------------------- |");
	cprintf("\t|"); textcolor(13); cprintf(" -> 6 - Encerrar a App e o Server.                        "); textcolor(14); cprintf("|\n");
	cprintf("\t|"); textcolor(13); cprintf(" -> 7 - Apagar historico de chaves.                       "); textcolor(14); cprintf("|\n");
	puts("\t|__________________________________________________________|\n");
	textcolor(15);
}

//Apresenta o conteúdo da função ajuda
void helpFunc() {
	textcolor(1);
	puts("\t __________________________________________________________");
	puts("\t|                                                          |");
	puts("\t|             Opcoes de comandos disponiveis:              |");
	puts("\t| -------------------------------------------------------- |");
	puts("\t| -> 1 - Obter uma chave do euromilhoes.                   |");
	puts("\t| -> 2 - Obter mais que uma chave do euromilhoes.          |");
	puts("\t| -> 3 - Obter chaves do Euromilhoes ja atribuidas.        |");
	puts("\t| -> 4 - Ajuda.                                            |");
	puts("\t| -> 5 - Sair do Programa.                                 |");
	puts("\t| -> 6 - Encerrar a App e o Server.                        |");
	puts("\t| -> 7 - Apagar historico de chaves.                       |");
	puts("\t|__________________________________________________________|\n");
	textcolor(15);
}

//Função de dados a enviar para o server!!
void dataToSend(char* option,char* message) {
	if (option == '1')
	{
		PlaySound(TEXT("Click.wav"), NULL, SND_FILENAME || SND_ASYNC);
		strcpy(message, "chave");
	}
	else if (option == '2')
	{
		PlaySound(TEXT("Click.wav"), NULL, SND_FILENAME || SND_ASYNC);
		char chave[12] = "chave_";
		char opt[4] = "";
		int valido = 0;
		while (valido!=1)
		{
			textcolor(2);
			puts("Quantas chaves quer? (2-99)");
			scanf("%s", &opt);
			if ((atoi(opt) < 2) || (atoi(opt) > 99)) { puts("Intervalo Invalido!"); }
			else
			{
				valido = 1; 
				strcat(chave, opt);
				strcpy(message, chave);
			}
		}
		textcolor(15);
	}
	else if (option == '3')
	{
		PlaySound(TEXT("Click.wav"), NULL, SND_FILENAME || SND_ASYNC);
		textcolor(4);
		strcpy(message, "hist");
		puts("\nHistorico de chaves por ordem de saida=> 5 Numeros + 2 Estrelas:\n");
		textcolor(15);
	}
	else if (option == '4')
	{
		PlaySound(TEXT("Click.wav"), NULL, SND_FILENAME || SND_ASYNC);
		helpFunc();
		strcpy(message, " ");
	}
	else if (option == '5')
	{
		PlaySound(TEXT("Click.wav"), NULL, SND_FILENAME || SND_ASYNC);
		Sleep(1500);
		strcpy(message, "quit");
	}
	else if (option == '6')
	{
		PlaySound(TEXT("Click.wav"), NULL, SND_FILENAME || SND_ASYNC);
		Sleep(1500);
		strcpy(message, "quits");
	}
	else if (option == '7')
	{
		PlaySound(TEXT("Error.wav"), NULL, SND_FILENAME || SND_ASYNC);
		strcpy(message, "delete");
	}
	else {
		PlaySound(TEXT("Error.wav"), NULL, SND_FILENAME || SND_ASYNC);
		puts("Opcao invalida!");
		menuDisplay();
	}
}

void printData(char str[],int size)
{

	char data[20];
	char hora[20];
	char chaves_atribuidas[10];
	char numeros[50];
	char estrelas[20];
	char delim[] = ";";
	int i = 1;
	//função acedida
	char* ptr = strtok(str, delim);
	if (strcmp(ptr, "1") == 0) //1 chave atribuida
	{
		ptr = strtok(NULL, delim);
		strcpy(data, ptr);
		ptr = strtok(NULL, delim);
		strcpy(hora, ptr);
		ptr = strtok(NULL, delim);
		strcpy(chaves_atribuidas, ptr);
		ptr = strtok(NULL, delim);
		strcpy(numeros, ptr);
		ptr = strtok(NULL, delim);
		strcpy(estrelas, ptr);

		printf("Chave atribuida no dia %s %s \n", data, hora);
		printf("Numero de chaves já atribuidas: %s\n\n", chaves_atribuidas);
		printf("Chave 1=> Numeros: %s Estrelas: %s\n", numeros, estrelas);
	}
	else if (strcmp(ptr, "2")==0) //multiplas chaves
	{
		ptr = strtok(NULL, delim);
		strcpy(data, ptr);
		ptr = strtok(NULL, delim);
		strcpy(hora, ptr);
		ptr = strtok(NULL, delim);
		strcpy(chaves_atribuidas, ptr);
		ptr = strtok(NULL, delim);
		printf("Chave atribuida no dia %s %s \n", data, hora);
		printf("Numero de chaves já atribuidas: %s\n\n", chaves_atribuidas);
		while (ptr != NULL)
		{
			strcpy(numeros, ptr);
			ptr = strtok(NULL, delim);
			strcpy(estrelas, ptr);
			ptr = strtok(NULL, delim);
			printf("Chave %d=> Numeros: %s Estrelas: %s\n",i,numeros, estrelas);
			i++;
		}
	}
	else if (strcmp(ptr, "3")==0)
	{
		ptr = strtok(NULL, delim);
		strcpy(chaves_atribuidas, ptr);
		printf("Numero de chaves já atribuidas: %s\n\n", chaves_atribuidas);
		ptr = strtok(NULL, delim);
		while (ptr != NULL)
		{
			strcpy(numeros, ptr);
			ptr = strtok(NULL, "\n");
			strcpy(estrelas, ptr);
			ptr = strtok(NULL, delim);
			printf("Chave %d=> Numeros: %s Estrelas: %s\n", i, numeros, estrelas);
			i++;
		}
	}
	else if (strcmp(ptr, "7") == 0)
	{
		ptr = strtok(NULL, delim);
		if (strcmp(ptr, "1") == 0)
		{
			printf("Histórico de chaves apagado com sucesso!\n");
		}
		else {
			printf("Erro a apagar histórico!\n");
		}
	}
	else {
		puts(str);
	}
		
}

int main(int argc, char* argv[MAX_SIZE])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char message[100]=" ";
	char server_reply[MAX_SIZE];
	int recv_size;
	int ws_result;
	int port=0;
	char ip_addr[20];
	char option='x';

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

	printf("Inicializado Com Sucesso.\n");

	//Create a socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		PlaySound(TEXT("Error.wav"), NULL, SND_FILENAME || SND_ASYNC);
		printf("Nao foi possível criar o Socket: %d", WSAGetLastError());
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
		PlaySound(TEXT("Error.wav"), NULL, SND_FILENAME || SND_ASYNC);
		puts("Erro de Conexao!");
		return 1;
	}

	puts("Conexao com o servidor efetuada com sucesso!");
	 
	//quando conectados temos que receber mensagem do servidor antes de enviar...
	recv_size = recv(s, server_reply, MAX_SIZE, 0); //4000->MAX_SIZE
	if (recv_size == SOCKET_ERROR)
	{
		PlaySound(TEXT("Error.wav"), NULL, SND_FILENAME || SND_ASYNC);
		puts("recv failed");
	}

	server_reply[recv_size - 1] = '\0';
	if (strcmp(server_reply, "\n100 OK\n") == 0)
	{
		textcolor(2);
		puts("Servidor Operacional, Bem Vindo!");
	}
	else
	{
		textcolor(14);
		puts(server_reply);
	}
	textcolor(15);
	//Menu
	PlaySound(TEXT("StartupSound.wav"), NULL, SND_FILENAME || SND_ASYNC);
	menuDisplay();
	
	while (1)
	{
		if (strcmp(message, "quit") == 0)
		{
			closesocket(s);
			WSACleanup();
			break;
		}
		else if (strcmp(message, "quits") == 0)
		{
			closesocket(s);
			WSACleanup();
			break;
		}

		scanf("%c", &option);
		if (option != '\n')
		{
			dataToSend(option, &message);
			option = 'x';

			ws_result = send(s, message, strlen(message), 0);

			if (ws_result < 0)
			{
				PlaySound(TEXT("Error.wav"), NULL, SND_FILENAME || SND_ASYNC);
				puts("Send failed");
				return 1;
			}
			//puts("Data Sent\n");

			//Receive a reply from the server

			//verificar se tamanho de envio do servidor corresponde ao tamanho que o cliente recebe!!!
			recv_size = recv(s, server_reply, MAX_SIZE, 0); //2000 -> MAX_SIZE
			if (recv_size == SOCKET_ERROR)
			{
				PlaySound(TEXT("Error.wav"), NULL, SND_FILENAME || SND_ASYNC);
				puts("recv failed");
			}

			//puts("Reply received\n");

			//Add a NULL terminating character to make it a proper string before printing
			server_reply[recv_size - 1] = '\0';

			
			if (strcmp(server_reply, "\n400 BYE\n") == 0)
			{
				textcolor(13);
				puts("Adeus, ate a proxima!");
			}
			else
			{
				textcolor(14);
				//new print function
				printData(server_reply,recv_size);

			}
			textcolor(15);
			Sleep(2000);
		}
	}
	
	
	// Close the socket
	closesocket(s);

	//Cleanup winsock
	WSACleanup();
	
	return 0;
}
