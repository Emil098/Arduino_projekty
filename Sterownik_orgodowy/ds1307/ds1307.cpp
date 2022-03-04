#include "ds1307.h"

void ds_class::init(void){
	i2c.init();
//	ds_enableSQW();
}
//void ds_class::setTime(uint8_t sec,uint8_t  min,uint8_t  hour,uint8_t dayOfWeek,uint8_t day,uint8_t mounth,uint8_t  year){ //rok to liczba 2 cyfrowa znaczy 17
//	sec = ((sec/10)<<4)|(sec%10);
//	min = ((min/10)<<4)|(min%10);
//	hour = ((hour/10)<<4)|(hour%10);
//	hour &= ~(0b11<<6);
//	dayOfWeek &= 7;
//	day = ((day/10)<<4)|(day%10);
//	mounth = ((mounth/10)<<4)|(mounth%10);
//	year = ((year/10)<<4)|(year%10);
//	i2c.start();
//	i2c.sendTo(ds_addr);
//	i2c.send_byte(0);
//	i2c.send_byte(sec);
//	i2c.send_byte(min);
//	i2c.send_byte(hour);
//	i2c.send_byte(dayOfWeek);
//	i2c.send_byte(day);
//	i2c.send_byte(mounth);
//	i2c.send_byte(year);
//	i2c.stop_trans();
//
//}

void ds_class::setTime(ds_val in_val){ //rok to liczba 2 cyfrowa np. 17 oznacza rok 2017
	in_val.sec = ((in_val.sec/10)<<4)|(in_val.sec%10);
	in_val.mins = ((in_val.mins/10)<<4)|(in_val.mins%10);
	in_val.hour = ((in_val.hour/10)<<4)|(in_val.hour%10);
	in_val.hour &= ~(0b11<<6);
	in_val.day = ((in_val.day/10)<<4)|(in_val.day%10);
	in_val.mounth = ((in_val.mounth/10)<<4)|(in_val.mounth%10);
	in_val.year = ((in_val.year/10)<<4)|(in_val.year%10);
	i2c.start();
	i2c.sendTo(ds_addr);
	i2c.send_byte(0);	// adres rozpoczêcia zapisu danych
	i2c.send_byte(in_val.sec);
	i2c.send_byte(in_val.mins);
	i2c.send_byte(in_val.hour);
	i2c.send_byte(in_val.dayofWeek);
	i2c.send_byte(in_val.day);
	i2c.send_byte(in_val.mounth);
	i2c.send_byte(in_val.year);
	i2c.stop_trans();

}
void ds_class::setSingleReg(uint8_t regNr,uint8_t data){
	i2c.start();
	i2c.sendTo(ds_addr);
	i2c.send_byte(regNr);
	i2c.send_byte(data);
	i2c.stop_trans();
}
uint8_t ds_class::readSingleReg(uint8_t  regNr){
	i2c.start();
	i2c.sendTo(ds_addr);
	i2c.send_byte(regNr);
	i2c.start();
	i2c.requestFrom(ds_addr);
	uint8_t ret=i2c.read_byte(0);

	return ret;
}
uint16_t ds_class::readTime(void){  //starsze 8 bitów godzina, m³odsze 8 bitów minuty
	uint8_t min = readSingleReg(minReg);
	min = (((min>>4)&0x0F)*10)+(min&0x0F);
	uint8_t hour = readSingleReg(hourReg);
	hour = (((hour>>4)&0x0F)*10)+(hour&0x0F);
	uint16_t ret = (hour<<8)|min;
	return ret;
}
uint16_t ds_class::readDate(void){  //starsze 8 bitów miesi¹c, m³odsze 8 bitów dzieñ
	uint8_t day = readSingleReg(dayReg);
	day = (((day>>4)&0x0F)*10)+(day&0x0F);
	uint8_t mon = readSingleReg(mounthReg);
	mon = (((mon>>4)&0x0F)*10)+(mon&0x0F);
	uint16_t ret = (mon<<8)|day;
	return ret;
}
ds_val ds_class::readAll(void){
	ds_val ret;
	uint16_t Time,Date;
	uint8_t second,year,doW;
	Time = readTime();
	Date = readDate();
	second = readSingleReg(secReg);
	second = (((second>>4)&0x0F)*10)+(second&0x0F);
	doW = readSingleReg(dTReg);
	doW = (((doW>>4)&0x0F)*10)+(doW&0x0F);
	year = readSingleReg(yearReg);
	year = (((year>>4)&0x0F)*10)+(year&0x0F);
	ret.sec = second;
	ret.mins = Time & 0xFF;
	ret.hour = (Time >> 8) & 0xFF;
	ret.dayofWeek = doW;
	ret.day = Date & 0xFF;
	ret.mounth = (Date >> 8) & 0xFF;
	ret.year = year;
	return ret;
}


#ifdef SQW
void ds_enableSQW(void){
	DDRC |= (1<<PC7);
	PORTC |= (1<<PC7);
	DDRB &= ~(1<<PB2);
	i2c.start();
	i2c.sendTo(ds_addr);
	i2c.send_byte(7); // adres dla "control register"
	i2c.send_byte(0b00010000); // czêstotliwoœc wyjœciowa 1 Hz
	i2c.stop_trans();
}
void ds_checkSQW(void){
	if((PINB>>PB2)&1)PORTC |= (1<<PC7); //sprawdza pors sqw z ds1307 i ustawia diode na wyœwietlaczu
	else PORTC &= ~(1<<PC7);
}
void ds_SQWblink(void){
	PORTC |= (1<<PC7);
	_delay_ms(50);
	PORTC &= ~(1<<PC7);
	_delay_ms(50);
	PORTC |= (1<<PC7);
}
#endif
