typedef unsigned char   INT8U;
typedef signed char     INT8S;
typedef unsigned short  INT16U;
typedef signed short    INT16S;
typedef unsigned long   INT32U;
typedef signed long     INT32S;

#define UBYTE INT8U
#define SBYTE INT8S
#define UWYDE INT16U
#define SWYDE INT16S

#define ISR __interrupt void

#define FALSE 0
#define TRUE 1

#define FOREVER()       while(1)
#define TRAP()          while(1){}
#define SWI()           asm("\tswi\n")
#define ENABLE_INT()    asm("\tcli\n")
#define DISABLE_INT()   asm("\tsei\n")

#define DBUG_PORT_DIR _H12DDRP
#define DBUG_PORT _H12PORTP
#define DBUG_STSK 0x01
#define DBUG_TSK1 0x02
#define DBUG_TSK2 0x04

#define STARTTASK_PRIO  4
#define TASK1_PRIO      6
#define TASK2_PRIO     10

#define STARTTASK_STK_SIZE 64
#define TASK1_STK_SIZE     64
#define TASK2_STK_SIZE     64

#include "hc912b32.h"
#include "ucos12.h"

