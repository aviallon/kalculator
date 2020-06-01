#ifndef STDIO_EXT_H
#define STDIO_EXT_H
#include <knightos/display.h>

float strtof (unsigned char* str, char** endptr);
int strtoi (unsigned char* str, char** endptr);

void ftoa (float x, unsigned char* str);
char* itoa(int x, unsigned char* str);

void draw_float(SCREEN* screen, unsigned char x, unsigned char y, float value);
void draw_signed(SCREEN* screen, unsigned char x, unsigned char y, signed int value);

#define log_of_10 2.302585092994046
#define inv_log_of_10 0.43429448190325176
#define log10(value) (logf(value)*inv_log_of_10)

unsigned log10u (unsigned x);

#define PI 3.141592653589793
#define E 2.718281828459045

#endif
