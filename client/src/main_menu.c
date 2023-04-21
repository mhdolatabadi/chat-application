#include <stdio.h>
#include "chat_menu.c"

char channel[20];

void logout()
{
}

void join_channel()
{
    printf("Enter Channel Name\n");
}

void create_channel()
{
    printf("Enter Channel Name\n");
    scanf("%s", channel);
    chat_menu();
}

void main_menu()
{
    printf("1: Create Channel\n2: Join Channel\n3: Logout\n");
    int selection;
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
        create_channel();
        break;
    case 2:
        join_channel();
        break;
    case 3:
        logout();
        break;
    default:
        printf("Illegal number.\n");
    }
}