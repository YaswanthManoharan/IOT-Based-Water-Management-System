//Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Configuring LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Declaring Needed Variables Globally
//------------------------------------------
//Variables for Flowsensor
int X;
int Y;
float TIME = 0;
float FREQUENCY = 0;
float WATER = 0;
float TOTAL = 0;
float LS = 0;
const int input = A2;
//------------------------------------------
//Variables for Ultrasonic Sensor
const int trigPin = 9;
const int echoPin = 8;
const int pinSound = A0;
const int speakerPin = 3;
long duration;
int distance;
//------------------------------------------
//Variables of RGB LED
int red_light_pin= 5;
int green_light_pin = 6;
int blue_light_pin = 7;
int solenoidPin = 4;
//-------------------------------------------

int thresholdValue = 250;  // determines loudness for sound sensor
const unsigned long waterUseAllowedDuration = 3000;
bool monitoring = false;

//Function for Buzzer
void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

//Function for RGB LED
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

void setup(){
    //Input pins
    pinMode(input,INPUT);
    pinMode(echoPin, INPUT);

    //Output pins
    pinMode(red_light_pin, OUTPUT);
    pinMode(green_light_pin, OUTPUT);
    pinMode(blue_light_pin, OUTPUT);
    pinMode(solenoidPin, OUTPUT); 
    pinMode(trigPin, OUTPUT); 

    Serial.begin(9600);
    lcd.begin();
}

void loop(){
    X = pulseIn(input, HIGH);
    Y = pulseIn(input, LOW);
    TIME = X + Y;
    FREQUENCY = 1000000/TIME;
    WATER = FREQUENCY/7.5;
    LS = WATER/60;
    if(FREQUENCY >= 0)
    {
      if(isinf(FREQUENCY))
        {
          Serial.print("TOTAL: ");
          Serial.print( TOTAL);
          Serial.println(" L");
          delay(1000);
        }
       else
        {
          TOTAL = TOTAL + LS;
          //Serial.println(FREQUENCY);
          //Serial.print("VOL.: ");
          //Serial.print(WATER);
          //Serial.println("L/M");
          Serial.print("TOTAL: ");
          Serial.print(TOTAL);
          Serial.println(" L");
          delay(1000);
        }
      }
    digitalWrite(solenoidPin, HIGH);
    int sensorValue = analogRead(pinSound);
    //Serial.println(sensorValue);
    if (sensorValue > thresholdValue) {
      lcd.setCursor(0, 0);
      lcd.print("Monitoring use  ");
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;
      //Serial.print("distance: ");
      //Serial.println(distance);
        if(distance>20){
          RGB_color(255, 0, 0);
          lcd.clear();
          lcd.print("Please Close Tap!");
          lcd.setCursor(0, 1);
          lcd.print("Conserve Water!");
          playTone(1700, 4500);
          lcd.clear();
          lcd.print("Solenoid valve");
          lcd.setCursor(0, 1);
          lcd.print("Is Closed:)");
          digitalWrite(solenoidPin, LOW);
          //delay(5000);
        }
        else if(distance<=20){
          RGB_color(0, 0, 255);
          delay(waterUseAllowedDuration);
          sensorValue = analogRead(pinSound);
          //Serial.print("Sound when person there: ");
          //Serial.println(sensorValue);
          if (sensorValue > thresholdValue - 200) {
            RGB_color(255, 0, 0);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Excess water use");
            lcd.setCursor(0, 1);
            lcd.print("Try to conserve water");
            playTone(1700, 1000);
            delay(5000);
      }
        }
    }
      else {
        RGB_color(0, 255, 0);
        lcd.clear();
        lcd.print("Water conserved");
    }
  delay(5000);
}
