#include"pitches.h"

//Notes input
const int noteCH = 9;
const int noteB = 8;
const int noteA = 7;
const int noteG = 6;
const int noteF = 5;
const int noteE = 4; 
const int noteD = 13;
const int noteCL = 12;

//Metronome interrupt
const int pinToInterrupt = 2;
volatile int mspB = 600; //100 bpm in microsecond per beat
const int metroIncrement= 50;
const int minMetronome = 0;


unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

//ON or OFF 
volatile int powerState = HIGH;

//Overflow interrupt
volatile float counter_time= 1.024; //(64*256*1000)/16e6 to get 1.024ms every 256 clock cycles
volatile int overflowCount =0;
volatile int prevCount=0;
volatile bool playTone = false;

int startTime=0;
int currentTime=0;
int startCount=0;

/*---- Custom Millis() ----*/
long frequencyCPU = 16000000L; //16MHz
long clockCyclesPerMicrosecond = frequencyCPU / 10000000L; //16

int prescaler = 64; 
int rollover = 256;

long MS_PER_TIMER2_OVERFLOW = ((prescaler * rollover) * 1000)/(frequencyCPU/1000); //1024 microseconds
long MILLIS_INC = 1.024; //MS_PER_TIMER2_OVERFLOW / 1000; // milliseconds

int FRACT_INC = 3; //(MS_PER_TIMER2_OVERFLOW % 1000) >> 3; // 
int FRACT_MAX = 125; //1000 >> 3; //125
volatile unsigned long custom_timer2_millis = 0;
volatile unsigned char custom_timer2_fract = 0;
volatile unsigned int custom_timer2_overflow_count = 0;

unsigned long custom_millis() //Custom millis function
{
    unsigned long m;
    uint8_t oldSREG = SREG;
    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer2_millis)
    cli();
    m = custom_timer2_millis;
    SREG = oldSREG;
 
    return m;
}

ISR(TIMER2_OVF_vect){
    unsigned long m = custom_timer2_millis;
    unsigned char f = custom_timer2_fract;
 
    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX){
        f -= FRACT_MAX;
        m += 1;
    }
 
    custom_timer2_fract = f;
    custom_timer2_millis = m;
    custom_timer2_overflow_count++;

//    overflowCount++;
//    if(((overflowCount * counter_time) - prevCount) >= mspB){
//    prevCount= overflowCount * counter_time;
//    playTone= true;
//  }
}
/*---- end Custom Millis ()----*/

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(noteCH, INPUT);
  pinMode(noteB, INPUT);
  pinMode(noteA, INPUT);
  pinMode(noteG, INPUT);
  pinMode(noteF, INPUT);
  pinMode(noteE, INPUT);
  pinMode(noteD, INPUT);
  pinMode(noteCL, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinToInterrupt), adjustTempoInterrupt, RISING);

  TIMSK2 |= (1<< TOIE2); //enables Timer2
  sei();                 //set enable interrupt
}

void loop() {
    
// put your main code here, to run repeatedly:

unsigned long currentMillis = custom_millis(); //updating current time

//Logic for ON or OFF
if ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH))
    {
      if (startCount == 0){
          startTime = custom_millis();
          startCount = 1;
        }
        currentTime = custom_millis();
      if (currentTime - startTime >= 30000){
         powerState = LOW;
      }
    }

  //Piano On
  if (powerState == HIGH){
    digitalWrite(A0,HIGH);  //LED on, indicates power on

//    //Metronome On
//    if(playTone ==true){
//      tone(10,NOTE_C4,20);
//      playTone=false;
//    }

    //Handle single note00
    //Note C High only
    while ((digitalRead(noteCH) == LOW) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      tone(11, NOTE_C5);
      startCount=0;
    }

    //Note B only
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == LOW) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      tone(11, NOTE_B4);
      startCount=0;
    }

    //Note A only
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == LOW) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      tone(11, NOTE_A4);
      startCount=0;      
    }

    //Note G only
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == LOW) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      tone(11, NOTE_G4);
      startCount=0;
    }

    //Note F only
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == LOW) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      tone(11, NOTE_F4);
      startCount=0;
    }

    //Note E only
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == LOW) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      tone(11, NOTE_E4);
      startCount=0;
    }

    //Note D only
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == LOW) && (digitalRead(noteCL) == HIGH)){
      tone(11, NOTE_D4);
      startCount=0;
    }

    //Note C Low only
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == LOW)){
      tone(11, NOTE_C4);
      startCount=0;
    }
  
  //Handling chords
  //C major (C Low, E, G)
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == LOW) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == LOW) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == LOW)){
      digitalWrite(A1, HIGH); //Enables chords
      digitalWrite(A3, LOW);
      digitalWrite(A2, LOW);
      tone(11, NOTE_C4);
      startCount=0;
      //Arduino 2 plays NOTE_E4
      //Arduino 3 plays NOTE_G4
    }
  
  //D minor (D, F, A)
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == LOW) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == LOW) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == LOW) && (digitalRead(noteCL) == HIGH)){
      digitalWrite(A1, HIGH); //Enables chords
      digitalWrite(A3, LOW);
      digitalWrite(A2, HIGH);
      tone(11, NOTE_D4);
      startCount=0;
      //Arduino 2 plays NOTE_F4
      //Arduino 3 plays NOTE_A4
    }
  
  //E minor (E, G, B)
    while ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == LOW) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == LOW) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == LOW) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      digitalWrite(A1, HIGH); //Enables chords
      digitalWrite(A3, HIGH);
      digitalWrite(A2, LOW);
      tone(11, NOTE_E4);
      startCount=0;
      //Arduino 2 plays NOTE_G4
      //Arduino 3 plays NOTE_B4
    }
  
  //F major (F, A, C)
    while ((digitalRead(noteCH) == LOW) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == LOW) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == LOW) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      digitalWrite(A1, HIGH); //Enables chords
      digitalWrite(A3, HIGH);
      digitalWrite(A2, HIGH);
      tone(11, NOTE_F4);
      Serial.println("f");
      //Arduino 2 plays NOTE_A4
      //Arduino 3 plays NOTE_C5
    }
  
   //nothing
    if ((digitalRead(noteCH) == HIGH) && (digitalRead(noteB) == HIGH) && (digitalRead(noteA) == HIGH) && (digitalRead(noteG) == HIGH) && (digitalRead(noteF) == HIGH) && (digitalRead(noteE) == HIGH) && (digitalRead(noteD) == HIGH) && (digitalRead(noteCL) == HIGH)){
      digitalWrite(A1, LOW); //Disables chords
  //    digitalWrite(A3, LOW); DON'T CARE
  //    digitalWrite(A2, LOW); DON'T CARE
      //Arduino 2 plays nothing
      //Arduino 3 plays nothing
    }  
    
    delay(100);
    noTone(11);
    
  //  digitalWrite(A1, LOW); //Mutes arduino 2 and 3
    
    if (currentMillis - previousMillis >= mspB ){
      tone(10,NOTE_C4,20);
      previousMillis = currentMillis;
    }
  
  }
  else{
    digitalWrite(A0,LOW);     //LED off, indicates power off
  }
}

void adjustTempoInterrupt(){
// if button 1 clicked, increase tempo
// if button 2 clicked, decrease tempo
// Button system use pulldown

if(digitalRead(A5) == HIGH && digitalRead(A4) == LOW)
{
  mspB += metroIncrement; //Slowing the metronome down
   Serial.println(mspB);
}
else if (digitalRead(A5) == LOW && digitalRead(A4) == HIGH)
{
  mspB -= metroIncrement; //Speeding the metronome up
  Serial.println(mspB);
}

}


//Serial.println(mspB);

//if(mspB < minMetronome)
//{
//  mspB == minMetronome;
//}
//
//}

//void stateChange() {
//  powerState== !powerState;
//  Serial.println("Jeremy hates pokemon");
//}


