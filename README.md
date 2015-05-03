HT12 Arduino library
=====

This is a simple library for sending and receiving data over a HT12A/E and D pair.

Installation
=====

Download the library and put it into your Arduino libraries directory.

To install it with git, change to your libraries directory and run:

```
git clone --depth=1 https://github.com/C4K3/HT12
```

Usage
=====

To use it, include HT12.h

```
#include <HT12.h>
```
Then in your setup, call ht12_begin as

```
ht12_begin(int pins[4], int unit_interval, char mode, int vt_te_pin);
```
Example sender:

```
int pins[] = {4, 5, 6, 7};
int unit_interval = 100;
char mode = OUTPUT;
int te_pin = 8;

ht12_begin(pins, unit_interval, mode, te_pin);
```
pins defines the pins used for the data transfer.
unit_interval is the delay between each signal in milliseconds. So for example, a value of 100 would mean 4 bits are sent every 100 milliseconds, giving a data transfer rate of 5 bytes/second, ignoring headers. The lower the setting the faster data transfer obviously, but also less table. 100 has worked for me over a very unstable wireless signal, but you can experiment to find the value that works best for you.
mode is whether this is unit is sending or receiving (HT12s are one-way only.)
vt_te_pin is the TE pin (if sending) or VT pin (if receiving.)

You should then connect the TE pin on the HT12 to pin 8, and the data pins to 4, 5, 6, 7 in the order defined in the pins variable (ie. pin 4 should go to data 0.)

For receivers, the same is true except connect the vt_te_pin to the VT pin on the HT12.

To send, call
```
ht12_send(char message[], int length);
```

On the receiver, call ht12_recv. This will however block until it receives a message.

```
char *message = ht12_recv();
```
Important to note, is that the message must be freed after use. For example:

```
char *message = ht12_recv();
Serial.println(message);
free(message);
```

Sending other data types
-----
It is possible to also send other data types (i.e. integers) using memcpy. For an example on how to do this, read the int-send and int-recv examples. This works the same for other data types.
