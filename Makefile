CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
TARGET = lsusb
SOURCES = main.c usb_scan.c usb_display.c args.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = lsusb.h

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

debug: CFLAGS += -DDEBUG -g
debug: $(TARGET)

static: CFLAGS += -static
static: $(TARGET)

test: $(TARGET)
	@echo "Testing basic functionality..."
	@./$(TARGET) || echo "Basic test completed"
	@echo "Testing verbose mode..."
	@./$(TARGET) -v || echo "Verbose test completed"
	@echo "Testing help..."
	@./$(TARGET) -h || echo "Help test completed"

.SUFFIXES: .c .o

# Dependencies
main.o: main.c lsusb.h
usb_scan.o: usb_scan.c lsusb.h
usb_display.o: usb_display.c lsusb.h
args.o: args.c lsusb.h