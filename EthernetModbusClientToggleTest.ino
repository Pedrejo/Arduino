/*
  Ethernet Modbus TCP Client Toggle

  This sketch toggles the coil of a Modbus TCP server connected
  on and off every second.

  Circuit:
   - Any Arduino MKR Board
   - MKR ETH Shield

  created 16 July 2018
  by Sandeep Mistry
*/
#include <P1AM.h>
#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
// The IP address will be dependent on your local network:
byte mac[] = {
  0x60, 0x52, 0xD0, 0x06, 0xEF, 0xE5
};
IPAddress ip(192, 168, 1, 120);

EthernetClient ethClient;
ModbusTCPClient modbusTCPClient(ethClient);

IPAddress server(192, 168, 1, 121); // update with the IP Address of your Modbus server

void setup() {
  Ethernet.begin(mac, ip);
  modbusTCPClient.setTimeout(500);//Adjust Response Timeout from 30 seconds to 500 ms.
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {};// wait for serial port to connect.
  Serial.println("Modbus TCP Client Simple Example");
  Serial.print("P1AM-ETH at IP:");
  Serial.println(ip);

  // start the Ethernet connection and the server:
  


}

void loop() {
  if (!modbusTCPClient.connected()) {
    // client not connected, start the Modbus TCP client
    Serial.println("Attempting to connect to Modbus TCP server");
    
    if (!modbusTCPClient.begin(server, 502)) {
      Serial.println("Modbus TCP Client failed to connect!");
    } else {
      Serial.println("Modbus TCP Client connected");
    }
  } else {
    // client connected

    // write the value of 0x01, to the coil at address 0x00
    if (!modbusTCPClient.coilWrite(0x00, 0x01)) {
      Serial.print("Failed to write coil! ");
      Serial.println(modbusTCPClient.lastError());
    }

    // wait for 1 second
    delay(10000);

    // write the value of 0x00, to the coil at address 0x00
    if (!modbusTCPClient.coilWrite(0x00, 0x00)) {
      Serial.print("Failed to write coil! ");
      Serial.println(modbusTCPClient.lastError());
    }

    // wait for 1 second
    delay(10000);
  }
}
