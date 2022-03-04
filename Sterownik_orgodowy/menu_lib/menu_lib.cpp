#include "menu_lib.h"

Adafruit_TFTLCD screen::initScreen(Adafruit_TFTLCD tft_in){
	tft_in.reset();

	uint16_t identifier = tft_in.readID();

	if(identifier == 0x9341) {
		Serial.println(F("Found ILI9341 LCD driver"));
	}

	tft_in.begin(identifier);
	tft_in.fillScreen(BLACK);
	tft_lib = tft_in;

	prev_var = {0};
	prev_var.dateVar.sec = 0xFF;
	prev_var.dateVar.mins = 0xFF;
	prev_var.dateVar.hour = 0xFF;

	return tft_in;
}
void screen::exit_sign(int x0, int y0){
	tft_lib.setTextSize(3);
	tft_lib.fillRoundRect(x0,y0,50,50,10,RED);
	tft_lib.setCursor(x0+10,y0+10);
	tft_lib.setTextColor(WHITE);
	tft_lib.print("<");
}
void screen::ok_sign(int x0, int y0){
	tft_lib.setTextSize(3);
	tft_lib.fillRoundRect(x0,y0,50,50,10,GREEN);
	tft_lib.setCursor(x0+10,y0+15);
	tft_lib.setTextColor(WHITE);
	tft_lib.print("ok");
}
void screen::up_sign(int x0, int y0,uint16_t bg_colour ,uint16_t colour){
	uint8_t w = 50;
	tft_lib.fillRoundRect(x0,y0,50,50,10,bg_colour);
	uint8_t i;
	for(i=0;i<3;i++){
		tft_lib.drawLine(x0+10 + i, y0+w-10 + i, x0+23 + i, y0+10 + i,colour);
		tft_lib.drawLine(x0+10 + 1 + i, y0+w-10 + i, x0+23 + i, y0+10 + 1 + i,colour);

		tft_lib.drawLine(x0+w-10 - i ,y0+w-10 + i ,x0+25 - i ,y0+10 + i, colour);
		tft_lib.drawLine(x0+w-10 - 1 - i ,y0+w-10 + i ,x0+25 - 1 - i ,y0+10 + i, colour);
	}
}
void screen::down_sign(int x0, int y0,uint16_t bg_colour ,uint16_t colour){
	uint8_t w = 50;
	tft_lib.fillRoundRect(x0,y0,50,50,10,bg_colour);
	uint8_t i;
	for(i=0;i<3;i++){
		tft_lib.drawLine(x0+10 +i, y0+10 -i, x0+23+i, y0+w-10-i,colour);
		tft_lib.drawLine(x0+10 +i, y0+10+1 -i, x0+23+i, y0+w-10+1-i,colour);
		//
		tft_lib.drawLine(x0+w-10 - i ,y0+10 - i ,x0+25-i ,y0+w-10-i , colour);
		tft_lib.drawLine(x0+w-10-1 - i ,y0+10 - i ,x0+25-1-i ,y0+w-10-i , colour);
	}
}

void screen::setting_sign(int x0, int y0,uint16_t colour){
	uint8_t w = 32;
	    tft_lib.fillCircle(x0+16, y0+16, 10, colour);
	    tft_lib.fillRect(x0+13, y0, 6, w, colour);
	    tft_lib.fillRect(x0, y0+13, w, 6, colour);
	    uint8_t i;
	    for (i = 0; i < 4; i++) {
	    tft_lib.drawLine(x0 + 2 + i, (y0+w) - 2 - 4 + i, (x0+w) - 2 - 4 + i, y0 + 2 + i, colour);
	    tft_lib.drawLine(x0 + 2 + 1 + i, (y0+w) - 2 - 4 + i, (x0+w) - 2 - 4 + i, y0 + 2 + 1 + i, colour);
	    tft_lib.drawLine(x0 + 2 + i, y0 + 2 + 4 - i, (x0+w) - 2 - 4 + i, (y0+w) - 2 - i, colour);
	    tft_lib.drawLine(x0 + 2 + i, y0 + 2 + 4 - 1 - i, (x0+w) - 2 - 4 - 1 + i, (y0+w) - 2 - i, colour);
	  }
}
void screen::print_main_screen(){

	tft_lib.fillScreen(BLACK);
	tft_lib.fillRect(0,0,240,40,CYAN);
	tft_lib.drawRect(0,0,240,40,RED);
	tft_lib.drawRect(1,1,238,38,RED);
	setting_sign(204,4,BLACK);
	tft_lib.setCursor(10, 5);
	tft_lib.setTextColor(BLACK);
	tft_lib.setTextSize(3);
	tft_lib.print("Ustawienia");

	tft_lib.setCursor(0, 60);
	tft_lib.setTextColor(WHITE);
	tft_lib.print("Wilg.");

	tft_lib.setCursor(120, 60);
	tft_lib.println("Temp.");
	tft_lib.drawCircle(195, 100, 3, WHITE); // znak stopnia Celciusza
	tft_lib.setCursor(200, 100);
	tft_lib.println("C");
}

void screen::print_main_menu_screen(){
	uint8_t btn_w = 240, btn_h = 40, i;
	char* btn_txt[4] ={
			"godzina",
			"data",
			"pompka",
			"swiatlo"
	};
	tft_lib.fillScreen(BLACK);
	for(i=0;i<4;i++){
		tft_lib.fillRect(0,(i * btn_h)+(10 * i) + 10,btn_w,btn_h,CYAN);
		tft_lib.drawRect(0,(i * btn_h)+(10 * i) + 10,btn_w,btn_h,RED);
		tft_lib.drawRect(1,(i * btn_h)+(10 * i) + 11,btn_w - 2,btn_h - 2,RED);
		tft_lib.setCursor(50 ,(i * btn_h)+(10 * i) + 20);
		tft_lib.setTextColor(BLACK);
		tft_lib.setTextSize(3);
		tft_lib.print(btn_txt[i]);
	}
	exit_sign(180,260);
}

void screen::updateMainScreen(screen_var curr_var){
	float recHum = curr_var.Hum;
	float recTemp = curr_var.Temp;
	ds_val DateNtime = curr_var.dateVar;
	ds_val last = prev_var.dateVar;
	float lastHum = prev_var.Hum;
	float lastTemp = prev_var.Temp;
	tft_lib.setTextSize(3);
	if ((int)recHum != (int)lastHum) {   // wilgotnoœæ
	    tft_lib.fillRect(10, 100, 34, 30, BLACK);
	    tft_lib.setCursor(10, 100);
	    tft_lib.print((int)recHum);
	    tft_lib.print("%");
	  }
	  if (recTemp != lastTemp) {  // temperatura
	    tft_lib.fillRect(120, 100, 70, 22, BLACK);
	    tft_lib.setCursor(120, 100);
	    tft_lib.print((int)recTemp);
	    tft_lib.print(".");
	    recTemp *= 10;
	    int a = (int)recTemp;
	    a %= 10;
	    tft_lib.print(a);
	  }
	  tft_lib.setTextSize(5);
	  if (last.hour != DateNtime.hour) {       // godzina
		tft_lib.fillRect(0, 170, 90, 38, BLACK);
	    tft_lib.setCursor(0, 170);
	    if (DateNtime.hour < 10)tft_lib.print("0");
	    tft_lib.print(DateNtime.hour);
	    tft_lib.println(":");
	  }
	  if (last.mins != DateNtime.mins) {        // minuta
	    tft_lib.fillRect(90, 170, 90, 38, BLACK);
	    tft_lib.setTextColor(WHITE);
	    tft_lib.setCursor(90, 170);
	    if (DateNtime.mins < 10)tft_lib.print("0");
	    tft_lib.print(DateNtime.mins);
	    tft_lib.println(":");
	  }
	  if (last.sec != DateNtime.sec) {        // sekunda
	    tft_lib.fillRect(180, 170, 90, 38, BLACK);
	    tft_lib.setTextColor(WHITE);
	    tft_lib.setCursor(180, 170);
	    if (DateNtime.sec < 10)tft_lib.print("0");
	    tft_lib.print(DateNtime.sec);
	  }
	  tft_lib.setTextSize(4);
	  if (last.day != DateNtime.day) {        // dzieñ miesi¹ca
	    tft_lib.fillRect(2, 230, 70, 38, BLACK);
	    tft_lib.setTextColor(WHITE);
	    tft_lib.setCursor(2, 230);
	    if (DateNtime.day < 10)tft_lib.print("0");
	    tft_lib.print(DateNtime.day);
	    tft_lib.println("-");
	  }
	  if (last.mounth != DateNtime.mounth) {        // miesi¹c
	    tft_lib.fillRect(72, 230, 70, 38, BLACK);
	    tft_lib.setTextColor(WHITE);
	    tft_lib.setCursor(72, 230);
	    if (DateNtime.mounth < 10)tft_lib.print("0");
	    tft_lib.print(DateNtime.mounth);
	    tft_lib.println("-");
	  }
	  if (last.year != DateNtime.year) {        // rok
	    tft_lib.fillRect(142, 230, 98, 38, BLACK);
	    tft_lib.setTextColor(WHITE);
	    tft_lib.setCursor(142, 230);
	    tft_lib.print("20");
	    tft_lib.print(DateNtime.year);
	  }

	  prev_var = curr_var;
}
menu_state screen::print_screen(menu_state scr){
	ds_class ds_set;
	ds_val tim_set = ds_set.readAll();
	tim_set.sec = 0;
	scr.time = tim_set;

	if(scr.time.hour > 23 || scr.time.mins > 59 || scr.time.day > 31
			|| scr.time.mounth > 12 || scr.time.day == 0 || scr.time.mounth == 0){
		scr.time.hour = 0;
		scr.time.mins = 0;
		scr.time.day = 13;
		scr.time.mounth = 10;
		scr.time.year = 21;
	}

	uint16_t tim_px = 40,
		tim_py = 80,
		w = 160;
	uint8_t scr_vals[3];

	tft_lib.fillScreen(BLACK);

	switch(scr.screen_state){
	case LIGHT_SCREEN:
		break;

	case SET_TIME_SCREEN:
		tim_px = 60;
		tim_py = 50;
		scr_vals[0] = scr.time.hour;
		scr_vals[1] = scr.time.mins;
		break;

	case SET_DATE_SCREEN:
		tim_px = 0;
		tim_py = 10;
		scr_vals[0] = scr.time.day;
		scr_vals[1] = scr.time.mounth;
		scr_vals[2] = scr.time.year;
		w = 240;
		break;

	case PUMP_START_TIME_SCREEN:
		tim_px = 60;
		tim_py = 80;

		scr_vals[0] = scr.time.hour;
		scr_vals[1] = scr.time.mins;

		tft_lib.fillRect(0,0,240,25,CYAN);
		tft_lib.fillRect(0,25,90,40,GREEN);
		tft_lib.drawRect(0,25,90,40,CYAN);
		tft_lib.fillRect(90,25,150,40,RED);
		tft_lib.drawRect(90,25,150,40,CYAN);
		tft_lib.setTextSize(2);
		tft_lib.setTextColor(BLACK);
		tft_lib.setCursor(30,5);
		tft_lib.print("Wybierz godzine:");
		tft_lib.setCursor(10,35);
		tft_lib.print("startu");
		tft_lib.setCursor(100,35);
		tft_lib.print("zatrzymania");
		break;
	}

	tft_lib.setTextSize(4);
	tft_lib.setTextColor(BLACK);


	if(scr.screen_state != SET_DATE_SCREEN){
		tft_lib.fillRoundRect(tim_px - 20,tim_py + 60,w,50,10,WHITE);
	}else{
		tft_lib.fillRoundRect(tim_px,tim_py + 60,w,50,10,WHITE);
	}
	tft_lib.setCursor(tim_px,tim_py + 70);
	if(scr_vals[0] < 10)tft_lib.print("0");
	tft_lib.print(scr_vals[0]);
	if(scr.screen_state == SET_DATE_SCREEN)tft_lib.print("-");
	else tft_lib.print(":");
	if(scr_vals[1] < 10)tft_lib.print("0");
	tft_lib.print(scr_vals[1]);
	if(scr.screen_state == SET_DATE_SCREEN){
		tft_lib.print("-20");
		tft_lib.print(scr_vals[2]);
	}

	up_sign(tim_px,tim_py,WHITE,BLACK);
	up_sign(tim_px + 70,tim_py,WHITE,BLACK);
	down_sign(tim_px,tim_py + 120,WHITE,BLACK);
	down_sign(tim_px + 70,tim_py + 120,WHITE,BLACK);

	if(scr.screen_state == SET_DATE_SCREEN){
		up_sign(tim_px + 160,tim_py,WHITE,BLACK);
		down_sign(tim_px + 160,tim_py + 120,WHITE,BLACK);
	}

	exit_sign(180,260);
	ok_sign(10,260);

	return scr;
}

menu_state screen::check_screen_routine(menu_state scr){
	uint16_t px = scr.px,
			py = scr.py;

	uint16_t tim_px = 50,
			tim_py = 0;

	uint16_t btn_pos_x[6];
	uint16_t btn_pos_y[6];

	switch(scr.screen_state){
	case SET_TIME_SCREEN:
		tim_px = 60;
		tim_py = 50;
		break;

	case SET_DATE_SCREEN:
		tim_px = 0;
		tim_py = 10;
		break;

	case PUMP_START_TIME_SCREEN:
		tim_px = 60;
		tim_py = 80;
		break;

	}

	btn_pos_x[0] = tim_px;
	btn_pos_y[0] = tim_py;

	btn_pos_x[1] = tim_px + 70;
	btn_pos_y[1] = tim_py;

	btn_pos_x[2] = tim_px;
	btn_pos_y[2] = tim_py + 120;

	btn_pos_x[3] = tim_px + 70;
	btn_pos_y[3] = tim_py + 120;

	btn_pos_x[4] = tim_px + 160;
	btn_pos_y[4] = tim_py;

	btn_pos_x[5] = tim_px + 160;
	btn_pos_y[5] = tim_py + 120;

	uint8_t i,btn_quantity = 4 , nr_of_clicked_button = 0xFF;
	if(scr.screen_state == SET_DATE_SCREEN)btn_quantity = 6;
	for(i=0;i<btn_quantity;i++){

		if(px > btn_pos_x[i] && px < btn_pos_x[i] + 50 && py > btn_pos_y[i] && py < btn_pos_y[i] + 50){
			nr_of_clicked_button = i;
			if(scr.screen_state != SET_DATE_SCREEN){
				switch(i){
				case 0:
					if(scr.time.hour < 23)scr.time.hour++;
					break;
				case 1:
					if(scr.time.mins < 59)scr.time.mins++;
					break;
				case 2:
					if(scr.time.hour > 0)scr.time.hour--;
					break;
				case 3:
					if(scr.time.mins > 0)scr.time.mins--;
					break;
				}
			}else{
				switch(i){
				case 0:
					if(scr.time.day < 31)scr.time.day++;
					break;
				case 1:
					if(scr.time.mounth < 12)scr.time.mounth++;
					break;
				case 2:
					if(scr.time.day > 1)scr.time.day--;
					break;
				case 3:
					if(scr.time.mounth > 1)scr.time.mounth--;
					break;
				case 4:
					scr.time.year++;
					break;
				case 5:
					if(scr.time.year > 21)scr.time.year--;
					break;
				}
			}
		}
	}

	if( nr_of_clicked_button == 0 || nr_of_clicked_button == 1 || nr_of_clicked_button == 4){
		up_sign(btn_pos_x[nr_of_clicked_button],btn_pos_y[nr_of_clicked_button],WHITE,GREEN);
		delay(30);
		up_sign(btn_pos_x[nr_of_clicked_button],btn_pos_y[nr_of_clicked_button],WHITE,BLACK);
	}
	if( nr_of_clicked_button == 2 ||nr_of_clicked_button == 3 || nr_of_clicked_button == 5){
		down_sign(btn_pos_x[nr_of_clicked_button],btn_pos_y[nr_of_clicked_button],WHITE,GREEN);
		delay(30);
		down_sign(btn_pos_x[nr_of_clicked_button],btn_pos_y[nr_of_clicked_button],WHITE,BLACK);
	}

	tft_lib.setTextColor(BLACK);
	tft_lib.setTextSize(4);

	if(scr.screen_state != SET_DATE_SCREEN){
	if( nr_of_clicked_button == 0 ||nr_of_clicked_button == 2){
		tft_lib.fillRoundRect(tim_px,tim_py + 60,50,50,10,WHITE);
		tft_lib.setCursor(tim_px,tim_py + 70);
		if(scr.time.hour < 10)tft_lib.print("0");
		tft_lib.print(scr.time.hour);
	}
	if( nr_of_clicked_button == 1 ||nr_of_clicked_button == 3){
		tft_lib.fillRoundRect(tim_px + 70,tim_py + 60,70,50,10,WHITE);
		tft_lib.setCursor(tim_px + 70,tim_py + 70);
		if(scr.time.mins < 10)tft_lib.print("0");
		tft_lib.print(scr.time.mins);
	}
	}else{
		if( nr_of_clicked_button == 0 ||nr_of_clicked_button == 2){
			tft_lib.fillRoundRect(tim_px,tim_py + 60,50,50,10,WHITE);
			tft_lib.setCursor(tim_px,tim_py + 70);
			if(scr.time.day < 10)tft_lib.print("0");
			tft_lib.print(scr.time.day);
		}
		if( nr_of_clicked_button == 1 ||nr_of_clicked_button == 3){
			tft_lib.fillRoundRect(tim_px + 70,tim_py + 60,50,50,10,WHITE);
			tft_lib.setCursor(tim_px + 70,tim_py + 70);
			if(scr.time.mounth < 10)tft_lib.print("0");
			tft_lib.print(scr.time.mounth);
		}
		if( nr_of_clicked_button == 4 ||nr_of_clicked_button == 5){
			tft_lib.fillRoundRect(tim_px + 190,tim_py + 60,50,50,10,WHITE);
			tft_lib.setCursor(tim_px + 192,tim_py + 70);
			tft_lib.print(scr.time.year);
		}
	}
	return scr;
}

void screen::print_light_screen(menu_state scr){
	tft_lib.fillScreen(BLACK);
	tft_lib.fillRect(0,0,240,40,CYAN);
	tft_lib.drawRect(0,0,240,40,GREEN);
	tft_lib.setCursor(5,10);
	tft_lib.setTextSize(3);
	tft_lib.setTextColor(BLACK);
	tft_lib.print("ustaw jasnosc");
	tft_lib.setTextSize(2);
	tft_lib.drawRect(20,100,200,20,CYAN);
	tft_lib.fillRect(20,120,2,10,CYAN);
	tft_lib.fillRect(218,120,2,10,CYAN);
	tft_lib.fillRect(119,120,2,10,CYAN);
	tft_lib.setCursor(10,140);
	tft_lib.setTextColor(CYAN);
	tft_lib.print("0%");
	tft_lib.setCursor(103,140);
	tft_lib.print("50%");
	tft_lib.setCursor(190,140);
	tft_lib.print("100%");
	tft_lib.fillRect(scr.light_px,101,4,18,YELLOW);
	ok_sign(180,260);
}
menu_state screen::check_light_screen_routine(menu_state scr){

	uint16_t px = scr.px,
			py = scr.py;
	if(px >= 0xFFF)return scr;
	uint8_t light_val;
	if(py > 80 && py < 140){
		tft_lib.fillRect(21,101,198,18,BLACK);
		if(px > 20 && px < 216){
			light_val = px - 20;
			light_val /= 2;
			light_val *= 2.55;
		}
		if(px<=20){
			light_val=0;
			px = 21;
		}
		else if(px>=216){
			light_val=255;
			px = 215;
		}
		tft_lib.fillRect(px,101,4,18,YELLOW);
		pwm(light_val);
		scr.light_px = px;
	}
	return scr;
}

menu_state screen::check_btn_routine(menu_state screenState){
	uint16_t px = screenState.px;
	uint16_t py = screenState.py;

	switch(screenState.screen_state){
	case MAIN_SCREEN:
		if(py < 40){  // zostanie wciœniêty przycisk 'ustawienia'
			print_main_menu_screen();
			screenState.screen_state = MAIN_MENU_SCREEN;
		}
		break;
	case MAIN_MENU_SCREEN:
		if(px > 180 && px < 230 && py > 260 && py < 310){  // zosta³ wciœniêty przycisk powrotu do ekranu g³ównego
			prev_var = {0};   // aby odœwierzyc ekran startowy
			prev_var.dateVar.sec = 0xFF;
			prev_var.dateVar.mins = 0xFF;
			prev_var.dateVar.hour = 0xFF;
			print_main_screen();
			screenState.screen_state = MAIN_SCREEN;
		}
		if(py > 10 && py < 50){  // zosta³ wciœciêty przycisk ustawienia godziny
			screenState.screen_state = SET_TIME_SCREEN;
			screenState = print_screen(screenState);
		}
		if(py > 60 && py < 100){  // zosta³ wciœciêty przycisk ustawienia godziny
			screenState.screen_state = SET_DATE_SCREEN;
			screenState = print_screen(screenState);
		}
		if(py > 110 && py < 150){  // zosta³ wciœciêty przycisk ustawieñ czasu pracy pompki
			screenState.screen_state = PUMP_START_TIME_SCREEN;
			screenState = print_screen(screenState);
			screenState.pump_val.end_time = {0};
		}
		if(py > 160 && py < 200){  // zosta³ wciœciêty przycisk ustawieñ jasnosci
			screenState.screen_state = LIGHT_SCREEN;
			print_light_screen(screenState);
		}
		break;
	case SET_TIME_SCREEN:
		if(px > 180 && px < 230 && py > 260 && py < 310){  // zosta³ wciœniêty przycisk powrotu do menu g³ównego
			print_main_menu_screen();
			screenState.screen_state = MAIN_MENU_SCREEN;
		}
		if(px > 10 && px < 60 && py > 260 && py < 310){  // zosta³ wciœniêty przycisk zatwierdzenia godziny
			ds_class ds_set;
			screenState.time.sec = 0;
			ds_set.setTime(screenState.time);
			print_main_menu_screen();
			screenState.screen_state = MAIN_MENU_SCREEN;

		}
		break;
	case SET_DATE_SCREEN:
		if(px > 180 && px < 230 && py > 260 && py < 310){  // zosta³ wciœniêty przycisk powrotu do menu g³ównego
			prev_var = {0};   // aby odœwierzyc ekran startowy
			print_main_menu_screen();
			screenState.screen_state = MAIN_MENU_SCREEN;
		}
		if(px > 10 && px < 60 && py > 260 && py < 310){  // zosta³ wciœniêty przycisk zatwierdzenia godziny
			ds_class ds_set;
			ds_set.setTime(screenState.time);
			prev_var = {0};   // aby odœwierzyc ekran startowy
			print_main_menu_screen();
			screenState.screen_state = MAIN_MENU_SCREEN;

		}
		break;
	case PUMP_START_TIME_SCREEN:
		if(px > 180 && px < 230 && py > 260 && py < 310){  // zosta³ wciœniêty przycisk powrotu do menu g³ównego
			print_main_menu_screen();
			screenState.last_var = 0;
			screenState.screen_state = MAIN_MENU_SCREEN;
		}
		if(px > 10 && px < 60 && py > 260 && py < 310 && screenState.pump_val.in_use == END_TIME_SETTINGS){
			// zosta³ wciœniêty przycisk zatwierdzenia godziny
			if(screenState.last_var == END_TIME_SETTINGS)screenState.pump_val.end_time = screenState.time;
			switch(screenState.last_var){
			case END_TIME_SETTINGS:
				screenState.pump_val.end_time = screenState.time;
				break;
			case START_TIME_SETTINGS:
				screenState.pump_val.start_time = screenState.time;
				break;
			}

			screenState.pump_val.in_use = PUMP_WAITING_ST;
			print_main_menu_screen();
			screenState.screen_state = MAIN_MENU_SCREEN;
			screenState.last_var = 0;

		}
		if(px < 90 && py > 25 && py < 65 && screenState.last_var != START_TIME_SETTINGS){
			// zosta³ wciœniêty przycisk wyboru godziny rozpoczêcia podlewania
			screenState.last_var = START_TIME_SETTINGS;
			tft_lib.fillRect(0,25,90,40,GREEN);
			tft_lib.drawRect(0,25,90,40,CYAN);
			tft_lib.fillRect(90,25,150,40,RED);
			tft_lib.drawRect(90,25,150,40,CYAN);
			tft_lib.setCursor(10,35);
			tft_lib.setTextSize(2);
			tft_lib.print("startu");
			tft_lib.setCursor(100,35);
			tft_lib.print("zatrzymania");
			screenState.pump_val.end_time = screenState.time;
		}
		if(px > 90 && py > 25 && py < 65 && screenState.last_var != END_TIME_SETTINGS){
			// zosta³ wciœniêty przycisk wyboru godziny zakoñczenia podlewania
			screenState.last_var = END_TIME_SETTINGS;
			tft_lib.fillRect(0,25,90,40,RED);
			tft_lib.drawRect(0,25,90,40,CYAN);
			tft_lib.fillRect(90,25,150,40,GREEN);
			tft_lib.drawRect(90,25,150,40,CYAN);
			tft_lib.setCursor(10,35);
			tft_lib.setTextSize(2);
			tft_lib.print("startu");
			tft_lib.setCursor(100,35);
			tft_lib.print("zatrzymania");
			screenState.pump_val.start_time = screenState.time;
			screenState.pump_val.in_use = END_TIME_SETTINGS;
		}
		break;
	case LIGHT_SCREEN:
		screenState = check_light_screen_routine(screenState);
		if(px > 180 && px < 230 && py > 260 && py < 310){  // zosta³ wciœniêty przycisk powrotu do menu g³ównego
			print_main_menu_screen();
			screenState.last_var = 0;
			screenState.screen_state = MAIN_MENU_SCREEN;
		}
		break;
	}
	if(screenState.screen_state == SET_DATE_SCREEN || screenState.screen_state == SET_TIME_SCREEN
			|| screenState.screen_state == PUMP_START_TIME_SCREEN)
		screenState = check_screen_routine(screenState);
	return screenState;
}

