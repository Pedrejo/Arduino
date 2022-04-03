#include <SPI.h>
#include <Ethernet.h>
#include <P1AM.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

byte mac[] = {
  0x60, 0x52, 0xD0, 0x06, 0xEF, 0xE5
};
IPAddress ip(192, 168, 1, 120);
EthernetClient ethClient;
ModbusTCPClient modbusTCPClient(ethClient);
IPAddress server(192, 168, 1, 121);

int simInput = 0;
int pinState = 0;
int lastPinState = 0;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  while (!P1.init()){
    ;
  }
}

void loop() {

  pinState = P1.readDiscrete(2,1);
  if (pinState != lastPinState) {
    modbusTCPClient.begin(server, 502);
    
    if (pinState) {
      relaysOn();
      modbusTCPClient.coilWrite(0x00, 0x01);
      modbusTCPClient.stop();
     // relaysOn();
    } else {
      relaysOff();
      modbusTCPClient.coilWrite(0x00, 0x00);
      modbusTCPClient.stop();
    //  relaysOff();
    }   
  }

  lastPinState = pinState;

}

void relaysOn() {
      
  P1.writeDiscrete(0x01,1);
  delay(80);
  P1.writeDiscrete(0x04,1);
  delay(80);
  P1.writeDiscrete(0x10,1);
  delay(80);
  P1.writeDiscrete(0x40,1);
  delay(80);
  P1.writeDiscrete(0x00,1);
   
}

void relaysOff() {

  P1.writeDiscrete(0x02,1);
  delay(80);
  P1.writeDiscrete(0x08,1);
  delay(80);
  P1.writeDiscrete(0x20,1);
  delay(80);
  P1.writeDiscrete(0x80,1);
  delay(80);
  P1.writeDiscrete(0x00,1);

}
