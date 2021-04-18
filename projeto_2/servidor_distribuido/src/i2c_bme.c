#include "i2c_bme.h"

void _user_delay_us(uint32_t period, void *intf_ptr);
int8_t _user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t _user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 * @brief This API reads the sensor temperature, pressure and humidity data in forced mode.
 */
int8_t read_temperature_i2c(struct bme280_dev *dev, float *temp)
{
    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    /* Variable to store minimum wait time between consecutive measurement in force mode */
    uint32_t req_delay;

    /* Structure to get the pressure, temperature and humidity values */
    struct bme280_data comp_data;

    /*Calculate the minimum delay required between consecutive measurement based upon the sensor enabled
     *  and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&dev->settings);

    /* Set the sensor to forced mode */
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);

        return rslt;
    }

    /* Wait for the measurement to complete and print data */
    dev->delay_us(req_delay, dev->intf_ptr);
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);

        return rslt;
    }

    *temp = (float)comp_data.temperature;

    return rslt;
}

void setup_i2c_bme(struct identifier *id, struct bme280_dev *dev){
    id->dev_addr = BME280_I2C_ADDR_PRIM;
    dev->intf = BME280_I2C_INTF;
    dev->read = _user_i2c_read;
    dev->write = _user_i2c_write;
    dev->delay_us = _user_delay_us;
}

void open_i2c_conn(struct identifier *id, struct bme280_dev *dev){
    if ((id->fd = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        fprintf(stderr, "Failed to open the i2c bus /dev/i2c-1\n");
        exit(1);
    }


    if (ioctl(id->fd, I2C_SLAVE, id->dev_addr) < 0)
    {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    dev->intf_ptr = id;
}

void initialize_I2C(struct bme280_dev *dev){
    int8_t rslt = BME280_OK;

    rslt = bme280_init(dev);

    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
        exit(1);
    }

    // ================== Set sensor initial configuration ==================

    /* Variable to define the selecting sensors */
    uint8_t settings_sel = 0;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor settings (code %+d).", rslt);
    }

}

// ================================== LOCAL FUNCTIONS ==================================

/*!
 * @brief This function provides the delay for required time (Microseconds) as per the input provided in some of the
 * APIs
 */
void _user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

/*!
 * @brief This function reading the sensor's registers through I2C bus.
 */
int8_t _user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
}

/*!
 * @brief This function for writing the sensor's registers through I2C bus.
 */
int8_t _user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}
