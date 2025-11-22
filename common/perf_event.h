
#ifndef PERF_EVENT_H
#define PERF_EVENT_H

#include<stdint.h>

void init_cycle_counter(void);
uint64_t get_cycle(void);
uint64_t hal_get_time(void);

#endif
