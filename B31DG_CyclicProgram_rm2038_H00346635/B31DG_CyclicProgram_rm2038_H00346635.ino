/*
 * Name: Rowan Maxwell
 * H No.: H00346635
 * Class: B31DG
 * Title: Assignment 2 - Cyclic Executive
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
void TickerTasks();

/* Pin Definitions */
#define GREENLED 19 // Green LED PIN
#define REDLED 21 // Red LED PIN
#define YELLOWLED 13 // Yellow LED Pin
#define DoWorkReadButton 23 // Pushbutton to call task 6
#define F1 5 // Frequency signal 1 input pin
#define F2 17 // Frequency signal 2 input pin

/* Signal Delay Values */
// Signal 1
const uint delay250 = 250; // 250 microsecond delay
const uint delay300 = 300; // 300 microsecond delay
// Signal 2
const uint delay100 = 100; // 100 microsecond delay
const uint delay200 = 200; // 200 microsecond delay
// Both Signals
const uint delay50 = 50;   // 50 microsecond delay

/* Frequency Read Variables */
// Frequency 1
int F1PulseHigh = 0; // Value for high F1 square wave pulse
int F1PulseLow = 0; // Value for LOW F1 square wave pulse
int F1Total = 0; // Value for total period of F1 square wave
int F1Freq = 0; // Frequency of F1 square wave
// Frequency 2
int F2PulseHigh = 0; // Value for HIGH F2 square wave
int F2PulseLow = 0; // Value for LOW F2 square wave
int F2Total = 0; // Value for period of F2 square wave
int F2Freq = 0; // Value for frequency of F2 square wave

/* Ticker Setup */
Ticker tickerTimer; // Ticker for Task 1
unsigned long frameCounter = 0; // Frame counter
bool frameToggle = true; // Toggle to switch between signals called at framecounter % 2
const uint tickerDelay = 3; // 3ms delay

/* Interrupt Setup */
bool toggleLED = false; /* Starting toggleLED as false means LED will light up on first
                           button press */

/////////////////////////////////////
////////// Set Up Function //////////
/////////////////////////////////////

void setup() 
{
  // Outputs
  pinMode(GREENLED, OUTPUT); // Set green LED as output
  pinMode(REDLED, OUTPUT); // Set red LED as output
  pinMode(YELLOWLED, OUTPUT); // Set yellow LED as output

  // Inputs
  attachInterrupt(digitalPinToInterrupt(DoWorkReadButton), ButtonInterrupt, HIGH); // Set ISR for doWorkButton press
  attachInterrupt(F1Freq + F2Freq > 1500, FrequencyInterrupt, true);
  pinMode(F1, INPUT); // Set frequency signal 1 as input
  pinMode(F2, INPUT); // Set frequency signal 2 as input

  monitor.startMonitoring(); // Start the monitoring function

  // Ticker Start //
  tickerTimer.attach_ms(tickerDelay, TickerTasks);
  TickerTasks();

  Serial.begin(9600);
}

/////////////////////////////////////
/////////// Loop Function ///////////
/////////////////////////////////////

void loop() 
{ }

/////////////////////////////////////
////////// Ticker Function //////////
/////////////////////////////////////
void TickerTasks()
{
  int FrameSelect = frameCounter % 20;

  /* Main Task Start */

  switch (FrameSelect)
  {
    case 0:
      DigitalSignal_1(); DigitalSignal_2(); break;
    case 1:
      DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 2:
      DigitalSignal_1(); DigitalSignal_2(); break;
    case 3:
      DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 4:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); break; 
    case 5:
      DigitalSignal_2(); CallDoWork(); break;
    case 6:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); break;
    case 7:
      DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 8:
      DigitalSignal_1(); DigitalSignal_2(); break;
    case 9:
      DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 10:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); break;
    case 11:
      DigitalSignal_2(); CallDoWork(); break;
    case 12:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); break;
    case 13:
      DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 14:
      DigitalSignal_1(); DigitalSignal_2(); break;
    case 15:
      DigitalSignal_2(); ReadSignal_1(); CallDoWork();  break;
    case 16:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); break;
    case 17:
      DigitalSignal_2(); CallDoWork(); break;
    case 18:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); break;
    case 19:
      DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 20:
      DigitalSignal_1(); DigitalSignal_2(); break;
    default:
      Serial.print("Default triggered");
  }

  frameCounter++;
  /* Main Task End */
}

/////////////////////////////////////
///// Digital Signal 1 Function /////
/////////////////////////////////////

/* Start Digital Signal 2 Output Function */
void DigitalSignal_1()
{
  // runs for 614us
  monitor.jobStarted(1); // Start task 1 monitor
  /* Main Task Start */

  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay250); // Keep on for 250us
  digitalWrite(GREENLED, LOW); // Green LED OFF
  delayMicroseconds(delay50); // keep off for 50us
  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay300); // Keep on for 300us

  /* Main Task End */
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
  /* Main Task Start */

  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay100); // Keep on for 100us
  digitalWrite(GREENLED, LOW); // Green LED OFF
  delayMicroseconds(delay50); // Keep off for 50us
  digitalWrite(GREENLED, HIGH); // Green LED ON
  delayMicroseconds(delay200); // Keep on for200us

  /* Main Task End */
  monitor.jobEnded(2); // End task 2 monitor
}
/* End Digital Signal 2 Output Function */

//////////////////////////////////////
// Frequency Signal 1 Read Function //
//////////////////////////////////////

/* Start F1 Read Function */
void ReadSignal_1()
{
  // runs for 2ms
  monitor.jobStarted(3); // Start task 3 monitor
  /* Main Task Start */

  /* pulseIn() gets period of input signal */
  F1PulseHigh = pulseIn(F1, HIGH); // Read F1 Pin when square wave is HIGH
  F1PulseLow = pulseIn(F1, LOW); // Read F1 Pin when square wave in LOW
  F1Total = F1PulseHigh + F1PulseLow; // Get full square wave signal period
  F1Freq = 1/F1Total; // Get frequency of signal

  /* Main Task End */
  monitor.jobEnded(3); // End task 3 monitor
}
/* End F1 Read Function */

//////////////////////////////////////
// Frequency Signal 2 Read Function //
//////////////////////////////////////

/* Start F2 Read Function */
void ReadSignal_2()
{
  // runs for 2ms
  monitor.jobStarted(4); // Start task 4 monitor
  /* Main Task Start */

  /* pulseIn() gets period of input signal */
  F2PulseHigh = pulseIn(F2, HIGH); // Read F1 Pin when square wave is HIGH
  F2PulseLow = pulseIn(F2, LOW); // Read F1 Pin when square wave in LOW
  F2Total = F2PulseHigh + F2PulseLow; // Get full square wave signal period
  F2Freq = 1/F2Total; // Get frequency of signal

  /* Main Task End */
  monitor.jobEnded(4); // End task 4 monitor
}
/* End F1 Read Function */

//////////////////////////////////////
////////// DoWork Function ///////////
//////////////////////////////////////

/* Start the DoWork function */
void CallDoWork()
{
  // Runs for 503us
  monitor.jobStarted(5); // Start task 5
  /* Main Task */

  monitor.doWork(); // Call doWork()
  
  /* Main Task end */
  monitor.jobEnded(5); // End task 5
}
/* End the DoWork function */

//////////////////////////////////////
/////// Button Interrupt ISR /////////
//////////////////////////////////////
void ButtonInterrupt()
{
  delay(500); // Switch debounce
  toggleLED = !toggleLED; // Change state of toggle
  digitalWrite(YELLOWLED, toggleLED); // Acitvate/Deactivate LED depending on state of toggle
  monitor.doWork(); // Call doWork()
}

//////////////////////////////////////
///// Frequency Interrupt ISR ////////
//////////////////////////////////////
void FrequencyInterrupt()
{
  digitalWrite(REDLED, HIGH);
}
