#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

#include <ESP32Servo.h>

Servo servo;

#define SERVO_PIN 26  // ESP32 pin GIOP26 connected to Servo Motor's pin
#define DISTANCE_THRESHOLD  50 // centimeters


#define TRIG_PIN 13 // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 12//
float duration_us, distance_cm;


int kled = 32;
int yled = 33;

#define SS_PIN 5
#define RST_PIN 23
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  servo.attach(SERVO_PIN);   // attaches the servo on pin 9 to the servo object
  servo.write(0);
  pinMode(kled, OUTPUT);
  pinMode(yled, OUTPUT);
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);

}
void loop() 
{
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;
  // Look for new cards
  if (distance_cm<15){
    lcd.clear();
    lcd.print("Kapi aciliyor");
    servo.write(90);
    Serial.print("kartsız gecis");
    digitalWrite(kled,LOW);
    digitalWrite(yled,HIGH);
    delay(5000);
    digitalWrite(kled,HIGH);
    digitalWrite(yled,LOW);
    lcd.clear();
    lcd.print("kapi kapaniyor");
    servo.write(0);
    
  }
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "59 D0 CA B2") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Tanimli Kart");
    digitalWrite(kled,LOW);
    digitalWrite(yled,HIGH);
    lcd.setCursor(0, 0);
    // print message
    lcd.clear();
    lcd.print("Kapi aciliyor");
    servo.write(90);
    delay(5000);
    digitalWrite(kled,HIGH);
    digitalWrite(yled,LOW);
    lcd.clear();
    lcd.print("kapi kapaniyor");
    servo.write(0);
    
    
    Serial.println();
    
  }
  
 
 else   {
    Serial.println("Tanimsiz Kart");
    digitalWrite(yled,LOW);
    digitalWrite(kled,HIGH);
    lcd.setCursor(0, 0);
    // print message
    lcd.print("tanımsız kart");
    delay(5000);
    lcd.clear();
  }
}
