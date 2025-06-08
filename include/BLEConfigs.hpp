#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

// UUID del servicio que estamos buscando (cámbialo por el de tu dispositivo)
#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"

// Parámetros de escaneo BLE
static esp_ble_scan_params_t ble_scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval = 0x50,
    .scan_window = 0x30,
    .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
};

#endif // BLE_CONFIG_H