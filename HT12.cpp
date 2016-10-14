/*
 * This is a simple Arduino library for providing easy access to send and
 * receive data using HT12A/E and HT12D ICs by Sebastian Blunt.
 *
 * For usage information please see the README.md file.
 */

#include "HT12.h"

#include <Arduino.h>
#include <string.h>

#define INIT_VALUE1 10
#define INIT_VALUE2 122

int ht12_pins[4];
int ht12_unit_interval;
int ht12_vte_pin;

/*
 * Defines the GPIO pins to be used and sets their pinMode.
 * @ht12_pins: the 4 GPIO pins connected to the HT12 data pins
 * @unit_interval: unit interval (in milliseconds) between each 4-bit signal
 * @mode: receive or send mode (receive is 0, send is 1)
 * @vte_pin: the pin number connected to TE (HT12A/E) or VT (HT12D)
 */
void ht12_begin(int pins[4], int unit_interval, char mode, int vte_pin)
{
	int i;
	for (i = 0; i < 4; i++) {
		ht12_pins[i] = pins[i];
		pinMode(ht12_pins[i], mode);
	}

	ht12_vte_pin = vte_pin;
	ht12_unit_interval = unit_interval;

	pinMode(ht12_vte_pin, mode);
}

/*
 * Send a single byte raw.
 *
 * start is used for keeping track of when to send/receive the next message
 */
void send_byte(char msg, unsigned long int *start)
{
	int i;
	for (i = 0; i < 4; i++) {
		digitalWrite(ht12_pins[i], (msg & 1) ? HIGH : LOW);
		msg >>= 1;
	}

	*start += ht12_unit_interval;
	if (millis() < *start)
		delay(*start - millis());

	for (i = 0; i < 4; i++) {
		digitalWrite(ht12_pins[i], (msg & 1) ? HIGH : LOW);
		msg >>= 1;
	}

	*start += ht12_unit_interval;
	if (millis() < *start)
		delay(*start - millis());

}

/*
 * Send an array of bytes
 *
 * This function automatically handles sending the header
 * len is the length of the message
 */
void ht12_send(char *msg, int len)
{
	digitalWrite(ht12_vte_pin, LOW);

	unsigned long int start = millis();

	send_byte(INIT_VALUE1, &start);
	send_byte(INIT_VALUE2, &start);

	send_byte(len, &start);

	int i;
	for (i = 0; i < len; i++)
		send_byte(msg[i], &start);

	digitalWrite(ht12_vte_pin, HIGH);
}

/*
 * Reads a single byte raw
 */
char read_byte(unsigned long int *start)
{
	char byte = 0;

	if (millis() < *start)
		delay(*start - millis());

	byte += digitalRead(ht12_pins[0]);
	byte += digitalRead(ht12_pins[1]) * 2;
	byte += digitalRead(ht12_pins[2]) * 4;
	byte += digitalRead(ht12_pins[3]) * 8;

	*start += ht12_unit_interval;
	if (millis() < *start)
		delay(*start - millis());

	byte += digitalRead(ht12_pins[0]) * 16;
	byte += digitalRead(ht12_pins[1]) * 32;
	byte += digitalRead(ht12_pins[2]) * 64;
	byte += digitalRead(ht12_pins[3]) * 128;

	*start += ht12_unit_interval;

	return byte;
}

/*
 * Called to listen for a single incoming message
 *
 * This function will block until it has received a message
 *
 * It returns a pointer to the null-terminated char array that it has read
 *
 * It waits until the VT pin has gone high, and it's received two bytes that
 * match the INIT_VALUEs. It then reads the length header, and then it tries
 * to read that amount of bytes.
 *
 * It's a known bug that if it misreads the length header, it'll block for a
 * long time trying to read a message that isn't there. This mostly happens if
 * there's a poor signal and/or the unit interval is too low
 */
char* ht12_recv()
{
	unsigned long int start;

	int one = 0;
	while (1) {
		/* wait until VT pin is high */
		while (digitalRead(ht12_vte_pin) == 0) {
			delay(ht12_unit_interval / 5);
		}

		start = millis();

		if (read_byte(&start) == INIT_VALUE1) {
			if (read_byte(&start) == INIT_VALUE2) {
				break;
			}
		}

	}

	int len = read_byte(&start);

	char *msg = (char*) malloc(len + 1);

	int i;
	for (i = 0; i < len; i++) {
		msg[i] = read_byte(&start);
	}

	/* null-terminate */
	msg[i] = 0;

	return msg;

}

