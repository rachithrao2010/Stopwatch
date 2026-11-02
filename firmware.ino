
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SDA_PIN 6
#define SCL_PIN 5

#define START_BTN 2
#define STOP_BTN  3
#define RESET_BTN 4

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

unsigned long startTime = 0;
unsigned long elapsedTime = 0;

bool running = false;

void setup() {
  Serial.begin(115200);

  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(STOP_BTN, INPUT_PULLUP);
  pinMode(RESET_BTN, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  showTime();
}

void loop() {
  // Start
  if (digitalRead(START_BTN) == LOW && !running) {
    startTime = millis() - elapsedTime;
    running = true;
    delay(200);
  }

  // Stop
  if (digitalRead(STOP_BTN) == LOW && running) {
    elapsedTime = millis() - startTime;
    running = false;
    delay(200);
  }

  // Reset
  if (digitalRead(RESET_BTN) == LOW) {
    elapsedTime = 0;
    running = false;
    delay(200);
  }

  if (running) {
    elapsedTime = millis() - startTime;
  }

  showTime();
  delay(30);
}

void showTime() {
  unsigned long totalMs = elapsedTime;

  unsigned int minutes = totalMs / 60000;
  unsigned int seconds = (totalMs / 1000) % 60;
  unsigned int milliseconds = totalMs % 1000;

  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("   OLED STOPWATCH");

  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  // Time display
  display.setTextSize(2);
  display.setCursor(8, 24);

  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");

  if (seconds < 10) display.print("0");
  display.print(seconds);

  display.setTextSize(1);
  display.print(".");
  if (milliseconds < 100) display.print("0");
  if (milliseconds < 10) display.print("0");
  display.print(milliseconds);

  // Status
  display.setCursor(0, 52);

  if (running) {
    display.print("RUNNING");
  } else if (elapsedTime > 0) {
    display.print("PAUSED");
  } else {
    display.print("READY");
  }

  display.setCursor(75, 52);
  display.print("D2 D3 D4");

  display.display();
}
