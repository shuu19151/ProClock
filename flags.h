/*==============EEPROM flags==============*/
#define COMMIT_EEPROM_FLAG      (0)
#define COMMIT_WIFI_FLAG        (1)
#define COMMIT_CRYPTO_FLAG      (2)

/*==============BLE flags==============*/

/**
 * @brief New WiFi credentials arrived
*/
#define BLE_NEW_WIFI_FLAG       (1)

/**
 * @brief New crypto settings arrived
*/
#define BLE_NEW_CRYPTO_FLAG     (2)

/**
 * @brief Flag for notify to client
*/
#define BLE_NOTIFY_FLAG         (3)

/**
 * @brief Flag to indicate that the BLE client is connected
*/
#define BLE_CLIENT_CONNECT_FLAG (4)

/**
 * @brief Flag for display symbol on LCD
*/
#define BLE_DISPLAY_SYMBOL_FLAG (5)
/*==============app flags==============*/

/**
 * @brief Flag to indicate that the app is running or not
 *      Running(1): App is running
 *      Cancel(0): App is not running and waiting to be created
*/
#define APP_IS_RUNNING_FLAG     (0)
/**
 * @brief Flag to indicate that the app is running in normal mode
 *      Normal mode(1): run the app normally
 *      Config mode(0): Receive new settings from BLE
*/
#define APP_RUN_NORMAL_FLAG     (1)

/**
 * @brief Flag to indicate wiFi connection
 *      Online(1): WiFi is connected
 *      Offline(0): WiFi is disconnected
*/
#define APP_RUN_ONLINE_FLAG     (2)