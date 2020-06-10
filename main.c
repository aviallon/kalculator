#include <knightos/keys.h>
#include <knightos/display.h>
#include <knightos/system.h>
#include <stdbool.h>
#include <experimental/debug.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <corelib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <kernel.h>

#include "stack.h"
#include "lib.h"

//#define DISABLE_DEBUG

#include "debug.h"
#include "consts.h"


// temp must be a "token" variable
#define push_num(stack, num) \
temp.op = NONE; \
temp.x = num; \
push(stack, &temp);

#define push_op(stack, operator) \
temp.op = (operator); \
push(stack, &temp);

#define pop_el(stack, dest) \
memcpy(dest, pop(stack), sizeof(Token))

void prepare(SCREEN *screen) {
    screen_clear(screen);
    draw_window(screen, "Kalculator", WIN_DEFAULTS);
	draw_string(screen, 2, 8, "NPI. (-)=space");
}

enum TokenType {
    NUMBER,
    OPERATOR,
    UNKNOWN,
    SPACE,
};

Operator getOperatorFromChar(unsigned char c){
    switch (c){
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '/':
            return DIV;
        case '*':
            return MUL;
    }
    return NONE;
}

unsigned char getCharFromOperator(Operator op){
    switch (op){
        case ADD:
            return '+';
        case SUB:
            return '-';
        case DIV:
            return '/';
        case MUL:
            return '*';
    }
    return '?';
}

enum TokenType getTokenType(const unsigned char c){
    if(c == ' ') {
        return SPACE;
    } else if ( getOperatorFromChar(c) != NONE ) {
        return OPERATOR;
    } else if (c <= '9' && c >= '0' || c == '.'){
        return NUMBER;
    }
    return UNKNOWN;
}

unsigned getMaxTokenSize(enum TokenType tokenType){
    switch (tokenType) {
        case OPERATOR:
            return 1;
        default:
            return INT_MAX;
    }
}

int tokenizer(Stack *stack, SCREEN* screen, unsigned char* input){
    Operator op;
    Token temp;
    unsigned begin = 0;
    unsigned n = 1;
    float number;
    enum TokenType tokenType = UNKNOWN;
    enum TokenType charTokenType;
    enum TokenType lookaheadTokenType;
    unsigned char c = '\0';
    unsigned char lookahead_c = '\0';
    unsigned char *token_str;

    size_t input_len = strlen(input);
    token_str = malloc(32);

    tokenType = getTokenType(input[0]);

    while(begin+n <= input_len) {
        c = input[begin+n-1];
        //debug_hinted(screen, c, "Current char:", draw_char);
        lookahead_c = (begin+n < input_len) ? input[begin+n] : '\0';
        charTokenType = getTokenType(c);
        lookaheadTokenType = getTokenType(lookahead_c);

        if(charTokenType == SPACE){
            begin++;
            n = 1;
            continue;
        }
        //debug_hinted(screen, charTokenType == lookaheadTokenType, "continueTok? ", draw_short);
        //debug_hinted(screen, charTokenType, "c type:", draw_short);
        tokenType = charTokenType;
        if(charTokenType == lookaheadTokenType && n < getMaxTokenSize(tokenType)){
            tokenType = charTokenType;
            n++;
            //debug_hinted(screen, tokenType, "Continuing token:", draw_short);
            continue;
        }

        {
            //debug_str(screen, "Reached token boundary.");
            /* NUMBER : 0
             * OPERATOR : 1
             * UNKNOWN : 2
             * SPACE : 3
             */
            //debug_hinted(screen, tokenType, "Token type:", draw_short);
            //debug_short(screen, OPERATOR);
            switch (tokenType){
                case NUMBER:
                    strncpy(token_str, input + begin, n);
                    number = strtof(token_str, NULL);
                    debug_float_hinted(screen, number, "Num:");
                    push_num(stack, number);
                    break;
                case OPERATOR:
                    debug_hinted(screen, c, "Op:", draw_char);
                    push_op(stack, getOperatorFromChar(c));
                    break;
                default:
                    debug_str(screen, "Unknown token!");
                    return false;
            }
            begin += n;
            n = 1;
        }
    }

    free(token_str);
    return true;
}

/*
int shunting_yard(Stack *stack, SCREEN* screen){
    Operator op;
    Token temp;
    float number;
    bool end_token;
    Stack* first_stack;

    return true;
}*/

int eval(Stack *stack, SCREEN* screen){
    Token* op1;
    Token* op2;
    Token* operateur;
    Token* temp;
    
    unsigned i = 0;
    
    op1 = malloc(sizeof(Token));
    op2 = malloc(sizeof(Token));
    operateur = malloc(sizeof(Token));
    temp = malloc(sizeof(Token));

    temp->op = NONE;

    while (length(stack) > 2){

        i++;
        
        pop_el(stack, op1);
        debug_float_hinted(screen, op1->x, "op1:");
        
        pop_el(stack, op2);
        debug_float_hinted(screen, op2->x, "op2:");
        
        pop_el(stack, operateur);
        debug_hinted(screen, getCharFromOperator(operateur->op), "Operator:", draw_char);
        
        switch(operateur->op){
            case ADD:
                temp->x = op1->x + op2->x;
                break;
            case SUB:
                temp->x = op1->x - op2->x;
                break;
            case MUL:
                temp->x = op1->x * op2->x;
                break;
            case DIV:
                temp->x = op1->x / op2->x;
                break;
            default:
                return 0;
        }
        debug_float_hinted(screen, temp->x, "Result:");
        push(stack, temp);
    }
    return 1;
}

/*bool clockSupported() __naked {
    __asm
    ld iy,#2
    add iy, sp
    
    PCALL(CLOCKSUPPORTED)
    
    jr nz, unsupported
supported:
    ld l, #1
unsupported:
    ld l, #0
    ret
    __endasm;
}

short getTimeInTicks() __naked {
    __asm
    ld iy,#2
    add iy,sp

    
    PCALL(GETTIMEINTICKS)
    
    ret
    __endasm;
}*/

unsigned get_char(unsigned char keycode) {
    switch(keycode){
        case KEY_0:
            return '0';
        case KEY_1:
            return '1';
        case KEY_2:
            return '2';
        case KEY_3:
            return '3';
        case KEY_4:
            return '4';
        case KEY_5:
            return '5';
        case KEY_6:
            return '6';
        case KEY_7:
            return '7';
        case KEY_8:
            return '8';
        case KEY_9:
            return '9';
        case KEY_DOT:
            return '.';
        case KEY_PLUS:
            return '+';
        case KEY_MINUS:
            return '-';
        case KEY_MUL:
            return '*';
        case KEY_DIV:
            return '/';
        case KEY_ENTER:
            return '=';
        case KEY_STORE: // for z80e
        case KEY_NEG:
            return ' ';
        case KEY_DEL:
            return 'D';
    }
    return INT_MAX;
}

#define CURSOR_CHR '_'/*127*/ /*178*/

void main() {
	SCREEN *screen;
    unsigned short i = 0;
    unsigned i2 = 0;
    unsigned cursor = 0;
    const unsigned len = 32;
    unsigned chr = 0;
    unsigned last_key = 0;
    unsigned short del_num = 0;
    Num number = 0;
    unsigned char* str;
    unsigned char _;
    unsigned char key;
    Token temp = {0, NONE};
    const unsigned short STACK_SIZE = 16;
    bool success = false;
    
    Stack* stk;
    
    // must be used before any other statement which is not a variable declaration...
    load_library("/lib/core");
    
    get_lcd_lock();
    get_keypad_lock();
    screen = screen_allocate();
    
    debug_str(screen, "DEBUG MODE");
    
    stk = createStack(STACK_SIZE);
    
    str = malloc(len);
    str[0] = '\0';
    for(i2=1; i2<len-1; i2++){
        str[i2] = '\0';
    }
    str[len-1] = '\0';

    /*tokenizer(stk, screen, "+ 40 4.2");
    //shunting_yard(stk, screen, " 3 2 + ");

    for(i2=0; i2<length(stk); i2++){
         temp.op = stk->array[i2].op;
         temp.x = stk->array[i2].x;
         debug_hinted(screen, i2, "Index:", draw_short);
         if (temp.op == NONE){
             debug_hinted(screen, temp.x, "Number:", draw_float);
         } else {
             debug_hinted(screen, getCharFromOperator(temp.op), "Op:", draw_char);
         }
    }
    
    
    success = eval(stk, screen);
    if(!success){
        debug_str(screen, "Eval failed!");
    }
    number = pop(stk)->x;
    debug_float_hinted(screen, number, "Eval res:");
     */
    while (1) {
        i++;
        prepare(screen);
        key = app_get_key(&_);
        
        if (key == KEY_DEL) {
            if (cursor > 0 && del_num%5 == 0){
                cursor -= 1;
                str[cursor] = '\0';
                //str[cursor+1] = '\0';
                del_num = 0;
            }
            del_num += 1;
        } else if (key == KEY_ENTER) {
            clear(stk);
            tokenizer(stk, screen, str);
            reverse(stk);
            if(eval(stk, screen)) {
                number = pop(stk)->x;
            } else {
                number = -1;
            }
        } else {
            last_key = chr;
            chr = get_char(key);
            if(chr != INT_MAX && cursor < len && chr != last_key){
                str[cursor] = chr;
                cursor += 1;
            }
        }

        // Detect bugs
        if(i%15 < 8)
            invert_pixel(screen, DISPLAY_WIDTH-2, 7);
        //draw_short(screen, 2, 8*2, i);
        //draw_string(screen, 24, 8*2, "STK:");
        //draw_short(screen, 24+4*4, 8*2, length(stk));

        draw_string(screen, 2, 8*4, "Res:");
        draw_float(screen, 2 + 4*4, 8*4, number);


        draw_string(screen, 2, 8*6, str);
        if(i%40 < 20)
            draw_char(screen, 2 + 4*cursor, 8*6, '_');

#ifndef DISABLE_DEBUG
        draw_short(screen, 2, 8*2, key);
        draw_char(screen, 2 + 3*4, 8*2, '=');
        draw_char(screen, 2+4*4, 8*2, chr);
#endif

        screen_draw(screen);
    }

    freeStack(stk);
    //free(screen);
}
