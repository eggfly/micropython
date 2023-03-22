
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

#include "macos_face.h"

#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>


#define ENABLE_DEEP_SLEEP 0

#define FORCE_ADJUST_RTC 0

#define SHARP_SCK 6
#define SHARP_MOSI 7
#define SHARP_SS 8
#define SHARP_DISP 10

// Set the size and color depth, e.g. 3 bits for LS013B7DH06 (8 colors 128x128 display)
Adafruit_SharpMem canvas(&SPI, SHARP_SS, 128, 128, 1, 2000000); // 2100000 ok, 2500000 wrong, 230000 LDO ok

#define BLACK 0
#define WHITE 1

RTC_PCF8563 rtc;

long boot_time;

RTC_DATA_ATTR uint32_t last_cost;

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  float result;
  result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return result;
}


void show_macos_time() {
  // canvas1.fillScreen(BLACK);

  //canvas1.writeFastHLine(1, 1, 9, WHITE);
  //canvas1.writeFastHLine(2, 2, 9, BLACK);

  canvas.clearDisplay();
  canvas.drawRGBBitmap(0, 0, (uint16_t*)macos_128x128, 128, 128);
  canvas.fillRect(3, 37, 107, 33, 0b100);
  canvas.setCursor(3, 37 + 25);
  canvas.setTextSize(1);
  // canvas.setFont(&FreeMono24pt7b);
  canvas.setFont(&FreeMonoBold12pt7b);
  canvas.setTextColor(0b111);
  char buf[16];
  DateTime now = rtc.now();
  snprintf(buf, sizeof(buf) / sizeof(buf[0]), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  canvas.println(buf);
  auto sensorValue = analogRead(2);
  // Serial.println(sensorValue);
  float vbat = mapf(sensorValue, 0, 4095, 0, 3.3) * 2;
  float real_vbat = 0.8681 * vbat + 0.0987;
  canvas.setTextColor(0b000);
  canvas.printf("%.4f", real_vbat);
  canvas.println("V");
  canvas.print(last_cost);
  canvas.println("ms");
  // canvas.drawRGBBitmap(0, 0, canvas1.getBuffer(), 32, 32);
  canvas.refresh();
  last_cost = millis() - boot_time;
}


void init_rtc() {
  Wire.begin(21, 20);   // sda=  /scl=

  if (!rtc.begin()) {
    printf("Couldn't find RTC\n");
    fflush(stdout);
    while (1) delay(10);
  }

  if (rtc.lostPower() || FORCE_ADJUST_RTC) {
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

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  // When the RTC was stopped and stays connected to the battery, it has
  // to be restarted by clearing the STOP bit. Let's do this to ensure
  // the RTC is running.
  rtc.start();
}

// void show_date_time();

void setup(void) {
  boot_time = millis();
//   if (!ENABLE_DEEP_SLEEP) {
//     Serial.begin(115200);
//   }
  pinMode(SHARP_DISP, OUTPUT);
  digitalWrite(SHARP_DISP, HIGH);
  SPI.begin(SHARP_SCK, 11, SHARP_MOSI, SHARP_SS);

  // start & clear the display
  canvas.begin();

  init_rtc();
  if (!ENABLE_DEEP_SLEEP) {
    // show_datetime();
  }
  // show_anim();
  // show_time();
  // canvas.setRotation(0);
  // t_rex_setup();
}


void loop(void) {
  //  while (1) {
  //    t_rex_loop();
  //    delay(1);
  //  }
  // flappy_bird_loop();
  show_macos_time();
}

void count_down_enter_mpy() {
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

    for (int i = 3; i >= 0; i--)
    {
        printf("T-minus counting to enter MicroPython in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    // printf("Restarting now.\n");
    fflush(stdout);
}


extern "C" void watch_clock_main(void) {
    auto bootTime1 = millis();
    printf("DONE!!! In %d ms\n", bootTime1);
    // printf("Hello world!\n");
    setup();
    loop();
    count_down_enter_mpy();
    printf("Enter MicroPython now.\n");
    fflush(stdout);
    // esp_restart();
}

extern "C" void backup_main(void) {
    auto bootTime1 = millis();
    printf("DONE!!! In %d ms\n", bootTime1);
    // printf("Hello world!\n");
    setup();
    loop();
    Wire.begin(21, 20); // sda=  /scl=

    // auto sensorValue = analogRead(2);
    if (!rtc.begin())
    {
        printf("Couldn't find RTC\n");
        fflush(stdout);
        while (1){
            delay(10);
        }
    }

    // if (rtc.lostPower())
    // {
    //     printf("RTC is NOT initialized, let's set the time!\n");
    //     // When time needs to be set on a new device, or after a power loss, the
    //     // following line sets the RTC to the date & time this sketch was compiled
    //     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //     // This line sets the RTC with an explicit date & time, for example to set
    //     // January 21, 2014 at 3am you would call:
    //     // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //     //
    //     // Note: allow 2 seconds after inserting battery or applying external power
    //     // without battery before calling adjust(). This gives the PCF8523's
    //     // crystal oscillator time to stabilize. If you call adjust() very quickly
    //     // after the RTC is powered, lostPower() may still return true.
    // }
    rtc.start();

    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    DateTime now = rtc.now();

    // printf("%04d-%02d-%02d(%s) %02d:%02d:%02d\n", now.year(), now.month(), now.day(), daysOfTheWeek[now.dayOfTheWeek()], now.hour(), now.minute(), now.second());

    // pinMode(SHARP_DISP, OUTPUT);
    // digitalWrite(SHARP_DISP, HIGH);
    SPI.begin(SHARP_SCK, 11, SHARP_MOSI, SHARP_SS);

    // start & clear the display
    canvas.begin();
    canvas.clearDisplay();
    canvas.refresh();
    auto bootTime = millis();
    printf("DONE!!! In %d ms\n", bootTime);
    count_down_enter_mpy();
    // esp_restart();
}
