/**
 * @file mid_nvs_types.h
 * @author Mauricio Farina
 * @brief Middleware NVS Types
 * @version 0.1
 * @date 2019-11-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef __MID_NVS_TYPES_H__
#define __MID_NVS_TYPES_H__

#include "commonDefs.h"

#include "mid_nvs_config.h"

#include "nvs.h"

/**
 * @brief NVS Item Info
 * 
 */
typedef struct {

    char partition[a_MID_NVS_MAX_PARTITION_LENGTH]; /**< Partition Name */
    char nameSpace[a_MID_NVS_MAX_NAMESPACE_LENGTH]; /**< Namespace Name */
    char key[a_MID_NVS_MAX_KEY_LENGTH];             /**< Key Name */
    uint64_t value;                                 /**< Default value if NVS Type is Fixed-Length */
    char string[a_MID_NVS_MAX_STRING_LENGTH];       /**< Default String or Blob if NVS Type is Variable-Length */
    size_t length;                                  /**< Default Blob Size if NVS Type is Variable-Length  */
    nvs_type_t type;                                /**< NVS Type */

} a_midNvsKeyInfo_t;

#endif