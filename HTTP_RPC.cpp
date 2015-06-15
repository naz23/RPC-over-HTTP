/*
Copyright (c) 2010 ARM Ltd
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "mbed.h"
#include "EthernetInterface.h"
#include "HTTPServer.h"
#include "mbed_rpc.h"

RpcDigitalOut led2(LED2,"led2");
RpcDigitalOut led3(LED3,"led3");
RpcDigitalOut led4(LED4, "led4");


EthernetInterface eth;  
HTTPServer svr;

int main() {
  //Turn the LEDs off
  led2.write(1);
  led3.write(1);
  led4.write(1);
  
  RPC::add_rpc_class<RpcDigitalOut>();

  printf("Setting up...\n");
  eth.init();
  int ethErr = eth.connect();
  if(ethErr < 0)
  {
    printf("Error %d in setup.\n", ethErr);
    return -1;
  }
  
  svr.addHandler<HTTPRpcRequestHandler>("/rpc");
  
  //attach server to port 80
  svr.start(80, &eth);
  
  printf("Listening...\n");
    
  Timer tm;
  tm.start();
  //Listen indefinitely
  while(true)
  {
    svr.poll();
    if(tm.read()>.5)
    {
      tm.start();
    }
  }

}
