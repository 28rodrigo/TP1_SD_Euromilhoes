#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>
#define TRUE 1
#define DS_TEST_PORT 4800

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)


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
		strcpy(result, "Error!");
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
	return;
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
	char fichMatrix[200][25];
	char teste[25];
	getChavesFromFile(fich);
	sprintf(teste, "%d,%d,%d,%d,%d;%d,%d\n", numeros[0], numeros[1], numeros[2], numeros[3], numeros[4], estrelas[0], estrelas[1]);

	if (strstr(fich, teste)==NULL)
	{
		printf("ya coisa e tal"); //não encontrou
	}
	else
	{
		printf("banana"); //encontrou
	}
}

void chavesEuromilhoes() {
	int numeros[5];
	int estrelas[2];
	time_t t;
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
		//teste para ver se é igual a uma existente
		validation = 1;
	}
}

int fileTest()
{
	FILE* fl = fopen("chavesEuromilhoes.txt", "r");
	if (fl == NULL) { return 1; }
	else { fclose(fl); return 0; }
}

void saveChavesToFile(int* numeros, int* estrelas) {
	FILE* fl;
	
	if (fileTest()==0)
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
	
	return;
}

int main()
{
	int stuff1[5];
	stuff1[0] = 1; stuff1[1] = 2; stuff1[2] = 3; stuff1[3] = 4; stuff1[4] = 5;
	int stuff2[2];
	stuff2[0] = 1; stuff2[1] = 2;
	KeyTest(&stuff1,&stuff2);
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
			handleconnection,       // thread function name
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

	// Close the socket
	closesocket(clientSocket);

	// Close listening socket
	closesocket(listening);

	//Cleanup winsock
	WSACleanup();
}



DWORD WINAPI handleconnection(LPVOID lpParam)
{

	char strMsg[1024];
	char strRec[1024];

	int i = 1;
	SOCKET cs;
	SOCKET* ptCs;

	ptCs = (SOCKET*)lpParam;
	cs = *ptCs;

	strcpy(strMsg, "\nHello! welcome to the server...\n");
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

		if (strcmp(strRec, "date") == 0) {
			// current date/time based on current system
			time_t now = time(0);
			// convert now to string form
			char* dt = ctime(&now);

			strcpy(strMsg, "\n\nThe local date and time is: ");
			strcat(strMsg, dt);
			strcat(strMsg, "\n");

			send(cs, strMsg, strlen(strMsg) + 1, 0);

			// just to echo!
			// send(cs, strRec, bytesReceived + 1, 0);
		}
		else if (strcmp(strRec, "quit") == 0) {

			strcpy(strMsg, "\nBye Client...\n");
			send(cs, strMsg, strlen(strMsg) + 1, 0);

			// Close the socket
			closesocket(cs);
			return 0;
		}
		else if (strcmp(strRec, "bye") == 0) { //legacy exit

			strcpy(strMsg, "\nBye Client...\n");
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
		else if (strcmp(strRec, "bye") == 0) {

			strcpy(strMsg, "\nBye client...\n");
			send(cs, strMsg, strlen(strMsg) + 1, 0);
		}
		else if (strcmp(strRec, " ") == 0) {

			strcpy(strMsg, " ");
			send(cs, strMsg, strlen(strMsg) + 1, 0);
		}
		else
		{
			send(cs, strRec, bytesReceived + 1, 0);
		}
	}
}