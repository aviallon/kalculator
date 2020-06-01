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

#include "stack.h"
#include "lib.h"
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
    draw_window(screen, "Calculator", WIN_DEFAULTS);
	draw_string(screen, 2, 8, "In progress...");
}

enum TokenType {
    NUMBER,
    OPERATOR,
    UNKNOWN,
    SPACE,
};

int shunting_yard(Stack *stack, SCREEN* screen, unsigned char* input){
    Operator op;
    size_t begin = 0;
    size_t n = 1;
    Token temp;
    float number;
    bool end_token;
    Stack* first_stack;
    enum TokenType token_type;
    enum TokenType new_token_type;
    unsigned char* cur_token;
    unsigned char last_char = 0;
    size_t input_len = strlen(input);
    
    end_token = false;
    cur_token = malloc(32);
    //temp = malloc(sizeof(Token));
    first_stack = createStack(64);
    op = NONE;
    token_type = UNKNOWN;
    new_token_type = UNKNOWN;
    
    while(begin+n < input_len){
        //strncpy(cur_token, input + begin, n);
        last_char = input[begin+n-1];
        token_type = new_token_type;
        if(isspace(last_char)){
            if(token_type == SPACE){
                begin++;
            } else {
                end_token = true;
                begin += n;
                n = 1;
                new_token_type = SPACE;
            }
        } else if(isdigit(last_char) || last_char == '.') {
            if(token_type == NUMBER){
                n++;
            } else {
                end_token = true;
                begin += n;
                n = 1;
                new_token_type = NUMBER;
            }
        } else if(last_char == '+' || last_char == '-' || last_char == '/' || last_char == '*'){
            if(token_type == OPERATOR){
                n++;
            } else {
                end_token = true;
                begin += n;
                n = 1;
                new_token_type = OPERATOR;
            }
        }
        if(end_token){
            end_token = false;
            switch(token_type){
                case NUMBER:
                    strncpy(cur_token, input + begin, n);
                    number = strtof(cur_token, NULL);
                    push_num(stack, number);
                    break;
                case OPERATOR:
                    switch(input[begin]){
                        case '+':
                            push_op(stack, ADD);
                            break;
                        case '-':
                            push_op(stack, SUB);
                            break;
                        case '*':
                            push_op(stack, MULT);
                            break;
                        case '/':
                            push_op(stack, DIV);
                            break;
                        default:
                            debug_str(screen, "Unknown Op");
                            return 3;
                    }
                    break;
                default:
                    debug_str(screen, "Unknown token");
            }
        }
    }
    
    return 0;
}

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
    
    //debug_signed(screen, length(stack));
    
    while (length(stack) > 2){

        i++;
        
        pop_el(stack, op1);
        debug_float_hinted(screen, op1->x, "op1:");
        
        pop_el(stack, op2);
        debug_float_hinted(screen, op2->x, "op2:");
        
        pop_el(stack, operateur);
        debug_hinted(screen, operateur->op, "Operator:", draw_short);
        
        switch(operateur->op){
            case ADD:
                temp->x = op1->x + op2->x;
                break;
            case SUB:
                temp->x = op1->x - op2->x;
                break;
            case MULT:
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

void main() {
	SCREEN *screen;
    unsigned short i = 0;
    Num number = 0;
    Token temp = {0, NONE};
    const unsigned short STACK_SIZE = 16;
    bool success = false;
    
    Stack* stk;
    
    // must be used before any other statement which is not a variable declaration...
    load_library("/lib/core");
    get_lcd_lock();
    screen = screen_allocate();
    
    stk = createStack(STACK_SIZE);
    
    push_op(stk, ADD);
    push_num(stk, 3);
    push_op(stk, DIV);
    push_num(stk, 2);
    push_op(stk, MULT);
    push_num(stk, 7);
    push_num(stk, -192);
    
    //debug_hinted(screen, -3.14, "neg float:", draw_float);
    debug_float_hinted(screen, strtof("-43.45678e2", NULL), "strtof:");
    debug_float_hinted(screen, log10(100), "log10(100)=");
    //debug(screen, 3.43, draw_float);
    //debug(screen, 5.34, draw_float);
    //debug_hinted(screen, -4, "Signed:", draw_signed);
    
    
    success = eval(stk, screen);
    if(!success){
        debug_str(screen, "Eval failed!");
    }
    number = pop(stk)->x;
    debug_float_hinted(screen, number, "Eval res:");
    
    while (1) {
        unsigned char _;
        unsigned char key;
        i++;
        prepare(screen);
        key = app_get_key(&_);
        
        draw_short(screen, 2, 8*2, i);
        draw_string(screen, 24, 8*2, "STK:");
        draw_short(screen, 24+4*4, 8*2, length(stk));
        
        draw_float(screen, 2, 8*3, number);
        draw_short(screen, 2, 8*6, key);
        screen_draw(screen);
    }
}
