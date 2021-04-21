#include <serializer.h>

void serialize(char *buff, int *size, char key[], int value, char flag[]){
    memcpy(buff + *size, (const void*) key, strlen(key));
    *size += strlen(key);

    sprintf(&buff[(*size)++], flag, value);
    buff[(*size)++] = ';';

    buff[*size] = '\0';
}

void serialize_temperature(char *buff, float temperature, float humidity){
    char flag[] = "%d.%04d";
    int size = 0;

    char str_temperature[] = "temperature:";
    memcpy(buff + size, (const void*) str_temperature, strlen(str_temperature));
    size += strlen(str_temperature);

    int int_temperature = temperature;
    float dec = temperature - int_temperature;
    int int_dec_temperature = dec * 10000;
    sprintf(&buff[size], flag, int_temperature, int_dec_temperature);
    size += 7; // <2_digit_number>.<4_digit_decimal>

    buff[size++] = ';';

    char str_humidity[] = "humidity:";
    memcpy(buff + size, (const void*) str_humidity, strlen(str_humidity));
    size += strlen(str_humidity);

    int int_humidity = humidity;
    dec = humidity - int_humidity;
    int int_dec_humidity = dec * 10000;
    sprintf(&buff[size], flag, int_humidity, int_dec_humidity);
    size += 7; // <number>.<4_digit_decimal>

    buff[size++] = ';';

    buff[size] = '\0';
}
