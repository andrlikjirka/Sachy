#include <Keypad.h>
 
int LED_CLOSE = 11; // červená LED
int LED_OPEN = 12; // zelená LED
 
 
const byte ROWS = 4; // 4 řádky
const byte COLS = 4; // 4 sloupce
 
// sem se budou ukládat stiskuté znaky
char pressedKey[5] = {'0','0','0','0'};
// náš kód pro otevření dveří
char code[5] = {'1','9','4','7'};
 
 
// zde si napíšete jak Vaše
// membránová klávesnice vypadá
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //čísla pinů s řadkem 1 2 3 4
byte colPins[COLS] = {5, 4, 3, 2}; //čísla pinu se sloupcem 1 2 3 4
 
//initializuje objekt klávesnice s názvem customKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
void setup(){
  Serial.begin(9600);
  Serial.println("Odesilani stisknutych klaves na seriovou linku");
  pinMode(LED_OPEN, OUTPUT); // pin jako výstup
  pinMode(LED_CLOSE, OUTPUT); // pin jako výstup
 
  digitalWrite(LED_CLOSE, HIGH); // rozsvítí červenou
}
 
void loop(){
  // přečte stiskuté tlačítko a uloží do customKey
  char customKey = customKeypad.getKey();
 
  if (customKey){
    // když je stisknuto potvrzovací tlačítko #
    Serial.println(customKey);
    if (customKey == '#')
    {
    // když se pole kódu rovná poli stiskutých talčítek
      if ((pressedKey[0] == code[0]) && (pressedKey[1] == code[1]) && (pressedKey[2] == code[2]) && (pressedKey[3] == code[3]))
        {
         
          digitalWrite(LED_CLOSE, LOW); // zhasni červenou
          digitalWrite(LED_OPEN, HIGH); // rozsvit zelenou
          tone(10,2400,4000); // zahraj tón na pinu 10 (2400Hz, 4sec)
          delay(4000); // čekej 4 sekundy
          digitalWrite(LED_OPEN, LOW); // zhasni zelenou
          digitalWrite(LED_CLOSE, HIGH); // zhasni červenou
          // vynuluj stiskuté tlačítka
          pressedKey[0] = '0';
          pressedKey[1] = '0';
          pressedKey[2] = '0';
          pressedKey[3] = '0';
        }
    // jinak přo špatném kódu zahraj jiný tón a nic nedělej
    else tone(10,500,1000); //tón na pinu 10 (500Hz, 1 vteřina)      
    }
   
    // když není stisknuto potvrzovací tlačítko #
     else
    {
   
    //přidej znak do pole a znaky posuň
    pressedKey[0] = pressedKey[1];
    pressedKey[1] = pressedKey[2];
    pressedKey[2] = pressedKey[3];
    pressedKey[3] = customKey;  
 
    // zahraje tón při stisku tlačítka
    tone(10,1200,80);
    }
}
}
