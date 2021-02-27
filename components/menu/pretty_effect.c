/*
   This code generates an effect that should pass the 'fancy graphics' qualification
   as set in the comment in the spi_master code.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_system.h>
#include <math.h>
#include "pretty_effect.h"
#include "decode_image.h"
#include "driver/gpio.h"
#include "charPixels.c"
#include "esp_deep_sleep.h"
#include "menu.h"

uint16_t **pixels;
int yOff;
int newX;
int bootTV;
int whiteN;
int slow;
int page;
int test;
int change;
int choosen;
int inputDelay;
int lineMax;
int selRom;

bool xStretch;
bool yStretch;

bool turboA;
bool turboB;

void setBright(int bright){
	setBr(bright);
}
	
bool peGetPixel(char peChar, int pe1, int pe2){
	return cpGetPixel(peChar,pe1,pe2);
}

bool getTurboA(){
	return turboA;
}

bool getTurboB(){
	return turboB;
}

void setTurboA(bool en) {
	turboA=en;
}

void setTurboB(bool en) {
	turboB=en;
}

bool getYStretch(){
	return yStretch;
}

bool getXStretch(){
	return xStretch;
}

void setXStretch(bool str){
	xStretch=str;
}

void setYStretch(bool str){
	yStretch=str;
}
void setLineMax(int lineM){
	lineMax = lineM;
}

void setSelRom(int selR){
	selRom=selR;
}

int getSelRom(){
	return selRom;
}

#ifdef CONFIG_LCD_FACE_1
#define pixels_w	220
#define pixels_h	102
#define pixels_x1	((240 - pixels_w) / 2)
#define pixels_x2	(pixels_x1 + pixels_w)
#define pixels_y1	((240 - pixels_h) / 2)
#define pixels_y2	(pixels_y1 + pixels_h)
#define bg_color	0x41fe
#endif

#ifdef CONFIG_LCD_FACE_2
#define bg_color	0xffff
#endif


int getBackground(){
	return bg_color;
}



static inline uint16_t bootScreen(int x, int y, int yOff, int bootTV){
	if(gpio_get_level(14)==1)test=0;
#ifdef CONFIG_LCD_FACE_1
	if(y>=pixels_y1 && y < pixels_y2) {
		if (x>=pixels_x1 && x < pixels_x2){
			return pixels[x-pixels_x1][y-pixels_y1];
		} else {
			return getBackground();
		}
	} else {
			return getBackground();
	}
#else
	if (x>=114 && x<=240 && y >=0 && y <= 78) { // ribbon
		return pixels[x][y];
	} else if (x>=31 && x<=52 && y >= 105 && y <= 134) { // left eye
		return pixels[x-31][y-105];
	} else if (x>=182 && x<=203 && y >= 105 && y <= 134) { // right eye
		return pixels[x-182][y-105];
	} else if (x>=103 && x<=137 && y >= 132 && y <= 157) { // nose
		return pixels[x-103][33+y-132];
	} else {
		return getBackground();
	}
#endif
}


//run "boot screen" (intro) and later menu to choose a rom
static inline uint16_t get_bgnd_pixel(int x, int y, int yOff, int bootTV, int choosen1)
{
	page=0;
	if(test>=0){
		if(y==0)test--;
		return bootScreen(x,y,yOff,bootTV);
	}
	
	if(x<=6 || x>=313 || y<3 || y>236 || (x>=23 && x<=25)) return 0x0000;
	if((y-3)%18<2 || (y-3)%18>=16) return 0x0000;

	return getCharPixel(x, y, change, choosen1);
}


//This variable is used to detect the next frame.
static int prev_frame=-1;

//Instead of calculating the offsets for each pixel we grab, we pre-calculate the valueswhenever a frame changes, then re-use
//these as we go through all the pixels in the frame. This is much, much faster.
/*static int8_t xofs[240], yofs[240];
static int8_t xcomp[240], ycomp[240];*/

//Calculate the pixel data for a set of lines (with implied line size of 240). Pixels go in dest, line is the Y-coordinate of the
//first line to be calculated, linect is the amount of lines to calculate. Frame increases by one every time the entire image
//is displayed; this is used to go to the next frame of animation.

void pretty_effect_calc_lines(uint16_t *dest, int line, int frame, int linect)
{
	if(bootTV>0)bootTV--;
    if(yOff>0 && bootTV==0)yOff--;
	
	if(inputDelay>0)inputDelay-=1;
	if(gpio_get_level(34)==1 && inputDelay==0 && choosen>0){
		choosen-=1;
		inputDelay=100;
	}
	if(gpio_get_level(33)==1 && inputDelay==0 && choosen<lineMax){
		choosen+=1;
		inputDelay=100;
	}
	if(gpio_get_level(13)==1) selRom=choosen;
	if(gpio_get_level(12)==1){
		//gpio_set_level(5, 0);
		esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO);
		gpio_pullup_dis(12);
		gpio_pulldown_en(12);
		esp_deep_sleep_enable_ext0_wakeup(12, 1);
		
		vTaskDelay(1000);
		esp_deep_sleep_start();
	}

	if (frame!=prev_frame) {
		//variable for blinking icons - very ugly solution, i know
		change+=1;
		if(change == 30)change = 0;
        prev_frame=frame;
    }
	
    for (int y=line; y<line+linect; y++) {
		for (int x=0; x<240; x++){
			*dest++=get_bgnd_pixel(x, y, yOff, bootTV, choosen);
        }
    }
}

void initGPIO(int gpioNo){
	gpio_set_direction(gpioNo, GPIO_MODE_INPUT);
	gpio_pulldown_en(gpioNo);
}

void freeMem(){
	for (int i=0; i<256; i++) {
            free((pixels)[i]);
        }
    free(pixels);
	freeRL();
}

//initialize varibles for "timers" and input, gpios and load picture
esp_err_t pretty_effect_init() 
{
	slow=4;
    yOff=slow*880;
	bootTV=slow*250;
	test=slow*1500;
	choosen=0;
	inputDelay=0;
	lineMax = 0;
	yStretch=0;
	xStretch=0;
	turboA=0;
	turboB=0;
	initRomList();
	initGPIO(34);
	initGPIO(35);
	initGPIO(32);
	initGPIO(39);
	initGPIO(17);
	initGPIO(14);
	initGPIO(12);
	initGPIO(13);
	initGPIO(33);
	initGPIO(16);
	return decode_image(&pixels);
}
