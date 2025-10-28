// SMART PORT PROTOTYPE – BOARDING NOW SCREEN
// Displays a real-time clock and countdown timer that ends
// with a flashing “BOARDING NOW!” message.

// Import Libraries
#include <Arduino.h>    // Core Arduino functions
#include <U8g2lib.h>    // OLED display control
#include <I2C_RTC.h>    // Real-Time Clock module
#include <Wire.h>       // I2C communication

/*
  Overview:
  This version demonstrates a 30-second countdown for a short demo.
  The display shows the current time and a flashing “BOARDING NOW!” message 
  once the countdown reaches zero.
*/

// RTC Setup 
static DS1307 RTC; // Create RTC object for timekeeping

// OLED Setup 
U8G2_SH1106_128X64_WINSTAR_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/10, /* dc=*/9, /* reset=*/8);

// Countdown Configuration
const unsigned long COUNTDOWN_SECONDS = 30UL;  // 30 seconds for demo
unsigned long startTime;
bool flashState = false;  // used for flashing text animation

void setup() {
  Serial.begin(115200);
  Wire.begin();
  RTC.begin();
  u8g2.begin();

  // Verifying RTC connection
  if (!RTC.isConnected()) {
    Serial.println("RTC Not Connected!");
    while (true);
  }

  Serial.println("*** SmartPort Prototype ***");

  startTime = millis(); // Record the start time for countdown
}

void loop() {
  // Read the current time from RTC 
  uint8_t hour   = RTC.getHours();
  uint8_t minute = RTC.getMinutes();

  // Format current time as HH:MM
  char timeString[6];
  sprintf(timeString, "%02d:%02d", hour, minute);

  // Calculating remaining countdown time
  unsigned long elapsed = (millis() - startTime) / 1000; // elapsed seconds
  unsigned long remaining;
  if (elapsed >= COUNTDOWN_SECONDS) {
    remaining = 0;
  } else {
    remaining = COUNTDOWN_SECONDS - elapsed;
  }

  // Convert remaining time to hours:minutes:seconds
  unsigned long remHours   = remaining / 3600;
  unsigned long remMinutes = (remaining % 3600) / 60;
  unsigned long remSeconds = remaining % 60;

  char countdownString[9];
  sprintf(countdownString, "%02lu:%02lu:%02lu", remHours, remMinutes, remSeconds);

  // updating OLED Display 
  u8g2.clearBuffer();

  // Top line --> flashes "BOARDING NOW!" once timer ends
  u8g2.setFont(u8g2_font_DigitalDisco_tu);
  if (remaining == 0) {
    if ((millis() / 500) % 2 == 0) { // Toggle text every 0.5s for flashing effect
      u8g2.drawStr(8, 20, "BOARDING NOW!");
    }
  } else {
    u8g2.drawStr(5, 20, "BOARDING SOON!");
  }

  // Middle line: real-time clock
  u8g2.setFont(u8g2_font_8bitclassic_tf);
  u8g2.setCursor(40, 40);
  u8g2.print(timeString);

  // Bottom line: countdown timer
  u8g2.setCursor(26, 60);
  u8g2.print(countdownString);

  u8g2.sendBuffer(); // Send data to display

  delay(100); // Refresh every 1 second
}
