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
B31DGCyclicExecutiveMonitor monitor;


// Set up core use
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


/* Function call */
void DigitalSignal_1(void *pvParameters);
void DigitalSignal_2(void *pvParameters);
void ReadSignal_1(void *pvParameters);
void ReadSignal_2(void *pvParameters);
void CallDoWork(void *pvParameters);
void ButtonDoWork(void *pvParameters);
void IRAM_ATTR ButtonHandle();
void Freq1Freq2(void *pvParameters);


/* Pin Definitions */
#define GREENLED 6 // Green LED PIN
#define REDLED 7 // Red LED PIN
#define YELLOWLED 0 // Yellow LED Pin
#define ORANGELED 10 // Orange LED Pin
#define DoWorkReadButton 3 // Pushbutton to call task 6
#define F1 5 // Frequency signal 1 input pin
#define F2 4 // Frequency signal 2 input pin


/* Delay Values */
// Task 1 Delays //
#define delay250 250   // 250 microsecond delay
#define delay300 300   // 300 microsecond delay
// Task 2 Delays //
#define delay100 100   // 100 microsecond delay
#define delay200 200   // 200 microsecond delay
// Both Task Delays
#define delay50  50    // 50 microsecond delay


/* Frequency Read Variables */
// Frequency 1 //
float F1PulseHIGH = 0;    // Value for high F1 square wave pulse
float F1Freq = 0;       // Frequency of F1 square wave
// Frequency 2 //
float F2PulseHIGH = 0;    // Value for HIGH F2 square wave
float F2Freq = 0;      // Value for frequency of F2 square wave


/* FreeRtos Setup */
// Semaphores //
SemaphoreHandle_t xMutex;      // Create mutex
SemaphoreHandle_t xBinSem;     // Create binary semaphore
SemaphoreHandle_t xButtonSem;  // Create binary semaphore for button interrupt

// Task Handlers //
TaskHandle_t Task1; // Task 1 handle
TaskHandle_t Task2; // Task 2 handle
TaskHandle_t Task3; // Task 3 handle
TaskHandle_t Task4; // Task 4 handle
TaskHandle_t Task5; // Task 5 handle
TaskHandle_t Task6; // Task 6 handle
TaskHandle_t Task7; // Task 7 handle

// Task Periods //
#define task1Period 4  // Set Period Task 1 is called
#define task2Period 3  // Set Period Task 2 is called
#define task3Period 10 // Set Period Task 3 is called
#define task4Period 10 // Set Period Task 4 is called
#define task5Period 5  // Set Period Task 5 is called

// Task Stack Sizes //
#define xTask1Stack 2048 // Task 1 stack size
#define xTask2Stack 2048 // Task 2 stack size
#define xTask3Stack 2048 // Task 3 stack size
#define xTask4Stack 2048 // Task 4 stack size
#define xTask5Stack 2048 // Task 5 stack size
#define xTask6Stack 1024 // Task 6 stack size
#define xTask7Stack 1024 // task 7 stack size

// Task Priorities //
#define xTask1Priority 2 // Task 1 priority
#define xTask2Priority 2 // Task 2 priority
#define xTask3Priority 3 // Task 3 priority
#define xTask4Priority 3 // Task 4 priority
#define xTask5Priority 2 // Task 5 priority
#define xTask6Priority 3 // Task 6 priority
#define xTask7Priority 1 // task 7 priority


/* Button Interrupt Varaibles */
bool toggleLED = false; /* Starting toggleLED as false means LED will light up on first
                          button press */
BaseType_t xHighPriorWake = pdFALSE;


/////////////////////////////////////
////////// Set Up Function //////////
/////////////////////////////////////

void setup()
{
  // Begin Serial //
  Serial.begin(19200);

  // Outputs //
  pinMode(GREENLED, OUTPUT);   // Set green LED as output
  pinMode(REDLED, OUTPUT);     // Set red LED as output
  pinMode(YELLOWLED, OUTPUT);  // Set yellow LED as output
  pinMode(ORANGELED, OUTPUT);  // Set orange LED as output

  // Inputs //
  pinMode(F1, INPUT);                                                               // Set frequency signal 1 as input
  pinMode(F2, INPUT);                                                               // Set frequency signal 2 as input
  attachInterrupt(digitalPinToInterrupt(DoWorkReadButton), ButtonHandle, FALLING);  // Button ISR

  /* FreeRTOS */

  // Create Mutex //
  xMutex = xSemaphoreCreateMutex();
  // Create Binary Semaphores //
  xBinSem = xSemaphoreCreateBinary();
  xButtonSem = xSemaphoreCreateBinary();

  // FreeRTOS Task Create //
  xTaskCreatePinnedToCore(DigitalSignal_1, "Task1", xTask1Stack, NULL, xTask1Priority, &Task1, app_cpu); // Create Task 1 on acceptible core
  xTaskCreatePinnedToCore(DigitalSignal_2, "Task2", xTask2Stack, NULL, xTask2Priority, &Task2, app_cpu); // Create Task 2 on acceptible core
  xTaskCreatePinnedToCore(ReadSignal_1, "Task3", xTask3Stack, NULL, xTask3Priority, &Task3, app_cpu);    // Create Task 3 on acceptible core
  xTaskCreatePinnedToCore(ReadSignal_2, "Task4", xTask4Stack, NULL, xTask4Priority, &Task4, app_cpu);    // Create Task 4 on acceptible core
  xTaskCreatePinnedToCore(CallDoWork, "Task5", xTask5Stack, NULL, xTask5Priority, &Task5, app_cpu);      // Create Task 5 on acceptible core
  xTaskCreatePinnedToCore(Freq1Freq2, "Task6", xTask6Stack, NULL, xTask6Priority, &Task6, app_cpu);      // Create Task 6 on acceptible core
  xTaskCreatePinnedToCore(ButtonDoWork, "Task7", xTask7Stack, NULL, xTask7Priority, &Task7, app_cpu);    // Create Task 7 on acceptible core

  /* B31DG Monitor */
  monitor.startMonitoring();  // Start the B31DG Monitor
}


/////////////////////////////////////
/////////// Loop Function ///////////
/////////////////////////////////////

void loop() 
{ /* Empty as functions handle all tasks */ }


//////////////////////////////////////
////////// Task 1 Function ///////////
//////////////////////////////////////

/* Start Task 1 Function */
void DigitalSignal_1(void *pvParameters)
{
  TickType_t xTask1LastTick;                                // Set tick handle for task 1
  const TickType_t xTask1Rate = pdMS_TO_TICKS(task1Period); // Set tick rate of task 1

  xTask1LastTick = xTaskGetTickCount();                     // Get tick of task 1 call

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      // runs for 614us
      monitor.jobStarted(1);        // Start task 1 monitor

      /* Main Task Start */

      digitalWrite(GREENLED, HIGH); // Green LED ON
      delayMicroseconds(delay250);  // Keep on for 250us
      digitalWrite(GREENLED, LOW);  // Green LED OFF
      delayMicroseconds(delay50);   // keep off for 50us
      digitalWrite(GREENLED, HIGH); // Green LED ON
      delayMicroseconds(delay300);  // Keep on for 300us
      digitalWrite(GREENLED, LOW);  // Turn off

      /* Main Task End */

      monitor.jobEnded(1);          // End task 1 monitor

      /* Give mutex */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask1LastTick, xTask1Rate); // Wait tick rate from last tick call
  }
}
/* End Task 1 Function */


//////////////////////////////////////
////////// Task 2 Function ///////////
//////////////////////////////////////

/* Start Task 2 Function */
void DigitalSignal_2(void *pvParameters)
{
  TickType_t xTask2LastTick;                                // Set tick handle for task 2
  const TickType_t xTask2Rate = pdMS_TO_TICKS(task2Period); // Set tick rate of task 2

  xTask2LastTick = xTaskGetTickCount();                     // Get tick of task 2 call

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
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
      digitalWrite(YELLOWLED, LOW); // Turn off

      /* Main Task End */

      monitor.jobEnded(2); // End task 2 monitor

      /* Give Mutex */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask2LastTick, xTask2Rate); // Wait tick rate from last tick call
  }
}
/* End Task 2 Function */


//////////////////////////////////////
////////// Task 3 Function ///////////
//////////////////////////////////////

/* Start Task 3 Function */
void ReadSignal_1(void *pvParameters)
{
  TickType_t xTask3LastTick;                                // Set handle for task 3
  const TickType_t xTask3Rate = pdMS_TO_TICKS(task3Period); // Set tick rate of task 3

  xTask3LastTick = xTaskGetTickCount();                     // Get tick of task 3 call

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      // runs for 2.944ms
      monitor.jobStarted(3);            // Start task 3 monitor
      /* Main Task Start */

      /* pulseIn() gets period of input signal */
      F1PulseHIGH = pulseIn(F1, HIGH);  // Read F1 Pin when square wave is HIGH
      F1Freq = 1000000/(F1PulseHIGH*2); // Get frequency of signal

      /* Main Task End */
      monitor.jobEnded(3);              // End task 3 monitor

      /* Give Semaphore */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask3LastTick, xTask3Rate); // Wait tick rate from last tick call
  }
}
/* End Task 3 Function */


//////////////////////////////////////
////////// Task 4 Function ///////////
//////////////////////////////////////

/* Start Task 4 Function */
void ReadSignal_2(void *pvParameters)
{
  TickType_t xTask4LastTick;                                // Set task 4 tick handle
  const TickType_t xTask4Rate = pdMS_TO_TICKS(task4Period); // Get tick rate of task 4

  xTask4LastTick = xTaskGetTickCount();                     // Get tick of task 4 call

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      // runs for 2.948ms
      monitor.jobStarted(4);            // Start task 4 monitor

      /* Main Task Start */

      /* pulseIn() gets period of input signal */
      F2PulseHIGH = pulseIn(F2, HIGH);  // Read F1 Pin when square wave is HIGH
      F2Freq = 1000000/(F2PulseHIGH*2); // Get frequency of signal

       /* Main Task End */

      /* Give Semaphore for Task 7 */
      monitor.jobEnded(4);              // End task 4 monitor

      /* Give Semaphore */
      xSemaphoreGive(xBinSem);
      /* Give Mutex */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask4LastTick, xTask4Rate); // Wait tick rate from last tick call
  }
}
/* End Task 4 Function */


//////////////////////////////////////
////////// Task 5 Function ///////////
//////////////////////////////////////

/* Start Task 5 function */
void CallDoWork(void *pvParameters)
{
  TickType_t xTask5LastTick;                                // Set task 5 tick handle
  const TickType_t xTask5Rate = pdMS_TO_TICKS(task5Period); // Set tick rate of task 5 

  xTask5LastTick = xTaskGetTickCount();                     // Get tick rate of task 5 call

  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      // Runs for 503us
      monitor.jobStarted(5); // Start task 5
      /* Main Task */

      monitor.doWork();      // Call doWork()

      /* Main Task end */
      monitor.jobEnded(5);   // End task 5

      /* Give Semaphore */
      xSemaphoreGive(xMutex);
    }
    vTaskDelayUntil(&xTask5LastTick, xTask5Rate); // Wait tick rate from last tick call
  }
}
/* End Task 5 function */


//////////////////////////////////////
////////// Task 6 Function ///////////
//////////////////////////////////////

/* Start Task 6 function */
void Freq1Freq2(void *pvParameters)
{
  /*
  TickType_t xTask6LastTick;                                // Set task 6 tick handle
  const TickType_t xTask6Rate = pdMS_TO_TICKS(5); // Set tick rate of task 6 

  xTask6LastTick = xTaskGetTickCount();                     // Get tick rate of task 6 call
  */
  while(1)
  {
    if (xSemaphoreTake(xBinSem, portMAX_DELAY) == pdTRUE)
    {

      if ((F1Freq + F2Freq) >= 1500)
      {
        digitalWrite(REDLED, HIGH); // If combined freuqencies greater than or equal to 1500 ACTIVATE red LED
      }
      else 
      {
        digitalWrite(REDLED, LOW); // If combined frequencies less than 1500 DEACTIVATE red LED
      }
    }    
    // vTaskDelayUntil(&xTask6LastTick, xTask6Rate); // Wait tick rate from last tick call
  }
}
/* End Task 6 function */


//////////////////////////////////////
////////// Task 7 Function ///////////
//////////////////////////////////////

/* Start Main Task 7 Function */
void ButtonDoWork(void *pvParamters)
{
  while(1)
  {
    if (xSemaphoreTakeFromISR(xButtonSem, &xHighPriorWake) == pdTRUE)
    {
      toggleLED = !toggleLED;             // Change state of toggle
      digitalWrite(ORANGELED, toggleLED); // Acitvate/Deactivate LED depending on state of toggle
      monitor.doWork();                   // Call doWork()
      Serial.println("BUTTON PRESSED: Do Work Finished!");
    }
  }
}
/* End Main Task 7 Function */

/* Start Task 7 Interrupt Handler */
void IRAM_ATTR ButtonHandle()
{
  xSemaphoreGiveFromISR(xButtonSem, &xHighPriorWake);

  if (xHighPriorWake == pdFALSE)
  {
    portYIELD_FROM_ISR(); // If priority handle isn't given don't send semaphore
  }
}
/* End Task 7 Interrupt Handler */
