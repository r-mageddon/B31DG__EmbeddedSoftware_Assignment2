/*
 * Name: Rowan Maxwell
 * H No.: H00346635
 * Class: B31DG
 * Title: Assignment 2 - Cyclic Monitoring
 * Date Created: 06/03/2025
 * Description: This is the cyclic program for assignment 2 of the B31DG
 *              embedded software class at Heriot Watt University
*/

#include <B31DGMonitor.h>
#include <Ticker.h>

/////////////////////////////////////
/// Variable and Pin Declarations ///
/////////////////////////////////////

/* Monitor Library Declaration */
B31DGCyclicExecutiveMonitor monitor;

/* Function call */
void DigitalSignal_1();
void DigitalSignal_2();
void ReadSignal_1();
void ReadSignal_2();
void CallDoWork();

/* Pin Definitions */
#define GREENLED 19 // Green LED PIN
#define REDLED 21 // Red LED PIN
#define YELLOWLED 33 // Yellow LED Pin
#define DoWorkReadButton 15 // Pushbutton 1 PIN
#define PB2 23 // Pushbutton 2 PIN
#define F1 5 // Frequency signal 1 input pin
#define F2 17 // Frequency signal 2 input pin

/* Delay Values */
const uint delay50 = 50;   // 50 microsecond delay
const uint delay100 = 100; // 100 microsecond delay
const uint delay200 = 200; // 200 microsecond delay
const uint delay250 = 250; // 250 microsecond delay
const uint delay300 = 300; // 300 microsecond delay
const uint timer1Delay = 0.004; // 4 millisecond delay
const uint timer2Delay = 0.003; // 3 millisecond delay
const uint timer3Delay = 0.01; // 10 milliseconds delay
const uint timer4Delay = 0.01; // 10 millisecond delay
const uint timer5Delay = 0.005; // 5 millisecond delay

/* Frequency Read Variables */
int F1PulseHigh = 0; // Value for high F1 square wave pulse
int F1PulseLow = 0; // Value for LOW F1 square wave pulse
int F1Total = 0; // Value for total period of F1 square wave
int F1Freq = 0; // Frequency of F1 square wave
int F2PulseHigh = 0; // Value for HIGH F2 square wave
int F2PulseLow = 0; // Value for LOW F2 square wave
int F2Total = 0; // Value for period of F2 square wave
int F2Freq = 0; // Value for frequency of F2 square wave

/* Button Toggle State */
bool toggle = false;

/* Frame Variables */
unsigned long timert = 0;
unsigned long frameTime = 0; // Get the time of the frame
unsigned long frameCounter = 0; // Count the number of frames incremented

/* Ticker Variables */
int endlessRun = 0; // Set varaible to make ticker tasks run endlessly

/* Ticker Setup */
Ticker timer1; // Ticker for Task 1
Ticker timer2; // Ticker for Task 2
Ticker timer3; // Ticker for Task 3
Ticker timer4; // Ticker for Task 4
Ticker timer5; // Ticker for Task 5

/////////////////////////////////////
////////// Set Up Function //////////
/////////////////////////////////////

void setup() 
{
  // Outputs
  pinMode(GREENLED, OUTPUT); // Set green LED as output
  pinMode(REDLED, OUTPUT); // Set red LED as output

  // Inputs
  pinMode(DoWorkReadButton, INPUT); // Set pushbutton 1 as input
  pinMode(PB2, INPUT); // Set pushbutton 2 as input
  pinMode(F1, INPUT); // Set frequency signal 1 as input
  pinMode(F2, INPUT); // Set frequency signal 2 as input

  monitor.startMonitoring(); // Start the monitoring function

  Serial.begin(300); // Start Serial monitor

  // Ticker Star//
  /*
  timer1.attach(timer1Delay, DigitalSignal_1);
  timer2.attach(timer2Delay, DigitalSignal_2);
  timer3.attach(timer3Delay, ReadSignal_1);
  timer4.attach(timer4Delay, ReadSignal_2);
  timer5.attach(timer5Delay, CallDoWork);
  */
}

/////////////////////////////////////
/////////// Loop Function ///////////
/////////////////////////////////////

void loop() 
{
  bool doWorkButton = digitalRead(DoWorkReadButton);
  /* Write red LED high when combined signals are greater than 1500Hz */
  if (F1Freq + F2Freq > 1500)
  {
    digitalWrite(REDLED, HIGH);
  }

  if (doWorkButton == true)
  {
    delay(50);
    Serial.print("doworkbutton = "); Serial.println(doWorkButton);
    toggle = !toggle; // Change state of toggle
    digitalWrite(YELLOWLED, toggle); // Acitvate/Deactivate LED depending on state of toggle
    monitor.doWork(); // Call doWork()
    Serial.println("Button push Dowork() finished"); // Show that doWork() is finished
  }
}

/////////////////////////////////////
///// Digital Signal 1 Function /////
/////////////////////////////////////

/* Start Digital Signal 2 Output Function */
void DigitalSignal_1()
{
  // runs for 614us
  monitor.jobStarted(1); // Start task 1 monitor
  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay250); // Keep on for 250us
  digitalWrite(GREENLED, LOW); // Green LED OFF
  delayMicroseconds(delay50); // keep off for 50us
  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay300); // Keep on for 300us
  monitor.jobEnded(1); // End task 1 monitor
}
/* End Digital Signal 1 Output Function */

/////////////////////////////////////
///// Digital Signal 2 Function /////
/////////////////////////////////////

/* Start Digital Signal 2 Output Function */
void DigitalSignal_2()
{
  // runs for 362us
  monitor.jobStarted(2); // Start task 2 monitor 

  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay100); // Keep on for 100us
  digitalWrite(GREENLED, LOW); // Green LED OFF
  delayMicroseconds(delay50); // Keep off for 50us
  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay200); // Keep on for200us

  monitor.jobEnded(2); // End task 2 monitor
}
/* End Digital Signal 2 Output Function */

//////////////////////////////////////
// Frequency Signal 1 Read Function //
//////////////////////////////////////

/* Start F1 Read Function */
void ReadSignal_1()
{
  // runs for 6us
  monitor.jobStarted(3); // Start task 3 monitor

  /* pulseIn() gets period of input signal */
  F1PulseHigh = pulseIn(F1, HIGH); // Read F1 Pin when square wave is HIGH
  F1PulseLow = pulseIn(F1, LOW); // Read F1 Pin when square wave in LOW
  F1Total = F1PulseHigh + F1PulseLow; // Get full square wave signal period
  F1Freq = 1/F1Total; // Get frequency of signal

  monitor.jobEnded(3); // End task 3 monitor
}
/* End F1 Read Function */

//////////////////////////////////////
// Frequency Signal 2 Read Function //
//////////////////////////////////////

/* Start F2 Read Function */
void ReadSignal_2()
{
  // runs for 6us
  monitor.jobStarted(4); // Start task 4 monitor

  /* pulseIn() gets period of input signal */
  F2PulseHigh = pulseIn(F2, HIGH); // Read F1 Pin when square wave is HIGH
  F2PulseLow = pulseIn(F2, LOW); // Read F1 Pin when square wave in LOW
  F2Total = F2PulseHigh + F2PulseLow; // Get full square wave signal period
  F2Freq = 1/F2Total; // Get frequency of signal

  monitor.jobEnded(4); // End task 4 monitor
}
/* End F1 Read Function */

//////////////////////////////////////
////////// DoWork Function ///////////
//////////////////////////////////////

/* Start the DoWork function */
void CallDoWork()
{
  monitor.jobStarted(5); // Start task 5
  monitor.doWork(); // Call doWork()
  monitor.jobEnded(5); // End task 5
}
/* End the DoWork function */

//////////////////////////////////////
///////// Time Taken Code ////////
//////////////////////////////////////

/*
int timert = micros();
code;
int deltat = micros() - timert;

Serial.print("Time Taken = "); Serial.println(deltat);
*/
