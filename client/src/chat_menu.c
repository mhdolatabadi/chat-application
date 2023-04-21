#include <stdio.h>

void leave_channel()
{
}

void channel_members()
{
}

void refresh()
{
}

void send_message()
{
}

void chat_menu()
{
    printf("1: Send Message\n2: Refresh\n3: Channel Members\n4: Leave Channel\n");
    int selection;
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
        send_message();
        break;
    case 2:
        refresh();
        break;
    case 3:
        channel_members();
        break;
    case 4:
        leave_channel();
        break;
    default:
        printf("Illegal number.\n");
    }
}