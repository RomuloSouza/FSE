#include <serializer.h>

void serialize(char *buff, int *size, char key[], int value, char flag[]){
    char d_flag[] = "%d";

    memcpy(buff + *size, (const void*) key, strlen(key));
    *size += strlen(key);

    sprintf(&buff[(*size)++], flag, value);
    buff[(*size)++] = ';';
}
