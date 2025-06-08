#include "BleClient.h"
#include "nvs_flash.h"

extern "C" void app_main() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    BleClient clienteBLE(0x1809, 0x2A1C);  // Health Thermometer
    clienteBLE.init();
    clienteBLE.startScan();
}
