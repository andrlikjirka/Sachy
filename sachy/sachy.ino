#include <Keypad.h>

#define ECHOPIN A1 // Echo pin z HC-SC04 na pin A1
#define TRIGPIN A0 // Trig pin z HC-SC04 na pin A0 
#define ECHOPIN2 A3 // echopin cidla 2 na A3
#define TRIGPIN2 A2 // trigpin cidla 2 na A2
#define NECITLIVOST 1.5
#define RYCHLOST_SPOD 255
#define RYCHLOST_HOR 150
#define RELEPIN A4
#define ST_KLID 0
#define ST_PRO_FIG 1
#define ST_S_FIG 2
#define ST_UCHOP 3
#define DELAY_UCHOP 10

int stav = 0;
int stavM1 = 0;
int stavM2 = 0;
int motorstop = 1;
int uchop_cekani = 0;

//klavesnice
const byte ROWS = 4; // 4 řádky
const byte COLS = 4; // 4 sloupce

char hexaKeys[ROWS][COLS] = {
  {'A','B','C','D'},
  {'E','F','G','H'},
  {'1','2','3','4'},
  {'5','6','7','8'}
};
byte rowPins[ROWS] = {9, 8, 7, 13}; //čísla pinů s řadkem 1 2 3 4
byte colPins[COLS] = {12, 4, 3, 2}; //čísla pinu se sloupcem 1 2 3 4
 
//initializuje objekt klávesnice s názvem customKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
char lastkeyX = '0';
char lastkeyY = '0';
char lastkey1X = '0';
char lastkey1Y = '0';

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
    lastkey1X = lastkeyX;
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
    lastkey1Y = lastkeyY;
    lastkeyY = customKey;
  }
}
void setup() { 
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  //Nastaví sériovou komunikaci 
  Serial.begin(9600); 
  pinMode(RELEPIN, OUTPUT);

  //Nastaví pin 2 jako vstupní 
  pinMode(ECHOPIN, INPUT); 
  //Nastaví pin 3 jako výstupní 
  pinMode(TRIGPIN, OUTPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN2, INPUT);

} 

void planovac(){
  
  if(stav == ST_KLID){
    if((lastkeyX != '0')&&
       (lastkeyY != '0')&&
       (lastkey1X != '0')&&
       (lastkey1Y != '0')){
      stav = ST_PRO_FIG;
      motorstop = 1;
    } 
    digitalWrite(A4, LOW);
  }
  
  if(stav == ST_PRO_FIG){
    motorstop = 0;
    digitalWrite(A4, LOW);
    pozadX = souradniceX[lastkey1X - 'A' + 1];
    pozadY = souradniceY[lastkey1Y - '1']; 
    if((stavM1 == 1)&&(stavM2 == 1)){
      stav = ST_UCHOP;
      
      
    }
  }
  if(stav == ST_UCHOP){
    uchop_cekani = uchop_cekani + 1;
    if(uchop_cekani == DELAY_UCHOP){
      stav = ST_S_FIG;
      uchop_cekani = 0;
      stavM1 = 0;
      stavM2 = 0;
    }
  }
    
    
  if(stav == ST_S_FIG){
    pozadX = souradniceX[lastkeyX - 'A' + 1];
    pozadY = souradniceY[lastkeyY - '1']; 
    digitalWrite(A4, HIGH);
    if((stavM1 == 1)&&(stavM2 == 1)){
      stavM1 = 0;
      stavM2 = 0;
      stav = ST_KLID;
      motorstop = 1;
      lastkeyX = '0';
      lastkeyY = '0';
      lastkey1X = '0';
      lastkey1Y = '0';

    }
  }
}
  
void motor2(float vzdalY, float vzdalYpozadovana){
  //spodni motor
  stavM2 = 0;
  if(motorstop == 1){
     analogWrite(5, 0);
     analogWrite(6, 0);
      
  }
  else{
    
    if(vzdalY < (vzdalYpozadovana - NECITLIVOST)){
      analogWrite(6, 0);
      analogWrite(5, RYCHLOST_SPOD);
      stavM2 = 3;
    }
    if(vzdalY > (vzdalYpozadovana + NECITLIVOST)){
      analogWrite(5, 0);
      analogWrite(6, RYCHLOST_SPOD);
      stavM2 = 4;
    }
    if ((vzdalY > (vzdalYpozadovana - NECITLIVOST)) && (vzdalY < (vzdalYpozadovana + NECITLIVOST))){
      analogWrite(5, 0);
      analogWrite(6, 0);
      stavM2 = 1;
    }
  }
}

void motor1(float vzdalX, float vzdalXpozadovana){
  //horni motor
  stavM1 = 0;
  if(motorstop == 1){
    analogWrite(10, 0);
    analogWrite(11, 0);
  }
  else{
    if(vzdalX < (vzdalXpozadovana - NECITLIVOST)){
      analogWrite(11, 0);
      analogWrite(10, RYCHLOST_HOR);
      stavM1 = 5;
    }
    if(vzdalX > (vzdalXpozadovana + NECITLIVOST)){
      analogWrite(10, 0);
      analogWrite(11, RYCHLOST_HOR);
      stavM1 = 6;
    }
    if ((vzdalX > (vzdalXpozadovana - NECITLIVOST)) && (vzdalX < (vzdalXpozadovana + NECITLIVOST))){
      analogWrite(10, 0);
      analogWrite(11, 0);
      stavM1 = 1;
    }
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
Serial.print("=pozadX ");
Serial.print(pozadY);
Serial.print("=pozadY ");
Serial.print("stav=");
Serial.print(stav);
Serial.print("  ");
Serial.print(lastkeyX);
Serial.print(lastkeyY);
Serial.print(lastkey1X);
Serial.print(lastkey1Y);
Serial.print("  ");
Serial.print("stavM1=");
Serial.print(stavM1);
Serial.print("  ");
Serial.print(" stavM2=");
Serial.print(stavM2);
Serial.print("\n");
delay(100); 
}


