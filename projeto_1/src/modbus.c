#include <modbus.h>

#define DELAY 100000

int _request_temperature(char code);
float _read_temperature();

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

float read_temperature(char code){
    int status = _request_temperature(code);
    if (status == -1)
        return status;

    usleep(DELAY);

    return _read_temperature();

}

int _request_temperature(char code){
    unsigned char tx_buffer[20] = { 0x01, 0x23, code, 0x07, 0x06, 0x00, 0x01 };

    short crc = calcula_CRC(tx_buffer, 7);
    memcpy(&tx_buffer[7], &crc, 2);

    if (uart0_filestream == -1)
        return -1;

    int count = write(uart0_filestream, tx_buffer, 7+2); // 2 bytes of CRC
    if (count < 0){
        printf("UART TX error\n");
        return -1;
    }

    printf("escrito.\n");
    return 0;
}


float _read_temperature(){
    // Read up to 255 characters from the port if they are there
    unsigned char rx_buffer[256];
    float temperature = -1;

    if (uart0_filestream == -1)
        return -1;

    int rx_length = read(uart0_filestream, (void*)rx_buffer, 255); //Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0){
        printf("Error reading from UART.\n"); //An error occured (will occur if there are no bytes)
    } else if (rx_length == 0){
        printf("No data available from UART.\n"); //No data waiting
    } else{
        // printf("CRC calculado = %d\n", calcula_CRC(rx_buffer, 7));

        short crc_rec;
        memcpy(&crc_rec, &rx_buffer[7], 2);
        // printf("CRC recebido = %d\n", crc_rec);

        memcpy(&temperature, &rx_buffer[3], 4);
        // printf("%i Bytes lidos\n", rx_length);
    }

    return temperature;
}


