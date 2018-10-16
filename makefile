CC = gcc
CFLAGS = -Wall
OBJFILES = project3.o node0.o node1.o node2.o node3.o
TARGET = p3

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
