/**
 * @file commonDefs.h
 * @author Mauricio Farina
 * @brief Generic Definitions
 * @version 0.1
 * @date 2019-10-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"

#include "esp_bit_defs.h"

//KB Sizes
#define SIZE_1K 1024
#define SIZE_2K 2048
#define SIZE_3K 3072
#define SIZE_4K 4096
#define SIZE_5K 5120
#define SIZE_6K 6144
#define SIZE_7K 7168
#define SIZE_8K 8192
#define SIZE_9K 9216
#define SIZE_10K 10240

#define ERROR_CHECK(error)                                                                                  \
    {                                                                                                       \
        if (error != ESP_OK) {                                                                              \
            ESP_LOGE(TAG, "");                                                                              \
            ESP_LOGE(TAG, "ESP ERROR: %s  File: %s  Line: %d", esp_err_to_name(error), __FILE__, __LINE__); \
            ESP_LOGE(TAG, "");                                                                              \
        }                                                                                                   \
    }

#endif