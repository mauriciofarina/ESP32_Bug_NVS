# ESP32 NVS Bug


## Problem Description

In this Firmware, 4 NVS Keys are written to the NVS partition. After running the firmware, for the first time, and rebooting the device, the Strings and Blobs are lost. Other Fixed-Length variables are not affected by this problem.

### Expected Behavior

Write the variables to the NVS partition, reboot the device, be able to retrieve saved data.

### Actual Behavior

After writing the NVS Keys for the first time and rebooting the device, the variable-length Keys are lost.

# How to reproduce the error

## Environment

- Development Kit: none
- Module or chip used: ESP32-WROOM-32
- IDF version: v4.0-dev-1287-gd7e659df2 
- Build System: Make
- Compiler version: xtensa-esp32-elf-gcc (crosstool-NG esp32-2019r1) 8.2.0
- Operating System:	Windows
- Power Supply: external 3.3V


## Steps to reproduce

### Initial Setup

1. Erase the ESP32 Flash
2. Compile the Firmware
3. Flash the Firmware to the ESP32
4. Open the Serial Monitor

```
make -j9 erase_flash all flash monitor
```

### Runtime

In order to reproduce the problem, a demo will run after the boot. This demo tries to save 4 variables to the NVS partition.

| Partition |  Namespace  |      Key      |   Type   |
| :-------: | :---------: | :-----------: | :------: |
|  config   | SYSTEM_INFO | NVS_KEY_STR_0 |  String  |
|  config   | SYSTEM_INFO | NVS_KEY_STR_1 |  String  |
|  config   | SYSTEM_INFO | NVS_KEY_U32_0 | uint32_t |
|  config   | SYSTEM_INFO | NVS_KEY_U32_1 | uint32_t |


---
**NOTE**

Step numbers can be found on the comments of the app_main function

---

#### First Run

In step 2 it is possible to see that the `config` partition is empty:

```
I (3551) MAIN: *********************************************
I (3551) MAIN: *********************************************
I (3557) MAIN: Loading Partitions Status
I (3562) MID_NVS: NVS Partition nvs Status: Used: 0  Free: 504  Total: 504  Count: 0
I (3570) MID_NVS: NVS Partition config Status: Used: 0  Free: 28728  Total: 28728  Count: 0
I (3579) MID_NVS: NVS Partition otaConfig Status: Used: 0  Free: 504  Total: 504  Count: 0
I (3588) MAIN: *********************************************
I (3594) MAIN: *********************************************
```

In step 3, the demo checks if the keys exist and, since they don't, creates them on the NVS:

```
I (4605) MAIN: *********************************************
I (4605) MAIN: *********************************************
I (4611) MAIN: Checking NVS Keys
E (4615) MID_NVS: NULL Iterator (Partition: config  Namespace: SYSTEM_INFO  Type: 33)
W (4623) MID_NVS: Key STR_0 Not Found on Partition: config  Namespace: SYSTEM_INFO
W (4632) MAIN: NVS_KEY_STR_0 Not Found
I (4636) MAIN: Initializing NVS_KEY_STR_0
W (4641) MID_NVS: Initializating Key: STR_0 on Partition: config  Namespace: SYSTEM_INFO
E (4653) MID_NVS:
E (4653) MID_NVS: ESP ERROR: ESP_ERR_NVS_NOT_FOUND  File: <PROJECT_PATH>/ESP32_Bug_NVS/components/mid_nvs/mid_nvs.c  Line: 169
E (4666) MID_NVS:
W (4671) MID_NVS: Key STR_1 Not Found on Partition: config  Namespace: SYSTEM_INFO
W (4676) MAIN: NVS_KEY_STR_1 Not Found
I (4681) MAIN: Initializing NVS_KEY_STR_1
W (4686) MID_NVS: Initializating Key: STR_1 on Partition: config  Namespace: SYSTEM_INFO
E (4695) MID_NVS:
E (4697) MID_NVS: ESP ERROR: ESP_ERR_NVS_NOT_FOUND  File: <PROJECT_PATH>/ESP32_Bug_NVS/components/mid_nvs/mid_nvs.c  Line: 169
E (4710) MID_NVS:
E (4717) MID_NVS: NULL Iterator (Partition: config  Namespace: SYSTEM_INFO  Type: 4)
W (4722) MID_NVS: Key U32_0 Not Found on Partition: config  Namespace: SYSTEM_INFO
W (4730) MAIN: NVS_KEY_U32_0 Not Found
I (4734) MAIN: Initializing NVS_KEY_U32_0
W (4739) MID_NVS: Initializating Key: U32_0 on Partition: config  Namespace: SYSTEM_INFO
W (4749) MID_NVS: Key U32_1 Not Found on Partition: config  Namespace: SYSTEM_INFO
W (4756) MAIN: NVS_KEY_U32_1 Not Found
I (4760) MAIN: Initializing NVS_KEY_U32_1
W (4765) MID_NVS: Initializating Key: U32_1 on Partition: config  Namespace: SYSTEM_INFO
I (4775) MAIN: Check Complete
I (4777) MAIN: *********************************************
I (4784) MAIN: *********************************************
```

In step 4, it can be seen that now, the `config` partition has 5 used entries:

```
I (5795) MAIN: *********************************************
I (5795) MAIN: *********************************************
I (5801) MAIN: Loading Partitions Status
I (5806) MID_NVS: NVS Partition nvs Status: Used: 0  Free: 504  Total: 504  Count: 0
I (5814) MID_NVS: NVS Partition config Status: Used: 5  Free: 28723  Total: 28728  Count: 1
I (5823) MID_NVS: NVS Partition otaConfig Status: Used: 0  Free: 504  Total: 504  Count: 0
I (5832) MAIN: *********************************************
I (5838) MAIN: *********************************************
```

Finally, in step 5 the values are correctly loaded from the NVS partition:

```
I (6849) MAIN: *********************************************
I (6849) MAIN: *********************************************
I (6855) MAIN: Reading NVS Keys
I (6859) MAIN: NVS_KEY_STR_0: InitString0
I (6864) MAIN: NVS_KEY_STR_1: InitString1
I (6868) MAIN: NVS_KEY_U32_0: 0
I (6872) MAIN: NVS_KEY_U32_1: 0
I (6876) MAIN: Read Complete
I (6879) MAIN: *********************************************
I (6886) MAIN: *********************************************
```

After step 6, the demo waits forever in an infinite loop. At this point, the device has to be reset.
To do that, close the `esptool` monitor and reopen it with the command `make monitor`


#### Second Run

In step 2 it can be seen that the `config` partition has now only 3 used entries instead of 5:

```
I (3552) MAIN: *********************************************
I (3552) MAIN: *********************************************
I (3558) MAIN: Loading Partitions Status
I (3563) MID_NVS: NVS Partition nvs Status: Used: 0  Free: 504  Total: 504  Count: 0
I (3571) MID_NVS: NVS Partition config Status: Used: 3  Free: 28725  Total: 28728  Count: 1
I (3580) MID_NVS: NVS Partition otaConfig Status: Used: 0  Free: 504  Total: 504  Count: 0
I (3589) MAIN: *********************************************
I (3595) MAIN: *********************************************
```

In step 3, the two Strings can't be found on the `config` partition and are created again. Also, the two integers can be found correctly with no complications:

```
I (4606) MAIN: *********************************************
I (4606) MAIN: *********************************************
I (4612) MAIN: Checking NVS Keys
E (4617) MID_NVS: NULL Iterator (Partition: config  Namespace: SYSTEM_INFO  Type: 33)
W (4624) MID_NVS: Key STR_0 Not Found on Partition: config  Namespace: SYSTEM_INFO
W (4633) MAIN: NVS_KEY_STR_0 Not Found
I (4637) MAIN: Initializing NVS_KEY_STR_0
W (4642) MID_NVS: Initializating Key: STR_0 on Partition: config  Namespace: SYSTEM_INFO
E (4651) MID_NVS:
E (4653) MID_NVS: ESP ERROR: ESP_ERR_NVS_NOT_FOUND  File: <PROJECT_PATH>/ESP32_Bug_NVS/components/mid_nvs/mid_nvs.c  Line: 169
E (4667) MID_NVS:
W (4673) MID_NVS: Key STR_1 Not Found on Partition: config  Namespace: SYSTEM_INFO
W (4677) MAIN: NVS_KEY_STR_1 Not Found
I (4682) MAIN: Initializing NVS_KEY_STR_1
W (4687) MID_NVS: Initializating Key: STR_1 on Partition: config  Namespace: SYSTEM_INFO
E (4696) MID_NVS:
E (4698) MID_NVS: ESP ERROR: ESP_ERR_NVS_NOT_FOUND  File: <PROJECT_PATH>/ESP32_Bug_NVS/components/mid_nvs/mid_nvs.c  Line: 169
E (4711) MID_NVS:
I (4717) MID_NVS: Key U32_0 Found on Partition: config  Namespace: SYSTEM_INFO
I (4722) MID_NVS: Key U32_1 Found on Partition: config  Namespace: SYSTEM_INFO
I (4730) MAIN: Check Complete
I (4734) MAIN: *********************************************
I (4740) MAIN: *********************************************
```

#### Third Run

If the demo is run for the third time, the problem is "magically" solved and never happens again



### Notes

- In some tests, using the esp_reset() function didn't cause the bug to happen.
- If the While Loop is removed and the process continues the bug doesn't occur