/*SHOUT OUT SA MGA DI NANINIWALA SAKEN AT MGA HADLANG SA AKING PANGARAP 
  - MAMINTA, GAEMUEL BJORN T. 5/10/2025 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pins
#define coinSlot 6
#define motorPumpPin1 9
#define motorPumpPin2 10

const int pump1Button = 7;
const int pump2Button = 8;

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Variables
int coinslotSignal;
int coinCount = 0;
int requiredCoins = 2;
int amount;
int duration;

int buttonState1 = 0;
int buttonState2 = 0;

boolean coinInserted = false;

void setup() {
  Serial.begin(9600);
  delay(500); // allow serial to initialize

  pinMode(coinSlot, INPUT); // sensor is idle HIGH
  pinMode(pump1Button, INPUT);
  pinMode(pump2Button, INPUT);

  pinMode(motorPumpPin1, OUTPUT);
  pinMode(motorPumpPin2, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("HEALTHY BEVERAGE");
  lcd.setCursor(2, 2);
  lcd.print("VENDING MACHINE");

  Serial.println("System Start...");
  delay(2000);
}

void loop() {

  // Prompt
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("Insert Coins");
  lcd.setCursor(1, 2);
  lcd.print("According to Size");

  Serial.println("Waiting for coin...");
  delay(100);

  // ================= COIN INSERT DETECTION =================
  while (!coinInserted) {

    coinslotSignal = digitalRead(coinSlot);

    // Coin detected (LOW)
    if (coinslotSignal == LOW) {
      delay(20); // debounce

      if (digitalRead(coinSlot) == LOW) {
        coinCount++;

        amount = coinCount * 5;
        duration = (amount * 1000) / 2;

        Serial.print("Coin Count: ");
        Serial.println(coinCount);

        Serial.print("Amount: ");
        Serial.println(amount);

        Serial.print("Duration: ");
        Serial.println(duration);

        lcd.clear();
        lcd.setCursor(6, 1);
        lcd.print("Amount:");
        lcd.print(amount);

        lcd.setCursor(3, 2);
        lcd.print("Duration:");
        lcd.print(amount / 2);
        lcd.print(" sec");

        coinInserted = true;
        delay(100);
      }
    }

    delay(50);
  }

  // ================= WAIT FOR COIN TO LEAVE =================
  while (coinInserted) {
    coinslotSignal = digitalRead(coinSlot);

    // Back to HIGH (coin passed)
    if (coinslotSignal == HIGH) {
      coinInserted = false;

      // Prevent double count (optional behavior)
      // coinCount -= 1;   // <-- keep commented unless needed

      delay(100);
    }

    delay(50);
  }

  // ================= DISPENSING LOGIC =================
  while ((amount / 5) >= requiredCoins) {

    buttonState1 = digitalRead(pump1Button);
    buttonState2 = digitalRead(pump2Button);

    if (buttonState1 == HIGH) {

      Serial.println("Pump 1 Dispensing");

      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("Dispensing...");
      lcd.setCursor(5, 2);
      lcd.print("Please wait");

      digitalWrite(motorPumpPin1, HIGH);
      analogWrite(motorPumpPin1, 255);
      delay(duration);
      analogWrite(motorPumpPin1, 0);
      digitalWrite(motorPumpPin1, LOW);

      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("Transaction");
      lcd.setCursor(7, 2);
      lcd.print("Complete");

      coinCount = 0;
      amount = 0;

      delay(2000);
      break;
    }

    else if (buttonState2 == HIGH) {

      Serial.println("Pump 2 Dispensing");

      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("Dispensing...");
      lcd.setCursor(5, 2);
      lcd.print("Please wait");

      digitalWrite(motorPumpPin2, HIGH);
      analogWrite(motorPumpPin2, 255);
      delay(duration);
      analogWrite(motorPumpPin2, 0);
      digitalWrite(motorPumpPin2, LOW);

      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("Transaction");
      lcd.setCursor(7, 2);
      lcd.print("Complete");

      coinCount = 0;
      amount = 0;

      delay(2000);
      break;
    }

    else {
      Serial.println("Waiting for selection...");

      lcd.setCursor(3, 2);
      lcd.print("CHOOSE A DRINK");
      lcd.setCursor(3, 3);
      lcd.print("PRESS A BUTTON");

      digitalWrite(motorPumpPin1, LOW);
      digitalWrite(motorPumpPin2, LOW);
    }

    delay(300);
  }

  delay(300);
}
