INCLUDEDIR = header
SRCDIR = src
BINDIR = bin
EXECUTABLE = rshell

CC = g++
CFLAGS = -c -Wall -Werror -ansi -pedantic -I$(INCLUDEDIR)/
LDFLAGS = 


TARGET = $(BINDIR)/$(EXECUTABLE)
SOURCES = $(wildcard $(addsuffix /*.cpp,$(SRCDIR)))
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all
all: $(TARGET) 

.PHONY: rshell
rshell: $(TARGET)

$(OBJECTS): %.o : %.cpp
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): check $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

check:
	@if [ ! -d $(BINDIR) ]; then \
		mkdir $(BINDIR); \
		fi

.PHONY: clean
clean:
	rm -f $(SRCDIR)/*.o
	rm -f $(TARGET)
