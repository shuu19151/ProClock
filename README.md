# ProClock
---

## Create SPIFFS .bin file
1. Open Powershell.
2. cd to the folder you want to generate the bin file.
3. write these lines:
```shell
C:\Users\ `UserName`\AppData\Local\Arduino15\packages\esp32\tools\mkspiffs\0.2.3\mkspiffs.exe -c `SourceFolder` -b 4096 -p 256 -s 0x160000 ProClock.spiffs.bin
```

Output:
```shell
/favicon.ico
/index.html
/script.js
/settings.png
/style.css
```

## Upload SPIFFS .bin file
- Stay where you are, keep on write these lines:
```shell
C:\Users\ `UserName`\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\4.5.1\esptool.exe --chip esp32 --port `COMPORT` --baud 921600 --before default_reset --after hard_reset write_flash -z `0x210000` main.spiffs.bin
```
- `0x210000` is offset address, you can find it in partitions.csv.

- Upload other address sample:
```shell
C:\Users\lamqu\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\4.5.1\esptool.exe --chip esp32 --port COM8 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 ProClock.ino.bootloader.bin 0x8000 ProClock.ino.partitions.bin 0xe000 boot_app0.bin 0x10000 ProClock.ino.bin 0x210000 ProClock.spiffs.bin
```
---
# WEBSERVER

## UI

![image](images/ProClock_Web_Server.png)

## GUIDE

1. Open Wifi on any OS, access to Acess Point `ProClock`.
2. Open browser, go to address `192.168.4.1`.
3. Update RTC date/time with `Set Date/Time` button (#_this will update the date/time from local browser, not SNTP_).
4. Give the device new WiFi credentials and hit `Set WiFi` button.
5. Choose new crypto comparison for showing on display, between `Currency` & `Crypto`.
6. There are 2 application on the device (`display date/time` or `display crypto value`), select which application will be run and hit `Set App` button.