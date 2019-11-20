/**
 * @file mid_nvs_private.h
 * @author Mauricio Farina
 * @brief NVS Middleware Private Functions
 * @version 0.1
 * @date 2019-11-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef __MID_NVS_PRIVATE_H__
#define __MID_NVS_PRIVATE_H__

#include "mid_nvs_types.h"

/**
 * @brief Open NVS Partition
 * 
 * @param partition[in] Partition Name
 * @param nameSpace[in] Namespace Name
 * @param nvsHandler[in] NVS Handler
 * @param mode[in] Open Mode
 * @return true Success
 * @return false Fail
 */
static inline bool openNvs(const char *partition, const char *nameSpace, nvs_handle_t *nvsHandler, nvs_open_mode_t mode);

/**
 * @brief Load NVS Value from Key
 * 
 * @param partition[in] Partition Name 
 * @param nameSpace[in] Namespace Name
 * @param key[in] Key Name
 * @param buffer[out] Buffer where data will be saved 
 * @param size[out] Loaded data length
 * @param type[in] NVS Type
 */
static inline void loadValue(const char *partition, const char *nameSpace, const char *key, void *buffer, size_t *size, nvs_type_t type);

/**
 * @brief Save Fixed-Length NVS Value
 * 
 * @param partition[in] Partition Name 
 * @param nameSpace[in] Namespace Name
 * @param key[in] Key Name
 * @param value[in] Value to be saved
 * @param type[in] NVS Type
 */
static inline void writeValue(const char *partition, const char *nameSpace, const char *key, uint64_t value, nvs_type_t type);

/**
 * @brief Save String to NVS
 * 
 * @param partition[in] Partition Name 
 * @param nameSpace[in] Namespace Name
 * @param key[in] Key Name
 * @param buffer[in] String Buffer
 */
static inline void writeString(const char *partition, const char *nameSpace, const char *key, const void *buffer);

/**
 * @brief Save Blob to NVS
 * 
 * @param partition[in] Partition Name
 * @param nameSpace[in] Namespace Name
 * @param key[in] Key Name
 * @param buffer[in] Blob Buffer 
 * @param length[in] Blob Length
 */
static inline void writeBlob(const char *partition, const char *nameSpace, const char *key, const void *buffer, size_t length);

#endif