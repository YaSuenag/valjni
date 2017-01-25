.PHONY: clean

CC ?= gcc
CPPFLAGS += -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
CFLAGS += -c -g -fPIC -O3
LDFLAGS += -lpthread

TARGET = libvaljni.so

all: $(TARGET)

$(TARGET): valjni.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET) *.o
