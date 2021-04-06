#include "ESC.h"
#define LED_PIN (13)                    // Pin for the LED 
#define POT_PIN (A0)                    // Analog pin used to connect the potentiometer

ESC myESC (9, 1000, 2000, 1500);         // ESC_Name (PIN, Minimum Value, Maximum Value, Arm Value)

int val = 1500;                                // variable to read the value from the analog pin
int val_buffer = 1500;
bool forward_dir;
bool forward_dir_prev;
int arming = 1;
int arming_lock = 0;
int arming_interval = 30;
int time_now;
String readString;
int i;
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);             // LED Visual Output
  myESC.arm();                          // Send the Arm value
  digitalWrite(LED_PIN, HIGH);          // LED High Once Armed
  delay(5000);                          // Wait for a while
}

void loop() {
   while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

    if (readString.length() >0) {
   // Serial.println(readString);  //so you can see the captured string
    val_buffer = readString.toInt();  //convert readString into a number
    }

 readString=""; //empty for next input

  if (val_buffer >= 1500) {
    forward_dir = HIGH;
  }
  if (val_buffer < 1500){
    forward_dir = LOW;
  }
  //arming
     
   if(arming == 0 || arming_lock == 1){        
        arming_lock = 1;
        val = 1500;
        i++;
    if(i > arming_interval){
        time_now += arming_interval;        
        arming_lock = 0;
        i = 0;
    }
              
   }        
  else {
  arming = 1;
  val = val_buffer; 
 
 }


 
  // scale it to use it with the ESC (value between 1000 and 2000)
  myESC.speed(val);                     // sets the ESC speed according to the scaled value
  Serial.print(val);
  Serial.print(" arming : ");
  Serial.print(arming);
  Serial.print(" arming lock : ");
  Serial.print(arming_lock);
  Serial.print(" forward_dir : ");
  Serial.print(forward_dir);
  
  Serial.print(" timer : ");
  Serial.print(i);
   Serial.print(" forward_dir_prev : ");
  Serial.println(forward_dir_prev);
  
 
 
 if (forward_dir != forward_dir_prev){
  arming = 0;
 }  else {
  arming = 1;
 }

   
 
 forward_dir_prev = forward_dir; 
}
