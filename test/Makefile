.PHONY: clean
.SUFFIXES: .java .class

CC ?= gcc
JAVAC ?= $(JAVA_HOME)/bin/javac
CPPFLAGS += -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
CFLAGS += -c -g -fPIC
LDFLAGS += -lpthread

TARGET = libjni-sample.so
JAVA_CLASSES = GlobalRefLeak.class

all: $(TARGET) $(JAVA_CLASSES)

$(TARGET): jni-sample.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

.java.class:
	$(JAVAC) $<

clean:
	$(RM) $(TARGET) $(JAVA_CLASSES) *.o

test:
	valgrind --leak-check=yes --suppressions=../jvm.supp \
	--gen-suppressions=all \
	java -agentpath:../libvaljni.so -Djava.library.path=. GlobalRefLeak
