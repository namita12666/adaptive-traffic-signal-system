#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// -------- SENSOR PINS --------
const int S1A=2, S2A=3;
const int S1B=4, S2B=5;
const int S1C=6, S2C=7;

// -------- LED PINS --------
const int RA=8, YA=9, GA=10;
const int RB=11, YB=12, GB=13;
const int RC=A0, YC=A1, GC=A2;


// -------- SYSTEM VARIABLES --------
int qA=0,qB=0,qC=0;

int countA=0,countB=0,countC=0;
float arrA=0,arrB=0,arrC=0;

int waitA=0,waitB=0,waitC=0;

bool prevS2A=HIGH;
bool prevS2B=HIGH;
bool prevS2C=HIGH;

unsigned long lastArrivalCalc=0;
unsigned long lastSecond=0;


// -------- SIGNAL STATE MACHINE --------
enum State {GREEN,YELLOW,ALLRED};
State signalState = GREEN;

int activeLane = 1;
int nextLane = 1;

int greenTime=10;
int timeLeft=10;


// -------- RESET LIGHTS --------
void resetLights()
{
 digitalWrite(RA,LOW); digitalWrite(YA,LOW); digitalWrite(GA,LOW);
 digitalWrite(RB,LOW); digitalWrite(YB,LOW); digitalWrite(GB,LOW);
 digitalWrite(RC,LOW); digitalWrite(YC,LOW); digitalWrite(GC,LOW);
}


// -------- QUEUE DETECTION --------
void updateQueues()
{
 int s1a=digitalRead(S1A);
 int s2a=digitalRead(S2A);

 int s1b=digitalRead(S1B);
 int s2b=digitalRead(S2B);

 int s1c=digitalRead(S1C);
 int s2c=digitalRead(S2C);


 if(s1a==HIGH && s2a==HIGH) qA=0;
 else if(s1a==LOW && s2a==HIGH) qA=1;
 else if(s1a==LOW && s2a==LOW) qA=2;

 if(s1b==HIGH && s2b==HIGH) qB=0;
 else if(s1b==LOW && s2b==HIGH) qB=1;
 else if(s1b==LOW && s2b==LOW) qB=2;

 if(s1c==HIGH && s2c==HIGH) qC=0;
 else if(s1c==LOW && s2c==HIGH) qC=1;
 else if(s1c==LOW && s2c==LOW) qC=2;
}


// -------- ARRIVAL COUNT --------
void updateArrivalCounters()
{
 int s2a=digitalRead(S2A);
 int s2b=digitalRead(S2B);
 int s2c=digitalRead(S2C);

 if(prevS2A==HIGH && s2a==LOW) countA++;
 if(prevS2B==HIGH && s2b==LOW) countB++;
 if(prevS2C==HIGH && s2c==LOW) countC++;

 prevS2A=s2a;
 prevS2B=s2b;
 prevS2C=s2c;
}


// -------- ARRIVAL RATE --------
void calculateArrivalRate()
{
 if(millis()-lastArrivalCalc >= 10000)
 {
  arrA = countA/10.0;
  arrB = countB/10.0;
  arrC = countC/10.0;

  countA=0;
  countB=0;
  countC=0;

  lastArrivalCalc = millis();
 }
}


// -------- PRIORITY SCORES --------
float scoreA(){ return qA*5 + arrA*2 + waitA; }
float scoreB(){ return qB*5 + arrB*2 + waitB; }
float scoreC(){ return qC*5 + arrC*2 + waitC; }


// -------- LANE SELECTION --------
void chooseLane()
{
 float pA=scoreA();
 float pB=scoreB();
 float pC=scoreC();

 if(qA==0 && arrA==0) pA=-1;
 if(qB==0 && arrB==0) pB=-1;
 if(qC==0 && arrC==0) pC=-1;

 activeLane=1;

 if(pB>pA && pB>pC) activeLane=2;
 if(pC>pA && pC>pB) activeLane=3;

 int q = (activeLane==1?qA:activeLane==2?qB:qC);

 greenTime = 10 + q*5;
 timeLeft = greenTime;
}


// -------- LED CONTROL --------
void setGreen(int lane)
{
 resetLights();

 if(lane==1){ digitalWrite(GA,HIGH); digitalWrite(RB,HIGH); digitalWrite(RC,HIGH);}
 if(lane==2){ digitalWrite(GB,HIGH); digitalWrite(RA,HIGH); digitalWrite(RC,HIGH);}
 if(lane==3){ digitalWrite(GC,HIGH); digitalWrite(RA,HIGH); digitalWrite(RB,HIGH);}
}

void setYellow(int lane)
{
 resetLights();

 if(lane==1){ digitalWrite(YA,HIGH); digitalWrite(RB,HIGH); digitalWrite(RC,HIGH);}
 if(lane==2){ digitalWrite(YB,HIGH); digitalWrite(RA,HIGH); digitalWrite(RC,HIGH);}
 if(lane==3){ digitalWrite(YC,HIGH); digitalWrite(RA,HIGH); digitalWrite(RB,HIGH);}
}

void setAllRed()
{
 resetLights();
 digitalWrite(RA,HIGH);
 digitalWrite(RB,HIGH);
 digitalWrite(RC,HIGH);
}


// -------- LCD DISPLAY --------
void updateLCD()
{
 lcd.clear();

 lcd.setCursor(0,0);
 lcd.print("LN:");
 lcd.print((activeLane==1?'A':activeLane==2?'B':'C'));
 lcd.print(" T:");
 lcd.print(timeLeft);

 lcd.setCursor(0,1);
 lcd.print("Q ");
 lcd.print(qA);
 lcd.print(" ");
 lcd.print(qB);
 lcd.print(" ");
 lcd.print(qC);
}


// -------- SETUP --------
void setup()
{
 pinMode(S1A,INPUT_PULLUP);
 pinMode(S2A,INPUT_PULLUP);
 pinMode(S1B,INPUT_PULLUP);
 pinMode(S2B,INPUT_PULLUP);
 pinMode(S1C,INPUT_PULLUP);
 pinMode(S2C,INPUT_PULLUP);

 pinMode(RA,OUTPUT); pinMode(YA,OUTPUT); pinMode(GA,OUTPUT);
 pinMode(RB,OUTPUT); pinMode(YB,OUTPUT); pinMode(GB,OUTPUT);
 pinMode(RC,OUTPUT); pinMode(YC,OUTPUT); pinMode(GC,OUTPUT);

 lcd.init();
 lcd.backlight();

 lcd.print("SMART TRAFFIC");
 delay(2000);

 chooseLane();
 setGreen(activeLane);
}


// -------- MAIN LOOP --------
void loop()
{

 updateQueues();
 updateArrivalCounters();
 calculateArrivalRate();


 if(millis() - lastSecond >= 1000)
 {
  lastSecond = millis();
  timeLeft--;

  updateLCD();


  if(timeLeft <= 0)
  {

   if(signalState == GREEN)
   {
    signalState = YELLOW;
    timeLeft = 3;
    setYellow(activeLane);
   }

   else if(signalState == YELLOW)
   {
    signalState = ALLRED;
    timeLeft = 2;
    setAllRed();
   }

   else if(signalState == ALLRED)
   {
    signalState = GREEN;

    waitA += greenTime;
    waitB += greenTime;
    waitC += greenTime;

    if(activeLane==1) waitA=0;
    if(activeLane==2) waitB=0;
    if(activeLane==3) waitC=0;

    chooseLane();
    setGreen(activeLane);
   }

  }

 }

}
