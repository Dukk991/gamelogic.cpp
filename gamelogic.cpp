#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#include <arduino.h>

volatile int OCR1A_settings = 15624;                       //(15624 x 1024) / 16MHz = 0,999936s
volatile int randomNumbers[100];
volatile int userNumbers[100];
volatile int cycleCounter;
volatile int arrayCounter = 0;
volatile int numbersIndex = 0;

volatile unsigned long button_0DebounceTime;
volatile unsigned long button_1DebounceTime;
volatile unsigned long button_2DebounceTime;
volatile unsigned long button_3DebounceTime;
volatile unsigned long startButtonDebounceTime;
const unsigned long buttonDebounceDelay = 300;
//-------------------------------------------------------------------------------------

void setup() {    //Alustaa ledit, napit ja näyttö.
  
  Serial.begin(9600);  
  initializeLeds();
  initButtonsAndButtonInterrupts();
  initializeDisplay();
}

//-------------------------------------------------------------------------------------

void loop() {   

}  

//-------------------------------------------------------------------------------------

ISR(PCINT2_vect) { 

  if(digitalRead(button_0) == LOW && (millis() - button_0DebounceTime >= buttonDebounceDelay)){                       //Nappien kytkinvärähtelyn poisto käyttäen millis()-funktiota, joka palauttaa ohjelman käynnistyksestä kuluneen ajan millisekunteina.
      button_0DebounceTime = millis();                                                                                //Kun nappia painetaan, ei samasta napista oteta huomioon inputteja seuraavan 300 millisekunnin ajan. Tämä ei estä käyttäjää painamasta muita nappeja.
      userNumbers[numbersIndex] = 0;                                                                                  //Nappien 0-3 painaminen asettaa userNumbers-taulukkoon arvon 0-3 riippuen mitä nappia on painettu.
      Serial.println("Button 0 pressed.");                                                                            //Kutsuu checkGame-funktiota, joka tarkistaa vastaako painettu nappi randomNumbers-taulukon arvoa samalla indeksillä.
      checkGame();                                                                                                    //Kasvattaa userNumbers-taulukon indeksiä yhdellä.
      numbersIndex++; 
                                                                                                                    
  
  if(digitalRead(button_1) == LOW && (millis() - button_1DebounceTime >= buttonDebounceDelay)){
      button_1DebounceTime = millis();
      userNumbers[numbersIndex] = 1;
      Serial.println("Button 1 pressed.");
      checkGame();
      numbersIndex++;  
  }
  
  if(digitalRead(button_2) == LOW && (millis() - button_2DebounceTime >= buttonDebounceDelay)){
      button_2DebounceTime = millis();
      userNumbers[numbersIndex] = 2;
      Serial.println("Button 2 pressed.");
      checkGame();
      numbersIndex++;
  }

  if(digitalRead(button_3) == LOW && (millis() - button_3DebounceTime >= buttonDebounceDelay)){
      button_3DebounceTime = millis();
      userNumbers[numbersIndex] = 3;
      Serial.println("Button 3 pressed.");
      checkGame();
      numbersIndex++;
  }
  
  if(digitalRead(startButton) == LOW && (millis() - startButtonDebounceTime >= buttonDebounceDelay)){    //startButtonin painaminen kutsuu startTheGame-funktiota, joka aloittaa pelin. userNumbers-taulukon indeksi asetetaan 0:aan, jotta taulukkoa päästään täyttämään alusta alkaen.
      startButtonDebounceTime = millis();
      Serial.println("Game started.");
      startTheGame();
      numbersIndex = 0;
  }
}

//-------------------------------------------------------------------------------------

ISR(TIMER1_COMPA_vect) {
  
  int previousNumber;
  
  if (arrayCounter < 100) {                              //95 - 104. Arpoo sata numeroa 0 ja 3 välillä. Ei arvo peräkkäisiä numeroita. Arpomis tahti riippuu timeristä

    for (int i = 0; i < 1; i++) {
    
    randomSeed(analogRead(0));
    previousNumber = randomNumbers[arrayCounter - 1];

    do {
      randomNumbers[arrayCounter] = random(0, 4);
    } while (previousNumber == randomNumbers[arrayCounter]);

    switch (randomNumbers[arrayCounter]) {                //106 - 125. Sytyttää arvottua numeroa vastaavan ledin kutsumalla setLed funktiota, joka on määritelty leds.cpp tiedostossa
      
      case 0: {
        setLed(0);
        break;
      }

      case 1: {
        setLed(1);
        break;
      }

      case 2: {
        setLed(2);
        break;
      }

      case 3: {
        setLed(3);
        break;
      }
    }
    
    cycleCounter++;
  }
  
  if (cycleCounter == 10) {                     //10 arvotun numeron jälkeen, nopeuttaa arpomis tahtia kertomalla OCR1A rekisteriin laitetun OCR1A_settings muuttujna 0.9:llä. Nollaa cycleCounterin
      OCR1A_settings = OCR1A_settings * 0.9;
      OCR1A = OCR1A_settings;
      cycleCounter = 0;
    }
  }

  if (arrayCounter == 99) {
    endGame();
  }

  arrayCounter++;
}

//-------------------------------------------------------------------------------------
  
  void initializeTimer(void) {                                //alustaa timerin

  noInterrupts();                                             //keskeyttää keskeytykset
  
  TCCR1A = 0;                                                 //nollaa TCCR1 A ja B rekisterit
  TCCR1B = 0;                                                 //
  TCNT1 = 0;                                                  //nollaa TCNT1 rekisterin, joka pitää kirjaa laskurista
  OCR1A = OCR1A_settings;                                     //asettaa OCR1A_settings muuttujan OCR1A rekisteriin, joka määrittää milloin keskeytykset laukeavat
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);         //asettaa timerin CTC-tilaan (WGM12). Asettaa prescaleriksi 1024 (CS12 ja CS10).
  TIMSK1 |= (1 << OCIE1A);                                    //mahdollistaa keskeytykset kun OCR1A rekisteriin asetettu arvo saavutetaan

  interrupts();                                               //käynnistää keskeytykset
}

//-------------------------------------------------------------------------------------

void checkGame() {

  if (randomNumbers[numbersIndex] == userNumbers[numbersIndex]) {    //vertaa random ja user taulukoita numbersIndex:llä. Jos taulukot ovat samanarvoiset: nostaa näyttöjen arvoa yhdellä
    showResult(numbersIndex);
    Serial.print("DISPLAY ")
    Serial.println(numbersIndex - 1);
  }

  else {                                                             //jos taulukot eivät ole samanarvoiset: kutsuu endGame funktiota, joka lopettaa pelin
    endGame();
    Serial.println("GAME OVER");
  }
}

//-------------------------------------------------------------------------------------

void initializeGame() {          //alustaa/nollaa muuttujat jotka muuttuvat ohjelman pyöriessä
  
  buttonCounter = 0;
  cycleCounter = 0;
  OCR1A_settings = 15624;
  numbersIndex = 0;
  kaakkeli = 1;
  arrayCounter = 0;
}

//-------------------------------------------------------------------------------------

void startTheGame() {                   //startTheGame funktio aloittaa pelin

  showResult(0);                        //nollaa näytöt

  for (int i = 0; i < 100; i++) {       //nollaa user ja random taulukot
    userNumbers[i] = 0;
    randomNumbers[i] = 0;
  }

  for (int i = 0; i < 3; i++) {         //vilkuttaa ledejä kolme kertaa
    setAllLeds();
    delay(150000);
    clearAllLeds();
    delay(150000);
  }
  
  initializeGame();                     //kutsuu initializeGame ja initializeTimer funktioita: käynnistää pelin
  initializeTimer();

}

//-------------------------------------------------------------------------------------

void endGame() {                       //endGame funktio lopettaa pelin

  TIMSK1 = 0;                          //lopettaa keskeytykset

  for (int i = 0; i < 3; i++) {        //vilkuttaa ledejä kolme kertaa
    setAllLeds();
    delay(150000);
    clearAllLeds();
    delay(150000);
  }
}
