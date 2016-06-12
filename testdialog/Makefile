#common makefile header

DIR_INC = iflytek/include
DIR_BIN = iflytek/bin
DIR_LIB = iflytek/libs

TARGET	= iat_class
BIN_TARGET = $(DIR_BIN)/$(TARGET)

CROSS_COMPILE = 
CFLAGS = -g -Wall -I$(DIR_INC)

LDFLAGS := -L$(DIR_LIB)/x64
LDFLAGS += -lmsc -lrt -ldl -lpthread
OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

$(BIN_TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CROSS_COMPILE)gcc -c $(CFLAGS) $< -o $@
clean:
	@rm -f *.o $(BIN_TARGET)

.PHONY:clean

#common makefile foot
