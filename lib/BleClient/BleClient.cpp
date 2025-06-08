#include "BleClient.h"
#include <string.h>

#define TAG "BLE_CLIENT"

uint16_t BleClient::remoteServiceUUID = 0;
uint16_t BleClient::remoteCharUUID = 0;
esp_gatt_if_t BleClient::clientIf = 0;
esp_bd_addr_t BleClient::serverAddress = {0};
bool BleClient::deviceFound = false;

esp_ble_scan_params_t BleClient::scanParams = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = 0x50,
    .scan_window            = 0x30,
    .scan_duplicate         = BLE_SCAN_DUPLICATE_DISABLE
};

BleClient::BleClient(uint16_t service_uuid, uint16_t char_uuid) {
    remoteServiceUUID = service_uuid;
    remoteCharUUID = char_uuid;
}

void BleClient::init() {
    ESP_LOGI(TAG, "Inicializando BLE Cliente");

    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_ble_gattc_register_callback(gattcEventHandler));
    ESP_ERROR_CHECK(esp_ble_gattc_app_register(0));
}

void BleClient::startScan() {
    esp_ble_gap_register_callback(gapEventHandler);
    esp_ble_gap_set_scan_params(&scanParams);
}

void BleClient::gapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {
        case ESP_GAP_BLE_SCAN_RESULT_EVT: {
            esp_ble_gap_cb_param_t::ble_scan_result_evt_param scan_result = param->scan_rst;
            if (scan_result.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT && !deviceFound) {
                ESP_LOGI(TAG, "Dispositivo encontrado. Intentando conectar...");
                memcpy(serverAddress, scan_result.bda, sizeof(esp_bd_addr_t));
                esp_ble_gap_stop_scanning();
                esp_ble_gattc_open(clientIf, serverAddress, BLE_ADDR_TYPE_PUBLIC, true);
                deviceFound = true;
            }
            break;
        }
        case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
            ESP_LOGI(TAG, "Escaneo detenido");
            break;
        default:
            break;
    }
}

void BleClient::gattcEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) {
    switch (event) {
        case ESP_GATTC_REG_EVT:
            ESP_LOGI(TAG, "Registrado GATT Client");
            clientIf = gattc_if;
            esp_ble_gap_set_scan_params(&scanParams);
            break;
        case ESP_GATTC_CONNECT_EVT:
            ESP_LOGI(TAG, "Conectado al servidor BLE");
            break;
        case ESP_GATTC_DIS_SRVC_CMPL_EVT:
            ESP_LOGI(TAG, "Servicios descubiertos");
            break;
        default:
            break;
    }
}
