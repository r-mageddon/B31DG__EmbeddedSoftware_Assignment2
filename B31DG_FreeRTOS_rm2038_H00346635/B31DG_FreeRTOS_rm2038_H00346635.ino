/*
 * Name: Rowan Maxwell
 * H No.: H00346635
 * Class: B31DG
 * Title: Assignment 2 - FreeRTOS
 * Date Created: 26/03/2025
 * Description: This is the FreeRTOS program for assignment 2 of the 
 *              B31DG embedded software class at Heriot Watt University
*/

#include <B31DGMonitor.h>

/////////////////////////////////////
/// Variable and Pin Declarations ///
/////////////////////////////////////

/* Monitor Library Declaration */
B31DGCyclicExecutiveMonitor monitor(2000);

/* Function call */
void DigitalSignal_1(void *pvParameters);
void DigitalSignal_2(void *pvParameters);
void ReadSignal_1(void *pvParameters);
void ReadSignal_2(void *pvParameters);
void CallDoWork(void *pvParameters);
void ButtonDoWork();
void Freq1Freq2(void *pvParameters);

/* Pin Definitions */
#define GREENLED 19 // Green LED PIN
#define REDLED 21 // Red LED PIN
#define YELLOWLED 13 // Yellow LED Pin
#define DoWorkReadButton 23 // Pushbutton to call task 6
#define F1 5 // Frequency signal 1 input pin
#define F2 17 // Frequency signal 2 input pin

/* Signal Periods */
const uint task1Period = 3;
const uint task2Period = 2;
const uint task3Period = 9;
const uint task4Period = 8;
const uint task5Period = 4;

/* Delay Values */
const uint delay50 = 50;   // 50 microsecond delay
const uint delay100 = 100; // 100 microsecond delay
const uint delay200 = 200; // 200 microsecond delay
const uint delay250 = 250; // 250 microsecond delay
const uint delay300 = 300; // 300 microsecond delay
const uint tickerDelay = 2; // 2 millisecond delay

/* Frequency Read Variables */
// Frequency 1 //
float F1PulseHigh; // Value for high F1 square wave pulse
float F1PulseLow; // Value for LOW F1 square wave pulse
float F1Total; // Value for total period of F1 square wave
float F1Freq; // Frequency of F1 square wave
// Frequency 2 //
float F2PulseHigh; // Value for HIGH F2 square wave
float F2PulseLow; // Value for LOW F2 square wave
float F2Total; // Value for period of F2 square wave
float F2Freq; // Value for frequency of F2 square wave

/* FreeRtos Setup */
SemaphoreHandle_t xMutex; // Create mutex
SemaphoreHandle_t xBinSem; // Create binary semaphore
const int numTasks = 5; // Number of tasks for counting semaphore

/* Task Handlers */
TaskHandle_t Task1; // Task 1 handle
TaskHandle_t Task2; // Task 2 handle
TaskHandle_t Task3; // Task 3 handle
TaskHandle_t Task4; // Task 4 handle
TaskHandle_t Task5; // Task 5 handle
TaskHandle_t Task6; // Task 6 handle
TaskHandle_t Task7; // Task 7 handle

/* Button Interrupt Varaibles */
bool toggleLED = false; /* Starting toggleLED as false means LED will light up on first
                          button press */

/*
 * Place in ButtonDoWork function
Interrupt Setup 
bool toggleLED = false; 
*/

/////////////////////////////////////
////////// Set Up Function //////////
/////////////////////////////////////

void setup() 
{
  // Begin Serial //
  Serial.begin(9600);
  
  // Outputs //
  pinMode(GREENLED, OUTPUT); // Set green LED as output
  pinMode(REDLED, OUTPUT); // Set red LED as output
  pinMode(YELLOWLED, OUTPUT); // Set yellow LED as output

  // Inputs //
  pinMode(F1, INPUT); // Set frequency signal 1 as input
  pinMode(F2, INPUT); // Set frequency signal 2 as input
  attachInterrupt(digitalPinToInterrupt(DoWorkReadButton), ButtonDoWork, RISING); // Button ISR

  /* FreeRTOS */

  // Create Mutex //
  xMutex = xSemaphoreCreateMutex();

  // Create Binary Semaphore //
  xBinSem = xSemaphoreCreateBinary();


  // FreeRTOS Task Create //
  xTaskCreate(DigitalSignal_1, "DigitalSignal1", 2048, NULL, 2, &Task1);
  xTaskCreate(DigitalSignal_2, "DigitalSignal2", 2048, NULL, 1, &Task2);
  xTaskCreate(ReadSignal_1, "ReadFrequency1", 2048, NULL, 1, &Task3);
  xTaskCreate(ReadSignal_2, "ReadSignal2", 2048, NULL, 1, &Task4);
  xTaskCreate(CallDoWork, "CallDoWork", 2048, NULL, 1, &Task5);
  xTaskCreate(Freq1Freq2, "Freq1Freq2", 2048, NULL, 1, &Task7);

  /* B31DG Monitor */

  // Start Monitor //
  monitor.startMonitoring(); // Start the monitoring function
}

/////////////////////////////////////
/////////// Loop Function ///////////
/////////////////////////////////////

void loop() 
{ }

/////////////////////////////////////
///// Digital Signal 1 Function /////
/////////////////////////////////////

/* Start Digital Signal 2 Output Function */
void DigitalSignal_1(void *pvParameters)
{
  TickType_t xTask1LastTick;
  const TickType_t xTask1Rate = pdMS_TO_TICKS(4);

  xTask1LastTick = xTaskGetTickCount();

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
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

      /* Give mutex */
      xSemaphoreGive(xMutex);
    }

    vTaskDelayUntil(&xTask1LastTick, xTask1Rate);
  }
}
/* End Digital Signal 1 Output Function */

/////////////////////////////////////
///// Digital Signal 2 Function /////
/////////////////////////////////////

/* Start Digital Signal 2 Output Function */
void DigitalSignal_2(void *pvParameters)
{
  TickType_t xTask2LastTick;
  const TickType_t xTask2Rate = pdMS_TO_TICKS(3);

  xTask2LastTick = xTaskGetTickCount();

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
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

      /* Give Mutex */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask2LastTick, xTask2Rate);
  }
}
/* End Digital Signal 2 Output Function */

//////////////////////////////////////
// Frequency Signal 1 Read Function //
//////////////////////////////////////

/* Start F1 Read Function */
void ReadSignal_1(void *pvParameters)
{
  TickType_t xTask3LastTick;
  const TickType_t xTask3Rate = pdMS_TO_TICKS(10);

  xTask3LastTick = xTaskGetTickCount();

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      // runs for 2.944ms
      monitor.jobStarted(3); // Start task 3 monitor
      /* Main Task Start */

      /* pulseIn() gets period of input signal */
      F1PulseHigh = pulseIn(F1, HIGH); // Read F1 Pin when square wave is HIGH
      F1PulseLow = pulseIn(F1, LOW); // Read F1 Pin when square wave in LOW
      F1Total = F1PulseHigh + F1PulseLow; // Get full square wave signal period
      F1Freq = 1000000/F1Total; // Get frequency of signal

      /* Main Task End */
      monitor.jobEnded(3); // End task 3 monitor

      /* Give Semaphore */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask3LastTick, xTask3Rate);
  }
}
/* End F1 Read Function */

//////////////////////////////////////
// Frequency Signal 2 Read Function //
//////////////////////////////////////

/* Start F2 Read Function */
void ReadSignal_2(void *pvParameters)
{
  TickType_t xTask4LastTick;
  const TickType_t xTask4Rate = pdMS_TO_TICKS(10);

  xTask4LastTick = xTaskGetTickCount();

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      // runs for 2.948ms
      monitor.jobStarted(4); // Start task 4 monitor
      /* Main Task Start */

      /* pulseIn() gets period of input signal */
      F2PulseHigh = pulseIn(F2, HIGH); // Read F1 Pin when square wave is HIGH
      F2PulseLow = pulseIn(F2, LOW); // Read F1 Pin when square wave in LOW
      F2Total = F2PulseHigh + F2PulseLow; // Get full square wave signal period
      F2Freq = 1000000/F2Total; // Get frequency of signal

      /* Main Task End */
      monitor.jobEnded(4); // End task 4 monitor

      /* Give Semaphore for Task 7 */
      xSemaphoreGive(xBinSem);

      /* Give Mutex */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask4LastTick, xTask4Rate);
  }
}
/* End F1 Read Function */

//////////////////////////////////////
////////// DoWork Function ///////////
//////////////////////////////////////

/* Start the DoWork function */
void CallDoWork(void *pvParameters)
{
  TickType_t xTask5LastTick;
  const TickType_t xTask5Rate = pdMS_TO_TICKS(5);

  xTask5LastTick = xTaskGetTickCount();

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      // Runs for 503us
      monitor.jobStarted(5); // Start task 5
      /* Main Task */

      monitor.doWork(); // Call doWork()
      
      /* Main Task end */
      monitor.jobEnded(5); // End task 5

      /* Give Semaphore */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask5LastTick, xTask5Rate);
  }
}
/* End the DoWork function */

//////////////////////////////////////
//////// Button Do Work Call /////////
//////////////////////////////////////

/* Monitor Pushbutton function for Yellow LED */
void ButtonDoWork()
{ 
  delay(500); // Switch debounce
  toggleLED = !toggleLED; // Change state of toggle
  digitalWrite(YELLOWLED, toggleLED); // Acitvate/Deactivate LED depending on state of toggle
  monitor.doWork(); // Call doWork()
}

//////////////////////////////////////
////////// Frequency Task ////////////
//////////////////////////////////////

/* Frequency 1 and Frequency 2 comparison function */
void Freq1Freq2(void *pvParameters)
{
  while(1)
  {
      xSemaphoreTake(xBinSem, portMAX_DELAY);

      if ((F1Freq + F2Freq) >= 1500)
      {
        digitalWrite(REDLED, HIGH);
      }
      else 
      {
        digitalWrite(REDLED, LOW);
      }
    }
}