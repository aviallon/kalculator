#include "consts.h"
#include "lib.h"
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

float strtof (unsigned char* str, char** endptr) {
    register unsigned char *ptr = str;
    bool is_neg = false;
    bool overflow = false;
    float result = 0;
    int c;
    float frac_base = 0.1;
    int exponent = 0;
    
    while ( isspace(*ptr) )
        ptr += 1;
    
    if(*ptr == '+') {
        ptr += 1;
    } else if (*ptr == '-'){
        ptr += 1;
        is_neg = true;
    }
    
    // Integer part
    while (1) {
        c = *ptr;
        if (c == '\0' || c < '0' || c > '9') {
            break;
        }
        
        c = c - '0';
        result = result * 10 + c;
        ptr += 1;
    }
    
    // Fractionnal part
    if ( *ptr == '.' ){
        ptr += 1;
        while(1) {
            c = *ptr;
            if (c == '\0' || c < '0' || c > '9') {
                break;
            }
            c = c - '0';
            
            result += frac_base * c;
            frac_base /= 10;
            ptr += 1;
        }
    }
    
    // Exponent part
    if ( *ptr == 'e' ){
        ptr += 1;
        while(1) {
            c = *ptr;
            if (c == '\0' || c < '0' || c > '9') {
                break;
            }
            c = c - '0';
            
            exponent = exponent * 10 + c;
            ptr += 1;
        }
        
        result = result * powf(10, exponent);
    }
    
    if(is_neg)
        result = -result;
    
    if (endptr != 0)
        *endptr = ptr;
    
    return result;
}

int strtoi (unsigned char* str, char** endptr) {
    register unsigned char *ptr = str;
    bool is_neg = false;
    bool overflow = false;
    int result = 0;
    unsigned base = 10;
    
    while ( isspace(*ptr) ) {
        ptr += 1;
    }
    
    if (*ptr == '+') {
        ptr += 1;
    } else if(*ptr == '-') {
        ptr += 1;
        is_neg = true;
    }
    
    while (1) {
        int c = *ptr;
        if (c == '\0' || c < '0' || c > '9') {
            break;
        }
        c = c - '0';
        if (result > (INT_MAX/base - c)) {
            overflow = true;
            ptr += 1;
            continue;
        }
        result = result * base + c;
        ptr += 1;
    }
    if (endptr != 0){
        *endptr = ptr;
    }
    
    if (is_neg){
        result = -result;
    }
    
    return result;
}

void ftoa (float x, unsigned char* str) {
    unsigned int integer_part;
    unsigned int frac_part;
    bool sgn = (x < 0);
    
    x = sgn ? -x : x;
    
    integer_part = (unsigned int)x;
    frac_part = ((x - (float)integer_part)*10000);
    
//     if(sgn) {
//         snprintf(str, 15, "-%d.%04d", integer_part, frac_part);
//     } else {
//         snprintf(str, 15, "%d.%04d", integer_part, frac_part);
//     }
    str = "BROKEN";
    return;
}

char* itoa(int x, unsigned char* str) {
    snprintf( str, 12, "%d", x );
    return str;
}

unsigned log10u(unsigned x){
    return (x >= 10000000u) ? 7 : (x >= 1000000u) ? 6 : 
        (x >= 100000u) ? 5 : (x >= 10000u) ? 4 :
        (x >= 1000u) ? 3 : (x >= 100u) ? 2 : (x >= 10u) ? 1u : 0u; 
}

void draw_signed(SCREEN* screen, unsigned char x, unsigned char y, signed int value){
    if(value < 0){
        draw_char(screen, x, y, '-');
        x += 4;
        value = -value;
    }
    draw_short(screen, x, y, value);
    return;
}

void draw_float(SCREEN* screen, unsigned char x, unsigned char y, float value){
    unsigned int integer_part;
    unsigned int frac_part;
    unsigned int integer_part_len;
    bool sgn = (value < 0);
    
    value = sgn ? -value : value;
    
    integer_part = (unsigned int)value;
    frac_part = ((value - (float)integer_part)*10000);
    
    integer_part_len = (log10u(integer_part)) + 1;
    
    if(sgn){
        draw_char(screen, x, y, '-');
        x += 4;
    }
    
    draw_short(screen, x, y, integer_part);
    x +=  integer_part_len*4;
    draw_char(screen, x, y, '.');
    
    x += 4;
    draw_short(screen, x, y, frac_part);

    return;
}
