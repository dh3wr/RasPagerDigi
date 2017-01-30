MMCU=atmega8

CFLAGS = -O2 -mmcu=$(MMCU) -Wall -std=c++0x

CXX = avr-g++
CXXFLAGS = -O2 -mmcu=$(MMCU) -Wall -std=c++0x

LD = avr-g++
LDFLAGS = -O2 -mmcu=$(MMCU) -Wall

OBJCOPY = avr-objcopy

AVRDUDE = avrdude
AVRDUDEFLAGS = -C +./avrdude.conf -c raspberry_pi -p m8

MAIN = ATMega_FIFO
OBJ = ATMega_FIFO.o

all: $(MAIN).hex

$(MAIN).hex: $(MAIN).elf
	$(OBJCOPY) -O ihex $^ $@

$(MAIN).elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CXX) $(CFLAGS) -o $@ -c $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

clean:
	rm -f $(OBJ) $(MAIN).elf $(MAIN).hex

program: $(MAIN).hex
	sudo $(AVRDUDE) $(AVRDUDEFLAGS) -U flash:w:$(MAIN).hex

fuses:
	sudo $(AVRDUDE) $(AVRDUDEFLAGS) -U lfuse:w:0xff:m -U hfuse:w:0xc9:m

test:
	sudo $(AVRDUDE) $(AVRDUDEFLAGS) -v

start:
	sudo bash -c "echo 17 > /sys/class/gpio/export" || true
	sudo bash -c "echo 22 > /sys/class/gpio/export" || true
	sudo bash -c "echo out > /sys/class/gpio/gpio17/direction" || true
	sudo bash -c "echo in > /sys/class/gpio/gpio22/direction" || true
	sudo bash -c "echo 0 > /sys/class/gpio/gpio17/value; sleep 1" || true
	sudo bash -c "echo 1 > /sys/class/gpio/gpio17/value" || true

stop:
	sudo bash -c "echo 0 > /sys/class/gpio/gpio17/value" || true
	sudo bash -c "echo 22 > /sys/class/gpio/unexport" || true
	sudo bash -c "echo 17 > /sys/class/gpio/unexport" || true

install_dependencies:
	sudo apt-get install avrdude gcc-avr binutils-avr avr-libc

.PHONY: all clean program fuses start stop test install_dependencies
