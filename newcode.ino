#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Constants
const int thresholdTemp = 25; // Temperature threshold in Celsius
const int highTemp = 35; // High temperature threshold in Celsius
const int servoPin = 9; // Servo motor pin
const int servoPin_2 = 6; // Servo motor pin
 int irSensor1 = 2; // IR sensor 1 pin
 int irSensor2 = 3; // IR sensor 2 pin
const int buzzerPin = 4; // Buzzer pin
const int greenLEDPin = 5; // Green LED pin
const int tempSensorPin = A1; // LM35 temperature sensor pin
const int fanPin = 8; // DC motor (fan) pin
const int ldrPin = A0; // LDR sensor pin
const int led1Pin = 10; // LED 1 pin
const int led2Pin = 11; // LED 2 pin
const int buttonPin = 7; // Push button pin

// Variables
bool isDoorOpen = 0;
bool isAlarmOn = 0;


int temp=0;

// Initialize the servo
Servo doorServo;
Servo doorServo_2;

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 2; // Two rows
const byte COLS = 2; // Two columns
char keys[ROWS][COLS] = {
  {'1','2'},
  {'4','5'}
};
byte rowPins[ROWS] = {12, 13}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {A2, A3}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char Password[]="2141";
char passwordIndex = sizeof(Password)-1;
char EPassword[sizeof(Password)-1];
char counter =0;
char Flag =0;

void setup() {
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  doorServo.attach(servoPin);
  doorServo_2.attach(servoPin_2);
  lcd.init(); // Initialize the LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
}

void loop() {

  int value1 = digitalRead(irSensor1) ;
  int value2 = digitalRead(irSensor2);
  // Door control logic
  if ((value1 == HIGH )&&(value2==LOW)) {
    doorServo.write(180); // Open the door

  } else if ((value1== LOW) && (value2 == HIGH )) {
    doorServo.write(0); // Close the door

  }

 AccessHome();   
 


  // Temperature control logic
 if(temp==1){
  int tempReading = analogRead(tempSensorPin);
  float voltage = tempReading * 5.0 / 1024.0;
  float temperature = voltage * 100; // Convert voltage to temperature
  

  if (temperature > thresholdTemp&&temperature<highTemp) {
    lcd.setCursor(0,0);
    digitalWrite(fanPin, HIGH); // Turn on the fan
    lcd.print("temp: ");
     lcd.print(temperature);
     lcd.print(" C");
  } 

  if (temperature > highTemp ) {
    lcd.setCursor(0,0);
    digitalWrite(buzzerPin, HIGH);
     // Turn on the high temperature alarm
     digitalWrite(fanPin, LOW); // Turn on the fan
    lcd.print("temp: ");
     lcd.print(temperature);
     lcd.print(" C");
     digitalWrite(4,HIGH);
  }
    if (digitalRead(buttonPin) == HIGH) {
    lcd.setCursor(0,0);
    digitalWrite(4,LOW); 
    digitalWrite(buzzerPin, LOW); // Turn off the high temperature alarm
    digitalWrite(fanPin, LOW); // Turn on the fan
    lcd.print("temp: ");
     lcd.print(temperature);
     lcd.print(" C");
  }
  

  

  // LDR and LEDs brightness control logic
  int ldrValue = analogRead(ldrPin);
  int ledBrightness = ldrValue;
  lcd.setCursor(0, 1);
  if(ledBrightness<20)
  {
  digitalWrite(led2Pin, HIGH);
  digitalWrite(led1Pin,LOW);
  lcd.print("LEDs: ");
  lcd.print(ledBrightness);
  delay(2000);
  }
  else {
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin,LOW);
  lcd.print("LEDs: ");
  lcd.print(ledBrightness);
  delay(2000);
  }

}
}
void AccessHome ()
{
  char customKey = keypad.getKey(); // Check and Store Pressed Keys
  
  if ((customKey>'0')&&(customKey != '5')) // Condition To Enter Password
    {
   lcd.print('*'); // Print Star When We Enter Our Password
   EPassword[counter] = customKey; // Store Our Password 
   counter++;
    }
  else if (customKey == '5') // When We press D We  make Sure We Enter A password 
  {
    if(counter == passwordIndex) // if We Enter The Number Of Password Numbers is true
    {
      for(char counter1 = 0;counter1<passwordIndex;counter1++)
      {
        if(EPassword[counter1] == Password[counter1]) // Check if The Enter Password Numbers is True
        {
           Flag++; // Count Number of True Condition
        }
      }
        if(Flag == passwordIndex)
        {
          PrintRightPassword();
                       
        }
        else
        {
          PrintWrongPassword();         
        }      
    }
    else
    {
          PrintWrongPassword();      
    }  
  }  
}

void PrintRightPassword()
{
          lcd.clear();         
          lcd.setCursor(0,0);
          lcd.print("Welcome Home   ");
          digitalWrite(5,HIGH);
          digitalWrite(4,LOW);
          temp=1;
          doorServo_2.write(180);    
          delay(5000);
          doorServo_2.write(0);
          counter = 0;
          Flag = 0;

}

void PrintWrongPassword()
{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Wrong Password");
          digitalWrite(4,HIGH);
          delay(2000);
          counter = 0;
          Flag = 0;
          lcd.setCursor(0,0);
          lcd.print("Enter Password");
          lcd.setCursor(0,1); 

}