/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/spi_master.h"

// Date and time functions using a PCF8563 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <esp32-hal-spi.h>

#include "Adafruit_SharpMem.h"

#define SHARP_SCK 6
#define SHARP_MOSI 7
#define SHARP_SS 8
#define SHARP_DISP 10

// Set the size and color depth, e.g. 3 bits for LS013B7DH06 (8 colors 128x128 display)
Adafruit_SharpMem display(&SPI, SHARP_SS, 128, 128, 1, 2000000); // 2100000 ok, 2500000 wrong, 230000 LDO ok

#define BLACK 0
#define WHITE 1

RTC_PCF8563 rtc;

extern "C" void watch_clock_main(void)
{
    printf("Hello world!\n");

    Wire.begin(21, 20); // sda=  /scl=

    if (!rtc.begin())
    {
        printf("Couldn't find RTC\n");
        fflush(stdout);
        while (1)
            delay(10);
    }

    if (rtc.lostPower())
    {
        printf("RTC is NOT initialized, let's set the time!\n");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        //
        // Note: allow 2 seconds after inserting battery or applying external power
        // without battery before calling adjust(). This gives the PCF8523's
        // crystal oscillator time to stabilize. If you call adjust() very quickly
        // after the RTC is powered, lostPower() may still return true.
    }
    rtc.start();

    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    DateTime now = rtc.now();

    printf("%04d-%02d-%02d(%s) %02d:%02d:%02d\n", now.year(), now.month(), now.day(), daysOfTheWeek[now.dayOfTheWeek()], now.hour(), now.minute(), now.second());

    // pinMode(SHARP_DISP, OUTPUT);
    // digitalWrite(SHARP_DISP, HIGH);
    SPI.begin(SHARP_SCK, 11, SHARP_MOSI, SHARP_SS);

    // start & clear the display
    display.begin();
    display.clearDisplay();

    delay(500);
    display.refresh();

    printf("DONE!!!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("It is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    for (int i = 10; i >= 0; i--)
    {
        printf("T-minus counting to enter MicroPython in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    // printf("Restarting now.\n");
    fflush(stdout);
    // esp_restart();
}
