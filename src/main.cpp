#include <Arduino.h>
#include <esp_system.h>
#include <stdint.h>
#include <string.h>

static uint8_t tx_buf[1024];
static bool sendTest = false;
static unsigned long testStart = 0;
static unsigned long bytesSent = 0;

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

    Serial.printf("\n========== CDC Speed Test ==========\n");
    Serial.printf("USB CDC initialized.\n");
    Serial.printf("Send 'START' to begin TX speed test.\n");
    Serial.printf("=====================================\n\n");
}

void loop() {
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd == "START") {
            sendTest = true;
            testStart = millis();
            bytesSent = 0;
            Serial.printf("\n--- TX Speed Test Started ---\n");
        } else if (cmd == "STOP") {
            sendTest = false;
            unsigned long elapsed = millis() - testStart;
            if (elapsed > 0) {
                float speedKBs = bytesSent / 1024.0 / (elapsed / 1000.0);
                Serial.printf("--- TX Speed Test Stopped ---\n");
                Serial.printf("Sent: %lu bytes in %lu ms\n", bytesSent, elapsed);
                Serial.printf("Speed: %.2f KB/s\n", speedKBs);
            }
        }
    }

    if (sendTest) {
        int bufSize = sizeof(tx_buf);
        for (int j = 0; j < 8; j++) {
            for (int i = 0; i < bufSize; i++) {
                tx_buf[i] = 'A' + (i % 26);
            }
            Serial.write(tx_buf, bufSize);
            bytesSent += bufSize;
        }

        if (millis() - testStart > 5000) {
            unsigned long elapsed = millis() - testStart;
            float speedKBs = bytesSent / 1024.0 / (elapsed / 1000.0);
            Serial.printf("\n--- TX Speed Test Result ---\n");
            Serial.printf("Sent: %lu bytes in %lu ms\n", bytesSent, elapsed);
            Serial.printf("Speed: %.2f KB/s\n", speedKBs);
            Serial.printf("----------------------------\n");
            sendTest = false;
        }
    }
}

int myFunction(int x, int y) {
    return x + y;
}