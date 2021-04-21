#ifndef SERVER_H_
#define SERVER_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void serialize(char *buff, int *size, char key[], int value, char flag[]);

#endif
