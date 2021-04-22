#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>


#define TRUE 1
#define DS_TEST_PORT 4800

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)

HANDLE ghMutex;


// function ot handle the incoming connection
//	param: the socket of the calling client

DWORD WINAPI handleconnection(LPVOID lpParam);

void getChavesFromFile(char* result) {

	
	char ch;
	int i = 0;
	char buffer[10000];
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

void swap(int* xp, int* yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// Function to perform Selection Sort
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

int KeyTest(int* numeros, int* estrelas)
{
	char fich[10000];
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

int fileTest()
{
	FILE* fl = fopen("chavesEuromilhoes.txt", "r");
	if (fl == NULL) { return 1; }
	else { fclose(fl); return 0; }
}

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

int getQuantity(char str[])
{
	int init_size = strlen(str);
	char delim[] = "_";
	int i = 0;
	char* ptr = strtok(str, delim);

	ptr = strtok(NULL, delim);

	return atoi(ptr);
	
}

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

void testar() {
	char result[10000];
	int number=0;
	getChavesFromFile(&result);
	number = contarChaves(&result);
	printf("numero de chaves: %d\n", number);

}

int main()
{
	// Initialise winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	testar();
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




DWORD WINAPI handleconnection(LPVOID lpParam)
{
	DWORD dwCount = 0, dwWaitResult;
	char strMsg[1024];
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
		else if (strcmp(strRec, "help") == 0) {

			strcpy(strMsg, "\n\t-> help - Listar tipos de comandos disponiveis;\n\t-> quit - Sair do programa;\n\t-> quits - Sair do programa, do lado do servidor;\n\t-> chave - Retorna uma chave do Euromilhoes;\n\t-> chaveN - Retorna N chaves, sendo N um numero inteiro;\n\t-> hist - Historico de chaves ja atribuidas;\n\t-> delete - Comando exclusivo do servidor para dar “reset” as chaves;\n");
			send(cs, strMsg, strlen(strMsg) + 1, 0);
		}
		else if (strcmp(strRec, "quits") == 0) {

			strcpy(strMsg, "\nBye client...\n");
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
		
						chavesEuromilhoes(&numeros, &estrelas);
						sprintf(strMsg, "\nChave => Numeros: %d,%d,%d,%d,%d; Estrelas: %d,%d.\n", numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);
						//strcpy(strMsg, );
						send(cs, strMsg, strlen(strMsg) + 1, 0);
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
			char strRes[1024];
			dwWaitResult = WaitForSingleObject( // pedir mutex 
				ghMutex,
				INFINITE
			);

			int quantidade=getQuantity(strRec);

			
			switch (dwWaitResult)
			{
			case WAIT_OBJECT_0:
				__try {

					for (int i = 1; i <= quantidade; i++)
					{
						chavesEuromilhoes(&numeros, &estrelas);
						sprintf(strMsg, "\nChave %d => Numeros: %d,%d,%d,%d,%d; Estrelas: %d,%d.",i,numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);
						if(i!=1)
						strcat(strRes, strMsg);
						else
						strcpy(strRes, strMsg);
					}	
					
					//strcpy(strMsg, );
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
		else if (strcmp(strRec, "hist") == 0) { //legacy exit

			getChavesFromFile(&strMsg);
			send(cs, strMsg, strlen(strMsg) + 1, 0);
		}
		else
		{
			strcpy(strMsg, "\nComando Inválido\n");
			send(cs, strRec, bytesReceived + 1, 0);
		}
	}
	return TRUE;
}