CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lncurses

SRCS = src/main.c src/calendar.c src/ui_calendar.c src/ui_todo.c src/input.c
OBJS = $(SRCS:.c=.o)
DEPS = src/calendar.h src/ui_calendar.h src/ui_todo.h src/input.h src/config.h src/common.h

TARGET = nacal

all: $(TARGET) clean

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

veryclean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

