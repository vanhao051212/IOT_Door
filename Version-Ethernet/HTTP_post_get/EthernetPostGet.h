#include <UIPEthernet.h>
#include <Ethernet.h>
class EthernetClientClass {
  public:
    EthernetClient client;
    char linebuf[81];
  public:
    void MacInitial(uint8_t mac[]);
    void EthernetConnectServer(char* host, int port);
    String GetDataFromServer(char* host, int port);
    String PostDataFromArduino(char* host, int port, String data);
    bool Connected();
    void Disconnect();
    void skipHeader(EthernetClient &client1);
    char* readClientLine(EthernetClient &client1);
};
