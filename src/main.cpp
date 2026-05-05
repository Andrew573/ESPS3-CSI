#include <Arduino.h>
#include <esp_system.h>

void printMemoryInfo() {
    size_t totalRam = ESP.getHeapSize();
    size_t freeRam = ESP.getFreeHeap();
    size_t usedRam = totalRam - freeRam;

    Serial.printf("\n========== Memory Info ==========\n");
    Serial.printf("RAM Total:  %zu bytes (%.2f KB)\n", totalRam, totalRam / 1024.0);
    Serial.printf("RAM Free:   %zu bytes (%.2f KB)\n", freeRam, freeRam / 1024.0);
    Serial.printf("RAM Used:   %zu bytes (%.2f KB)\n", usedRam, usedRam / 1024.0);

#if BOARD_HAS_PSRAM
    size_t psramSize = ESP.getPsramSize();
    size_t psramFree = ESP.getFreePsram();
    Serial.printf("PSRAM Total: %zu bytes (%.2f MB)\n", psramSize, psramSize / (1024.0 * 1024.0));
    Serial.printf("PSRAM Free:  %zu bytes (%.2f KB)\n", psramFree, psramFree / 1024.0);
#endif

    uint32_t flashSize = ESP.getFlashChipSize();
    Serial.printf("ROM Total:  %lu bytes (%.2f MB)\n", flashSize, flashSize / (1024.0 * 1024.0));

    Serial.printf("================================\n\n");
}

int myFunction(int, int);

void setup() {
    Serial.begin(115200);
    delay(500);
    printMemoryInfo();
    int result = myFunction(2, 3);
}

void loop() {
}

int myFunction(int x, int y) {
    return x + y;
}