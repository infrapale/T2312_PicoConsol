#include "task.h"
#include "main.h"

typedef struct
{
  task_st   *th;
} key_st;

key_st key;

void key_initialize(void)
{
    key.th = task_get_task(TASK_SCAN_KEY);
    key.th->state = 0;
}
void key_scan(void)
{
    switch(key.th->state)
    {
      case 0:
        digitalWrite(LED_BUILTIN, LOW); 
        key.th->state++;
        break;
      case 1:
        digitalWrite(LED_BUILTIN, HIGH); 
        key.th->state = 0;
        break;
    }
}

