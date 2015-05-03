#include <HT12.h>

void setup()
{
  Serial.begin(9600);
  
  Serial.println("Starting HT12 int-recv example");

  int pins[] = {4, 5, 6, 7};
  int unit_interval = 100;
  char mode = INPUT;
  int vt_pin = 8;
  
  ht12_begin(pins, unit_interval, mode, vt_pin);
}

void loop()
{
  Serial.println("Waiting for number");

  int number;

  char *message = ht12_recv();
  memcpy(&number, message, sizeof(int));
  Serial.print("The number is: ");
  Serial.println(number);

  free(message);
}
