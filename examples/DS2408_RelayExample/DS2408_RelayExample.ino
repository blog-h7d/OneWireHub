#include "OneWireHub.h"
#include "DS2408.h"
#include "Relay.h"

#define NUMBER_OF_DS2408_PINS   8
#define NUMBER_OF_RELAYS        16

// Relais Objekte anlegen und Pin zuweisen
Relay relays[NUMBER_OF_RELAYS] = {A5, A4, A3, A2, 12, 11, 10, 9, A1, A6, 3, 4, 5, 6, 7, 8};

constexpr uint8_t pin_onewire   { A0 };

auto hub = OneWireHub(pin_onewire);
// 1-Wire Slaves mit Adressen definieren:
auto ds2408_1 = DS2408( DS2408::family_code, 0x00, 0x00, 0x20, 0x24, 0xda, 0x00);
auto ds2408_2 = DS2408( DS2408::family_code, 0x00, 0x00, 0x21, 0x24, 0xda, 0x00);

void setup() 
{
  for (int i = 0; i < NUMBER_OF_RELAYS; i++)
    relays[i].init();
  
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
    hub.poll();
    uint8_t u8Ds1LastReceivedCmd = ds2408_1.getLastReceivedCmd();
    uint8_t u8Ds2LastReceivedCmd = ds2408_2.getLastReceivedCmd();
  
    if (u8Ds1LastReceivedCmd == 0x5A || u8Ds2LastReceivedCmd == 0x5A)
    {
        uint8_t u8Ds1NewPinState = ds2408_1.getPinState(); 
        uint8_t u8Ds2NewPinState = ds2408_2.getPinState(); 
        uint8_t u8PinBit = 1;

        for (int i = 0; i < NUMBER_OF_DS2408_PINS; i++)
        { 
             if (u8Ds1NewPinState & u8PinBit) 
                   relays[i].setRelay(true);  
             else
                   relays[i].setRelay(false);  

             if (u8Ds2NewPinState & u8PinBit) 
                   relays[8+i].setRelay(true);  
             else
                   relays[8+i].setRelay(false);  

             u8PinBit <<= 1;
        }
    }
}
