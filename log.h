#ifndef __LOG_H__
#define __LOG_H__


typedef enum
{
  LOG_OK  = 0,
  LOG_EMPTY,
  LOG_LAST
} log_status_et;


typedef struct
{
    aio_subs_et subs_indx;
    float       value;
    uint32_t    utime ;
} log_subs_data_st;


void log_initialize(void);

void log_add_subs_data(aio_subs_et subs_indx, uint32_t unix_time, float value);

log_status_et log_set_last_subs(void);

log_status_et log_prev_subs(void);

log_status_et log_get_subs(log_subs_data_st *subs_ptr);

log_status_et log_get_subs_ring_status(void);

void log_fill_test_data(uint16_t items);

#endif
