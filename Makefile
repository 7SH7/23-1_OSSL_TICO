# // Makefile
# tico : machine.o tico.o
# 	gcc -o tico machine.o tico.o
CC = gcc
TARGET = tico
OBJS = machine.o tico.o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

machine.o : 
	$(CC) -c machine.c 

tico.o :
	$(CC) -c tico.c