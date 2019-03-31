/*
Authors ::  Solayman Hossain Emon,
            Lamia Nazrin, 
            Nahid Hasan, 
            Md. Toasin Habib.
           
Aust CSE 37th Batch

IDs::       16.02.04.091
            16.02.04.103
            16.02.04.106
            16.02.04.107
*/

#include<Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Servo ServoMotor;
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Set the LCD address to 0x27 

//Initialize the Variables
byte SmileIcon[] = {
  B00000,
  B01010,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

byte SadIcon[] = {
  B00000,
  B01010,
  B00000,
  B00000,
  B01110,
  B10001,
  B00000,
  B00000
};

const int servoPin = 4;
const int trigPin = 2;
const int echoPin = 3;
long duration, distance, average_Distance = 0;
long distance_Sum = 0;
long temp_Distance[3]; 
int CloseCoverDelay = 7;         // Open the Cover of the bucket for 7 seconds


void setup() {
  
  pinMode(trigPin, OUTPUT);      // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);       // Sets the echoPin as an Input
  
  ServoMotor.attach(servoPin);   // Attaches the servo on pin 4 
  ServoMotor.write(100);         // Initially Closed the cover of the bucket
  delay(100);
  ServoMotor.detach(); 
  
  Serial.begin(9600);            // Starts the serial communication
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, SmileIcon);
  lcd.createChar(1, SadIcon);

}

void measure() { 
   
  digitalWrite(trigPin, LOW);         // Clears the trigPin 
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);        // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration*0.034/2;        // Calculating the distance

}

// Displays the object distance in LCD
void display_Distance(String msg1, String msg2, int distance) { 
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg1);               // Prints the given custom String 
  lcd.setCursor(0,1);
  lcd.print(msg2);               // Prints the given custom String 
  lcd.print(distance);           // Prints the distance on the LCD
  lcd.print(" cm");

}

// Displays message on LCD After Opening the Cover of the bucket
void OpenBucket_Display(String msg1, String msg2, String msg3){
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.home();
  lcd.write(0);
  lcd.setCursor(2,0);
  lcd.print(msg1);
  lcd.setCursor(15,0);
  lcd.write(0);
  lcd.setCursor(2,1);
  lcd.print(msg2);
  
// Seven(7) Seconds delay before Closing the Cover  
for(int i = CloseCoverDelay; i >= 0; i--)
{
   lcd.setCursor(6,1);
   lcd.print(i);
   lcd.setCursor(8,1);
   lcd.print(msg3);
   delay(1000);
   lcd.print(" ");
}
  
}


// Displays message on LCD After Closing the Cover of the bucket
void CloseBucket_Display(String msg1, String msg2){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.home();
  lcd.write(1);
  lcd.setCursor(2,0);
  lcd.print(msg1);
  lcd.setCursor(15,0);
  lcd.write(1);
  lcd.setCursor(0,1);
  lcd.print(msg2);
  delay(3000);
  
}

void loop() {

// Measure the distance of the object using Sonar Sensor
for (int i = 0; i < 3; i++)      
{
  measure();               
  temp_Distance[i] =  distance;           
  delay(10);               
}
 
  average_Distance = (temp_Distance[0] + temp_Distance[1] + temp_Distance[2])/3; //calculate the average distance   
  display_Distance("Measuring ...","Distance :", average_Distance);    // Diplay the average distance of the object on LCD

// Condition for Open the Cover of the bucket
if ( average_Distance < 50 )     
{
 
  ServoMotor.attach(servoPin);
  delay(1);
  ServoMotor.write(0);                                 // Open the Cover of the Bucket
  
  OpenBucket_Display("Opened Cover","For","Seconds");  // Display message on LCD after the Cover Open
  
  ServoMotor.write(100);                               // Close the Cover of the Bucket
  CloseBucket_Display("Cover Closed","Thank You !!");  // Display message on LCD after Closed the Cover
  ServoMotor.detach(); 

}

}

