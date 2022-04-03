#include <SPI.h>
#include <Ethernet.h>
#include <P1AM.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

byte mac[] = {
  0x60, 0x52, 0xD0, 0x06, 0xEF, 0xD7
};
IPAddress ip(192, 168, 1, 121);
EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;
int coilValue = 0;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  ethServer.begin();
  modbusTCPServer.begin();
  while (!P1.init()){
    ;
  }
  modbusTCPServer.configureCoils(0x00, 1);
}

void loop() {

  EthernetClient client = ethServer.available();
  if (client) {
    // let the Modbus TCP accept the connection 
    modbusTCPServer.accept(client);

    while (client.connected()) {
      // poll for Modbus TCP requests, while client connected
      modbusTCPServer.poll();
      //delay(50);
      coilValue = modbusTCPServer.coilRead(0x00);
    }
    updateRelays();
  }
}

void updateRelays() {
  
  if (coilValue) {
      P1.writeDiscrete(0x01,1);
      delay(80);
      P1.writeDiscrete(0x04,1);
      delay(80);
      P1.writeDiscrete(0x10,1);
      delay(80);
      P1.writeDiscrete(0x40,1);
      delay(80);
      P1.writeDiscrete(0x00,1);

  } else {
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
}
