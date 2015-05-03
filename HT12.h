#ifndef _HT12_h
#define _HT12_h

void ht12_begin(int ht12_pins[], int delay, char mode, int vte_pin);

void ht12_send(char msg[], int len);

char* ht12_recv();

#endif
