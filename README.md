# Counter V2 ESP32
An improved ESP32-based counter with enhanced features, better performance, and additional functionalities compared to the previous version.

## Get Started:
Clone the repository with submodules.
```bash
git clone --recurse-submodules -j8 https://github.com/pen787/counterv2.git
```

Recommend using VS Code with the [PlatformIO](https://platformio.org/) extension for development.

For Windows, install the appropriate USB-to-UART driver for your ESP32 board:  
- [CP210x driver (Silicon Labs)](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) for most official ESP32 DevKit boards  
- [CH340 driver (WCH)](https://www.wch.cn/downloads/CH341SER_ZIP.html) for boards using the CH340 chip

### TODO:
- [X] Counter.
- [X] Server.
- [X] Firebase
  - [X] Firestore.
  - [X] Realtime database.
- [X] Data Manager.
- [ ] ~~Saving Manager.~~ (Won't Implement)
- [X] Multi-Thread Sensor

### Note :
Secret variables are accessed via an auto-generated header file.
You can regenerate it by re-saving `platformio.ini`.
Include it in your code as follows:
```cpp 
#include <secrets.cpp>
```

[old version](https://github.com/pen787/250614-123010-esp12e)
[website](https://github.com/pen787/people-counter-app)