#pragma once

#include "Arduino.h"
#include "../i2c/i2c.h"

#define ds_addr 0b1101000
#ifdef SQW
#define ds_setSQW PORTC |= (1<<PC7);
#define ds_resetSQW PORTC &= ~(1<<PC7);
#endif

#define secReg 0
#define minReg 1
#define hourReg 2
#define dTReg 3
#define dayReg 4
#define mounthReg 5
#define yearReg 6

struct ds_val{
	uint8_t sec;
	uint8_t mins;
	uint8_t hour;
	uint8_t dayofWeek;
	uint8_t day;
	uint8_t mounth;
	uint8_t year;
};

class ds_class
{
public:
	i2c_class i2c;
	void init(void);
//	void setTime(uint8_t sec,uint8_t  min,uint8_t  hour,uint8_t dayOfWeek,uint8_t day,uint8_t mounth,uint8_t  year);
	void setTime(ds_val in_val);
	void setSingleReg(uint8_t regNr,uint8_t data);
	uint8_t readSingleReg(uint8_t  regNr);
	uint16_t readTime(void);
	uint16_t readDate(void);
	ds_val readAll(void);
	void enableSQW(void);
	void checkSQW(void);
	void SQWblink(void);
};

//#endif /* DS1307_DS1307_H_ */
