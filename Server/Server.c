#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>


#define TRUE 1
#define DS_TEST_PORT 4800 // a porta que usamos é esta
#define MAX_SIZE 16100

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)

HANDLE ghMutex;


// function ot handle the incoming connection
//	param: the socket of the calling client

DWORD WINAPI handleconnection(LPVOID lpParam);

void getChavesFromFile(char* result) {
	//Função que vai buscar as chaves ao ficheiro "chavesEuromilhoes.txt" e retorna (por memória) um string com o conteúdo deste
	char ch;
	int i = 0;
	char buffer[MAX_SIZE];
	FILE* fl;
	
	fl = fopen("chavesEuromilhoes.txt", "r+");

	if (fl == NULL) {
		printf("No file available!");
		strcpy(result, "Sem resultados anteriores para mostrar.");
	}
	else
	{
		while ((ch = fgetc(fl)) != EOF)
		{
			buffer[i] = ch;
			i++;
		}
		buffer[i] = '\0';
		strcpy(result, buffer);
		fclose(fl);
	}
}

//Função de assistencia do Selection Sort, que troca os números de posição
void swap(int* xp, int* yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

//Função de Selection Sort para ajudar na procura de chaves repetidas
void selectionSort(int arr[], int n)
{
	int i, j, min_idx;

	//One by one move boundary of unsorted subarray
	for (i = 0; i < n - 1; i++) {

		// Find the minimum element in unsorted array
		min_idx = i;
		for (j = i + 1; j < n; j++)
			if (arr[j] < arr[min_idx])
				min_idx = j;

		// Swap the found minimum element with the first element
		swap(&arr[min_idx], &arr[i]);
	}
}

//Função de teste de chaves para verificar se já foram atribuidas
int KeyTest(int* numeros, int* estrelas)
{
	char fich[MAX_SIZE];
	char teste[40];
	getChavesFromFile(fich);
	sprintf(teste, "%d,%d,%d,%d,%d;%d,%d\n", numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);

	if (strstr(fich, teste)==NULL)
	{
		return 1; //não encontrou
	}
	else
	{
		return 0; //encontrou
	}
}

//Testa se existe ou não o ficheiro "chavesEuromilhoes.txt"
int fileTest()
{
	FILE* fl = fopen("chavesEuromilhoes.txt", "r");
	if (fl == NULL) { return 1; }
	else { fclose(fl); return 0; }
}

//Função que guarda as chaves atribuidas no ficheiro "chavesEuromilhoes.txt"
void saveChavesToFile(int* numeros, int* estrelas) {

	DWORD dwCount = 0, dwWaitResult;
	FILE* fl;

	if (fileTest() == 0)
	{
		fl = fopen("chavesEuromilhoes.txt", "a");
		fprintf(fl, "%d,%d,%d,%d,%d;%d,%d\n", numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);
		fclose(fl);
	}
	else
	{
		fl = fopen("chavesEuromilhoes.txt", "w");
		fprintf(fl, "%d,%d,%d,%d,%d;%d,%d\n", numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);
		fclose(fl);
	}
}

//Função que gera as chaves do Euromilhões
void chavesEuromilhoes(int* num, int* est) {
	time_t t;
	int numeros[5];
	int estrelas[2];
	int n = 5;
	int e = 2;
	int validation = 0;
	int buffer = 0;
	srand((unsigned)time(&t));

	while (validation != 1)
	{
		for (int i = 0; i < n; i++) {
			
			do
			{
				buffer = (rand() % 50)+1;
			} while (numeros[0]==buffer || numeros[1]==buffer || numeros[2]==buffer || numeros[3]==buffer || numeros[4]==buffer);
			numeros[i] = buffer;
			buffer = 0;
		}
		selectionSort(numeros, 5);
		for (int j = 0; j < e; j++) {

			do
			{
				buffer = (rand() % 12)+1;
			} while (estrelas[0]==buffer || estrelas[1]==buffer);
			estrelas[j] = buffer;
			buffer = 0;
		}
		selectionSort(estrelas, 2);
		if (KeyTest(&numeros, &estrelas) == 1) //teste para ver se é igual a uma existente
		{
			validation = 1; //não existe
			saveChavesToFile(&numeros, &estrelas);
		}
		else{validation = 0;} //já existe
	}
	for (int i = 0; i < 5; i++) { num[i] = numeros[i]; }
	for (int j = 0; j < 2; j++) { est[j] = estrelas[j]; }
}

//Função que determina o número de chaves a atribuir, quando é feito um pedido de atribuição de 2 ou mais chaves
int getQuantity(char str[])
{
	int init_size = strlen(str);
	char delim[] = "_";
	int i = 0;
	char* ptr = strtok(str, delim);

	ptr = strtok(NULL, delim);

	return atoi(ptr);
}

//Acede a "chavesEuromilhoes.txt" e conta quantas já foram atribuidas até ao momento
int contarChaves(char* fich) {
	char chr = "1";
	int i = 0;
	int number = 0;
	while(fich[i]!=NULL)
	{
		//Count whenever new line is encountered
		if (chr == '\n')
		{
			number=number+1;	
		}
		i++;
		//take next character from file.
		chr = fich[i];
	}
	return number;
}

int main()
{
	// Initialise winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	printf("\nInitialising Winsock...");
	int wsResult = WSAStartup(ver, &wsData);
	if (wsResult != 0) {
		fprintf(stderr, "\nWinsock setup fail! Error Code : %d\n", WSAGetLastError());
		return 1;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		fprintf(stderr, "\nSocket creationg fail! Error Code : %d\n", WSAGetLastError());
		return 1;
	}

	printf("\nSocket created.");

	// Bind the socket (ip address and port)
	struct sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(DS_TEST_PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (struct sockaddr*)&hint, sizeof(hint));
	printf("\nSocket binded.");

	// Setup the socket for listening
	listen(listening, SOMAXCONN);
	printf("\nServer listening.");

	// Wait for connection
	struct sockaddr_in client;
	int clientSize;
	SOCKET clientSocket;
	SOCKET* ptclientSocket;
	DWORD dwThreadId;
	HANDLE  hThread;
	int conresult = 0;

	ghMutex = CreateMutex(
		NULL,
		FALSE,
		NULL);
	

	while (TRUE)
	{
		clientSize = sizeof(client);
		clientSocket = accept(listening, (struct sockaddr*)&client, &clientSize);

		ptclientSocket = &clientSocket;

		printf("\nHandling a new connection.");

		// Handle the communication with the client 

		hThread = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			(LPTHREAD_START_ROUTINE)handleconnection,       // thread function name
			ptclientSocket,          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadId);   // returns the thread identifier 


		// Check the return value for success.
		// If CreateThread fails, terminate execution. 

		if (hThread == NULL)
		{
			printf("\nThread Creation error.");
			ExitProcess(3);
		}
	}

	//close Mutex
	
	CloseHandle(ghMutex);
	// Close the socket
	closesocket(clientSocket);

	// Close listening socket
	closesocket(listening);

	//Cleanup winsock
	WSACleanup();
}

//Função de comunicação com o cliente
DWORD WINAPI handleconnection(LPVOID lpParam)
{
	DWORD dwCount = 0, dwWaitResult;
	char strMsg[MAX_SIZE];
	char strRec[1024];

	int i = 1;
	SOCKET cs;
	SOCKET* ptCs;

	ptCs = (SOCKET*)lpParam;
	cs = *ptCs;

	

	strcpy(strMsg, "\n100 OK\n");
	printf("\n%s\n", strMsg);
	send(cs, strMsg, strlen(strMsg) + 1, 0);

	while (TRUE) {
		ZeroMemory(strRec, 1024);
		int bytesReceived = recv(cs, strRec, 1024, 0);
		if (bytesReceived == SOCKET_ERROR) {
			printf("\nReceive error!\n");
			break;
		}
		if (bytesReceived == 0) {
			printf("\nClient disconnected!\n");
			break;
		}

		printf("%i : %s\n", i++, strRec);

		if (strcmp(strRec, "quit") == 0) {

			strcpy(strMsg, "\n400 BYE\n");
			send(cs, strMsg, strlen(strMsg) + 1, 0);

			// Close the socket
			closesocket(cs);
			return 0;
		}
		else if (strcmp(strRec, "bye") == 0) { //legacy exit

			strcpy(strMsg, "\n400 BYE\n");
			send(cs, strMsg, strlen(strMsg) + 1, 0);

			// Close the socket
			closesocket(cs);
			return 0;
		}
		else if (strcmp(strRec, "help") == 0) { //Legacy

			strcpy(strMsg, "\n\t-> help - Listar tipos de comandos disponiveis;\n\t-> quit - Sair do programa;\n\t-> quits - Sair do programa, do lado do servidor;\n\t-> chave - Retorna uma chave do Euromilhoes;\n\t-> chaveN - Retorna N chaves, sendo N um numero inteiro;\n\t-> hist - Historico de chaves ja atribuidas;\n\t-> delete - Comando exclusivo do servidor para dar “reset” as chaves;\n");
			send(cs, strMsg, strlen(strMsg) + 1, 0);
		}
		else if (strcmp(strRec, "quits") == 0) {

			strcpy(strMsg, "\n400 BYE\n");
			send(cs, strMsg, strlen(strMsg) + 1, 0);

			// Close the socket
			closesocket(cs);
			exit(0);
		}
		else if (strcmp(strRec, " ") == 0) {

			strcpy(strMsg, " ");
			send(cs, strMsg, strlen(strMsg) + 1, 0);
		}
		else if (strcmp(strRec, "chave") == 0) { //pedir uma chave - um thread de cada vez 
			//current time mechanism
			char dt[40] = "";
			time_t rawtime;
			struct tm* timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			sprintf(dt, "%d;%d/%d/%d;%d:%d:%d;",1, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			/*char data[MAX_SIZE] = "";
			strcpy(data, "\tChave atribuida na data: \0");
			strcat(data, dt);
			strcat(data, "\t");*/

			int numeros[5];
			int estrelas[2];
			dwWaitResult = WaitForSingleObject( // pedir mutex 
				ghMutex,    
				INFINITE
			);
			
			switch (dwWaitResult)
			{
				case WAIT_OBJECT_0:
					__try {
						char result[MAX_SIZE];
						char strRes[MAX_SIZE];
						int number = 0;
						char numberchar[10];
						getChavesFromFile(&result);
						number = contarChaves(&result);
						sprintf(numberchar, "%d;", number);
						
						chavesEuromilhoes(&numeros, &estrelas);
						sprintf(strRes,"%d,%d,%d,%d,%d;%d,%d.", numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);
						strcat(numberchar, strRes);
						strcat(dt, numberchar);
						send(cs, dt, strlen(dt) + 1, 0);
					}
					__finally {
						// Release ownership of the mutex object
						if (!ReleaseMutex(ghMutex))
						{
							// Handle error.
						}
					}
					break;

				case WAIT_ABANDONED:
					printf("Erro mutex");
					return FALSE;
					break;

			}

			
			
		}
		else if (!(strstr(strRec,"chave_") == NULL))
		{
			int numeros[5];
			int estrelas[2];
			char strRes[MAX_SIZE];

			//current time mechanism
			char dt[40] = "";
			time_t rawtime;
			struct tm* timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			sprintf(dt, "%d;%d/%d/%d;%d:%d:%d;",2, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			/*char data[MAX_SIZE] = "";
			strcpy(data, "\tChave atribuida na data: \0");
			strcat(data, dt);
			strcat(data, "\t");*/

			dwWaitResult = WaitForSingleObject( // pedir mutex 
				ghMutex,
				INFINITE
			);

			int quantidade=getQuantity(strRec);

			
			switch (dwWaitResult)
			{
			case WAIT_OBJECT_0:
				__try {
					char result[MAX_SIZE];
					int number = 0;
					getChavesFromFile(&result);
					number = contarChaves(&result);
					sprintf(strRes,"%d;", number);
					for (int i = 1; i <= quantidade; i++)
					{
						chavesEuromilhoes(&numeros, &estrelas);
						sprintf(strMsg, "%d,%d,%d,%d,%d;%d,%d;",numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);
						if(i!=0)
						strcat(strRes, strMsg);
						else
						strcpy(strRes, strMsg);
					}
					strcat(dt, strRes);
					send(cs, dt, strlen(dt) + 1, 0);
				}
				__finally {
					// Release ownership of the mutex object
					if (!ReleaseMutex(ghMutex))
					{
						// Handle error.
					}
				}
				break;

			case WAIT_ABANDONED:
				printf("Erro mutex");
				return FALSE;
				break;

			}

		}
		else if (strcmp(strRec, "hist") == 0) {
			dwWaitResult = WaitForSingleObject( // pedir mutex 
				ghMutex,
				INFINITE
			);

			switch (dwWaitResult)
			{
			case WAIT_OBJECT_0:
				__try {
					getChavesFromFile(&strMsg);
					char strRes[MAX_SIZE];
					char result[MAX_SIZE];
					int number = 0;
					getChavesFromFile(&result);
					number = contarChaves(&result);
					sprintf(strRes, "%d;%d;",3,number);
					strcat(strRes, strMsg);
					send(cs, strRes, strlen(strRes) + 1, 0);
				}
				__finally {
					// Release ownership of the mutex object
					if (!ReleaseMutex(ghMutex))
					{
						// Handle error.
					}
				}
				break;

			case WAIT_ABANDONED:
				printf("Erro mutex");
				return FALSE;
				break;

			}
		}
		else if (strcmp(strRec, "delete") == 0) {
		dwWaitResult = WaitForSingleObject( // pedir mutex 
			ghMutex,
			INFINITE
		);

		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			__try {
				char msg[100];
				if (remove("chavesEuromilhoes.txt") == 0)
				{
					sprintf(msg, "%d;%d",7,1);
					puts("Deleted successfully");
				}
				else
				{
					puts("Unable to delete the file");
					sprintf(msg, "%d;%d", 7, 0);
				}
					
				send(cs, msg, strlen(msg) + 1, 0);
			}
			__finally {
				// Release ownership of the mutex object
				if (!ReleaseMutex(ghMutex))
				{
					// Handle error.
				}
			}
			break;

		case WAIT_ABANDONED:
			printf("Erro mutex");
			return FALSE;
			break;

		}
		}
		else
		{
			strcpy(strMsg, "\nComando Inválido\n");
			send(cs, strRec, bytesReceived + 1, 0);
		}
	}
	return TRUE;
}