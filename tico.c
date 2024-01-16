#include "machine.h"

int main(int argc, char *argv[])
{
    Value * value = init_value(256);
    int execute_idx = 0;
    FILE * fp;
    char *s = 0x0;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Error opening file %s for reading ", argv[1]);
        exit(1);
    }

    while ((s = read_a_line(fp)) != NULL) {
        store_instruction(s, value);
        free(s);
    }
    fclose(fp);
    
    while (TRUE)
    {
        execute_idx = excute_instruction(value, execute_idx);
        if (execute_idx == -1)
            break;
    }
    
    return 0;
}
