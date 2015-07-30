#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

#include <project.h>
#include <stdint.h>

void current_init(void);
uint16_t get_current(void);
uint16_t get_soc(void);

#endif
