#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#DEFINE PROCESS_COUNT 1
#DEFINE RUN 2
#DEFINE USE 3
#DEFINE QUANTUM 4
#DEFINE PROCESS 5
#DEFINE ERROR -1

int parser(char *str)
{
    if (str == NULL)
    {
        return ERROR;
    }

    if (str == "processcount")
    {
        return PROCESS_COUNT;
    }
    else if (str == "runfor")
    {
        return RUN;
    }
    else if (str == "use")
    {
        return USE;
    }
    else if (str == "quantum")
    {
        return QUANTUM;
    }
    else if (str == "process")
    {
        return PROCESS;
    }
    else
    {
        printf("UNKNOWN TOKEN ERROR: parser() in parser.c\n");
        return ERROR;
    }

    return ERROR;
}
