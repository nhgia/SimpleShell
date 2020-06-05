CC = gcc
CFLAGS  = -g -Wall

TARGET = simple-shell
  
all: $(TARGET)
  
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS)