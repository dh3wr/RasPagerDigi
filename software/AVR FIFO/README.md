Software by Christian Jansen Copyright apply.
Adapted from https://github.com/rwth-afu/RasPagerC9000

# Installing the dependencies

    make install_dependencies

Or manually:

    sudo apt-get install avrdude gcc-avr binutils-avr avr-libc

# Compiling

    make

# Burning the fuses

    make fuses

# Flashing the firmware

    make program

Wiring to the RasPager in the following way:
MISO	Pin40 on RasPi3 (GPIO21)
SCK		Pin38 on RasPi3 (GPIO20)
RST		Pin36 on RasPi3 (GPIO16)
MISO	Pin37 on RasPi3 (GPIO26)
