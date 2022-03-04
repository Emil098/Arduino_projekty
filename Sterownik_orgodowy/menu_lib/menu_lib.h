#include "Arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include "../i2c/i2c.h"
#include "../ds1307/ds1307.h"

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define SET_PUMP_ON digitalWrite(1,0)
#define SET_PUMP_OFF digitalWrite(1,1)
#define PUMP_TURN_OFF 0
#define PUMP_WAITING_ST 1
#define PUMP_TURN_ON 2
#define START_TIME_SETTINGS 3
#define END_TIME_SETTINGS 4

#define MAIN_SCREEN 0
#define MAIN_MENU_SCREEN 1
#define SET_TIME_SCREEN 2
#define SET_DATE_SCREEN 3
#define PUMP_START_TIME_SCREEN 4
#define PUMP_END_TIME_SCREEN 5
#define LIGHT_SCREEN 6
#define MISSED_TOUCH 0xFF

#define pwm(p) analogWrite(11,p)

struct screen_var{
	float Hum;
	float Temp;
	ds_val dateVar;
};

struct pump_settings{
	ds_val start_time;
	ds_val end_time;
	uint8_t in_use;
};

struct menu_state{
	uint8_t screen_state;	// poziom menu w którym jest obecnie wyœwietlacz
	ds_val time;			// zapisywane dane czasu w strukturze
	pump_settings pump_val;	// struktura z godzin¹ rozpoczêcia zakoñczenia podlewania
	uint8_t last_var;		// wartoœc bierz¹cej zmiennej (o ile jest w u¿ytku)
	uint16_t px;			// wspó³rzêdne punktu x wyœwietlacza dotykowego
	uint16_t py;			// wspó³rzêdne punktu y wyœwietlacza dotykowego
	uint8_t light_px = 21;	// po³o¿enie wskaŸnika jasnosci
};

class screen
{
public:
	Adafruit_TFTLCD initScreen(Adafruit_TFTLCD tft_in);
	void print_main_screen();
	void print_main_menu_screen();
	void updateMainScreen(screen_var curr_var);
	menu_state check_btn_routine(menu_state screen_state);
	void print_light_screen(menu_state scr);
	menu_state print_screen(menu_state scr);
private:
	Adafruit_TFTLCD tft_lib;
	screen_var prev_var;
	menu_state check_light_screen_routine(menu_state scr);
	menu_state check_screen_routine(menu_state scr);
	menu_state set_date_check_routine(menu_state set_date_st);
	menu_state set_time_check_routine(menu_state set_time_st);
	menu_state set_pump_check_routine(menu_state set_pump_st);
	void setting_sign(int x0, int y0,uint16_t colour);
	void up_sign(int x0, int y0,uint16_t bg_colour ,uint16_t colour);
	void down_sign(int x0, int y0,uint16_t bg_colour ,uint16_t colour);
	void exit_sign(int x0, int y0);
	void ok_sign(int x0, int y0);
};

