#include <Password.h> //http://playground.arduino.cc/uploads/Code/Password.zip //tells to use password library
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip  //tells to use keypad library
 //tells to use servo library
#include <SPI.h>
#include <RFID.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define SS_PIN 9
#define RST_PIN 8
bool rfidaccess=false;
bool keyaccess=false;
bool total=false;
bool pass=false;
RFID rfid(SS_PIN,RST_PIN);
int serNum[5];
int cards[][5] = {
  {70,74,120,65,53}
  
};

Password password = Password( "3693" ); //password to unlock, can be changed

const byte ROWS = 4; // Four rows
const byte COLS = 4; // columns
// Define the Keymap
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 22,24, 26, 28 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = {44, 32, 34, 36 };

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  SPI.begin();
  delay(200); 
 pinMode(45,OUTPUT);
pinMode(43,OUTPUT);
  lcd.begin(16,2);
   rfid.init();
    //red light
   //servo on digital pin 9 //servo
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  }

void loop(){
 keypad.getKey();
  rfid1();
  if(total==false && pass==false && keyaccess==false && rfidaccess==true)
  {lcd.clear();
    lcd.setCursor(0,0);
  lcd.print("press C to ");
  lcd.setCursor(0,1);
   lcd.print(" give password");
 
 // lcd.setCursor(0,1);
 // lcd.print("then press A ");
  }
  if(pass==true)
  {
    lcd.clear();
    lcd.println("press A");
    lcd.setCursor(0,1);
    lcd.println("after password");
  }
  if(total==true)
  {lcd.clear();
    lcd.print("swip card2close");
 // lcd.setCursor(0,1);
 // lcd.print("open");
  }
 
  
  if(keyaccess==false && rfidaccess == false && total ==false && pass==false)
  { lcd.clear();
    lcd.println("swip card2 open");
  }
 if( keyaccess==true && rfidaccess ==true && total==false)
 {lcd.clear();
   Serial.println("open");
   lcd.print("open");
   opendoor();
 delay(100);
 keyaccess=false;
 rfidaccess=false;
 total=true;
 delay(500);
  lcd.clear();
 }
 if(total==true&&rfidaccess==true)
 {lcd.clear();
   Serial.println("close");
   closedoor();
   lcd.println("close");
   //lcd.setCursor(0,1);
  
   delay(500);
   
   total=false;
   rfidaccess=false;
   delay(500);
    lcd.clear();
 }
}
  //take care of some special events
  void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
  case PRESSED:
  
  Serial.print("Enter:");
  Serial.println(eKey);
  delay(10);
  
  
  switch (eKey){
    case 'A': checkPassword(); delay(10); break;
    
    case 'C': 
    {password.reset();
    pass=true;
     break;
    }    
     default: password.append(eKey); delay(1);
}
}
}
void checkPassword(){
  
if (password.evaluate()){  //if password is right open

    lcd.clear();
    Serial.println("Accepted");
    lcd.print("accepted");

   
  
    //lcd.print("swipe ur rfid card");
    keyaccess=true;
    pass=false;
    delay(200);
     lcd.clear();
   
    //Add code to run if it works

    
       
    
}else{
  lcd.clear();
    Serial.println("Denied");
   lcd.print("wrong password");
  
   //if passwords wrong keep locked
    keyaccess=false;
    pass=false;
    //add code to run if it did not work
    delay(200);
     lcd.clear();
   
    
}
}
void rfid1()
{

  if(rfid.isCard()){
    
        if(rfid.readCardSerial()){
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");
            
            for(int x = 0; x < sizeof(cards); x++){
              for(int i = 0; i < sizeof(rfid.serNum); i++ ){
                  if(rfid.serNum[i] != cards[x][i]) {
                      rfidaccess = false;
                      break;
                  } else {
                      rfidaccess = true;
                  }
              }
              if(rfidaccess) break;
            }
           
        }
        
       if(rfidaccess){
          Serial.println("Welcome!");
          rfidaccess = true;
         // lcd.clear();
          
           

           
      } else {
        lcd.clear();
           Serial.println("Not allowed!"); 
           rfidaccess = false;
          
           lcd.println("wrong id card");
          
           delay(500);
           lcd.clear();
       }        
    }
    
    
    
    rfid.halt();

}
void closedoor()
{
 digitalWrite(45,HIGH);
delay(10000);
digitalWrite(45,LOW);
}
void opendoor()
{
  digitalWrite(43,HIGH);
  delay(50);
  digitalWrite(45,HIGH);
  delay(10000);
  digitalWrite(45,LOW);
   digitalWrite(43,LOW);
  
 // delay(50);

  

}
  


