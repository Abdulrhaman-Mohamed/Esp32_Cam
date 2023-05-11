#ifndef WebSocket_h
#define WebSocket_h


#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include "esp_websocket_client.h"
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>

class WebSocket
{
private:
    esp_websocket_client_handle_t client;
    // this part is for First Requests that indicate the GS try to connect to Rover
    static WebServer *server;
    static String ReceivedURL;
    static void HandleData();

    static void EventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    void AcceptText();
    void ExecuteCommand(char *Body);

public:
    WebSocket(const char *url);
    ~WebSocket();
    // used in online session
    void SendBinary(const char *data, size_t len, size_t chunkSize);
    

    static WebServer *InitWebSocketConnectListner();
    static bool IsConnected();
    static char *GetSocketsURL();
    // they are not implmented yet
    void Connect(const char *url);
    void Disconnect();
    void onMessage(void (*callback)(String message));
    void onOpen(void (*callback)());
    void onClose(void (*callback)());
    void onError(void (*callback)(String message));
};
#endif