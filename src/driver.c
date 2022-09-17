#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *ifp;
    char buffer[1024];

    if (argc < 2)
    {
        printf("Syntax: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file.
    if ((ifp = fopen(argv[0], "r")) == NULL)
    {
        printf("%s not found.\n", argv[0]);
        return 0;
    }

    fclose(argv[0]);

    return 0;
}
