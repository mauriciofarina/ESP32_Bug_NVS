/**
 * @file mid_nvs.h
 * @author Mauricio Farina
 * @brief NVS Middleware
 * @version 0.1
 * @date 2019-08-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef __MID_NVS_H__
#define __MID_NVS_H__

#include "FreeRTOS_Libraries.h"
#include "commonDefs.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mid_nvs_config.h"
#include "mid_nvs_types.h"

#include "nvs_flash.h"

/**
 * @brief NVS Inicialization
 * 
 */
void initMidNvs(void);

/**
 * @brief Init a NVS Partition
 * 
 * @param partition[in] Partition Name
 */
void initNvsPartition(const char *partition);

/**
 * @brief Check if NVS Key Exists
 * 
 * @param item[in] NVS Key
 * @return true Found
 * @return false Not Found
 */
bool hasNvsKey(const a_midNvsKeyInfo_t *item);

/**
 * @brief Read NVS Key
 * 
 * @param item[in] NVS Key
 * @param buffer[out] Buffer
 * @return size_t Size of Value of NVS Key
 */
size_t getNvsKey(const a_midNvsKeyInfo_t *item, void *buffer);

/**
 * @brief Write NVS Key
 * 
 * @param item[in] NVS Key
 * @param buffer[in] Buffer with data to be saved
 * @param size[in] Size of Buffer
 */
void setNvsKey(const a_midNvsKeyInfo_t *item, void *buffer, size_t size);

/**
 * @brief Print All NVS Keys on a Partition Namespace
 * 
 * @param partition[in] Partition Name
 * @param nameSpace[in] Namespace Name
 * @param type[in] NVS Type
 */
void logNvsKeyList(const char *partition, const char *nameSpace, nvs_type_t type);

/**
 * @brief Return the Status of a NVS Partition
 * 
 * @param partition[in] Partition Name
 * @return nvs_stats_t NVS Partition Status
 */
nvs_stats_t getNvsPartitionStatus(const char *partition);

#endif