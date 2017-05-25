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
#define ST_PRAVIDLA 1
#define ST_CHYBNY_TAH 2
#define ST_VYHAZOVANI 3
#define ST_PRO_FIGURKU 4
#define ST_S_FIG 5
#define ST_UCHOP 6
#define ST_NA_CARU 7
#define ST_POSUV_X 8
#define ST_POSUV_Y 9
#define ST_NA_POLE 10
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
  Serial.setTimeout(100);
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
char umisteni_figurek[8][8] = {
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

char kam_cislo = '0';
char kam_pismeno = '0';
char odkud_cislo = '0';
char odkud_pismeno = '0';

//Odměřené vzdálenosti políček
float souradniceX[] = {68, 58.5, 52, 42, 33, 23, 14, 5}; //vzdálenosti pro čísla od 1 po 8
float souradniceY[] = {9.5, 18.5, 28, 36.5, 46, 54, 62.5, 71.5,}; //vzdálenosti pro písmena od A po H

float pozadovana_X = 0;
float pozadovana_Y = 0;

#define VELIKOST_PAMETI_TAHU 32
char pamet_tahu[VELIKOST_PAMETI_TAHU][4]={
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'}, 
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'}, 
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'}, 
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'}, 
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'}, 
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'}, 
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0'},
};
int pamet_tahu_index = 0;
int pamet_tahu_pocet = 0;

#define DLST_NORMAL 0
#define DLST_PREHRAVANI_TAHU 1

int dlouhodoby_stav = DLST_NORMAL;

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
    odkud_pismeno = kam_pismeno;
    kam_pismeno = customKey;
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
    odkud_cislo = kam_cislo;
    kam_cislo = customKey;
  }

}

int p = 0; //cislo udavajici pocet zadanych tahu

void ulozeni_tahu_do_pameti() {
  pamet_tahu[p][0] = '0';
  pamet_tahu[p][1] = '0';
  pamet_tahu[p][2] = '0';
  pamet_tahu[p][3] = '0';
  
  pamet_tahu[p][0] = odkud_pismeno;
  pamet_tahu[p][1] = odkud_cislo;
  pamet_tahu[p][2] = kam_pismeno;
  pamet_tahu[p][3] = kam_cislo;
}

void pravidla() {
  if(tisk == 1){
    Serial.println("Pravidla tahu: ");
    if((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'V')||
       (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'v')){
      Serial.println("Vez se smi pohybovat o libovolne mnozstvi poli pouze ve svislem nebo vodorovnem smeru!");
    }
    if((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'J')||
       (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'j')){
      Serial.println("Jezdec se smi pohybovat jen do tvaru pismene L (dve pole rovne, jedno do strany / jedno rovne, dve do strany!)"); 
    }
    if((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'S')||
       (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 's')){
      Serial.println("Strelec se smi pohybovat pouze po diagonalach!");
    }
    if((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'D')||
       (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'd')){
      Serial.println("Dama se smi pohybovat po sloupcich (radach) nebo diagonalach!");  
    }
    if((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'K')||
       (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'k')){
      Serial.println("Kral se smi pohybovat o jedno pole v libovolnem smeru! Pozor na sach!");
    }
    if((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'P')||
       (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'p')){
      Serial.println("V zakladnim postaveni smi pesec tahnout az o dva kroky vpred (cilove pole i pole, pres ktere prechazi musi byt prazdna)!"); 
      Serial.println("Pokud neni pesec v zakladnim postaveni, smi se pohybovat jen o jedno pole vpred!"); 
      Serial.println("Pesec bere diagonalne!");
      Serial.println("Pozn.: Pesec je jedina figurka, ktera se nesmi vratit zpet na puvodni pole!");
    }
    tisk = 0;
  }
}

void prehravani_tahu_init(){
  Serial.println("Probiha prehravani tahu!");
  pamet_tahu_index = 0;
  pamet_tahu_pocet = p;
  dlouhodoby_stav = DLST_PREHRAVANI_TAHU;
  prehravani_tahu_iterace();
}

void prehravani_tahu_iterace(){
  if (pamet_tahu_index < pamet_tahu_pocet){
    odkud_pismeno = pamet_tahu[pamet_tahu_index][0];
    odkud_cislo = pamet_tahu[pamet_tahu_index][1];
    kam_pismeno = pamet_tahu[pamet_tahu_index][2];
    kam_cislo = pamet_tahu[pamet_tahu_index][3];
    stav = ST_PRO_FIGURKU;
    pamet_tahu_index++;
    planovac_klid_tisk();
  }
  else{
    dlouhodoby_stav = DLST_NORMAL;
  }
}

#define DLST_VYKLIZENI_SACHOVNICE 2

void vyklizeni_sachovnice_init() {                  
  Serial.println("Probiha vyklizeni sachovnice!");
  pamet_tahu_index = 0;
  pamet_tahu_pocet = p;
  dlouhodoby_stav = DLST_VYKLIZENI_SACHOVNICE;
  vyklizeni_sachovnice_iterace();
}

void vyklizeni_sachovnice_iterace(){
  if (pamet_tahu_index < pamet_tahu_pocet){
    kam_pismeno = pamet_tahu[pamet_tahu_index][0];
    kam_cislo = pamet_tahu[pamet_tahu_index][1];
    odkud_pismeno = pamet_tahu[pamet_tahu_index][2];
    odkud_cislo = pamet_tahu[pamet_tahu_index][3];
    stav = ST_PRO_FIGURKU;
    pamet_tahu_index++;
    planovac_klid_tisk();
  }
  else{
    dlouhodoby_stav = DLST_NORMAL;
  }
}

void pridej_tah_do_prehravace(char odkud_pismeno, char odkud_cislo, char kam_pismeno, char kam_cislo, char jaka_figurka){
  if ((odkud_pismeno == kam_pismeno) && (odkud_cislo == kam_cislo)) {
    return;
  } else {
    pamet_tahu[pamet_tahu_pocet][0] = odkud_pismeno;
    pamet_tahu[pamet_tahu_pocet][1] = odkud_cislo;
    pamet_tahu[pamet_tahu_pocet][2] = kam_pismeno;
    pamet_tahu[pamet_tahu_pocet][3] = kam_cislo;
    pamet_tahu_pocet++;  
    Serial.print(jaka_figurka);
    Serial.print(" - ");  
    Serial.print(odkud_pismeno);
    Serial.print(odkud_cislo);
    Serial.print(" na ");
    Serial.print(kam_pismeno);
    Serial.println(kam_cislo);
    Serial.println("");
   }
}

void vyklizeni_sachovnice_2() {                  
  Serial.println("Probiha vyklizeni sachovnice!");
  char jaka_figurka = 'U';

  int pocitadlo_bilych_strelcu = 0;
  int pocitadlo_cernych_strelcu = 0;
  int pocitadlo_bilych_jezdcu = 0;
  int pocitadlo_cernych_jezdcu = 0;
  int pocitadlo_bilych_vezi = 0;
  int pocitadlo_cernych_vezi = 0;
  int pocitadlo_bilych_pesaku = 0;
  int pocitadlo_cernych_pesaku = 0;
  pamet_tahu_pocet = 0;
  pamet_tahu_index = 0;

  
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
    
      jaka_figurka = umisteni_figurek[j][i];
      if (jaka_figurka == '0'){
        
      }else if ((jaka_figurka == 'K') || (jaka_figurka == 'k')){
        odkud_pismeno = 'A' + i;
        odkud_cislo = '1' + j;
        kam_pismeno = 'E';
        if (jaka_figurka == 'k') {
          kam_cislo = '8';
        } else {
          kam_cislo = '1';   
        }
        pridej_tah_do_prehravace(odkud_pismeno, odkud_cislo, kam_pismeno, kam_cislo, jaka_figurka);
                 
      }else if ((jaka_figurka == 'D') || (jaka_figurka == 'd')){
        odkud_pismeno = 'A' + i;
        odkud_cislo = '1' + j;
        kam_pismeno = 'D';
        if (jaka_figurka == 'd') {
          kam_cislo = '8';
        } else {
          kam_cislo = '1';   
        }
        pridej_tah_do_prehravace(odkud_pismeno, odkud_cislo, kam_pismeno, kam_cislo, jaka_figurka);
        
      }else if ((jaka_figurka == 'S') || (jaka_figurka == 's')){
        odkud_pismeno = 'A' + i;
        odkud_cislo = '1' + j;
        if (jaka_figurka == 's') {
          kam_pismeno = 'C' + pocitadlo_cernych_strelcu;
          pocitadlo_cernych_strelcu = 3;
        } else {
          kam_pismeno = 'C' + pocitadlo_bilych_strelcu;
          pocitadlo_bilych_strelcu = 3;
        }
        if (jaka_figurka == 's') {
          kam_cislo = '8';
        } else {
          kam_cislo = '1';   
        }
        pridej_tah_do_prehravace(odkud_pismeno, odkud_cislo, kam_pismeno, kam_cislo, jaka_figurka);
        
      }else if ((jaka_figurka == 'J') || (jaka_figurka == 'j')){
        odkud_pismeno = 'A' + i;
        odkud_cislo = '1' + j;
        if (jaka_figurka == 'j') {
          kam_pismeno = 'B' + pocitadlo_cernych_jezdcu;
          pocitadlo_cernych_jezdcu = 5;
        } else {
          kam_pismeno = 'B' + pocitadlo_bilych_jezdcu;
          pocitadlo_bilych_jezdcu = 5;
        }
        if (jaka_figurka == 'j') {
          kam_cislo = '8';
        } else {
          kam_cislo = '1';   
        }
        pridej_tah_do_prehravace(odkud_pismeno, odkud_cislo, kam_pismeno, kam_cislo, jaka_figurka);
                
      }else if ((jaka_figurka == 'V') || (jaka_figurka == 'v')){
        odkud_pismeno = 'A' + i;
        odkud_cislo = '1' + j;
        if (jaka_figurka == 'v') {
          kam_pismeno = 'A' + pocitadlo_cernych_vezi;
          pocitadlo_cernych_vezi = 7;
        } else {
          kam_pismeno = 'A' + pocitadlo_bilych_vezi;
          pocitadlo_bilych_vezi = 7;
        }
        if (jaka_figurka == 'v') {
          kam_cislo = '8';
        } else {
          kam_cislo = '1';   
        }
        pridej_tah_do_prehravace(odkud_pismeno, odkud_cislo, kam_pismeno, kam_cislo, jaka_figurka);
                
      }else if ((jaka_figurka == 'P') || (jaka_figurka == 'p')){
        odkud_pismeno = 'A' + i;
        odkud_cislo = '1' + j;
        if (jaka_figurka == 'p') {
          kam_pismeno = 'A' + pocitadlo_cernych_pesaku;
          pocitadlo_cernych_pesaku++;
        } else {
          kam_pismeno = 'A' + pocitadlo_bilych_pesaku;
          pocitadlo_bilych_pesaku++;
        }
        if (jaka_figurka == 'p') {
          kam_cislo = '7'; 
        } else {
          kam_cislo = '2';
        }
        pridej_tah_do_prehravace(odkud_pismeno, odkud_cislo, kam_pismeno, kam_cislo, jaka_figurka);
     }
      
    }
  }
  dlouhodoby_stav = DLST_PREHRAVANI_TAHU;
  stav = ST_PRO_FIGURKU;
}

void planovac_klid_tisk(){
            if(tisk == 1){
             Serial.print("Pozadovany tah: ");
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'V'){
               Serial.print("Bila vez z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'v'){
               Serial.print("Cerna vez z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'J'){
               Serial.print("Bily jezdec z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'j'){
               Serial.print("Cerny jezdec z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'S'){
               Serial.print("Bily strelec z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 's'){
               Serial.print("Cerny strelec z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'D'){
               Serial.print("Bila kralovna z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'd'){
               Serial.print("Cerna kralovna z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'K'){
               Serial.print("Bily kral z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'k'){
               Serial.print("Cerny kral z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'P'){
               Serial.print("Bily pesec z ");
             }
             if(umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'p'){
               Serial.print("Cerny pesec z ");
             }
             Serial.print(odkud_pismeno);
             Serial.print(odkud_cislo);
             Serial.print(" na ");
             Serial.print(kam_pismeno);
             Serial.println(kam_cislo);
             tisk = 0;
          }

}

void planovac_klid(){
    if ((kam_cislo != '0') &&
        (kam_pismeno != '0') &&
        (odkud_cislo != '0') &&
        (odkud_pismeno != '0')) {
          planovac_klid_tisk();
          
      //nastavení druhu figurky(zda-li se bude pohybovat v citybloku = 0, nebo jednoduše = 1)
      if ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'V') ||
          (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'v') ||
          (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'K') ||
          (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'k') ||
          (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'P') ||
          (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'p')) {
        druh_figurky = 1;
      }
      else {
        druh_figurky = 0;
      }
      stav = ST_PRAVIDLA;
      tisk = 1;
      motorstop = 1;
    }
    else if ((kam_cislo == '0') && (kam_pismeno == 'A') &&
            (odkud_cislo == '0') && (odkud_pismeno == 'A')) {
                  digitalWrite(piezo, HIGH);
                  delay(900);
                  digitalWrite(piezo, LOW);
                  
                  vyklizeni_sachovnice_init();
       }
    else if ((kam_cislo == '0') && (kam_pismeno == 'B') &&
            (odkud_cislo == '0') && (odkud_pismeno == 'B')) {
                  digitalWrite(piezo, HIGH);
                  delay(900);
                  digitalWrite(piezo, LOW);
                  
                  prehravani_tahu_init();
       }
    else if ((kam_cislo == '0') && (kam_pismeno == 'C') &&
            (odkud_cislo == '0') && (odkud_pismeno == 'C')) {
                  digitalWrite(piezo, HIGH);
                  delay(900);
                  digitalWrite(piezo, LOW);
                  
                  vyklizeni_sachovnice_2();
       }    
  
}

void planovac_pravidla(){
        if (((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'J') && (((abs(kam_cislo - odkud_cislo) == 1) && (abs(kam_pismeno - odkud_pismeno) == 2))||((abs(kam_cislo - odkud_cislo) == 2) && (abs(kam_pismeno - odkud_pismeno) == 1))))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'j') && (((abs(kam_cislo - odkud_cislo) == 1) && (abs(kam_pismeno - odkud_pismeno) == 2))||((abs(kam_cislo - odkud_cislo) == 2) && (abs(kam_pismeno - odkud_pismeno) == 1))))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'V') && ((abs(kam_cislo - odkud_cislo) == 0)||(abs(kam_pismeno - odkud_pismeno) == 0)))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'v') && ((abs(kam_cislo - odkud_cislo) == 0)||(abs(kam_pismeno - odkud_pismeno) == 0)))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'S') && (abs(kam_cislo - odkud_cislo)) == (abs(kam_pismeno - odkud_pismeno)))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 's') && (abs(kam_cislo - odkud_cislo)) == (abs(kam_pismeno - odkud_pismeno)))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'K') && (((abs(kam_cislo - odkud_cislo) == 1) && (abs(kam_pismeno - odkud_pismeno) == 0))||((abs(kam_cislo - odkud_cislo) == 0) && (abs(kam_pismeno - odkud_pismeno) == 1))||((abs(kam_cislo - odkud_cislo) == 1) && (abs(kam_pismeno - odkud_pismeno) == 1))))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'k') && (((abs(kam_cislo - odkud_cislo) == 1) && (abs(kam_pismeno - odkud_pismeno) == 0))||((abs(kam_cislo - odkud_cislo) == 0) && (abs(kam_pismeno - odkud_pismeno) == 1))||((abs(kam_cislo - odkud_cislo) == 1) && (abs(kam_pismeno - odkud_pismeno) == 1))))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'D') && ((abs(kam_cislo - odkud_cislo) == 0)||(abs(kam_pismeno - odkud_pismeno) == 0)||((abs(kam_cislo - odkud_cislo)) == (abs(kam_pismeno - odkud_pismeno)))))|| 
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'd') && ((abs(kam_cislo - odkud_cislo) == 0)||(abs(kam_pismeno - odkud_pismeno) == 0)||((abs(kam_cislo - odkud_cislo)) == (abs(kam_pismeno - odkud_pismeno)))))|| 
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'P') && ((odkud_cislo - '1') == (1)) && (abs(kam_cislo - odkud_cislo) <= 2) && (abs(kam_pismeno - odkud_pismeno) == 0))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'P') && ((odkud_cislo - '1') != (1)) && (abs(kam_cislo - odkud_cislo) == 1) && ((kam_pismeno - odkud_pismeno) == 0))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'p') && ((odkud_cislo - '1') == (6)) && (abs(kam_cislo - odkud_cislo) <= 2) && (abs(kam_pismeno - odkud_pismeno) == 0))||
          ((umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == 'p') && ((odkud_cislo - '1') != (6)) && (abs(kam_cislo - odkud_cislo) == 1) && ((kam_pismeno - odkud_pismeno) == 0))){
        tisk = 1;
        stav = ST_VYHAZOVANI;
           }
      else {
            digitalWrite(piezo, HIGH);
            delay(50);
            digitalWrite(piezo, LOW);
            delay(50);
            digitalWrite(piezo, HIGH);
            delay(50);
            digitalWrite(piezo, LOW);
            stav = ST_CHYBNY_TAH; 
            tisk = 1;        
            //Serial.println (odkud_cislo);
            //Serial.println (kam_cislo-odkud_cislo);
            //Serial.println (kam_pismeno-odkud_pismeno);
            }
}

void planovac_chybny_tah(){
      if (tisk == 1){
      Serial.println("Chybny tah!");
            Serial.println("**********");
            pravidla();
            Serial.println("**********");
            Serial.println(" ");
            Serial.println("Zadejte souradnice znovu:");
            tisk = 0;
            kam_cislo = '0';
            kam_pismeno = '0';
            klavesnice();
            stav = ST_KLID;
            tisk = 1;
    } 
}

void planovac_vyhazovani() {
  if (tisk == 1){

      if (umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] == '0'){
        stav = ST_PRO_FIGURKU;
        tisk = 1;
      }
      else {
        if (tisk == 1) {

          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'V'){
            Serial.println("Vyhozena figurka: Bila vez");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'v'){
            Serial.println("Vyhozena figurka: Cerna vez");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'J'){
            Serial.println("Vyhozena figurka: Bily jezdec");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'j'){
            Serial.println("Vyhozena figurka: Cerny jezdec");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'S'){
            Serial.println("Vyhozena figurka: Bily strelec");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 's'){
            Serial.println("Vyhozena figurka: Cerny strelec");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'D'){
            Serial.println("Vyhozena figurka: Bila dama");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'd'){
            Serial.println("Vyhozena figurka: Cerna dama");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'K'){
            Serial.println("Vyhozena figurka: Bily kral");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'k'){
            Serial.println("Vyhozena figurka: Cerny kral");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'P'){
            Serial.println("Vyhozena figurka: Bily pesec");
          }
          if(umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] == 'p'){
            Serial.println("Vyhozena figurka: Cerny pesec");
          }
          tisk = 0;
        }
        stav = ST_PRO_FIGURKU;
        tisk = 1;        
      }
    }
}

void planovac_pro_figurku() {
  if(tisk == 1){
       Serial.println("Jedu pro figurku ... ");
       tisk = 0;
    }
    motorstop = 0;
    digitalWrite(REL8MAGNET, HIGH);
    pozadovana_X = souradniceX[odkud_cislo - '1'];
    pozadovana_Y = souradniceY[odkud_pismeno - 'A'];
    if ((stav_motor_horni == 1) && (stav_motor_spodni == 1)) {
      stav = ST_UCHOP;
      tisk = 1;
      motorstop = 0;
    }
}

void planovac_uchop() {
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

void planovac_na_caru() {
  motorstop = 0;
    pozadovana_X = souradniceX[odkud_cislo - '1'] + 4.5;
    pozadovana_Y = souradniceY[odkud_pismeno - 'A'] + 4.5;
    if ((stav_motor_horni == 1) && (stav_motor_spodni == 1)) {
      stav_motor_horni = 0;
      stav_motor_spodni = 0;
      stav = ST_POSUV_X;

    }
}

void planovac_posuv_x() {
  motorstop = 0;
    pozadovana_X = souradniceX[kam_cislo - '1'] + 4.5;
    if (stav_motor_horni == 1) {
      stav_motor_horni = 0;
      stav_motor_spodni = 0;
      stav = ST_POSUV_Y;
    }
}

void planovac_posuv_y() {
    motorstop = 0;
    pozadovana_Y = souradniceY[kam_pismeno - 'A'] + 4.5;
    if (stav_motor_spodni == 1) {
      stav_motor_spodni = 0;
      stav = ST_NA_POLE;
    }
}

void planovac_na_pole() {
   motorstop = 0;
    pozadovana_X = souradniceX[kam_cislo - '1'];
    pozadovana_Y = souradniceY[kam_pismeno - 'A'];
    if ((stav_motor_horni == 1) && (stav_motor_spodni == 1)) {
      stav_motor_horni = 0;
      stav_motor_spodni = 0;
      //přepisování nové polohy figurky do matice a nulování výchozí polohy
      umisteni_figurek[kam_cislo - '1'][kam_pismeno - 'A'] = umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'];
      umisteni_figurek[odkud_cislo - '1'][odkud_pismeno - 'A'] = '0';

      ulozeni_tahu_do_pameti(); //ulozeni tahu do pameti
      //Serial.println("ulozeno");
      
      //nulování zadaných souřadnic
      kam_cislo = '0';
      kam_pismeno = '0';
      odkud_cislo = '0';
      odkud_pismeno = '0';
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
      
      if (dlouhodoby_stav == DLST_NORMAL) {
        p++; //
        //Serial.println(p);
      }  
      
      if (dlouhodoby_stav == DLST_PREHRAVANI_TAHU){
        prehravani_tahu_iterace();
      }

      if (dlouhodoby_stav == DLST_VYKLIZENI_SACHOVNICE) {
        vyklizeni_sachovnice_iterace();
      }

    }
}


void planovac() {
  //stav ve kterém se čeká na zadání souřadnic ODKUD-KAM
  if (stav == ST_KLID) {
    planovac_klid();
    } // pridana zavorka
    
  //kontrola tahu_pravidla
  if (stav == ST_PRAVIDLA) {
      planovac_pravidla();
    }
  //vypsani chybneho tahu, vynulovani souradnic, jejich zadani znovu
  if (stav == ST_CHYBNY_TAH) {
     planovac_chybny_tah();   
    }
  //vyhazovani figurek
  if (stav == ST_VYHAZOVANI) {
    planovac_vyhazovani();
  }  
  //motory jedou pro figurku (oba současně)
  if (stav == ST_PRO_FIGURKU) {
    planovac_pro_figurku();
  }  
  //Zde spíná magnet. Je zde chvilka hystereze pro bezpečné spojení magnetů.
  if (stav == ST_UCHOP) {
    planovac_uchop();
  }
  //fugurka popojede na kříž dolu doprava
  if (stav == ST_NA_CARU) {
    planovac_na_caru();
  }
  //figurka jede po ose X(1-8) a zastaví se na kříži pod požadovaným políčkem
  if (stav == ST_POSUV_X) {
    planovac_posuv_x();
  }
  //figurka jede po ose Y(A-H) a zastaví se na kříži vpravo od požadovaného políčka
  if (stav == ST_POSUV_Y) {
    planovac_posuv_y(); 
  }
  
  // v případě citybloku figurka popojede uhlopříčně na střed opožadovaného políčka
  //v případě přímého tahu figurka jede z výchozí hodnoty přímo na střed políčka bez zbytečného zdržování
  if (stav == ST_NA_POLE) {
   planovac_na_pole();
  }
  

}

void vypsani_stavu_hry() {
  char jaka_figurka = 'U';
  for (char i=0; i<8; i++) {
    for (char j=0; j<8; j++) {
    
      jaka_figurka = umisteni_figurek[j][i];
      if (jaka_figurka == '0'){
        
      }else{ 
        if (jaka_figurka == 'K'){
          Serial.print("Bily kral ");
          
        }else if (jaka_figurka == 'k'){
          Serial.print("Cerny kral ");
                 
        }else if (jaka_figurka == 'D'){
          Serial.print("Bila dama ");
          
        }else if (jaka_figurka == 'd'){
          Serial.print("Cerna dama ");
          
        }else if (jaka_figurka == 'S'){
          Serial.print("Bily strelec ");
          
        }else if (jaka_figurka == 's'){
          Serial.print("Cerny strelec ");
  
        }else if (jaka_figurka == 'J'){
          Serial.print("Bily jezdec ");
        
        }else if (jaka_figurka == 'j'){
          Serial.print("Cerny jezdec ");
                  
        }else if (jaka_figurka == 'V'){
          Serial.print("Bila vez ");
          
        }else if (jaka_figurka == 'v'){
          Serial.print("Cerna vez ");
                  
        }else if (jaka_figurka == 'P'){
          Serial.print("Bily pesec ");
          
        }else if (jaka_figurka == 'p'){
          Serial.print("Cerny pesec ");
        }
        Serial.print(char('A' + i));
        Serial.println(char('1' + j));
        
      }
    }
  }     
}

void serial_read(){
  String a;
  a = Serial.readString();// read the incoming data as string
  //Serial.println(a);
  //Serial.println(a.length());
  if (a.length() == 0) {
    return;
  }else {
    Serial.println(a);
    if (a.equals("Stav?")) {
      vypsani_stavu_hry();
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
  serial_read();
  
}






