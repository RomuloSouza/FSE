#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <crc16.h>
#include <string.h>

#define DELAY 1

int uart0_filestream = -1;

void initialize_UART(){
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY); //Open in non blocking read/write mode
    if (uart0_filestream == -1){
        printf("Error when trying to initialize UART.\n");
    } else{
        printf("UART initialized!\n");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void close_UART(){
    close(uart0_filestream);
}

float read_intern_temperature(){
    unsigned char tx_buffer[20] = { 0x01, 0x23, 0xC1, 0x07, 0x06, 0x00, 0x01 };

    short crc = calcula_CRC(tx_buffer, 7);
    memcpy(&tx_buffer[7], &crc, 2);

    if (uart0_filestream == -1)
        return -1;

    int count = write(uart0_filestream, tx_buffer, 7+2); // 2 bytes of CRC
    if (count < 0){
        printf("UART TX error\n");
    } else{
        printf("escrito.\n");
    }

    sleep(DELAY);

    if (uart0_filestream == -1)
        return -1;

    // Read up to 255 characters from the port if they are there
    unsigned char rx_buffer[256];
    float temperature = -1;
    int rx_length = read(uart0_filestream, (void*)rx_buffer, 255); //Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0){
        printf("Error reading from UART.\n"); //An error occured (will occur if there are no bytes)
    } else if (rx_length == 0){
        printf("No data available.\n"); //No data waiting
    } else{
        // rx_buffer[rx_length] = '\0';

        printf("CRC calculado = %d\n", calcula_CRC(rx_buffer, 7));

        short crc_rec;
        memcpy(&crc_rec, &rx_buffer[7], 2);
        printf("CRC recebido = %d\n", crc_rec);

        memcpy(&temperature, &rx_buffer[3], 4);
        printf("Temperature = %f\n", temperature);

        printf("%i Bytes lidos : %s\n", rx_length, rx_buffer);
    }

    return temperature;

}


// float read_ref_temperature(){

// }


// int main(int argc, const char * argv[]) {

//     // 7601
//     unsigned char tx_buffer[20] = { 0x01, 0x23, 0xC1, 0x07, 0x06, 0x00, 0x01 };

//     short crc = calcula_CRC(tx_buffer, 7);

//     memcpy(&tx_buffer[7], &crc, 2);

//     printf("Buffers de memória criados!\n");
    
//     if (uart0_filestream != -1)
//     {
//         printf("Escrevendo caracteres na UART ...");
//         int count = write(uart0_filestream, tx_buffer, 7+2); // 2 bytes of CRC
//         if (count < 0)
//         {
//             printf("UART TX error\n");
//         }
//         else
//         {
//             printf("escrito.\n");
//         }
//     }

//     sleep(1);

//     //----- CHECK FOR ANY RX BYTES -----
//     if (uart0_filestream != -1)
//     {
//         // Read up to 255 characters from the port if they are there
//         unsigned char rx_buffer[256];
//         int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
//         if (rx_length < 0)
//         {
//             printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
//         }
//         else if (rx_length == 0)
//         {
//             printf("Nenhum dado disponível.\n"); //No data waiting
//         }
//         else
//         {
//             rx_buffer[rx_length] = '\0';

//             printf("CRC calculado = %d\n", calcula_CRC(rx_buffer, 7));

//             short crc_rec;
//             memcpy(&crc_rec, &rx_buffer[7], 2);
//             printf("CRC recebido = %d\n", crc_rec);

//             float temperature;
//             memcpy(&temperature, &rx_buffer[3], 4);
//             printf("Temperature = %f\n", temperature);

//             printf("%i Bytes lidos : %s\n", rx_length, rx_buffer);
//         }
//     }

//     close(uart0_filestream);
//    return 0;
// }
