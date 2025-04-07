
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
#define GREENLED 6         // Green LED PIN
#define REDLED 7           // Red LED PIN
#define YELLOWLED 0        // Yellow LED Pin
#define ORANGELED 10       // Orange LED Pin
#define DoWorkReadButton 3 // Pushbutton to call task 6
#define F1 5               // Frequency signal 1 input pin
#define F2 4               // Frequency signal 2 input pin

/* Signal Delay Values */
// Task 1 Signal
#define delay250 250 // 250 microsecond delay
#define delay300 300 // 300 microsecond delay
// Task 2 Signal
#define delay100 100 // 100 microsecond delay
#define delay200 200 // 200 microsecond delay
// Both Signals
#define delay50 50   // 50 microsecond delay

/* Frequency Read Variables */
// Task 3 Frequency
long F1PulseHIGH = 0; // Value for high F1 square wave pulse
long F1Freq = 0;      // Frequency of F1 square wave
// Task 4 Frequency
long F2PulseHIGH = 0; // Value for HIGH F2 square wave
long F2Freq = 0;      // Value for frequency of F2 square wave

/* Ticker Setup */
Ticker tickerTimer;             // Ticker for schedule
unsigned long frameCounter = 0; // Frame counter
bool frameToggle = true;        // Toggle to switch between signals called at framecounter % 2
#define tickerDelay 2           // 2ms interrupt

/* Interrupt Setup */
volatile bool toggleLED = false; /* Starting toggleLED as false means LED will light up on first
                           button press */

/////////////////////////////////////
////////// Set Up Function //////////
/////////////////////////////////////

void setup() 
{
  // Start Serial //
  Serial.begin(19200);

  // Outputs
  pinMode(GREENLED, OUTPUT);  // Set green LED as output
  pinMode(REDLED, OUTPUT);    // Set red LED as output
  pinMode(YELLOWLED, OUTPUT); // Set yellow LED as output
  pinMode(ORANGELED, OUTPUT); // Set orange LED as output

  // Inputs
  pinMode(DoWorkReadButton, INPUT); // Set push button as input
  pinMode(F1, INPUT);               // Set frequency signal 1 as input
  pinMode(F2, INPUT);               // Set frequency signal 2 as input

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

/* Start Ticker Function */
void TickerTasks()
{
  unsigned long frameSelect = frameCounter % 30; // Select frame based on frame count

  /* Main Task Start */

  switch (frameSelect)
  {
    case 0:
      DigitalSignal_1(); DigitalSignal_2(); CallDoWork(); break;
    case 1:
      DigitalSignal_2(); ReadSignal_1(); break;
    case 2:
      DigitalSignal_1(); CallDoWork(); break;
    case 3:
      DigitalSignal_2(); ReadSignal_2(); break;   
    case 4:
      DigitalSignal_1(); DigitalSignal_2(); CallDoWork(); break;
    case 5:
      CallDoWork(); break;
    case 6:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); break;
    case 7:
      DigitalSignal_2(); CallDoWork(); break;
    case 8:
      DigitalSignal_1(); ReadSignal_2(); break;
    case 9:
      DigitalSignal_2(); CallDoWork(); break;
    case 10:
      DigitalSignal_1(); DigitalSignal_2(); CallDoWork(); break;
    case 11:
      ReadSignal_1(); ButtonDoWork(); break;
    case 12:
      DigitalSignal_1(); DigitalSignal_2(); CallDoWork(); break;
    case 13:
      DigitalSignal_2(); ReadSignal_2(); break;
    case 14:
      DigitalSignal_1(); CallDoWork(); break;
    case 15:
      DigitalSignal_2(); CallDoWork(); break;  
    case 16:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_1(); break;
    case 17:
      CallDoWork(); break;
    case 18:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); break;
    case 19:
      DigitalSignal_2(); CallDoWork(); break;
    case 20:
      DigitalSignal_1(); CallDoWork(); break;
    case 21:
      DigitalSignal_2(); ReadSignal_1(); break;
    case 22:
      DigitalSignal_1(); DigitalSignal_2(); CallDoWork(); break;
    case 23:
      ReadSignal_2(); break;
    case 24:
      DigitalSignal_1(); DigitalSignal_2(); CallDoWork(); break;
    case 25:
      DigitalSignal_2(); CallDoWork(); break;
    case 26:
      DigitalSignal_1(); ReadSignal_1(); break;
    case 27:
      DigitalSignal_2(); break;
    case 28:
      DigitalSignal_1(); DigitalSignal_2(); ReadSignal_2(); break;
    case 29:
      ButtonDoWork(); Freq1Freq2(); break;
    default:
      Serial.println("SAFTEY CATCH");
  }

  // Freq1Freq2(); // Call Task 7 at the end of the frame
  
  frameCounter++; // Increment Frame Counter

  /* Main Task End */
}
/* End Ticker Function */

//////////////////////////////////////
/////////// Task 1 Function //////////
//////////////////////////////////////

/* Start Task 1 Function */
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
  digitalWrite(GREENLED, LOW); // Turn off LED

  /* Main Task End */
  monitor.jobEnded(1); // End task 1 monitor
}
/* End Task 1 Function */

//////////////////////////////////////
/////////// Task 2 Function //////////
//////////////////////////////////////

/* Start Task 2 Function */
void DigitalSignal_2()
{
  // runs for 362us
  monitor.jobStarted(2); // Start task 2 monitor 

  /* Main Task Start */

  digitalWrite(YELLOWLED, HIGH); // Green LED ON
  delayMicroseconds(delay100); // Keep on for 100us
  digitalWrite(YELLOWLED, LOW); // Green LED OFF
  delayMicroseconds(delay50); // Keep off for 50us
  digitalWrite(YELLOWLED, HIGH); // Green LED ON
  delayMicroseconds(delay200); // Keep on for200us
  digitalWrite(YELLOWLED, LOW); // Turn off LED

  /* Main Task End */

  monitor.jobEnded(2); // End task 2 monitor
}
/* End Task 2 Function */

//////////////////////////////////////
/////////// Task 3 Function //////////
//////////////////////////////////////

/* Start Task 3 Function */
void ReadSignal_1()
{
  // runs for 2.944ms
  monitor.jobStarted(3); // Start task 3 monitor

  /* Main Task Start */

  /* pulseIn() gets period of input signal */
  F1PulseHIGH = pulseIn(F1, HIGH);  // Read F1 Pin when square wave is HIGH
  F1Freq = 1000000/(F1PulseHIGH*2); // Get frequency of signal

  /* Main Task End */

  monitor.jobEnded(3); // End task 3 monitor
}
/* End Task 3 Function */

//////////////////////////////////////
/////////// Task 4 Function //////////
//////////////////////////////////////

/* Start Task 4 Function */
void ReadSignal_2()
{
  // runs for 2.948ms
  monitor.jobStarted(4); // Start task 4 monitor

  /* Main Task Start */

  /* pulseIn() gets period of input signal */
  F2PulseHIGH = pulseIn(F2, HIGH); // Read F1 Pin when square wave is HIGH
  F2Freq = 1000000/(F2PulseHIGH*2);    // Get frequency of signal

  /* Main Task End */

  monitor.jobEnded(4); // End task 4 monitor
}
/* End Task 4 Function */

//////////////////////////////////////
////////// Task 5 Function ///////////
//////////////////////////////////////

/* Start Task 5 function */
void CallDoWork()
{
  // Runs for 503us
  monitor.jobStarted(5); // Start task 5 monitor

  /* Main Task */

  monitor.doWork();      // Call doWork()
  
  /* Main Task end */

  monitor.jobEnded(5);   // End task 5 monitor
}
/* End Task 5 function */


//////////////////////////////////////
////////// Task 6 Function ///////////
//////////////////////////////////////

/* Start Task 6 Function */
void Freq1Freq2()
{
  /* Main Task */

  if ((F1Freq + F2Freq) >= 1500)
  {
    digitalWrite(REDLED, HIGH); // LED ON when sum is greater than 1500
  }
  else 
  {
    digitalWrite(REDLED, LOW); // LED OFF whne sum is less than 1500
  }

  /* Main Task End */
}
/* End Task 6 Function */

//////////////////////////////////////
////////// Task 7 Function ///////////
//////////////////////////////////////

/* Start Task 7 Function */
void ButtonDoWork()
{
  /* Main Task */

  if (digitalRead(DoWorkReadButton) == HIGH)
  {
    toggleLED = !toggleLED;                              // Change state of toggle
    digitalWrite(ORANGELED, toggleLED);                  // Acitvate/Deactivate LED depending on state of toggle
    monitor.doWork();                                    // Call doWork()
    Serial.println("BUTTON PRESSED: Do Work Finished!"); // Show do work has been done
  }

  /* Main Task End */
}
/* End Task 7 Function */