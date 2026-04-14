#include <stdio.h>

int main() {
    long bot_age = 18;
    char bot_name[32] = "youngBot";
    char press_enter[1];

    printf("Welcome, my name is %s and i'm %ld years old\n", &bot_name, &bot_age);
    printf("Press <ENTER> to quit...");
    gets(press_enter);
    printf("Bye !\n");
}

