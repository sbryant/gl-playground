#-*- mode:makefile-gmake; -*-
ROOT = $(shell pwd)
TARGET = playground

INCLUDE += -I$(ROOT)/src
SOURCES = $(wildcard $(ROOT)/src/*.c)

OBJS = $(patsubst %.c,%.o,$(SOURCES))
CPPFLAGS = $(OPTCPPFLAGS)
LIBS = $(shell pkg-config --libs glfw3) $(OPTLIBS)
CFLAGS = -g -std=c99 $(INCLUDE) -Wall -Werror $(OPTFLAGS)

ifeq ($(shell uname),Darwin)
# OS X specific libs required by glfw3
LIBS += -framework GLUT -framework OpenGL -framework Cocoa -framework IOKit
else
# TODO: support EGL?
endif

.DEFAULT_GOAL = all
all: $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET) $(TARGET).o $(TARGET).new

.PHONY: all clean

$(TARGET): $(TARGET).o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -flto -o $@.new
	mv $@.new $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^
