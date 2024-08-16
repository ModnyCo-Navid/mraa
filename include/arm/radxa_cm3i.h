/*
 * Author: Nascs <nascs@radxa.com>
 * Copyright (c) 2023 Radxa Limited.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mraa_internal.h"

#define MRAA_RADXA_CM3I_GPIO_COUNT 59
#define MRAA_RADXA_CM3I_I2C_COUNT 2
#define MRAA_RADXA_CM3I_SPI_COUNT 0
#define MRAA_RADXA_CM3I_UART_COUNT 2
#define MRAA_RADXA_CM3I_PWM_COUNT 0
#define MRAA_RADXA_CM3I_AIO_COUNT 0
#define MRAA_RADXA_CM3I_PIN_COUNT 400
#define PLATFORM_NAME_RADXA_CM3I "Radxa CM3I Test"

mraa_board_t* mraa_radxa_cm3i();

#ifdef __cplusplus
}
#endif
