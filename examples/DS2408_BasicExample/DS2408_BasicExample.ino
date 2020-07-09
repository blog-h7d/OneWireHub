#include "OneWireHub.h"
#include "DS2408.h"

#define NUMBER_OF_DS2408_PINS   8

constexpr uint8_t pin_onewire   { A0 };

auto hub = OneWireHub(pin_onewire);
// 1-Wire Slaves mit Adressen definieren:
auto ds2408_1 = DS2408( DS2408::family_code, 0x00, 0x00, 0x20, 0x24, 0xda, 0x00);
auto ds2408_2 = DS2408( DS2408::family_code, 0x00, 0x00, 0x21, 0x24, 0xda, 0x00);

void setup() 
{
// DS2408 Ausgänge initialisieren
  for (int i = 0; i < NUMBER_OF_DS2408_PINS; i++)
  {
      ds2408_1.setPinState(i, false);
      ds2408_2.setPinState(i, false);
  }
          
  hub.attach(ds2408_1);
  hub.attach(ds2408_2);
}

void loop() 
{
  // following function must be called periodically
  hub.poll();
  // this part is just for debugging (USE_SERIAL_DEBUG in OneWire.h must be enabled for output)
  if (hub.hasError()) hub.printError();
}
