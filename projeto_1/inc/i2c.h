#ifndef I2C_H
#define I2C_H

#include <bme280.h>

struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;

    /* Variable that contains file descriptor */
    int8_t fd;
};


void open_i2c_conn(struct identifier *id);
void initialize_I2C(struct identifier *id, struct bme280_dev *dev);

void user_delay_us(uint32_t period, void *intf_ptr);
void print_sensor_data(struct bme280_data *comp_data);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
int8_t read_temperature_i2c(struct bme280_dev *dev, float *temp);

#endif /* I2C_H */
