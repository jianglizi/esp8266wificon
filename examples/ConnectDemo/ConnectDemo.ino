#include <wificon.h>

wificon wfc;

void setup() { 
  
  wfc.wifiBegin();

}

void loop() { 
  
  wfc.wifiLoop();

}
