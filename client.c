#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cJSON.h>
#include <cJSON.c>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr

void fLogin(char username[MAX], char password[MAX])
{
}

void login_menu()
{
	int n;
	char username[MAX];
	char password[MAX];

	n = 0;
	printf("Enter Username:");
	while ((username[n++] = getchar()) != '\n')
		;

	n = 0;
	printf("Enter Password:");
	while ((password[n++] = getchar()) != '\n')
		;

	return fLogin(username, password);
}

void fRegister(char username[MAX], char password[MAX])
{
}

void register_menu()
{
	int n;
	char username[MAX];
	char password[MAX];

	n = 0;
	printf("Enter Username:");
	while ((username[n++] = getchar()) != '\n')
		;

	n = 0;
	printf("Enter Password:");
	while ((password[n++] = getchar()) != '\n')
		;

	return fRegister(username, password);
}

void main_menu(int client_socket)
{
	printf("Account menu:\n1: Register\n2: Login");
	int input = 0;
	scanf("%d", &input);
	switch (input)
	{
	case 1:
		register_menu();
		break;
	case 2:
		login_menu();
		break;
	default:
		printf("You've entered invalid number!");
		break;
	}
	return main_menu(client_socket);
}

int main()
{
	int client_socket, server_socket;
	struct sockaddr_in servaddr, cli;

	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
	{
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	// Assign IP and port
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else
		printf("Successfully connected to the server..\n");

	// Function for chat
	main_menu(client_socket);

	// Close the socket
	shutdown(client_socket, SHUT_RDWR);
}