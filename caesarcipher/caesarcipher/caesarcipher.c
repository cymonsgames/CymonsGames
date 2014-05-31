#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    char mode = 0;
    char keychar = 0;
    int key = 1;
    int delta = 0;
    char message[1024];
    int c, cn;

    while ((mode != 'e') && (mode != 'd') && (mode != 'b')) {
        if (mode)
            printf("Please type only 'E', 'D', or 'B' and press ENTER\n");
        printf("(E)ncode, (D)ecode, or (B)reak? ");
        mode = tolower(getchar());
        while (iscntrl(mode))
            mode = tolower(getchar());
    }
    if (mode != 'b') {
        printf ("Input Key:\n What does A equal? ");
        keychar = tolower(getchar());
        while (!isalpha(keychar))
            keychar = tolower(getchar());
        getchar(); /* cleanup */
        key = keychar - 'a';
        printf ("Shift incrament per character?\n"
                "(Input 0 for a normal shift cypher)? ");
        scanf("%d", &delta);
    }
    printf("\nType the message string:\n");
    do {
        fgets(message, sizeof(message), stdin);
    } while (!message[0] || message[0] == '\n');
    cn = strlen(message);
    for (; key < ((mode == 'b')?26:(keychar - 'a' + 1)); key++) {
        /* bit of an ugly shortcut there so that I can do all modes with a
           single loop. */
        printf("\n");
        if (mode == 'b')
            printf("Key: A = %c\n", 'A' + key);
        for (c = 0; c < cn; c++) {
            char l = message[c];
            if (isalpha(l)) {
                int pos = tolower(l) - 'a';
                if (tolower(l) == l)
                    if (mode == 'd')
                        putchar('a' + (26 + pos - (key + c * delta) % 26 )% 26);
                    else
                        putchar('a' + (pos + key + c * delta) % 26);
                else
                    if (mode == 'd')
                        putchar('A' + (26 + pos - (key + c * delta) % 26) % 26);
                    else
                        putchar('A' + (pos + key + c * delta) % 26);
            } else if (isdigit(message[c])) {
                int num = l - '0';
                if (mode == 'd')
                    putchar('0' + (20 + num - (key + c * delta) % 10) % 10);
                else
                    putchar('0' + (num + key + c * delta) % 10);
            } else
                putchar(l);
        }
        printf("\n");
        if (mode == 'b') {
            printf("Does that look translated? (Y/N) ");
            if (tolower(getchar()) == 'y')
                break;
        }
    }
    printf("\nFarewell Super Sleuth.\n");
    getchar();
    return 0;
}
