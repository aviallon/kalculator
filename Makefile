include .knightos/variables.make

#INCLUDE+=add to your include path

HEADERS:=$(wildcard *.h)
SOURCES:=$(wildcard *.c)
#CFLAGS:=--std-c99 --opt-code-size --no-optsdcc-in-asm --fno-omit-frame-pointer --no-peep $(DEBUG)
CFLAGS:=--std-c99 $(DEBUG)


ALL_TARGETS:=$(BIN)calculator

$(BIN)calculator: $(OUT)main.o $(OUT)stack.o $(OUT)lib.o $(OUT)crt0.o
	mkdir -p $(BIN)
	scas $(ASFLAGS) $(OUT)crt0.o $(LIBRARIES) $(OUT)lib.o $(OUT)stack.o $(OUT)main.o -o $(BIN)kalculator

include .knightos/sdk.make
