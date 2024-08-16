/*
 * Author: Nascs <nascs@radxa.com>
 * Copyright (c) 2023 Radxa Limited.
 *
 * SPDX-License-Identifier: MIT
 */

#include "arm/radxa_cm3i.h"
#include "common.h"
#include <mraa/common.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

const char* radxa_cm3i_serialdev[MRAA_RADXA_CM3I_UART_COUNT] = { "/dev/ttyS0", "/dev/ttyS2", "/dev/ttyS5" };

void
mraa_radxa_cm3i_pininfo(mraa_board_t* board, int index, int gpio_chip, int gpio_line, mraa_pincapabilities_t pincapabilities_t, char* pin_name)
{

    if (index > board->phy_pin_count)
        return;

    mraa_pininfo_t* pininfo = &board->pins[index];
    strncpy(pininfo->name, pin_name, MRAA_PIN_NAME_SIZE);

    if (pincapabilities_t.gpio == 1) {
        pininfo->gpio.gpio_chip = gpio_chip;
        pininfo->gpio.gpio_line = gpio_line;
    }

    pininfo->capabilities = pincapabilities_t;

    pininfo->gpio.mux_total = 0;
}

mraa_board_t*
mraa_radxa_cm3i()
{
    mraa_board_t* b = (mraa_board_t*) calloc(1, sizeof(mraa_board_t));
    if (b == NULL) {
        return NULL;
    }

    b->adv_func = (mraa_adv_func_t*) calloc(1, sizeof(mraa_adv_func_t));
    if (b->adv_func == NULL) {
        free(b);
        return NULL;
    }

    // pin mux for buses are setup by default by kernel so tell mraa to ignore them
    b->no_bus_mux = 1;
    b->phy_pin_count = MRAA_RADXA_CM3I_PIN_COUNT + 1;

    if (mraa_file_contains("/proc/device-tree/model", PLATFORM_NAME_RADXA_CM3I)) {
        b->platform_name = PLATFORM_NAME_RADXA_CM3I;
    } else {
        printf("An unknown product detected. Fail early...\n");
        exit(-1);
    }

    b->chardev_capable = 1;

    // UART
    b->uart_dev_count = MRAA_RADXA_CM3I_UART_COUNT;
    b->def_uart_dev = 0;
    b->uart_dev[0].index = 0;
    b->uart_dev[1].index = 2;
    b->uart_dev[2].index = 5;
    b->uart_dev[0].device_path = (char*) radxa_cm3i_serialdev[0];
    b->uart_dev[1].device_path = (char*) radxa_cm3i_serialdev[1];
    b->uart_dev[2].device_path = (char*) radxa_cm3i_serialdev[2];

    // I2C
    b->i2c_bus_count = MRAA_RADXA_CM3I_I2C_COUNT;
    b->def_i2c_bus = 0;
    b->i2c_bus[0].bus_id = 3;
    b->i2c_bus[1].bus_id = 2;

    // SPI
    // b->spi_bus_count = MRAA_RADXA_CM3I_SPI_COUNT;
    // b->def_spi_bus = 0;
    // b->spi_bus[0].bus_id = 1;

    // // PWM
    // b->pwm_dev_count = MRAA_RADXA_CM3I_PWM_COUNT;
    // b->pwm_default_period = 500;
    // b->pwm_max_period = 2147483;
    // b->pwm_min_period = 1;

    b->pins = (mraa_pininfo_t*) malloc(sizeof(mraa_pininfo_t) * b->phy_pin_count);
    if (b->pins == NULL) {
        free(b->adv_func);
        free(b);
        return NULL;
    }

    // b->pins[7].pwm.parent_id = 12; // pwm12-m0
    // b->pins[7].pwm.mux_total = 0;
    // b->pins[7].pwm.pinmap = 0;

    // mraa_radxa_cm3i_pininfo(b, 0, -1, -1, (mraa_pincapabilities_t){0,0,0,0,0,0,0,0}, "INVALID");
    // mraa_radxa_cm3i_pininfo(b, 1, -1, -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    // mraa_radxa_cm3i_pininfo(b, 2, -1, -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_radxa_cm3i_pininfo(b, 21, 4, 22, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO4_C6");
    mraa_radxa_cm3i_pininfo(b, 24, 3, 21, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_C5");
    mraa_radxa_cm3i_pininfo(b, 25, 3, 5, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A5");
    mraa_radxa_cm3i_pininfo(b, 26, 3, 19, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 }, "GPIO3_C3");
    mraa_radxa_cm3i_pininfo(b, 27, 3, 6, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A6");
    mraa_radxa_cm3i_pininfo(b, 28, 2, 29, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D5");
    mraa_radxa_cm3i_pininfo(b, 29, 3, 4, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A4");
    mraa_radxa_cm3i_pininfo(b, 30, 2, 31, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D7");
    mraa_radxa_cm3i_pininfo(b, 31, 3, 15, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_B7");
    mraa_radxa_cm3i_pininfo(b, 34, 3, 0, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A0");
    mraa_radxa_cm3i_pininfo(b, 35, 3, 11, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_B3");
    mraa_radxa_cm3i_pininfo(b, 36, 3, 12, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_B4");
    mraa_radxa_cm3i_pininfo(b, 37, 0, 8, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_B0");
    mraa_radxa_cm3i_pininfo(b, 38, 2, 27, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D3");
    mraa_radxa_cm3i_pininfo(b, 39, 2, 26, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D2");
    mraa_radxa_cm3i_pininfo(b, 40, 2, 24, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D0");
    mraa_radxa_cm3i_pininfo(b, 41, 3, 2, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A2");
    mraa_radxa_cm3i_pininfo(b, 44, 2, 25, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D1");
    mraa_radxa_cm3i_pininfo(b, 45, 3, 1, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A1");
    mraa_radxa_cm3i_pininfo(b, 46, 3, 16, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_C0");
    mraa_radxa_cm3i_pininfo(b, 47, 3, 17, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_C1");
    mraa_radxa_cm3i_pininfo(b, 48, 3, 9, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_B1");
    mraa_radxa_cm3i_pininfo(b, 49, 3, 18, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 }, "GPIO3_C2");
    mraa_radxa_cm3i_pininfo(b, 50, 3, 10, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_B2");
    mraa_radxa_cm3i_pininfo(b, 51, 0, 24, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 }, "GPIO0_D0");
    mraa_radxa_cm3i_pininfo(b, 54, 2, 30, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D6");
    mraa_radxa_cm3i_pininfo(b, 55, 0, 25, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 }, "GPIO0_D1");
    mraa_radxa_cm3i_pininfo(b, 56, 3, 13, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 }, "GPIO3_B5");
    mraa_radxa_cm3i_pininfo(b, 57, 2, 2, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_A2");
    mraa_radxa_cm3i_pininfo(b, 58, 3, 14, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 }, "GPIO3_B6");
    mraa_radxa_cm3i_pininfo(b, 61, 2, 0, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_A0");
    mraa_radxa_cm3i_pininfo(b, 62, 2, 1, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_A1");
    mraa_radxa_cm3i_pininfo(b, 63, 1, 29, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO1_D5");
    mraa_radxa_cm3i_pininfo(b, 67, 1, 30, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO1_D6");
    mraa_radxa_cm3i_pininfo(b, 69, 1, 31, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO1_D7");
    mraa_radxa_cm3i_pininfo(b, 75, 0, 5, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_A5");
    mraa_radxa_cm3i_pininfo(b, 76, 0, 4, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_A4");
    mraa_radxa_cm3i_pininfo(b, 89, 2, 28, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO2_D4");
    mraa_radxa_cm3i_pininfo(b, 91, 3, 3, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A3");
    mraa_radxa_cm3i_pininfo(b, 95, 3, 20, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_C4");
    mraa_radxa_cm3i_pininfo(b, 97, 3, 8, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_B0");
    mraa_radxa_cm3i_pininfo(b, 304, 0, 14, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 }, "GPIO0_B6");
    mraa_radxa_cm3i_pininfo(b, 306, 0, 13, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 }, "GPIO0_B5");
    mraa_radxa_cm3i_pininfo(b, 338, 4, 26, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO4_D2");
    mraa_radxa_cm3i_pininfo(b, 340, 4, 21, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO4_C5");
    mraa_radxa_cm3i_pininfo(b, 344, 0, 11, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_B3");
    mraa_radxa_cm3i_pininfo(b, 346, 0, 12, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_B4");
    mraa_radxa_cm3i_pininfo(b, 370, 4, 20, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO4_C4");
    mraa_radxa_cm3i_pininfo(b, 374, 0, 0, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_A0");
    mraa_radxa_cm3i_pininfo(b, 375, 0, 16, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 }, "GPIO0_C0");
    mraa_radxa_cm3i_pininfo(b, 377, 0, 17, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 }, "GPIO0_C1");
    mraa_radxa_cm3i_pininfo(b, 379, 0, 18, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_C2");
    mraa_radxa_cm3i_pininfo(b, 381, 0, 19, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_C3");
    mraa_radxa_cm3i_pininfo(b, 383, 0, 20, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_C4");
    mraa_radxa_cm3i_pininfo(b, 385, 0, 21, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_C5");
    mraa_radxa_cm3i_pininfo(b, 387, 0, 22, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_C6");
    mraa_radxa_cm3i_pininfo(b, 389, 0, 23, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_C7");
    mraa_radxa_cm3i_pininfo(b, 391, 0, 15, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO0_B7");
    mraa_radxa_cm3i_pininfo(b, 395, 3, 20, (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, "GPIO3_A7");

    return b;
}
