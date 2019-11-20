/**
 * @file mid_nvs.c
 * @author Mauricio Farina
 * @brief NVS Middlware
 * @version 0.1
 * @date 2019-08-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "mid_nvs.h"

#include "esp_log.h"

#include "mid_nvs_private.h"

static const char *TAG = "MID_NVS"; /**< Logger Tag (Debugging) */

static SemaphoreHandle_t lockHandler; /**< Component Mutex Semaphore */

static inline bool openNvs(const char *partition, const char *nameSpace, nvs_handle_t *nvsHandler, nvs_open_mode_t mode) {

    esp_err_t error = ESP_OK;

    error = nvs_open_from_partition(partition, nameSpace, mode, nvsHandler);
    ERROR_CHECK(error);

    if (error == ESP_OK) {
        return true;
    } else {
        ESP_LOGE(TAG, "Faild to Open Partition: %s Namespace: %s", partition, nameSpace);
        return false;
    }
}

static inline void loadValue(const char *partition, const char *nameSpace, const char *key, void *buffer, size_t *size, nvs_type_t type) {

    esp_err_t error = ESP_OK;
    nvs_handle_t nvsHandler;

    bool openResult = openNvs(partition, nameSpace, &nvsHandler, NVS_READONLY);

    if (openResult) {

        switch (type) {

        case NVS_TYPE_U8:
            error = nvs_get_u8(nvsHandler, key, (uint8_t *)buffer);
            break;
        case NVS_TYPE_I8:
            error = nvs_get_i8(nvsHandler, key, (int8_t *)buffer);
            break;
        case NVS_TYPE_U16:
            error = nvs_get_u16(nvsHandler, key, (uint16_t *)buffer);
            break;
        case NVS_TYPE_I16:
            error = nvs_get_i16(nvsHandler, key, (int16_t *)buffer);
            break;
        case NVS_TYPE_U32:
            error = nvs_get_u32(nvsHandler, key, (uint32_t *)buffer);
            break;
        case NVS_TYPE_I32:
            error = nvs_get_i32(nvsHandler, key, (int32_t *)buffer);
            break;
        case NVS_TYPE_U64:
            error = nvs_get_u64(nvsHandler, key, (uint64_t *)buffer);
            break;
        case NVS_TYPE_I64:
            error = nvs_get_i64(nvsHandler, key, (int64_t *)buffer);
            break;
        case NVS_TYPE_STR: {
            *size = 0;
            error = nvs_get_str(nvsHandler, key, NULL, size);
            ERROR_CHECK(error);

            if (error == ESP_OK) {
                error = nvs_get_str(nvsHandler, key, (char *)buffer, size);
            }
        } break;
        case NVS_TYPE_BLOB: {
            *size = 0;
            error = nvs_get_blob(nvsHandler, key, NULL, size);
            ERROR_CHECK(error);

            if (error == ESP_OK) {
                error = nvs_get_blob(nvsHandler, key, buffer, size);
            }
        } break;
        case NVS_TYPE_ANY:
        default:
            ESP_LOGE(TAG, "Invalid NVS Type");
            break;
        }

        ERROR_CHECK(error);
    }

    nvs_close(nvsHandler);
}

static inline void writeValue(const char *partition, const char *nameSpace, const char *key, uint64_t value, nvs_type_t type) {

    esp_err_t error = ESP_OK;
    nvs_handle_t nvsHandler;

    bool openResult = openNvs(partition, nameSpace, &nvsHandler, NVS_READWRITE);

    if (openResult) {

        switch (type) {

        case NVS_TYPE_U8:
            error = nvs_set_u8(nvsHandler, key, (uint8_t)value);
            break;
        case NVS_TYPE_I8:
            error = nvs_set_i8(nvsHandler, key, (int8_t)value);
            break;
        case NVS_TYPE_U16:
            error = nvs_set_u16(nvsHandler, key, (uint16_t)value);
            break;
        case NVS_TYPE_I16:
            error = nvs_set_i16(nvsHandler, key, (int16_t)value);
            break;
        case NVS_TYPE_U32:
            error = nvs_set_u32(nvsHandler, key, (uint32_t)value);
            break;
        case NVS_TYPE_I32:
            error = nvs_set_i32(nvsHandler, key, (int32_t)value);
            break;
        case NVS_TYPE_U64:
            error = nvs_set_u64(nvsHandler, key, (uint64_t)value);
            break;
        case NVS_TYPE_I64:
            error = nvs_set_i64(nvsHandler, key, (int64_t)value);
            break;
        case NVS_TYPE_STR:
            ESP_LOGE(TAG, "This function can't save Strings");
            break;
        case NVS_TYPE_BLOB:
            ESP_LOGE(TAG, "This function can't save Blobs");
            break;
        case NVS_TYPE_ANY:
        default:
            ESP_LOGE(TAG, "Invalid NVS Type");
        }

        ERROR_CHECK(error);

        if (error == ESP_OK) {

            error = nvs_commit(nvsHandler);
            ERROR_CHECK(error);
        }
    }

    nvs_close(nvsHandler);
}

static inline void writeString(const char *partition, const char *nameSpace, const char *key, const void *buffer) {

    esp_err_t error = ESP_OK;
    nvs_handle_t nvsHandler;

    bool openResult = openNvs(partition, nameSpace, &nvsHandler, NVS_READWRITE);

    if (openResult) {

        error = nvs_erase_key(nvsHandler, key);
        ERROR_CHECK(error);
        error = nvs_set_str(nvsHandler, key, buffer);
        ERROR_CHECK(error);

        if (error == ESP_OK) {
            error = nvs_commit(nvsHandler);
            ERROR_CHECK(error);
        }
    }
    nvs_close(nvsHandler);
}

static inline void writeBlob(const char *partition, const char *nameSpace, const char *key, const void *buffer, size_t length) {

    esp_err_t error = ESP_OK;
    nvs_handle_t nvsHandler;

    bool openResult = openNvs(partition, nameSpace, &nvsHandler, NVS_READWRITE);

    if (openResult) {

        error = nvs_erase_key(nvsHandler, key);
        ERROR_CHECK(error);
        error = nvs_set_blob(nvsHandler, key, buffer, length);
        ERROR_CHECK(error);

        if (error == ESP_OK) {
            error = nvs_commit(nvsHandler);
            ERROR_CHECK(error);
        }
    }
    nvs_close(nvsHandler);
}

void initMidNvs(void) {

    lockHandler = xSemaphoreCreateMutex();
}

void initNvsPartition(const char *partition) {

    esp_err_t error = ESP_OK;

    ESP_LOGI(TAG, "Initializing NVS Partition %s", partition);

    xSemaphoreTake(lockHandler, portMAX_DELAY);

    error = nvs_flash_init_partition(partition);
    ERROR_CHECK(error);

    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "Erasing Partition %s", partition);
        error = nvs_flash_erase_partition(partition);
        error = nvs_flash_init_partition(partition);
    }

    xSemaphoreGive(lockHandler);
}

bool hasNvsKey(const a_midNvsKeyInfo_t *item) {

    xSemaphoreTake(lockHandler, portMAX_DELAY);

    nvs_iterator_t it = nvs_entry_find(item->partition, item->nameSpace, item->type);

    if (it == NULL) {
        ESP_LOGE(TAG, "NULL Iterator (Partition: %s  Namespace: %s  Type: %d)", item->partition, item->nameSpace, item->type);
    } else {

        while (it != NULL) {
            nvs_entry_info_t info;
            nvs_entry_info(it, &info);

            if (strcmp(item->key, info.key) == 0) {
                nvs_release_iterator(it);
                xSemaphoreGive(lockHandler);
                ESP_LOGI(TAG, "Key %s Found on Partition: %s  Namespace: %s", item->key, item->partition, item->nameSpace);
                return true;
            } else {
                it = nvs_entry_next(it);
            }
        }
    }

    nvs_release_iterator(it);
    xSemaphoreGive(lockHandler);
    ESP_LOGW(TAG, "Key %s Not Found on Partition: %s  Namespace: %s", item->key, item->partition, item->nameSpace);
    return false;
}

size_t getNvsKey(const a_midNvsKeyInfo_t *item, void *buffer) {

    xSemaphoreTake(lockHandler, portMAX_DELAY);
    size_t size = 0;

    loadValue(item->partition, item->nameSpace, item->key, buffer, &size, item->type);
    xSemaphoreGive(lockHandler);
    return size;
}

void setNvsKey(const a_midNvsKeyInfo_t *item, void *buffer, size_t size) {
    xSemaphoreTake(lockHandler, portMAX_DELAY);

    bool flag = (buffer == NULL && size == 0) ? true : false;

    if (flag) {
        ESP_LOGW(TAG, "Initializating Key: %s on Partition: %s  Namespace: %s", item->key, item->partition, item->nameSpace);
    }

    switch (item->type) {

    case NVS_TYPE_STR:

        if (flag) {
            writeString(item->partition, item->nameSpace, item->key, item->string);
        } else {
            writeString(item->partition, item->nameSpace, item->key, (char *)buffer);
        }
        break;

    case NVS_TYPE_BLOB:
        if (flag) {
            writeBlob(item->partition, item->nameSpace, item->key, item->string, item->length);
        } else {
            writeBlob(item->partition, item->nameSpace, item->key, (char *)buffer, size);
        }

        break;

    default:
        if (flag) {
            writeValue(item->partition, item->nameSpace, item->key, item->value, item->type);
        } else {
            writeValue(item->partition, item->nameSpace, item->key, *(uint64_t *)buffer, item->type);
        }

        break;
    }

    xSemaphoreGive(lockHandler);
}

void logNvsKeyList(const char *partition, const char *nameSpace, nvs_type_t type) {

    xSemaphoreTake(lockHandler, portMAX_DELAY);

    nvs_iterator_t it = nvs_entry_find(partition, nameSpace, type);

    if (it == NULL) {
        ESP_LOGE(TAG, "NULL Iterator (Partition: %s  Namespace: %s  Type: %d)", partition, nameSpace, type);
    } else {
        while (it != NULL) {
            nvs_entry_info_t info;
            nvs_entry_info(it, &info);
            it = nvs_entry_next(it);
            ESP_LOGI(TAG, "namespace: '%s'  key '%s'  type '%d'", info.namespace_name, info.key, info.type);
        }
    }

    nvs_release_iterator(it);

    xSemaphoreGive(lockHandler);
}

nvs_stats_t getNvsPartitionStatus(const char *partition) {

    nvs_stats_t nvsStatus;
    esp_err_t error = nvs_get_stats(partition, &nvsStatus);
    ERROR_CHECK(error);
    ESP_LOGI(TAG, "NVS Partition %s Status: Used: %d  Free: %d  Total: %d  Count: %d", partition, nvsStatus.used_entries, nvsStatus.free_entries, nvsStatus.total_entries, nvsStatus.namespace_count);

    return nvsStatus;
}