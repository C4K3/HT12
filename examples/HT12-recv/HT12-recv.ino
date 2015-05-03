#include <HT12.h>


void setup()
{
  Serial.begin(9600);
  
  Serial.println("Starting HT12-recv example");

  int pins[] = {4, 5, 6, 7};
  int unit_interval = 100;
  char mode = INPUT;
  int vt_pin = 8;
  
  ht12_begin(pins, unit_interval, mode, vt_pin);
}

void loop()
{
  Serial.println("Waiting for message");
  
  char *message = ht12_recv();
  Serial.println(message);
  free(message);
}
