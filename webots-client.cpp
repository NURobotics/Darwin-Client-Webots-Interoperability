
//websocket
#include "easywsclient.hpp" //#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#include "easywsclient.cpp"
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>

//webots 
#include "Walk.hpp"
#include <cstdlib>
#include "Walk.cpp"

using namespace webots;

using easywsclient::WebSocket;
static WebSocket::pointer ws = NULL;

void handle_message(const std::string & message, const argc, const **argv)
{
  printf(">>> %s\n", message.c_str());
  if (message == "walk") 
  {
      Walk *controller = new Walk();
      controller->run();
      delete controller;
      return EXIT_FAILURE;
      }
  if (message == "world") { ws->close(); }
}

int main (int argc, char **argv)
{
#ifdef _WIN32
  INT rc; 
  WSADATA wsaData;

  rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (rc) {
    printf("WSAStartup Failed.\n");
    return 1;
  }   
#endif

  ws = WebSocket::from_url("ws://localhost:8126/foo");
  assert(ws);
  ws->send("command_walk recieved");
  ws->send("goodbye");
  ws->send("hello");
  while (ws->getReadyState() != WebSocket::CLOSED) {
    ws->poll();
    ws->dispatch(handle_message);
  }   
  delete ws; 
#ifdef _WIN32
  WSACleanup();
#endif
  return 0;
}
