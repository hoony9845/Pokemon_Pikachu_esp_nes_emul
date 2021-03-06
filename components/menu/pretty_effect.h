#pragma once
#include <stdint.h>
#include "esp_err.h"


/**
 * @brief Calculate the effect for a bunch of lines.
 *
 * @param dest Destination for the pixels. Assumed to be LINECT * 240 16-bit pixel values.
 * @param line Starting line of the chunk of lines.
 * @param frame Current frame, used for animation
 * @param linect Amount of lines to calculate
 */
void pretty_effect_calc_lines(uint16_t *dest, int line, int frame, int linect);


/**
 * @brief Initialize the effect
 *
 * @return ESP_OK on success, an error from the jpeg decoder otherwise.
 */
esp_err_t pretty_effect_init();

bool peGetPixel(char peChar, int pe1, int pe2);

void setLineMax(int lineM);

int getSelRom();

void setSelRom(int selR);

void freeMem();

bool getTurboA();

bool getTurboB();

void setTurboA(bool en);

void setTurboB(bool en);

bool getYStretch();

bool getXStretch();

void setXStretch(bool str);

void setYStretch(bool str);

void setBright(int bright);
