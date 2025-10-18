/*
 * AtMega328P-LEDarray.c
 *
 * Created: 17.10.2025 12:26:39
 * Author : piszc
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define SW_START		PC0
#define SW_STOP			PC1
#define SW_SPEED_UP		PC2
#define SW_SPEED_DOWN	PC3

volatile uint8_t licznik = 0x03;
volatile bool startstop = false;

// Pocz¹tkowa prêdkoœæ (w ms)
volatile uint16_t speed = 1000;
// Minimalny i maksymalny zakres prêdkoœci
#define SPEED_MIN 100
#define SPEED_MAX 4000
#define SPEED_STEP 100

bool debounce(uint8_t pin) {
	if (!(PINC & (1 << pin))) {
		_delay_ms(20);
		if (!(PINC & (1 << pin))) return true;
	}
	return false;
}

void moveLED(void) {
	PORTD = licznik;
	licznik <<= 1;
	if (licznik == 0x80) licznik = 0x03;
}

void wait(void) {
	for (uint16_t i = 0; i < speed / 10; i++) { // podzia³ czasu na 10ms kroki
		_delay_ms(10);
		if (debounce(SW_STOP)) startstop = false;
		if (debounce(SW_SPEED_UP)) {
			if (speed > SPEED_MIN + SPEED_STEP) speed -= SPEED_STEP;
		}
		if (debounce(SW_SPEED_DOWN)) {
			if (speed < SPEED_MAX - SPEED_STEP) speed += SPEED_STEP;
		}
	}
}

void init(void) {
	DDRD = 0xFF; // Port D jako wyjœcie (LEDy)
	PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3); // wewnêtrzne pull-upy
}

int main(void) {
	init();

	while (1) {
		if (startstop == true) {
			moveLED();
			wait();
		}
		if (debounce(SW_START)) startstop = true;
	}
}



