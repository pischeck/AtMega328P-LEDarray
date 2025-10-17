/*
 * AtMega328P-LEDarray.c
 *
 * Created: 17.10.2025 12:26:39
 * Author : piszc
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = 0xFF;
	uint8_t licznik = 0x03;
    
	while (1) 
    {
		PORTD = licznik;
		_delay_ms(1000);
		licznik = licznik << 1;
		if(licznik == 0x80) licznik = 0x03;
    }
}

