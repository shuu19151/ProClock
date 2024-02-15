#include "clientRequest.h"

String Request::get(String url) {
    String payload;
    if(WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        uint8_t retry = 0;
        while(retry < this->m_retry && WiFi.status() == WL_CONNECTED) {
            retry++;
            http.begin(url);
            int httpCode = http.GET();
            if(httpCode > 0) {
                payload = http.getString();
                return payload;
            }
            else {
                DEBUGF("HTTP error: %s\n", http.errorToString(httpCode).c_str());
            }
            delay(this->m_sleepBetweenRetry * 1000);
        }
        http.end();
    }
    return payload;
}

String Request::parseJson(String url, String key) {
    String payload = this->get(url);
    if(payload == "error") {
        return payload;
    }
    DynamicJsonDocument doc(128);
    deserializeJson(doc, payload);
    String value = doc[key];
    return value;
}

String Request::parseJson(String url, String key, uint8_t index1) {
    String payload = this->get(url);
    if(payload == "error") {
        return payload;
    }
    DynamicJsonDocument doc(128);
    deserializeJson(doc, payload);
    String value = doc[key][index1];
    return value;
}

/**
 * @brief Parse json with 2 indexes
 *  
 * @param url : url to get json
 * @param key : key of value
 * @param index1, index2 : index of value
*/
String Request::parseJson(String url, String key, uint8_t index1, uint8_t index2) {
    String payload = this->get(url);
    if(payload == "error") { return payload; }
    DynamicJsonDocument doc(payload.length());
    deserializeJson(doc, payload);
    String value = doc[key][index1][index2];
    DEBUGF("%s\n", value.c_str());
    return value;
}