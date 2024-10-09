
#include  "main.h"
#include  "time_func.h"
#include  "aio_mqtt.h"
#include  "log.h"

#define  SUBS_LEN    (64u)


typedef struct
{
  uint16_t head;
  uint16_t tail;
  uint16_t cursor;
} ring_buff_st;

ring_buff_st subs_log_ring;


log_subs_data_st subs_log[SUBS_LEN];   

void log_initialize(void)
{
  subs_log_ring.head = 0;
  subs_log_ring.tail = 0;
  for (uint16_t i = 0; i < SUBS_LEN; i++)
  {
    subs_log[i].subs_indx = AIO_SUBS_VA_OD_TEMP;
    subs_log[i].utime = 0;
    subs_log[i].value = 0.0; 
  }
  //subs_log[SUBS_LEN];   
}

void log_add_subs_data(aio_subs_et subs_indx, uint32_t unix_time, float value)
{
  subs_log[subs_log_ring.head].subs_indx  = subs_indx; 
  subs_log[subs_log_ring.head].utime      = unix_time;
  subs_log[subs_log_ring.head].value      = value;

  if (++subs_log_ring.head >= SUBS_LEN ) subs_log_ring.head = 0;
  if (subs_log_ring.head == subs_log_ring.tail)
  {
    if (++subs_log_ring.tail >= SUBS_LEN) subs_log_ring.tail = 0;
  } 
}

log_status_et log_set_last_subs(void)
{
   log_status_et ret = LOG_OK;
   if (subs_log_ring.head == subs_log_ring.tail) 
   {
      ret = LOG_EMPTY;
   }
   else 
   {  
      if (subs_log_ring.head == 0) 
        subs_log_ring.cursor = SUBS_LEN-1;
      else  
        subs_log_ring.cursor = subs_log_ring.head-1;
   }   
   return ret;
}

log_status_et log_prev_subs(void)
{
  log_status_et ret = LOG_OK;
  if (subs_log_ring.cursor == 0) 
    subs_log_ring.cursor = SUBS_LEN-1;
  else  
    subs_log_ring.cursor--;

  if (subs_log_ring.cursor == subs_log_ring.tail)
    ret = LOG_LAST;
  return ret;  
}

log_status_et log_get_subs(log_subs_data_st *subs_ptr)
{
  subs_ptr = &subs_log[subs_log_ring.cursor];
  return log_get_subs_ring_status();
}

log_status_et log_get_subs_ring_status(void)
{
  log_status_et ret = LOG_OK;
  if ((subs_log_ring.head) == 0 && (subs_log_ring.tail == 0))
  {
    ret = LOG_EMPTY; 
  }
  else 
  {
    if (subs_log_ring.cursor == subs_log_ring.tail)
    {
      ret = LOG_LAST;
    }
  }
  return ret;
}

//log_get_latest_subs()

void log_fill_test_data(uint16_t items)
{
  log_initialize();

  Serial.println("Filling ");  Serial.print(items);  Serial.println(" items");
  for (uint16_t cntr = 0; cntr < items; cntr++)
  {
    uint8_t   subs_indx = cntr & 0x03;
    uint32_t  utime = time_get_epoc_time();
    float     value = (float)cntr*100;

    log_add_subs_data( (aio_subs_et) subs_indx, utime, value);
    Serial.print("# "); Serial.print(cntr); 
    Serial.print(" H "); Serial.print(subs_log_ring.head); 
    Serial.print(" T "); Serial.print(subs_log_ring.tail); 
    Serial.println("");
  }

  Serial.println("All logs");
  for (uint16_t i = 0; i < SUBS_LEN; i++)
  {
      Serial.print("# "); Serial.print(i); 
      Serial.print(" S "); Serial.print(subs_log[i].subs_indx); 
      Serial.print(" U "); Serial.print(subs_log[i].utime); 
      Serial.print(" V "); Serial.print(subs_log[i].value); 
      Serial.println("");

  }

  Serial.println("Latest logs");
  log_set_last_subs();
  log_status_et status ;
  uint16_t i = 0;
  for (uint16_t j = 0; j < 70; j++)
  {
      i = subs_log_ring.cursor;
      status = log_get_subs_ring_status();
      Serial.print("# "); Serial.print(j);
      Serial.print(" > "); Serial.print(i); 
      Serial.print(" S "); Serial.print(subs_log[i].subs_indx); 
      Serial.print(" U "); Serial.print(subs_log[i].utime); 
      Serial.print(" V "); Serial.print(subs_log[i].value); 
      Serial.print(" ! "); Serial.print(status); 
      Serial.println("");
      status = log_prev_subs();

      i++;

  }




}

