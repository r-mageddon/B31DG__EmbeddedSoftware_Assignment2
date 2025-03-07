/*
 * Name: Rowan Maxwell
 * H No.: H00346635
 * Class: B31DG
 * Title: Assignment 2 - Cyclic & RTOS Monitoring
 * Date: 06/03/2025
 * Description: This is the cyclic program for assignment 2 of the B31DG
 *              embedded software class at Heriot Watt University
*/

/* Pin Definitions */

#define GREENLED 19
#define REDLED 21
#define PB1 15
#define PB2 23
#define F1 5 // Frequency signal 1 input pin
#define F2 17 // Frequency signal 2 input pin

const uint delay50 = 50;   // 50 microsecond delay
const uint delay100 = 100; // 100 microsecond delay
const uint delay200 = 200; // 200 microsecond delay
const uint delay250 = 250; // 250 microsecond delay
const uint delay300 = 300; // 300 microsecond delay

int timer;
int TimeTaken;
bool bruh = true;



void setup() 
{
  // Outputs
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);

  // Inputs
  pinMode(PB1, INPUT);
  pinMode(PB2, INPUT);
  pinMode(F1, INPUT);
  pinMode(F2, INPUT);

  Serial.begin(9600);
}

void loop() 
{
  unsigned long frameTime = 0;
  unsigned long frameCounter = 0;
  if (bruh)
  {
    timer = micros();
    ReadSignal_2();
    TimeTaken = micros() - timer;
    Serial.print("time taken = "); Serial.println(TimeTaken);
    Serial.print("F2 = "); Serial.println(digitalRead(F2));
    bruh = false;
  }
}


void DigitalSignal_1()
{
  // runs for 614us
  digitalWrite(GREENLED, HIGH);
  delayMicroseconds(delay250);
  digitalWrite(GREENLED, LOW);
  delayMicroseconds(delay50);
  digitalWrite(GREENLED, HIGH);
  delayMicroseconds(delay300);
}

void DigitalSignal_2()
{
  // runs for 362us
  digitalWrite(GREENLED, HIGH); // Turn on green LED
  delayMicroseconds(delay100); // Delay for 100 microseconds
  digitalWrite(GREENLED, LOW); // Turn off green LED
  delayMicroseconds(delay50); // Delay 50 microseconds
  digitalWrite(GREENLED, HIGH); // Turn on green LED
  delayMicroseconds(delay200); // delay 200 microseconds
}

void ReadSignal_1()
{
  // runs for 6us
  digitalRead(F1); // Read F1
}

void ReadSignal_2()
{
  // runs for 6us
  digitalRead(F2); // Read F2
}
