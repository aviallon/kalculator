#ifndef CONSTS_H
#define CONSTS_H

#define ENOUGH 8 /* max 16 bit size */ + 1 /* minus sign */ + 1 /* null byte */

//void log(unsigned char* message);
#define DRAW_NUM draw_signed
#define Num int
#define strtonum strtoi

#define STACK_MAX 2

enum Operator {
    NONE,
    ADD,
    SUB,
    MUL,
    DIV,
};
typedef enum Operator Operator;

#define opname(operateur) ((operateur <= NONE) ? "NONE" : ((operateur <= ADD) ? "A" : "B"))

struct Token {
    Num x;
    Operator op;
};
typedef struct Token Token;

#endif
