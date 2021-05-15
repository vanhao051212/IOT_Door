#include "EthernetPostGet.h"

void EthernetClientClass::MacInitial(uint8_t mac[]) {
  while (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
  }
};

void EthernetClientClass::EthernetConnectServer(char* host, int port) {
  if (client.connect(host, port)) {
    Serial.print(F("Host: "));
    Serial.print(host);
    Serial.print(F("  ---  Port: "));
    Serial.println(port);
  } else {
    Serial.println(F("Connection to server failed"));
  }
};
String EthernetClientClass::GetDataFromServer(char* host, int port) {
  client.println("GET / HTTP/1.1");
  client.print("Host: ");
  client.print(host);
  client.print(":");
  client.println(port);
  client.println();

  String payload = "";
  if (client.connected()) {
    while (!client.available());
    while (client.available()) {
      char c = client.read();
      payload += c;
    }
  }
  return payload;
};

String EthernetClientClass::PostDataFromArduino(char* host, int port, String data) {
  if (client.connect(host, port)) {
    client.println("POST /receivedCmd HTTP/1.1");
    client.println("Host: ");
    client.print(host);
    client.print(":");
    client.println(port);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    client.flush();

    String payload = "";
    // ham skipHeader va readClientLine: lay do dai cua chuoi gui ve - bo qua chuoi header
    skipHeader(client);
    while (client.connected())
    {
      char* response = readClientLine(client);
      payload = response;
    }
    return payload;
  }
  return "";
};

bool EthernetClientClass::Connected() {
  return client.connected();
};
void EthernetClientClass::Disconnect() {
  client.stop();
};

void EthernetClientClass::skipHeader(EthernetClient & client1)
{
  boolean currentLineIsEmpty = true;
  while (client1.connected())
  {
    if (client1.available())
    {
      char c = client1.read();
      if (c == '\n' && currentLineIsEmpty) break;
      else if (c == '\n') currentLineIsEmpty = true;
      else if (c >= ' ') currentLineIsEmpty = false;
    }
  }
}

char* EthernetClientClass::readClientLine(EthernetClient & client1)
{
  int charcount = 0;
  memset(linebuf, 0, sizeof(linebuf));
  while (client1.connected())
  {
    if (client1.available())
    {
      char c = client1.read();
      if (c >= ' ' && charcount < sizeof(linebuf) - 1)
      {
        linebuf[charcount] = c;
        charcount++;
      }
      else if (c == '\n') break;
    }
  }
  return linebuf;
}
