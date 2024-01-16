#include "machine.h"


#if defined(_WIN32) || defined(_WIN64)
char *strndup( const char *s1, size_t n)
{
    char *copy= (char*)malloc( n+1 );
    memcpy( copy, s1, n );
    copy[n] = 0;
    return copy;
};
#endif

char * read_a_line(FILE * fp)
{
	static char buf[BUFSIZ] ;
	static int buf_n = 0 ;
	static int curr = 0 ;

	if (feof(fp) && curr == buf_n - 1)
		return 0x0 ;

	char * s = 0x0 ;
	size_t s_len = 0 ;
	do {
		int end = curr ;
		while (!(end >= buf_n || !iscntrl(buf[end]))) {
			end++ ;
		}
		if (curr < end && s != 0x0) {
			curr = end ;
			break ;
		}
		curr = end ;
		while (!(end >= buf_n || iscntrl(buf[end]))) {
			end++ ;
		}
		if (curr < end) {
			if (s == 0x0) {
				s = strndup(buf + curr, end - curr) ;
				s_len = end - curr ;
			}
			else {
				s = realloc(s, s_len + end - curr + 1) ;
				s = strncat(s, buf + curr, end - curr) ;
				s_len = s_len + end - curr ;
			}
		}
		if (end < buf_n) {
			curr = end + 1 ;
			break ;
		}

		buf_n = fread(buf, 1, sizeof(buf), fp) ;
		curr = 0 ;
	} while (buf_n > 0) ;
	return s ;
}

Value * init_value(int size)
{
    Value * value = malloc(sizeof(Value) * size);
    int i;
    for (i = 0; i < size; i++) {
        value[i].num = 0;
    }
    return value;
}

void store_instruction(char * line, Value * value)
{
    char * token = strtok(line, ": ");
    int idx;
    idx = atoi(token);
    
    token = strtok(NULL, " ");
    
    if (token == NULL)
        return;

    if (strcmp(token, "READ") == 0)                    value[idx].instruction.type = READ;
    else if (strcmp(token, "WRITE") == 0)              value[idx].instruction.type = WRITE;
    else if (strcmp(token, "ASSIGN") == 0)             value[idx].instruction.type = ASSIGN;
    else if (strcmp(token, "MOVE") == 0)               value[idx].instruction.type = MOVE;
    else if (strcmp(token, "LOAD") == 0)               value[idx].instruction.type = LOAD;
    else if (strcmp(token, "STORE") == 0)              value[idx].instruction.type = STORE;
    else if (strcmp(token, "ADD") == 0)                value[idx].instruction.type = ADD;
    else if (strcmp(token, "MINUS") == 0)              value[idx].instruction.type = MINUS;
    else if (strcmp(token, "MULT") == 0)               value[idx].instruction.type = MULT;
    else if (strcmp(token, "MOD") == 0)                value[idx].instruction.type = MOD;
    else if (strcmp(token, "DIV") == 0)                value[idx].instruction.type = DIV;
    else if (strcmp(token, "EQ") == 0)                 value[idx].instruction.type = EQ;
    else if (strcmp(token, "LESS") == 0)               value[idx].instruction.type = LESS;
    else if (strcmp(token, "JUMP") == 0)               value[idx].instruction.type = JUMP;
    else if (strcmp(token, "JUMPIF") == 0)             value[idx].instruction.type = JUMPIF;
    else if (strcmp(token, "TERM") == 0)               value[idx].instruction.type = TERM;
    else
    {
        value[idx].num = int_without_quotes(token);
        return;
    }

    store_instruction_map(token, value, idx);
}

int int_without_quotes(char *str)
{
    if (str[0] != '"' || str[strlen(str)-1] != '"')
        error_handling("Error: Invalid instruction.");

    char temp[100];

    strncpy(temp, str+1, strlen(str)-2);
    temp[strlen(str)-2] = '\0';

    return atoi(temp);
}

void store_instruction_map(char * line, Value * value, int idx)
{
    int count = 0;
    int i;
    if (value[idx].instruction.type == READ || value[idx].instruction.type == WRITE || value[idx].instruction.type == JUMP)
        count = 1;
    else if (value[idx].instruction.type == ASSIGN || value[idx].instruction.type == MOVE || value[idx].instruction.type == LOAD || value[idx].instruction.type == STORE || value[idx].instruction.type == JUMPIF)
        count = 2;
    else if (value[idx].instruction.type == ADD || value[idx].instruction.type == MINUS || value[idx].instruction.type == MULT || value[idx].instruction.type == MOD || value[idx].instruction.type == EQ || value[idx].instruction.type == LESS || value[idx].instruction.type == DIV)
        count = 3;
    
    char * token = strtok(line, " ");
    token[strlen(token)] = ' ';

    for(i=0; i<count; i++)
    {
        token = strtok(NULL, " ");
        if (token == NULL)
            error_handling("Error: Invalid instruction.");
        value[idx].instruction.mem[i] = atoi(token);
    }   
} 

int excute_instruction(Value * value, int idx)
{
    if (value[idx].instruction.type == READ)
    {
        int temp;
        scanf("%d", &temp);
        value[value[idx].instruction.mem[0]].num = temp;
    }
    else if (value[idx].instruction.type == WRITE)
        printf("%d\n", value[value[idx].instruction.mem[0]].num);

    else if (value[idx].instruction.type == ASSIGN)
        value[value[idx].instruction.mem[0]].num = value[idx].instruction.mem[1];

    else if (value[idx].instruction.type == MOVE)
        value[value[idx].instruction.mem[0]].num = value[value[idx].instruction.mem[1]].num;

    else if (value[idx].instruction.type == LOAD)
        value[value[idx].instruction.mem[0]].num = value[value[value[idx].instruction.mem[1]].num].num;

    else if (value[idx].instruction.type == STORE)
        value[value[value[idx].instruction.mem[0]].num].num = value[value[idx].instruction.mem[1]].num;

    else if (value[idx].instruction.type == ADD)
        value[value[idx].instruction.mem[0]].num = value[value[idx].instruction.mem[1]].num + value[value[idx].instruction.mem[2]].num;

    else if (value[idx].instruction.type == MINUS)
        value[value[idx].instruction.mem[0]].num = value[value[idx].instruction.mem[1]].num - value[value[idx].instruction.mem[2]].num;

    else if (value[idx].instruction.type == MULT)
        value[value[idx].instruction.mem[0]].num = value[value[idx].instruction.mem[1]].num * value[value[idx].instruction.mem[2]].num;

    else if (value[idx].instruction.type == MOD)
        value[value[idx].instruction.mem[0]].num = value[value[idx].instruction.mem[1]].num % value[value[idx].instruction.mem[2]].num;

    else if (value[idx].instruction.type == DIV)
        value[value[idx].instruction.mem[0]].num = value[value[idx].instruction.mem[1]].num / value[value[idx].instruction.mem[2]].num;
        
    else if (value[idx].instruction.type == EQ)
        value[value[idx].instruction.mem[0]].num = (value[value[idx].instruction.mem[1]].num == value[value[idx].instruction.mem[2]].num);

    else if (value[idx].instruction.type == LESS)
        value[value[idx].instruction.mem[0]].num = (value[value[idx].instruction.mem[1]].num < value[value[idx].instruction.mem[2]].num);

    else if (value[idx].instruction.type == JUMP)
        return value[idx].instruction.mem[0];

    else if (value[idx].instruction.type == JUMPIF)
    {
        if (value[value[idx].instruction.mem[1]].num != 0)
            return value[idx].instruction.mem[0];
    }
    else if (value[idx].instruction.type == TERM)
        return -1;

    return idx+1;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}