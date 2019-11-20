/**
 * @file main.c
 * @author Mauricio Farina
 * @brief Main
 * @version 0.1
 * @date 2019-07-18
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS_Libraries.h"
#include "commonDefs.h"

#include "esp_log.h"

#include "mid_nvs.h"

#define STEP_DELAY 1000 /**< Delay Between Demo Steps */

#define PARTITION_NVS "nvs"             /**< NVS Partition nvs */
#define PARTITION_CONFIG "config"       /**< NVS Partition config */
#define PARTITION_OTACONFIG "otaConfig" /**< NVS Partition otaConfig */

#define THE_NAMESPACE "SYSTEM_INFO" /**< NVS Namespace SYSTEM_INFO */

static const char *TAG = "MAIN"; /**< Logger Tag (Debugging) */

static const a_midNvsKeyInfo_t NVS_KEY_STR_0 = {.partition = PARTITION_CONFIG,
                                                .nameSpace = THE_NAMESPACE,
                                                .key = "STR_0",
                                                .value = 0,
                                                .string = "InitString0",
                                                .length = sizeof("InitString0"),
                                                .type = NVS_TYPE_STR};

static const a_midNvsKeyInfo_t NVS_KEY_STR_1 = {.partition = PARTITION_CONFIG,
                                                .nameSpace = THE_NAMESPACE,
                                                .key = "STR_1",
                                                .value = 0,
                                                .string = "InitString1",
                                                .length = sizeof("InitString1"),
                                                .type = NVS_TYPE_STR};

static const a_midNvsKeyInfo_t NVS_KEY_U32_0 = {.partition = PARTITION_CONFIG,
                                                .nameSpace = THE_NAMESPACE,
                                                .key = "U32_0",
                                                .value = 0,
                                                .string = "",
                                                .length = sizeof(uint32_t),
                                                .type = NVS_TYPE_U32};

static const a_midNvsKeyInfo_t NVS_KEY_U32_1 = {.partition = PARTITION_CONFIG,
                                                .nameSpace = THE_NAMESPACE,
                                                .key = "U32_1",
                                                .value = 0,
                                                .string = "",
                                                .length = sizeof(uint32_t),
                                                .type = NVS_TYPE_U32};

static void initNvsPartitions(void) {
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "Initializing Application");
    initMidNvs();
    initNvsPartition(PARTITION_NVS);
    initNvsPartition(PARTITION_CONFIG);
    initNvsPartition(PARTITION_OTACONFIG);
    ESP_LOGI(TAG, "Initialization Complete");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    vTaskDelay(STEP_DELAY);
}

static void printNvsPartitionsStatus(void) {
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "Loading Partitions Status");
    getNvsPartitionStatus(PARTITION_NVS);
    getNvsPartitionStatus(PARTITION_CONFIG);
    getNvsPartitionStatus(PARTITION_OTACONFIG);
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    vTaskDelay(STEP_DELAY);
}

static void checkNvsKeys(void) {
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "Checking NVS Keys");

    if (!hasNvsKey(&NVS_KEY_STR_0)) {
        ESP_LOGW(TAG, "NVS_KEY_STR_0 Not Found");
        ESP_LOGI(TAG, "Initializing NVS_KEY_STR_0");
        setNvsKey(&NVS_KEY_STR_0, NULL, 0);
    }

    if (!hasNvsKey(&NVS_KEY_STR_1)) {
        ESP_LOGW(TAG, "NVS_KEY_STR_1 Not Found");
        ESP_LOGI(TAG, "Initializing NVS_KEY_STR_1");
        setNvsKey(&NVS_KEY_STR_1, NULL, 0);
    }

    if (!hasNvsKey(&NVS_KEY_U32_0)) {
        ESP_LOGW(TAG, "NVS_KEY_U32_0 Not Found");
        ESP_LOGI(TAG, "Initializing NVS_KEY_U32_0");
        setNvsKey(&NVS_KEY_U32_0, NULL, 0);
    }

    if (!hasNvsKey(&NVS_KEY_U32_1)) {
        ESP_LOGW(TAG, "NVS_KEY_U32_1 Not Found");
        ESP_LOGI(TAG, "Initializing NVS_KEY_U32_1");
        setNvsKey(&NVS_KEY_U32_1, NULL, 0);
    }

    ESP_LOGI(TAG, "Check Complete");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    vTaskDelay(STEP_DELAY);
}

static void getNvsKeys(void) {
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "Reading NVS Keys");

    const size_t bufferSize = 128;
    char buffer[bufferSize];

    memset(buffer, 0, bufferSize);
    getNvsKey(&NVS_KEY_STR_0, buffer);
    ESP_LOGI(TAG, "NVS_KEY_STR_0: %s", buffer);

    memset(buffer, 0, bufferSize);
    getNvsKey(&NVS_KEY_STR_1, buffer);
    ESP_LOGI(TAG, "NVS_KEY_STR_1: %s", buffer);

    uint32_t value;

    value = 0;
    getNvsKey(&NVS_KEY_U32_0, &value);
    ESP_LOGI(TAG, "NVS_KEY_U32_0: %d", value);

    value = 0;
    getNvsKey(&NVS_KEY_U32_1, &value);
    ESP_LOGI(TAG, "NVS_KEY_U32_1: %d", value);

    ESP_LOGI(TAG, "Read Complete");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    vTaskDelay(STEP_DELAY);
}

static void setNvsKeys(void) {
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "Writting NVS Keys");
    setNvsKey(&NVS_KEY_STR_0, "NewString0", sizeof("NewString0"));
    setNvsKey(&NVS_KEY_STR_1, "NewString1", sizeof("NewString1"));
    uint32_t newValue = 0xFF;
    setNvsKey(&NVS_KEY_U32_0, &newValue, sizeof(uint32_t));
    setNvsKey(&NVS_KEY_U32_1, &newValue, sizeof(uint32_t));
    ESP_LOGI(TAG, "Write Complete");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "*********************************************");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    vTaskDelay(STEP_DELAY);
}

void app_main(void) {

    vTaskDelay(STEP_DELAY);
    ESP_LOGI(TAG, "\n\n\n\n\n\n\n\n\n");

    //STEP 1
    initNvsPartitions(); //Init Partitions

    //STEP 2
    printNvsPartitionsStatus(); //Print NVS Status

    //STEP 3
    checkNvsKeys(); //Check If Keys exist on NVS and init the ones that are not

    //STEP 4
    printNvsPartitionsStatus(); //Print NVS Status

    //STEP 5
    getNvsKeys(); //Load NVS Keys

    //STEP 6
    printNvsPartitionsStatus(); //Print NVS Status

    /*
     *  Close Monitor and Restart the ESP32 with "make monitor" 
     * 
     *  In some tests, using the esp_reset() function didn't cause the bug to happen 
     * 
     *  Also, after the second reboot the bug doesn't happen anymore
     * 
     *  If the While Loop is removed and the process continues the bug doesn't occur
     */

    //esp_restart();

    while (1) {
        vTaskDelay(5000);
    }

    //STEP 7
    setNvsKeys();

    //STEP 8
    printNvsPartitionsStatus(); //Print NVS Status

    //STEP 9
    getNvsKeys();

    //STEP 10
    printNvsPartitionsStatus(); //Print NVS Status

    vTaskDelay(5000);

    ESP_LOGI(TAG, "Aplicacao Concluida, Reiniciando ESP32");

    vTaskDelay(1000);

    esp_restart();

    while (1) {

        vTaskDelay(1000);
    }
}
