/*
 * at86rf212 ISM Band Radio IC driver
 *
 * Copyright 2016 Ryan Kurte
 */

#ifndef AT86RF212_H
#define AT86RF212_H

#include <stdint.h>

#include "at86rf212_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

enum at86rf212_result_e {
    AT86RF212_RES_OK = 0,
    AT86RF212_RES_DONE = 1,
    AT86RF212_DRIVER_INVALID = -1,
    AT86RF212_ERROR_DRIVER = -2,
    AT86RF212_ERROR_COMMS = -3,
    AT86RF212_ERROR_LEN = -4,
    AT86RF212_ERROR_RETRIES = -5,
    AT86RF212_ERROR_PLL = -6,
    AT86RF212_ERROR_DVDD = -7,
    AT86RF212_ERROR_AVDD = -8
};

// SPI interaction function for dependency injection
typedef int (*spi_transfer_f)(void* context, int len, uint8_t *data_out, uint8_t* data_in);
typedef int (*gpio_set_f)(void* context, uint8_t val);
typedef int (*gpio_get_f)(void* context, uint8_t *val);

// Driver object for passing in to AT86RF212 object
struct at86rf212_driver_s {
    spi_transfer_f spi_transfer;    //!< SPI transfer function
    gpio_set_f set_reset;           //!< Reset pin control
    gpio_set_f set_slp_tr;          //!< Sleep and transmit pin control
    gpio_get_f get_irq;             //!< Get IRQ pin value
    gpio_get_f get_dig1;            //!< Get DIG1 pin value
    gpio_get_f get_dig2;            //!< Get DIG2 pin value
};

/****       Initialization           ****/

// Create an at86rf212 device
// Note that the device and driver objects must continue to exist outside this scope.
int at86rf212_init(struct at86rf212_s *device, struct at86rf212_driver_s *driver, void* driver_ctx);

// Close an at86rf212 device
int at86rf212_close(struct at86rf212_s *device);

// State functions
int at86rf212_set_state(struct at86rf212_s *device, uint8_t state);
int at86rf212_set_state_blocking(struct at86rf212_s *device, uint8_t state);
int at86rf212_get_state(struct at86rf212_s *device, uint8_t *state);

// Channel functions
int at86rf212_set_channel(struct at86rf212_s *device, uint8_t channel);
int at86rf212_get_channel(struct at86rf212_s *device, uint8_t *channel);

int at86rf212_set_power_raw(struct at86rf212_s *device, uint8_t power);

// Address and filtering functions
int at86rf212_set_short_address(struct at86rf212_s *device, uint16_t address);
int at86rf212_set_pan_id(struct at86rf212_s *device, uint16_t pan_id);

// IRQ functions
int at86rf212_set_irq_mask(struct at86rf212_s *device, uint8_t mask);
int at86rf212_get_irq_status(struct at86rf212_s *device, uint8_t *status);

// Transmit functions
int at86rf212_start_tx(struct at86rf212_s *device, uint8_t length, uint8_t* data);
int at86rf212_check_tx(struct at86rf212_s *device);

// Receive functions
int at86rf212_start_rx(struct at86rf212_s *device);
int at86rf212_check_rx(struct at86rf212_s *device);
int at86rf212_get_rx(struct at86rf212_s *device, uint8_t* length, uint8_t* data);

// Register functions
int at86rf212_read_reg(struct at86rf212_s *device, uint8_t reg, uint8_t* val);
int at86rf212_write_reg(struct at86rf212_s *device, uint8_t reg, uint8_t val);
int at86rf212_update_reg(struct at86rf212_s *device, uint8_t reg, uint8_t mask, uint8_t val);

#ifdef __cplusplus
}
#endif

#endif
