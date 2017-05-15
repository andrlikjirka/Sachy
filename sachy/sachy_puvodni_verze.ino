//funkční verze pro posuv podle druhu figurky s prepisem pozice

//Vložení předem stažené knihovny
#include <Keypad.h>

// přejmenování pinu pro piezo
int piezo = 6;  //!!!

//Definování pinů, stavů a relátek
#define ECHOPIN A1 // Echo pin z HC-SC04 na pin A1
#define TRIGPIN A0 // Trig pin z HC-SC04 na pin A0 
#define ECHOPIN2 A5 // echopin cidla 2 na A5
#define TRIGPIN2 A4 // trigpin cidla 2 na A4
#define NECITLIVOST 1
#define RYCHLOST_HOR 250
#define ST_KLID 0
#define ST_PRO_FIGURKU 1
#define ST_S_FIG 2
#define ST_UCHOP 3
#define ST_NA_CARU 4
#define ST_POSUV_X 5
#define ST_POSUV_Y 6
#define ST_NA_POLE 7
#define DELAY_UCHOP 5
#define RELNAPAJENI 27
#define REL4 29
#define REL5 31
#define REL6 33
#define REL7 35
#define REL8MAGNET 37

void setup() {
  //Nastavení sériové komunikace
  Serial.begin(9600);
  //nastavení pinů IN/OUT
  pinMode(piezo, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(RELNAPAJENI, OUTPUT);
  pinMode(REL4, OUTPUT);
  pinMode(REL5, OUTPUT);
  pinMode(REL6, OUTPUT);
  pinMode(REL7, OUTPUT);
  pinMode(REL8MAGNET, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN2, INPUT);
  //zadání výchozích stavů pro relé
  digitalWrite(RELNAPAJENI, HIGH);  //privod napajeni 5v k rele 4 5 6 7 odpojen
  digitalWrite(REL8MAGNET, HIGH);
  digitalWrite(REL4, HIGH);
  digitalWrite(REL5, HIGH);
  digitalWrite(REL6, HIGH);
  digitalWrite(REL7, HIGH);
  Serial.println("Bily je na tahu ... ");

}

//matice pro umístění figurek a jejich následné posouvání a přemisťování
char umistni_figurek[8][8] = {
  {'V', 'J', 'S', 'D', 'K', 'S', 'J', 'V'},
  {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
  {'0', '0', '0', '0', '0', '0', '0', '0'},
  {'0', '0', '0', '0', '0', '0', '0', '0'},
  {'0', '0', '0', '0', '0', '0', '0', '0'},
  {'0', '0', '0', '0', '0', '0', '0', '0'},
  {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
  {'v', 'j', 's', 'd', 'k', 's', 'j', 'v'}
};
//natavení výchozích hodnot
int stav = 0;
int stav_motor_spodni = 0;
int stav_motor_horni = 0;
int motorstop = 1;
int uchop_cekani = 0;
int druh_figurky = 0;
int tisk = 1; //vypis na seriovy kanal zakazan
int naTahu = 1; //cerny je na tahu



//klavesnice
const byte ROWS = 4; // 4 řádky
const byte COLS = 4; // 4 sloupce

char hexaKeys[ROWS][COLS] = {
  {'A', 'B', 'C', 'D'},
  {'E', 'F', 'G', 'H'},
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'}
};
byte rowPins[ROWS] = {9, 8, 7, 13}; //čísla pinů s řadkem 1 2 3 4
byte colPins[COLS] = {12, 4, 3, 2}; //čísla pinu se sloupcem 1 2 3 4

//inicializuje objekt klávesnice s názvem customKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char kamX = '0';
char kamY = '0';
char odkudX = '0';
char odkudY = '0';

//Odměřené vzdálenosti políček
float souradniceX[] = {68, 58.5, 52, 42, 33, 23, 14, 5}; //vzdálenosti pro čísla od 1 po 8
float souradniceY[] = {9.5, 18.5, 28, 36.5, 46, 54, 62.5, 71.5,}; //vzdálenosti pro písmena od A po H

float pozadovana_X = 0;
float pozadovana_Y = 0;
//Výber znaku po zadání a uložení na výchozí a požadovanou hodnotu (ODKUD-KAM)
void klavesnice() {
  char customKey = customKeypad.getKey();
  if (
    (customKey == 'A') ||
    (customKey == 'B') ||
    (customKey == 'C') ||
    (customKey == 'D') ||
    (customKey == 'E') ||
    (customKey == 'F') ||
    (customKey == 'G') ||
    (customKey == 'H')) {
    //pípnutí po zadání písmene
    digitalWrite(piezo, HIGH);
    delay(200);
    digitalWrite(piezo, LOW);
    odkudY = kamY;
    kamY = customKey;
  }
  if (
    (customKey == '1') ||
    (customKey == '2') ||
    (customKey == '3') ||
    (customKey == '4') ||
    (customKey == '5') ||
    (customKey == '6') ||
    (customKey == '7') ||
    (customKey == '8')) {
    //pípnutí po zadání číslice
    digitalWrite(piezo, HIGH);
    delay(200);
    digitalWrite(piezo, LOW);
    odkudX = kamX;
    kamX = customKey;
  }
}
void planovac() {
  //stav ve kterém se čeká na zadání souřadnic ODKUD-KAM
  if (stav == ST_KLID) {
    if ((kamX != '0') &&
        (kamY != '0') &&
        (odkudX != '0') &&
        (odkudY != '0')) {
          if(tisk == 1){
             Serial.print("Pozadovany tah: ");
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'V'){
               Serial.print("Bila vez z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'v'){
               Serial.print("Cerna vez z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'J'){
               Serial.print("Bily jezdec z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'j'){
               Serial.print("Cerny jezdec z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'S'){
               Serial.print("Bily strelec z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 's'){
               Serial.print("Cerny strelec z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'D'){
               Serial.print("Bila kralovna z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'd'){
               Serial.print("Cerna kralovna z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'K'){
               Serial.print("Bily kral z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'k'){
               Serial.print("Cerny kral z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'P'){
               Serial.print("Bily pesec z ");
             }
             if(umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'p'){
               Serial.print("Cerny pesec z ");
             }
             Serial.print(odkudY);
             Serial.print(odkudX);
             Serial.print(" na ");
             Serial.print(kamY);
             Serial.println(kamX);
             tisk = 0;
          }
          
      //nastavení druhu figurky(zda-li se bude pohybovat v citybloku = 0, nebo jednoduše = 1)
      if ((umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'V') ||
          (umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'v') ||
          (umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'K') ||
          (umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'k') ||
          (umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'P') ||
          (umistni_figurek[odkudX - '1'][odkudY - 'A'] == 'p')) {
        druh_figurky = 1;
      }
      else {
        druh_figurky = 0;
      }
      stav = ST_PRO_FIGURKU;
      tisk = 1;
      motorstop = 1;
    }
  }
  //motory jedou pro figurku (oba současně)
  if (stav == ST_PRO_FIGURKU) {
    if(tisk == 1){
       Serial.println("Jedu pro figurku ... ");
       tisk = 0;
    }
    motorstop = 0;
    digitalWrite(REL8MAGNET, HIGH);
    pozadovana_X = souradniceX[odkudX - '1'];
    pozadovana_Y = souradniceY[odkudY - 'A'];
    if ((stav_motor_horni == 1) && (stav_motor_spodni == 1)) {
      stav = ST_UCHOP;
      tisk = 1;
      motorstop = 0;
    }
  }
  //Zde spíná magnet. Je zde chvilka hystereze pro bezpečné spojení magnetů.
  if (stav == ST_UCHOP) {
    uchop_cekani = uchop_cekani + 1;
    if (uchop_cekani == DELAY_UCHOP) {
      digitalWrite(REL8MAGNET, LOW);
      Serial.print("Spinam magnet, pohyb figurky zpusobem: ");
      if (druh_figurky == 0) {
        Serial.println("CITYBLOK");
        stav = ST_NA_CARU;

      }
      else {
        Serial.println("PRIMO");
        stav = ST_NA_POLE;
      }
      uchop_cekani = 0;
      stav_motor_horni = 0;
      stav_motor_spodni = 0;
    }
  }
  //fugurka popojede na kříž dolu doprava
  if (stav == ST_NA_CARU) {
    motorstop = 0;
    pozadovana_X = souradniceX[odkudX - '1'] + 4.5;
    pozadovana_Y = souradniceY[odkudY - 'A'] + 4.5;
    if ((stav_motor_horni == 1) && (stav_motor_spodni == 1)) {
      stav_motor_horni = 0;
      stav_motor_spodni = 0;
      stav = ST_POSUV_X;

    }
  }
  //figurka jede po ose X(1-8) a zastaví se na kříži pod požadovaným políčkem
  if (stav == ST_POSUV_X) {
    motorstop = 0;
    pozadovana_X = souradniceX[kamX - '1'] + 4.5;
    if (stav_motor_horni == 1) {
      stav_motor_horni = 0;
      stav_motor_spodni = 0;
      stav = ST_POSUV_Y;
    }
  }
  //figurka jede po ose Y(A-H) a zastaví se na kříži vpravo od požadovaného políčka
  if (stav == ST_POSUV_Y) {
    motorstop = 0;
    pozadovana_Y = souradniceY[kamY - 'A'] + 4.5;
    if (stav_motor_spodni == 1) {
      stav_motor_spodni = 0;
      stav = ST_NA_POLE;
    }
  }
  // v případě citybloku figurka popojede uhlopříčně na střed opožadovaného políčka
  //v případě přímého tahu figurka jede z výchozí hodnoty přímo na střed políčka bez zbytečného zdržování
  if (stav == ST_NA_POLE) {
    motorstop = 0;
    pozadovana_X = souradniceX[kamX - '1'];
    pozadovana_Y = souradniceY[kamY - 'A'];
    if ((stav_motor_horni == 1) && (stav_motor_spodni == 1)) {
      stav_motor_horni = 0;
      stav_motor_spodni = 0;
      //přepisování nové polohy figurky do matice a nulování výchozí polohy
      umistni_figurek[kamX - '1'][kamY - 'A'] = umistni_figurek[odkudX - '1'][odkudY - 'A'];
      umistni_figurek[odkudX - '1'][odkudY - 'A'] = '0';
      //nulování zadaných souřadnic
      kamX = '0';
      kamY = '0';
      odkudX = '0';
      odkudY = '0';
      digitalWrite(REL8MAGNET, HIGH);
      Serial.println("Byl dokoncen tah");
      Serial.println("");
      if(naTahu == 0){
         Serial.println("Bily je na tahu ... ");
         naTahu = 1;
      }
      else{
         Serial.println("Cerny je na tahu ... ");
         naTahu = 0;
      }
      stav = ST_KLID;
      motorstop = 1;
    }
  }
}
//Nastavování relátek pro posuv po Y což je spodní motor
void motor_spodni(float vzdalenost_Y, float vzdalenost_Y_pozadovana) {
  stav_motor_spodni = 0;
  if (motorstop == 1) {
    //Motor je zablokovaný a nikam se nehýbe. Čeká na zadání sopuřadnic.
    digitalWrite(RELNAPAJENI, HIGH);
    delay(200);
    digitalWrite(REL4, HIGH);
    digitalWrite(REL5, HIGH);
    digitalWrite(REL6, HIGH);
    digitalWrite(REL7, HIGH);
  }
  else {
    //pohyb doprava od A po H
    if (vzdalenost_Y < (vzdalenost_Y_pozadovana - NECITLIVOST)) {
      digitalWrite(REL4, HIGH);
      digitalWrite(REL5, LOW);
      digitalWrite(REL6, LOW);
      digitalWrite(REL7, HIGH);
      delay(200);
      digitalWrite(RELNAPAJENI, LOW);
      stav_motor_spodni = 3;
    }
    //pohyb doleva od H po A
    if (vzdalenost_Y > (vzdalenost_Y_pozadovana + NECITLIVOST)) {
      digitalWrite(REL4, LOW);
      digitalWrite(REL5, HIGH);
      digitalWrite(REL6, HIGH);
      digitalWrite(REL7, LOW);
      delay(200);
      digitalWrite(RELNAPAJENI, LOW);
      stav_motor_spodni = 4;
    }
    //motor je v cílové poloze
    if ((vzdalenost_Y > (vzdalenost_Y_pozadovana - NECITLIVOST)) && (vzdalenost_Y < (vzdalenost_Y_pozadovana + NECITLIVOST))) {
      digitalWrite(RELNAPAJENI, HIGH);
      delay(200);
      digitalWrite(REL4, HIGH);
      digitalWrite(REL5, HIGH);
      digitalWrite(REL6, HIGH);
      digitalWrite(REL7, HIGH);
      stav_motor_spodni = 1;
    }
  }
}
//nastavení pinů pro horní posuv
void motor_horni(float vzdalenost_X, float vzdalenost_X_pozadovana) {
  stav_motor_horni = 0;
  if (motorstop == 1) {
    //Motor je zablokovaný a nikam se nehýbe. Čeká na zadání souřadnic
    digitalWrite(11, 0);
    digitalWrite(10, 0);
  }
  else {
    //Motor jede dolu od 8 po 1
    if (vzdalenost_X < (vzdalenost_X_pozadovana - NECITLIVOST)) {
      analogWrite(11, 0);
      analogWrite(10, RYCHLOST_HOR);
      stav_motor_horni = 5;
    }
    //Motor jede nahoru od 1 po 8
    if (vzdalenost_X > (vzdalenost_X_pozadovana + NECITLIVOST)) {
      analogWrite(10, 0);
      analogWrite(11, RYCHLOST_HOR);
      stav_motor_horni = 6;
    }
    //Motor je v cílové poloze
    if ((vzdalenost_X > (vzdalenost_X_pozadovana - NECITLIVOST)) && (vzdalenost_X < (vzdalenost_X_pozadovana + NECITLIVOST))) {
      analogWrite(10, 0);
      analogWrite(11, 0);
      stav_motor_horni = 1;
    }
  }
}
//odměřování vzdálenosti ultrazvukem
float ultrazvuk2() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  float distance = pulseIn(ECHOPIN, HIGH);
  distance = distance * 0.017315f;
  return distance;
}

//odměřování vzdálenosti ultrazvukem
float ultrazvuk1() {
  digitalWrite(TRIGPIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN2, LOW);
  float distance = pulseIn(ECHOPIN2, HIGH);
  distance = distance * 0.017315f;
  return distance;
}

//hlavní smyčka programu
void loop() {
  klavesnice();
  float vzdalenost_X = ultrazvuk1();
  planovac();
  motor_horni(vzdalenost_X, pozadovana_X);
  float vzdalenost_Y = ultrazvuk2();
  motor_spodni(vzdalenost_Y, pozadovana_Y);
  delay(100);
}




