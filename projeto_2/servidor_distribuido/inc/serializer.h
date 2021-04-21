#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void serialize(char *buff, int *size, char key[], int value, char flag[]);
void serialize_temperature(char *buff, float temperature, float humidity);

#endif
