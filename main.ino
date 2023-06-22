#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>

#include "index.h"
#include "result.h"
#include "colors.h"

ESP8266WebServer server(80);

const int redPin = 13;
const int greenPin = 4;
const int bluePin = 5;

int currentRed = 0;
int currentGreen = 0;
int currentBlue = 0;

bool randomColor = true;
bool randomBG = false;
bool randomBR = false;
bool randomGR = false;
const unsigned long delayTime = 10;

void randomic() {
  uint8_t step;
  uint8_t red = random(255);
  uint8_t green = random(255);
  uint8_t blue = random(255);

  if (abs(currentRed - red) > abs(currentGreen - green)) {
    if (abs(currentRed - red) > abs(currentBlue - blue)) {
      step = abs(currentRed - red);
    } else {
      step = abs(currentBlue - blue);
    }
  } else if (abs(currentGreen - green) > abs(currentBlue - blue)) {
    step = abs(currentGreen - green);
  } else {
    step = abs(currentBlue - blue);
  }

  for (int i = 0; i < step; i++) {
    if (currentRed > red) {
      currentRed--;
    } else if (currentRed != red) {
      currentRed++;
    }
    analogWrite(redPin, currentRed);

    if (currentGreen > green) {
      currentGreen--;
    } else if (currentGreen != green) {
      currentGreen++;
    }
    analogWrite(greenPin, currentGreen);

    if (currentBlue > blue) {
      currentBlue--;
    } else if (currentBlue != blue) {
      currentBlue++;
    }
    analogWrite(bluePin, currentBlue);
    delay(delayTime);
  }
}

void randomicBG() {
  uint8_t step;
  uint8_t blue = random(255);
  uint8_t green = random(255);

  if (abs(currentBlue - blue) > abs(currentGreen - green)) {
    step = abs(currentBlue - blue);
  } else {
    step = abs(currentGreen - green);
  }

  for (int i = 0; i < step; i++) {
    if (currentBlue > blue) {
      currentBlue--;
    } else if (currentBlue != blue) {
      currentBlue++;
    }
    analogWrite(bluePin, currentBlue);

    if (currentGreen > green) {
      currentGreen--;
    } else if (currentGreen != green) {
      currentGreen++;
    }
    analogWrite(greenPin, currentGreen);

    delay(delayTime);
  }
}

void randomicBR() {
  uint8_t step;
  uint8_t red = random(255);
  uint8_t blue = random(255);

  if (abs(currentRed - red) > abs(currentBlue - blue)) {
    step = abs(currentRed - red);
  } else {
    step = abs(currentBlue - blue);
  }

  for (int i = 0; i < step; i++) {
    if (currentRed > red) {
      currentRed--;
    } else if (currentRed != red) {
      currentRed++;
    }
    analogWrite(redPin, currentRed);

    if (currentBlue > blue) {
      currentBlue--;
    } else if (currentBlue != blue) {
      currentBlue++;
    }
    analogWrite(bluePin, currentBlue);
    delay(delayTime);
  }
}

void randomicGR() {
  uint8_t step;
  uint8_t red = random(255);
  uint8_t green = random(255);

  if (abs(currentRed - red) > abs(currentGreen - green)) {
    step = abs(currentRed - red);
  } else {
    step = abs(currentGreen - green);
  }

  for (int i = 0; i < step; i++) {
    if (currentRed > red) {
      currentRed--;
    } else if (currentRed < red) {
      currentRed++;
    }
    analogWrite(redPin, currentRed);

    if (currentGreen > green) {
      currentGreen--;
    } else if (currentGreen < green) {
      currentGreen++;
    }
    analogWrite(greenPin, currentGreen);

    delay(delayTime);
  }
}


void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.autoConnect("RGB");

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  server.on("/", HTTP_GET, [&]() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/set", HTTP_POST, []() {
    String redValue = server.arg("red");
    String greenValue = server.arg("green");
    String blueValue = server.arg("blue");
    int red = redValue.toInt();
    int green = greenValue.toInt();
    int blue = blueValue.toInt();
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
    currentRed = red;
    currentGreen = green;
    currentBlue = blue;

    randomColor = false;
    randomBG = false;
    randomBR = false;
    randomGR = false;

    Serial.print("Random Colors => ");
    Serial.println(randomColor);
    server.send(200, "text/html", resultPage);
  });

  server.on("/colors", HTTP_GET, [&]() {
    server.send(200, "text/html", colorsPage);
  });

  server.on("/random", HTTP_GET, [&]() {
    randomColor = true;
    randomBG = false;
    randomBR = false;
    randomGR = false;
    Serial.print("Random Colors => ");
    Serial.println(randomColor);
    server.send(200, "text/html", resultPage);
  });

  server.on("/bg", HTTP_GET, [&]() {
    randomColor = false;
    randomBG = true;
    randomBR = false;
    randomGR = false;
    Serial.print("Random Blue + Green => ");
    Serial.println(randomBG);
    server.send(200, "text/html", resultPage);
  });

  server.on("/br", HTTP_GET, [&]() {
    randomColor = false;
    randomBG = false;
    randomBR = true;
    randomGR = false;
    Serial.print("Random Blue + Red => ");
    Serial.println(randomBR);
    server.send(200, "text/html", resultPage);
  });

  server.on("/gr", HTTP_GET, [&]() {
    randomColor = false;
    randomBG = false;
    randomBR = false;
    randomGR = true;
    Serial.print("Random Green + Red => ");
    Serial.println(randomGR);
    server.send(200, "text/html", resultPage);
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  while (randomColor) 
  {
    Serial.println("Random color");
    randomic();
    break;
  }

  while (randomBG)
  {
    Serial.println("Blue + Green");
    randomicBG();
    break;
  }

  while (randomBR)
  {
    Serial.println("Blue + Red");
    randomicBR();
    break;
  }

  while (randomGR)
  {
    Serial.println("Green + Red");
    randomicGR();
    break;
  }
}
