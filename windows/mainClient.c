#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAX 99999
#define PORT 12345
#define SA struct sockaddr

int clientSocket;
struct sockaddr_in sockaddrIn, cli;
int choice;
char username[MAX];
char password[MAX];
char string[MAX];
char buffer[MAX];
char message[MAX];
char **token;

void mainMenu();
void secondMenu(char *source);
void registerAccount();
void loginAccount();
void chat(char *source);

void secondMenu(char *source)
{
  printf("1. Create channel\n2. Join channel\n3. Log out\n");
  scanf("%d", &choice);
  switch (choice)
  {
  case 1:
  {
    printf("Enter the name of channel: ");
    scanf("%s", string);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "create channel ");
    strcat(buffer, string);
    strcat(buffer, " , ");
    strcat(buffer, source);
    send(clientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    if (buffer[9] == 'S')
    {
      // setColor(TC_GREEN);
      printf("Congratulations!\nChannel successfully created\n");
      // setColor(TC_WHITE);
      chat((char *)source);
    }
    else
    {
      // setColor(TC_RED);
      printf("The Name Is Not Available");
      // setColor(TC_WHITE);
    }

    break;
  }
  case 2:
  {
    printf("Enter the name of channel: ");
    scanf("%s", string);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "join channel ");
    strcat(buffer, string);
    strcat(buffer, " , ");
    strcat(buffer, (const char *)source);
    send(clientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    if (buffer[9] == 'S')
    {
      // setColor(TC_GREEN);
      printf("Congratulations!\nYou successfully Joined Channel\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    else
    {
      // setColor(TC_RED);
      printf("Something went wrong!");
      // setColor(TC_WHITE);
      secondMenu(source);
    }

    break;
  }
  case 3:
  {
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "logout ");
    strcat(buffer, string);
    strcat(buffer, ", ");
    strcat(buffer, (const char *)source);
    send(clientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    if (buffer[9] == 'S')
    {
      // setColor(TC_GREEN);
      printf("You successfully Log Out\n");
      // setColor(TC_WHITE);
      mainMenu();
    }
    else
    {
      // setColor(TC_RED);
      printf("Something went wrong!\n");
      // setColor(TC_WHITE);
      secondMenu(source);
    }
  }
  }
}

void chat(char *source)
{
  printf("1. Send Message\n2. Refresh\n3. Channel Members\n4. Leave Channel\n");
  printf("Enter Your Choice: ");
  scanf("%d", &choice);
  switch (choice)
  {
  case 1:
  {
    printf("Enter Your Message: ");
    scanf("%s", message);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "send ");
    strcat(buffer, message);
    strcat(buffer, " , ");
    strcat(buffer, source);
    send(clientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));

    recv(clientSocket, buffer, sizeof(buffer), 0);

    if (buffer[9] == 'S')
    {
      // setColor(TC_GREEN);
      printf("Message Successfully Sent\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    else
    {
      // setColor(TC_RED);
      printf("Message Not Sent\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    break;
  }
  case 2:
  {
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "refresh ");
    strcat(buffer, source);
    send(clientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));

    recv(clientSocket, buffer, sizeof(buffer), 0);
    cJSON *item0 = cJSON_Parse(buffer);
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(item0, "content");
    int size = cJSON_GetArraySize(item1);
    for (int i = 0; i < size; i++)
    {
      cJSON *item2 = cJSON_GetArrayItem(item1, i);
      cJSON *item3 = cJSON_GetObjectItemCaseSensitive(item2, "sender");
      cJSON *item4 = cJSON_GetObjectItemCaseSensitive(item2, "content");
      printf("%s: %s\n", item3->valuestring, item4->valuestring);
    }

    if (buffer[9] == 'S')
    {
      // setColor(TC_GREEN);
      printf("Channel Has Been Refreshed\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    else
    {
      // setColor(TC_RED);
      printf("Channel Has'nt Refreshed\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    break;
  }
  case 3:
  {
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "channel members ");
    strcat(buffer, source);

    send(clientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);

    cJSON *item0 = cJSON_Parse(buffer);
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(item0, "content");
    int size = cJSON_GetArraySize(item1);
    for (int i = 0; i < size; i++)
    {
      cJSON *item2 = cJSON_GetArrayItem(item1, i);
      //cJSON * item4 = cJSON_GetObjectItemCaseSensitive(item2, "content");
      printf("%s\n", item2->valuestring);
    }

    if (buffer[9] == 'S')
    {
      // setColor(TC_GREEN);
      printf("Channel Members Will Be Shown\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    else
    {
      // setColor(TC_RED);
      printf("Something Went Wrong\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    break;
  }
  default:
  {
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "leave ");
    strcat(buffer, source);
    send(clientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    if (buffer[9] == 'S')
    {
      // setColor(TC_GREEN);
      printf("You Successfully Left The Channel\n");
      // setColor(TC_WHITE);
      secondMenu(source);
    }
    else
    {
      // setColor(TC_RED);
      printf("Something Went Wrong\n");
      // setColor(TC_WHITE);
      chat(source);
    }
    break;
  }
  }
}

void loginAccount()
{

  printf("Enter Username\n");
  scanf("%s", username);
  printf("Enter Password\n");
  scanf("%s", password);

  memset(buffer, 0, sizeof(buffer));

  strcat(buffer, "login ");
  strcat(buffer, username);
  strcat(buffer, " , ");
  strcat(buffer, password);
  send(clientSocket, buffer, sizeof(buffer), 0);

  memset(buffer, 0, sizeof(buffer));
  recv(clientSocket, buffer, sizeof(buffer), 0);
  cJSON *jsonParse = cJSON_Parse(buffer);
  cJSON *content = cJSON_GetObjectItemCaseSensitive(jsonParse, "type");
  cJSON *authToken = cJSON_GetObjectItemCaseSensitive(jsonParse, "content");
  *token = cJSON_Print(authToken);
  if (strcmp(cJSON_Print(content), "AuthToken") == 0)
  {
    // setColor(TC_GREEN);
    printf("Successfully logged in\n");

    // setColor(TC_WHITE);
    secondMenu(token);
  }
  else if (strcmp(cJSON_Print(content), "Error") == 0)
  {
    // setColor(TC_RED);
    printf("The User already logged in\n");
    // setColor(TC_WHITE);
    secondMenu(token);
    return;
  }
  else
  {
    // setColor(TC_RED);
    printf("Invalid Username or Password\n");
    // setColor(TC_WHITE);
    mainMenu();
  }
}

void registerAccount()
{

  memset(buffer, 0, sizeof(buffer));
  // setColor(TC_YELLOW);
  printf("Enter Username\n");
  scanf("%s", username);

  printf("Enter Password\n");
  scanf("%s", password);

  // setColor(15);

  strcat(buffer, "register ");
  strcat(buffer, username);
  strcat(buffer, ", ");
  strcat(buffer, password);

  send(clientSocket, buffer, sizeof(buffer), 0);
  memset(buffer, 0, sizeof(buffer));
  recv(clientSocket, buffer, sizeof(buffer), 0);
  if (buffer[9] == 'S')
  {
    // setColor(TC_GREEN);
    printf("Successfully Registered\n");
    // setColor(TC_WHITE);
    mainMenu();
  }
  else
  {
    // setColor(TC_RED);
    printf("Something Went Wrong\n");
    // setColor(TC_WHITE);
    mainMenu();
  }
}

void mainMenu()
{

  printf("Account Menu:\n1: Register\n2: Login\n");
  scanf("%d", &choice);
  if (choice == 1)
  {
    registerAccount();
  }
  else
    loginAccount();
}

int main()
{

  struct sockaddr_in servaddr, cli;

  // Create and verify socket
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1)
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
  if (connect(clientSocket, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    printf("Connection to the server failed...\n");
    exit(0);
  }
  else
    printf("Successfully connected to the server..\n");

  mainMenu();
}