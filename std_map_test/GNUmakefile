CC = gcc
XX = g++
CFLAGS =-g
TARGET = ./map_test
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
$(TARGET) : $(OBJS)
	$(XX) $(OBJS) -o $(TARGET) -lpthread
	chmod a+x $(TARGET)
clean:
	 rm -rf *.o $(TARGET)
.PHONY:clean
