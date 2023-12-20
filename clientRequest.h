#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "WiFi.h"

class Request {
    public:
    // Request();
    // ~Request();

    String get(String url);
    String parseJson(String url, String key);
    String parseJson(String url, String key, uint8_t index1);
    String parseJson(String url, String key, uint8_t index1, uint8_t index2);

    private:
    uint8_t m_retry = 5;
    uint8_t m_sleepBetweenRetry = 5;
};