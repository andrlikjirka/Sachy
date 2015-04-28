#include <Keypad.h>

#define ECHOPIN A1 // Echo pin z HC-SC04 na pin A1
#define TRIGPIN A0 // Trig pin z HC-SC04 na pin A0 
#define ECHOPIN2 A3 // echopin cidla 2 na A3
#define TRIGPIN2 A2 // trigpin cidla 2 na A2
#define NECITLIVOST 0.8


//klavesnice
const byte ROWS = 4; // 4 řádky
const byte COLS = 4; // 4 sloupce

char hexaKeys[ROWS][COLS] = {
  {'A','B','C','D'},
  {'E','F','G','H'},
  {'1','2','3','4'},
  {'5','6','7','8'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //čísla pinů s řadkem 1 2 3 4
byte colPins[COLS] = {5, 4, 3, 2}; //čísla pinu se sloupcem 1 2 3 4
 
//initializuje objekt klávesnice s názvem customKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
char lastkeyX = 'A';
char lastkeyY = '1';

float souradniceX[] = {3.5, 11.5, 20.5, 29.5, 38.5, 47.5, 56.5, 65.5, 74.5,82.5};
float souradniceY[] = {11.5, 20.5, 29.5, 38.5, 47.5, 56.5, 65.5, 74.5};



float pozadX = 0;
float pozadY = 0;

void klavesnice(){
  char customKey = customKeypad.getKey();
  Serial.print (customKey);
  if(
  (customKey == 'A')||
  (customKey == 'B')||
  (customKey == 'C')||
  (customKey == 'D')||
  (customKey == 'E')||
  (customKey == 'F')||
  (customKey == 'G')||
  (customKey == 'H')){
   lastkeyX = customKey;
  }
  if(
  (customKey == '1')||
  (customKey == '2')||
  (customKey == '3')||
  (customKey == '4')||
  (customKey == '5')||
  (customKey == '6')||
  (customKey == '7')||
  (customKey == '8')){
   lastkeyY = customKey;
  }
}
void setup() { 
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  //Nastaví sériovou komunikaci 
  Serial.begin(9600); 


  //Nastaví pin 2 jako vstupní 
  pinMode(ECHOPIN, INPUT); 
  //Nastaví pin 3 jako výstupní 
  pinMode(TRIGPIN, OUTPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN2, INPUT);

} 

void planovac(){
//  if( lastkeyX == 'A'){
//    pozadX = souradniceX[1];
//  }
//  if( lastkeyX == 'B'){
//    pozadX = souradniceX[2];
//  }
//  if( lastkeyX == 'C'){
//    pozadX = souradniceX[3];
//  }
//  if( lastkeyX == 'D'){
//    pozadX = souradniceX[4];
//  }
//  if( lastkeyX == 'E'){
//    pozadX = souradniceX[5];
//  }
//  if( lastkeyX == 'F'){
//    pozadX = souradniceX[6];
//  }
//  if( lastkeyX == 'G'){
//    pozadX = souradniceX[7];
//  }
//  if( lastkeyX == 'H'){
//    pozadX = souradniceX[8];
//  }
  
  pozadX = souradniceX[lastkeyX - 'A' + 1];
  pozadY = souradniceY[lastkeyY - '1']; 
  
}
  
void motor2(float vzdalY, float vzdalYpozadovana){
  if(vzdalY < (vzdalYpozadovana - NECITLIVOST)){
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
  }
  if(vzdalY > (vzdalYpozadovana + NECITLIVOST)){
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
  }
  if ((vzdalY > (vzdalYpozadovana - NECITLIVOST)) && (vzdalY < (vzdalYpozadovana + NECITLIVOST))){
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
    
}

void motor1(float vzdalX, float vzdalXpozadovana){
  if(vzdalX < (vzdalXpozadovana - NECITLIVOST)){
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
  }
  if(vzdalX > (vzdalXpozadovana + NECITLIVOST)){
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
  }
  if ((vzdalX > (vzdalXpozadovana - NECITLIVOST)) && (vzdalX < (vzdalXpozadovana + NECITLIVOST))){
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }
    
}

float ultrazvuk2() {
  // Vyšle impuls do modulu HC-SR04 
digitalWrite(TRIGPIN2, LOW); 
delayMicroseconds(2); 
digitalWrite(TRIGPIN2, HIGH); 
delayMicroseconds(10); 
digitalWrite(TRIGPIN2, LOW); 
// Spočítá vzdálenost 
float distance = pulseIn(ECHOPIN2, HIGH); 
distance= distance*0.017315f; 
// odešle informace na sérivý port 

return distance;
}


float ultrazvuk1() {
  // Vyšle impuls do modulu HC-SR04 
digitalWrite(TRIGPIN, LOW); 
delayMicroseconds(2); 
digitalWrite(TRIGPIN, HIGH); 
delayMicroseconds(10); 
digitalWrite(TRIGPIN, LOW); 
// Spočítá vzdálenost 
float distance = pulseIn(ECHOPIN, HIGH); 
distance= distance*0.017315f; 
 

return distance;
}


void loop() { 
 klavesnice(); 
 float vzdalX = ultrazvuk1();
 planovac();
 motor1(vzdalX, pozadX);
 float vzdalY = ultrazvuk2();
 motor2(vzdalY, pozadY);
//počká 1 sekundu 
Serial.print(vzdalX); 
Serial.print("cm ");
Serial.print(vzdalY); 
Serial.print("cm ");
Serial.print(pozadX);
Serial.print("pozadX ");
Serial.print(pozadY);
Serial.print("pozadY\n");
delay(100); 
}


