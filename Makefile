CC = gcc
CFLAGS  = -g -Wall

TARGET = simple-shell

.PHONY: clean
  
all: clean $(TARGET)
  
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS)

clean:
	rm -f *.o $(TARGET)