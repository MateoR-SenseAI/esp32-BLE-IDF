#pragma once

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_log.h"
#include "esp_bt_main.h"

class BleClient {
public:
    BleClient(uint16_t service_uuid, uint16_t char_uuid);

    void init();
    void startScan();

private:
    static void gapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
    static void gattcEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

    static esp_ble_scan_params_t scanParams;
    static uint16_t remoteServiceUUID;
    static uint16_t remoteCharUUID;
    static esp_gatt_if_t clientIf;
    static esp_bd_addr_t serverAddress;
    static bool deviceFound;
};
