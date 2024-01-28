/*
* WiFi Events
*/

// WIFI_EVENT_WIFI_READY = 0,           /**< ESP32 WiFi ready */
// WIFI_EVENT_SCAN_DONE,                /**< ESP32 finish scanning AP */
// WIFI_EVENT_STA_START,                /**< ESP32 station start */
// WIFI_EVENT_STA_STOP,                 /**< ESP32 station stop */
// WIFI_EVENT_STA_CONNECTED,            /**< ESP32 station connected to AP */
// WIFI_EVENT_STA_DISCONNECTED,         /**< ESP32 station disconnected from AP */
// WIFI_EVENT_STA_AUTHMODE_CHANGE,      /**< the auth mode of AP connected by ESP32 station changed */

// WIFI_EVENT_STA_WPS_ER_SUCCESS,       /**< ESP32 station wps succeeds in enrollee mode */
// WIFI_EVENT_STA_WPS_ER_FAILED,        /**< ESP32 station wps fails in enrollee mode */
// WIFI_EVENT_STA_WPS_ER_TIMEOUT,       /**< ESP32 station wps timeout in enrollee mode */
// WIFI_EVENT_STA_WPS_ER_PIN,           /**< ESP32 station wps pin code in enrollee mode */
// WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP,   /**< ESP32 station wps overlap in enrollee mode */

// WIFI_EVENT_AP_START,                 /**< ESP32 soft-AP start */
// WIFI_EVENT_AP_STOP,                  /**< ESP32 soft-AP stop */
// WIFI_EVENT_AP_STACONNECTED,          /**< a station connected to ESP32 soft-AP */
// WIFI_EVENT_AP_STADISCONNECTED,       /**< a station disconnected from ESP32 soft-AP */
// WIFI_EVENT_AP_PROBEREQRECVED,        /**< Receive probe request packet in soft-AP interface */

// WIFI_EVENT_FTM_REPORT,               /**< Receive report of FTM procedure */

// /* Add next events after this only */
// WIFI_EVENT_STA_BSS_RSSI_LOW,         /**< AP's RSSI crossed configured threshold */
// WIFI_EVENT_ACTION_TX_STATUS,         /**< Status indication of Action Tx operation */
// WIFI_EVENT_ROC_DONE,                 /**< Remain-on-Channel operation complete */

// WIFI_EVENT_STA_BEACON_TIMEOUT,       /**< ESP32 station beacon timeout */

// WIFI_EVENT_MAX,                      /**< Invalid WiFi event ID */

#include "wifiClientEvents.h"
#include "esp_wifi.h"

// WARNING: This function is called from a separate FreeRTOS task (thread)!

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if(event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_WIFI_READY:
                Serial.println("WiFi interface ready");
                break;
            case WIFI_EVENT_SCAN_DONE:
                Serial.println("Completed scan for access points");
                break;
            case WIFI_EVENT_STA_START:
                Serial.println("WiFi client started");
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_STOP:
                Serial.println("WiFi clients stopped");
                break;
            case WIFI_EVENT_STA_CONNECTED:
                Serial.println("Connected to access point");
                SET_BIT(app_flags, APP_RUN_ONLINE_FLAG);
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                Serial.println("Disconnected from WiFi access point");
                esp_wifi_connect();
                CLR_BIT(app_flags, APP_RUN_ONLINE_FLAG);
                break;
            case WIFI_EVENT_STA_AUTHMODE_CHANGE:
                Serial.println("Authentication mode of access point has changed");
                break;
            // case WIFI_EVENT_STA_GOT_IP:
            //     Serial.print("Obtained IP address: ");
            //     Serial.println(WiFi.localIP());
            //     break;
            // case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            //     Serial.println("Lost IP address and IP address is reset to 0");
            //     break;
            case WIFI_EVENT_STA_WPS_ER_SUCCESS:
                Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
                break;
            case WIFI_EVENT_STA_WPS_ER_FAILED:
                Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
                break;
            case WIFI_EVENT_STA_WPS_ER_TIMEOUT:
                Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
                break;
            case WIFI_EVENT_STA_WPS_ER_PIN:
                Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
                break;
            case WIFI_EVENT_AP_START:
                Serial.println("WiFi access point started");
                break;
            case WIFI_EVENT_AP_STOP:
                Serial.println("WiFi access point  stopped");
                break;
            case WIFI_EVENT_AP_STACONNECTED:
                Serial.println("Client connected");
                break;
            case WIFI_EVENT_AP_STADISCONNECTED:
                Serial.println("Client disconnected");
                break;
            // case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            //     Serial.println("Assigned IP address to client");
            //     break;
            case WIFI_EVENT_AP_PROBEREQRECVED:
                Serial.println("Received probe request");
                break;
            // case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            //     Serial.println("AP IPv6 is preferred");
            //     break;
            // case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            //     Serial.println("STA IPv6 is preferred");
            //     break;
            // case ARDUINO_EVENT_ETH_GOT_IP6:
            //     Serial.println("Ethernet IPv6 is preferred");
            //     break;
            // case ARDUINO_EVENT_ETH_START:
            //     Serial.println("Ethernet started");
            //     break;
            // case ARDUINO_EVENT_ETH_STOP:
            //     Serial.println("Ethernet stopped");
            //     break;
            // case ARDUINO_EVENT_ETH_CONNECTED:
            //     Serial.println("Ethernet connected");
            //     break;
            // case ARDUINO_EVENT_ETH_DISCONNECTED:
            //     Serial.println("Ethernet disconnected");
            //     break;
            // case ARDUINO_EVENT_ETH_GOT_IP:
            //     Serial.println("Obtained IP address");
            //     break;
            default: break;
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        Serial.printf("got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch (event) {
        case ARDUINO_EVENT_WIFI_READY: 
            Serial.println("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Serial.println("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("Connected to access point");
            SET_BIT(app_flags, APP_RUN_ONLINE_FLAG);
            SET_BIT(ble_flags, BLE_NOTIFY_FLAG);
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            CLR_BIT(app_flags, APP_RUN_ONLINE_FLAG);
            SET_BIT(ble_flags, BLE_NOTIFY_FLAG);
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Serial.println("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Serial.println("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            Serial.println("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default: break;
    }}

