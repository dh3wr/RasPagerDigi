


// ############################################################################
// #                                 Config                                   #
// ############################################################################



// Frequency
#define F_CPU 8000000

// TRM_DATA -> This pin shifts out the data to the transmitter PB1
#define TRM_DATA_PORT      PORTB
#define TRM_DATA_DDR       DDRB
#define TRM_DATA_BIT       1

// RPI_HANDSHAKE -> This pin tells the RPI, when it is able to receive data PD3
#define RPI_HANDSHAKE_PORT PORTD
#define RPI_HANDSHAKE_DDR  DDRD
#define RPI_HANDSHAKE_BIT  3

// RPI_SHIFTING -> This pin tells the RPI, if it is still shifting out data PD4
#define RPI_SHIFTING_PORT  PORTD
#define RPI_SHIFTING_DDR   DDRD
#define RPI_SHIFTING_BIT   4

// RPI_DATA -> This pin receives the data from the RPI PD0
#define RPI_DATA_PORT      PORTD
#define RPI_DATA_PIN       PIND
#define RPI_DATA_BIT       0

// RPI_CLOCK -> This pin receives the data clock from the RPI PD1
#define RPI_CLOCK_PORT     PORTD
#define RPI_CLOCK_PIN      PIND
#define RPI_CLOCK_BIT      1



// ############################################################################
// #                              Preparations                                #
// ############################################################################



// Avr-libc include files
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Bit Manipulation
#define SET_BIT(var, bitnum)   (var) |=  (1<<(bitnum))
#define CLR_BIT(var, bitnum)   (var) &= ~(1<<(bitnum))
#define TOG_BIT(var, bitnum)   (var) ^=  (1<<(bitnum))
#define GET_BIT(var, bitnum)  (((var)>>(bitnum)) & 0x1)

// IO Defines
#define INIT_OUTPUT(signal)  {SET_BIT(signal##_DDR,  signal##_BIT);}
#define SET_OUTPUT(signal)   {SET_BIT(signal##_PORT, signal##_BIT);}
#define CLR_OUTPUT(signal)   {CLR_BIT(signal##_PORT, signal##_BIT);}
#define IS_INPUT(signal)     (GET_BIT(signal##_PIN,  signal##_BIT))



// ############################################################################
// #                                  FiFo                                    #
// ############################################################################



#define FIFO_Init(name, type, size)          \
static uint16_t FIFOBUF_##name##_push  = 0; \
static uint16_t FIFOBUF_##name##_pop   = 0; \
static uint16_t FIFOBUF_##name##_count = 0; \
static type FIFOBUF_##name##_data[size];

#define FIFO_Clear(name)      \
FIFOBUF_##name##_push  = 0; \
FIFOBUF_##name##_pop   = 0; \
FIFOBUF_##name##_count = 0;

#define FIFO_Empty(name) \
(!FIFOBUF_##name##_count)

#define FIFO_Full(name) \
(FIFOBUF_##name##_count == sizeof(FIFOBUF_##name##_data))

#define FIFO_Push(name, value)                          \
FIFOBUF_##name##_data[FIFOBUF_##name##_push] = value; \
FIFOBUF_##name##_count++;                             \
FIFOBUF_##name##_push++;                              \
if(FIFOBUF_##name##_push >= sizeof(FIFOBUF_##name##_data)) FIFOBUF_##name##_push = 0;

#define FIFO_Pop(name, var)                          \
var = FIFOBUF_##name##_data[FIFOBUF_##name##_pop]; \
FIFOBUF_##name##_count--;                          \
FIFOBUF_##name##_pop++;                            \
if(FIFOBUF_##name##_pop >= sizeof(FIFOBUF_##name##_data)) FIFOBUF_##name##_pop = 0;

FIFO_Init(buffer, uint8_t, 512);



// ############################################################################
// #                           1200 Hz Interrupt                              #
// ############################################################################


// Changes by DH3WR: Use Timer 1 instead of Timer 2
//ISR(TIMER2_COMP_vect)

ISR(TIMER1_COMPA_vect) {
	// Handle shifting out of data
	if(!FIFO_Empty(buffer))      // Data in buffer?
	{
		uint8_t databit;
		FIFO_Pop(buffer, databit); // Get bit from FIFO
		if(databit)                // Databit is "1"?
		{
			SET_OUTPUT(TRM_DATA);    // Set data output high
		}
		else                       // Databit is "0"?
		{
			CLR_OUTPUT(TRM_DATA);    // Set data output low
		}
		SET_OUTPUT(RPI_SHIFTING);  // Tell RPI that currently data is shifted out to the transmitter
	}
	else                         // No data in buffer anymore?
	{
		CLR_OUTPUT(TRM_DATA);      // Set data output low
		CLR_OUTPUT(RPI_SHIFTING);  // Tell RPI that no data is shifted out at the moment
	}

}

// ############################################################################
// #                               Main-Loop                                  #
// ############################################################################

int main() {
/* Changes by DH3WR, 22.1.17
Use Timer 1 instead of Timer 2, because it's 16 Bit

	// Initialize timer for 1200 Hz
	TCCR2 = (0<<FOC2)  // No force of output compare match
	| (1<<WGM21)
	| (0<<WGM20) // CTC
	| (0<<COM21)
	| (0<<COM20) // OCx disconnected
	| (1<<CS22)
	| (0<<CS21)
	| (0<<CS20); // Clock /  256 (from prescaler)
	OCR2  = F_CPU / 64 / 1207;
	TIMSK = (1<<OCIE2);
*/
	// Initialize timer 1 for 1200 Hz
	// No Output on compare match, no PWM
	TCCR1A = 0x00;
	// Clear Timer on match, CPU-Clock as input
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = 6666;
	TIMSK = (1 << OCIE1A);

	// Enable outputs
	INIT_OUTPUT(TRM_DATA);
	INIT_OUTPUT(RPI_HANDSHAKE);
	INIT_OUTPUT(RPI_SHIFTING);

	// Enable interrupts
	sei();

	while(1)
	{
		// Handle receiving of data
		if(FIFO_Full(buffer))                      // Buffer full?
		{
			CLR_OUTPUT(RPI_HANDSHAKE);               // Set handshake output to low -> Not able to receive anymore
		}
		else                                       // Buffer not full yet?
		{
			static uint8_t last_clk_state;
			uint8_t current_clk_state;
			uint8_t databit;

			current_clk_state = IS_INPUT(RPI_CLOCK); // Get current clock state
			databit           = IS_INPUT(RPI_DATA);  // Get data bit value

			if(    (last_clk_state    == 1)          // Clock edge from high to low?
			&& (current_clk_state == 0)
			)
			{
				cli();                                 // Disable interrupt
				FIFO_Push(buffer, databit);            // Push data to buffer
				sei();                                 // Enable interrupt
			}

			last_clk_state = current_clk_state;      // Remember clockstate for next cycle

			SET_OUTPUT(RPI_HANDSHAKE);               // Set handshake output to high -> Able to receive data
		}
	}

} // main()
// ============================================================================


