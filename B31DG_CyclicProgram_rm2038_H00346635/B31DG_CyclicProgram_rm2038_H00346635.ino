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
void ButtonDoWork();
void Freq1Freq2();
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
long F1PulseHIGH = 0; // Value for high F1 square wave pulse
long F1PulseLOW = 0; // Value for LOW F1 square wave pulse
long F1Total = 0; // Value for total period of F1 square wave
long F1Freq = 0; // Frequency of F1 square wave
// Frequency 2
long F2PulseHIGH = 0; // Value for HIGH F2 square wave
long F2PulseLOW = 0; // Value for LOW F2 square wave
long F2Total = 0; // Value for period of F2 square wave
long F2Freq = 0; // Value for frequency of F2 square wave

/* Ticker Setup */
Ticker tickerTimer; // Ticker for schedule
unsigned long frameCounter = 0; // Frame counter
bool frameToggle = true; // Toggle to switch between signals called at framecounter % 2
const uint tickerDelay = 3; // 3ms delay

/* Interrupt Setup */
volatile bool toggleLED = false; /* Starting toggleLED as false means LED will light up on first
                           button press */

/////////////////////////////////////
////////// Set Up Function //////////
/////////////////////////////////////

void setup() 
{
  // Start Serial //
  Serial.begin(9600);

  // Outputs
  pinMode(GREENLED, OUTPUT); // Set green LED as output
  pinMode(REDLED, OUTPUT); // Set red LED as output
  pinMode(YELLOWLED, OUTPUT); // Set yellow LED as output

  // Inputs
  attachInterrupt(digitalPinToInterrupt(DoWorkReadButton), ButtonInterrupt, HIGH); // Set ISR for doWorkButton press
  pinMode(F1, INPUT); // Set frequency signal 1 as input
  pinMode(F2, INPUT); // Set frequency signal 2 as input

  // Ticker Start //
  tickerTimer.attach_ms(tickerDelay, TickerTasks);

  // Start Monitor //
  monitor.startMonitoring();
}

/////////////////////////////////////
/////////// Loop Function ///////////
/////////////////////////////////////

void loop() 
{ 
 
}

/////////////////////////////////////
////////// Ticker Function //////////
/////////////////////////////////////
void TickerTasks()
{
  unsigned long frameSelect = frameCounter % 12; // Select frame based on scheduler

  /* Main Task Start */

  switch (frameSelect)
  {
    case 0:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 1:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 2:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 3:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 4:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 5:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 6:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 7:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 8:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 9:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 10:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
    case 11:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); CallDoWork(); break;
    case 12:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); CallDoWork(); break;
  }

  // Increment Frame Counter
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
  // runs for 2.944ms
  monitor.jobStarted(3); // Start task 3 monitor
  /* Main Task Start */

  /* pulseIn() gets period of input signal */
  F1PulseHIGH = pulseIn(F1, HIGH); // Read F1 Pin when square wave is HIGH
  F1PulseLOW = pulseIn(F1, LOW);
  F1Total = F1PulseHIGH + F1PulseLOW; // Get full square wave signal period
  F1Freq = 1000000/F1Total; // Get frequency of signal

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
  // runs for 2.948ms
  monitor.jobStarted(4); // Start task 4 monitor
  /* Main Task Start */

  /* pulseIn() gets period of input signal */
  F2PulseHIGH = pulseIn(F2, HIGH); // Read F1 Pin when square wave is HIGH
  F2PulseLOW = pulseIn(F2, LOW);
  F2Total = F2PulseHIGH + F2PulseLOW; // Get full square wave signal period
  F2Freq = 1000000/F2Total; // Get frequency of signal
  /* Main Task End */
  monitor.jobEnded(4); // End task 4 monitor

  Freq1Freq2();

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
void ButtonDoWork()
{
  delay(500); // Switch debounce
  toggleLED = !toggleLED; // Change state of toggle
  digitalWrite(YELLOWLED, toggleLED); // Acitvate/Deactivate LED depending on state of toggle
  monitor.doWork(); // Call doWork()
}

//////////////////////////////////////
///// Frequency Interrupt ISR ////////
//////////////////////////////////////
void Freq1Freq2()
{
  if ((F1Freq + F2Freq) >= 1500)
  {
    digitalWrite(REDLED, HIGH);
  }
  else 
  {
    digitalWrite(REDLED, LOW);
  }
}