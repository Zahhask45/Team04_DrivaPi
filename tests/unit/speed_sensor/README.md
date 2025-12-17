# Unit Testing Guide (STM32 + Ceedling)

This document outlines the unit testing strategy for the PiRacer firmware. We use **Ceedling** (Unity + CMock) to test hardware-dependent C code on a host PC by mocking the STM32 HAL and ThreadX RTOS dependencies.

## 1. Prerequisites

To run the tests, you must have the following installed on your host machine (Linux/Mac/WSL):

* **Ruby** (required for Ceedling)
* **GCC** (standard C compiler)
* **Ceedling** (The testing framework)

```bash
# Install Ruby (Debian/Ubuntu)
sudo apt update && sudo apt install ruby-full build-essential

# Install Ceedling
sudo gem install ceedling

# Navigate to Project Directory and create a new Project
ceedling new <testname>
```

## 2. Directory Structure

The project separates source code from test logic. We use a **"Support"** folder to mock hardware dependencies.

```text
Project_Root/
├── project.yml               # Ceedling configuration file
├── src/                      # ACTUAL Source Code
│   ├── speed_sensor.c
│   ├── speed_sensor.h
│   └── ...
└── test/                     # Test Logic
    ├── test_speed_sensor.c   # Unit Tests for speed sensor
    └── support/              # "FAKE" Drivers (Stubs)
        ├── stm32u5xx_hal.h   # Fakes the STM32 Hardware Abstraction Layer
        ├── app_threadx.h     # Fakes the RTOS (ThreadX)
        └── main.h            # Fakes global flags/variables
```

## 3. How It Works (The Mocking Strategy)

Since we cannot run `HAL_GPIO_WritePin` or `tx_time_get` on a PC, we use **Stub Headers** and **CMock**.

1.  **Stub Headers (`test/support/`)**: These define the *types* and *function prototypes* (like `TIM_HandleTypeDef` or `tx_time_get`) so the compiler is happy.
2.  **CMock**: Automatically generates `.c` files that implement these functions with tracking logic.
3.  **The Test**: We call functions like `tx_time_get_ExpectAndReturn(100)` to tell the mock: *"When the code asks for time, pretend it is tick 100."*

### Key Files in `test/support/`
* **`stm32u5xx_hal.h`**: Defines `TIM_TypeDef`, `TIM_HandleTypeDef`, and HAL prototypes.
* **`app_threadx.h`**: Defines `ULONG`, `UINT`, and RTOS prototypes.

## 4. Running Tests

### Run All Tests
```bash
ceedling test:all
```

### Run a Specific Test File
```bash
ceedling test:test_speed_sensor.c
```

### Clean Build (Fixes weird caching issues)
If you change `project.yml` or header files and tests start failing mysteriously:
```bash
ceedling clobber
ceedling test:all
```

## 5. Writing a New Test

### Step 1: Create the Test File
Create a file named `test_my_module.c` in the `test/` folder.

### Step 2: Include the "Mocks"
You must include the mock versions of the dependencies, not the real ones.
```c
#include "unity.h"
#include "my_module.h"

// Mock the dependencies
#include "mock_stm32u5xx_hal.h"
#include "mock_app_threadx.h"
```

### Step 3: Define Global Handles
If your source code uses `extern TIM_HandleTypeDef htim1;`, you must define it in the test file.
```c
TIM_HandleTypeDef htim1;
TIM_TypeDef fake_tim1_regs; // The actual memory storage for registers
```

### Step 4: Write `setUp`
Link the handle to your fake registers so you can read/write them.
```c
void setUp(void) {
    memset(&fake_tim1_regs, 0, sizeof(TIM_TypeDef));
    htim1.Instance = &fake_tim1_regs;
}
```

### Step 5: Write Test Cases
Use **Expect** functions to simulate hardware behavior.

```c
void test_MyFunction_Should_ReadTimer(void) {
    // 1. Setup: Pretend the hardware timer is at 500
    fake_tim1_regs.CNT = 500;

    // 2. Expect: The code will ask for time, return 1000
    tx_time_get_ExpectAndReturn(1000);

    // 3. Execute
    MyFunction();

    // 4. Assert
    TEST_ASSERT_EQUAL(500, some_result);
}
```

## 6. Common Issues & Fixes

| Error | Cause | Fix |
| :--- | :--- | :--- |
| `implicit declaration of function memset` | Missing include | Add `#include <string.h>` to the test file. |
| `undefined reference to speed_data_mutex` | Linker cannot find global var | Define the variable (e.g., `TX_MUTEX speed_data_mutex;`) at the top of the test file. |
| `Unity Floating Point Disabled` | Unity config issue | Ensure `project.yml` has `:unity: :defines: [- UNITY_INCLUDE_FLOAT]`. |
| `cannot open source file stm32u5xx_hal.h` | Missing stub | Ensure `test/support/stm32u5xx_hal.h` exists and paths are correct in `project.yml`. |
