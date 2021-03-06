#include <HT12.h>


void setup() {
  Serial.begin(9600);

  Serial.println("Starting HT12-send example");

  int pins[] = {4, 5, 6, 7};
  int unit_interval = 100; // in milliseconds
  char mode = OUTPUT;
  int te_pin = 8;
      
  ht12_begin(pins, unit_interval, mode, te_pin);
}

void loop()
{ 
  Serial.println("Sending messages");
  
  ht12_send("chocolate", strlen("chocolate"));
  ht12_send("cake", strlen("cake"));
  ht12_send("cupcakes", strlen("cupcakes"));
}
