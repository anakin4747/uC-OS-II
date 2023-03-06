#include "includes.h"

OS_EVENT *SecFlag;
// A one-second flag semaphore

static void StartTask(void *pdata);
static void Task1(void *pdata);
static void Task2(void *pdata);
// Task function prototypes
// They are static so these functions cannot be called from outside this file
// A function should never call a task 
// This is because they are handled by the kernel

static OS_STK StartTaskStk[STARTTASK_STK_SIZE];
static OS_STK Task1Stk[TASK1_STK_SIZE];
static OS_STK Task2Stk[TASK2_STK_SIZE];
// Allocating task stack space
// Each task uses its own stack space so we need to allocate that space 
// after declaring the prototypes
// The stack space should always be allocated in the same module of as the task
// The sizes of the stacks are macros defined in "includes.h"


// All the hardware and system initialization occurs before main()
void main(void){
// Main gets called after start-up code is complete or on system resets
// Therefore it is the entry point of the application code
//
// For ucOS-II projects the main typically consists of 4 things:
//      1. OSInit() to initialize the kernel
//      2. OSTaskCreate() on the startup task
//      3. Creation of any kernel event services
//      4. OSStart() to begin multitasking

    OSInit();   // 1.
    // Initialize uC/OS-II

    OSTaskCreate(StartTask, // 2.
            (void *)0,
            (void *)&StartTaskStk[STARTTASK_STK_SIZE],
            STARTTASK_PRIO);
    // Create start task by passing the StartTask function,
    // passing the address of the stack for that task,
    // and setting it the highest priority, STARTTASK_PRIO
    // so that this start-up task will never be interrupted

    SecFlag = OSSemCreate(0); // 3.
    // Create a semaphore flage using an OS_EVENT

    OSStart(); // 4.
    // Start running the operating system
    // Once this is called the kernel is running and control
    // never returns back to main()
    //
    // The StartTask will be the only task to run at first since its the only
    // task created in main()

}

static void StartTask(void *pdata){
// The application wide initialization is compeleted in the start up task
// StartTask is a mandetory function in ucOS-II
// StartTask has the highest priority so that the kernel never interrupts it
// until its finished creating all the tasks

    OSTickInit();
    // Initialize the OS ticker
    // This must run before any timer services can be used but after the kernel
    // starts, that is why it is initialized here
    
    DBUG_PORT_DIR = 0xff;
    DBUG_PORT = 0xff;
    // Set all bits high in the direction register 
    // and the data register for the debugger ports 

    OSTaskCreate(Task1,
            (void *)0,
            (void *)&Task1Stk[TASK1_STK_SIZE],
            TASK1_PRIO);

    OSTaskCreate(Task2,
            (void *)0,
            (void *)&Task2Stk[TASK2_STK_SIZE],
            TASK2_PRIO);
    // Create both tasks with their respective stacks and prioities

    FOREVER(){
        DBUG_PORT ^= DBUG_STSK;
        // Toggle debug port to show if StartTask runs again
        OSTaskSuspend(STARTTASK_PRIO);
        // Sets up a trap to ensure StartTask never runs again
    }
}

static void Task1(void *pdata){
// All task specific initialization is completed in the tasks themselves

    INT8U TimCntr = 0;
    // Counter for 1s flag

    FOREVER(){

        OSTimeDly(10);
        // Task period is 10ms

        DBUG_PORT ^= DBUG_TSK1;
        // Toggle the debug bit associated with task 1
        
        TimCntr++;
        if(TimCntr == 100){
            OSSemPost(SecFlag);
            TimCntr = 0;
        } // Signal 1 second with SecFlag
          // This flag is used to communicate to Task 2
          // which is waiting on this flag
    }
}

static void Task2(void *pdata){
    INT8U err;
    // Storage for an error code

    FOREVER(){
        OSSemPend(SecFlag, 0, &err);
        // Wait for SecFlag to be posted by task 1
        // This function puts this task in a waiting state
        DBUG_PORT ^= DBUG_TSK2;
        // Toggle task 2 debug bit to show sign of life
    }
}
