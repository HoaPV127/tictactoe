TARGET = game

CURDIR = $(PWD)
SUBDIRS = $(filter-out ./,  $(dir $(wildcard ./**/Makefile)))
SRCS = ./main.cpp $(wildcard ./**/*.cpp)
OBJS = $(SRCS:.cpp=.o)

CC = g++
INC = $(patsubst ./%, -I$(CURDIR)/%, $(SUBDIRS))
CPPFLAGS = -g -Wall $(INC)

export CPPFLAGS

.PHONY: default all clean $(SUBDIRS)

default: all

test:
	@echo $(SUBDIRS)

all:$(SUBDIRS) $(TARGET) 

clean:
	@rm -rf $(TARGET)
	@rm -rf $(OBJS)

$(SUBDIRS):
	$(MAKE) -C $@ all;

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o:%.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $< 

