#include "main_menu.c"

char user_name[20], password[20];

void login()
{
    printf("Enter Username\n");
    printf("Enter Password\n");
}

void _register()
{
    printf("Enter Username\n");
    scanf("%s", user_name);
    printf("Enter Password\n");
    scanf("%s", password);
    main_menu();
}

void user_menu()
{
    printf("Account Menu:\n1: Register\n2: Login\n");
    int selection;
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
        _register();
        break;

    case 2:
        login();
        break;

    default:
        printf("Illegal number.\n");
        break;
    }
}