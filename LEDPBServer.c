#define __LEDPBSERVER_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_LED_PB_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <ctype.h>

// Defines which port the server will listen on
#define SERVER_PORT	7777

/*****************************************************************************
  Function:
    void LEDTCPServer(void)

  Summary:
    Implements a simple LED TCP Server.

  Description:
    This function implements a simple TCP server.  The function is invoked
    periodically by the stack to listen for incoming connections.  When a 
    connection is made, the server reads all incoming data, processes the 
    LEDs.
	
    This example can be used as a model for many TCP server applications.

  Precondition:
    TCP is initialized.

  Parameters:
    None

  Returns:
    None
 ***************************************************************************/

void myCharArrayCopy(char * destination, char *source, int size){
    int i ;
    for(i=0;i<size;i++){
        destination[i] = source[i];
    }
}
void LEDPBServer(void) {
    static TCP_SOCKET MySocket;
    BYTE theChar = 0 ;
    static char command[7]; 
    BYTE myPB;
    static int commandIndex = 0 ;

    static enum _TCPServerState {
        SM_OPEN_SERVER_SOCKET = 0,
        SM_LISTEN_FOR_CLIENT_CONNECTION,
        SM_BUILD_MESSAGE_INCREMENTALLY,
        SM_PROCESS_COMMAND,
        SM_DISCONNECT_CLIENT
    } TCPServerState = SM_OPEN_SERVER_SOCKET;

    switch (TCPServerState) {
        case SM_OPEN_SERVER_SOCKET: // Allocate a socket for this server to listen and accept connections on
            MySocket = TCPOpen(0, TCP_OPEN_SERVER, SERVER_PORT, TCP_PURPOSE_LED_PB_SERVER);
            if (MySocket == INVALID_SOCKET) 
                return;
            TCPServerState = SM_LISTEN_FOR_CLIENT_CONNECTION;
            break;
            
        case SM_LISTEN_FOR_CLIENT_CONNECTION:
            if (TCPIsConnected(MySocket) == FALSE)
                return;
            else {
                TCPServerState = SM_BUILD_MESSAGE_INCREMENTALLY;
                break;
            }
        
        case SM_BUILD_MESSAGE_INCREMENTALLY :
            if(TCPIsConnected(MySocket)==FALSE){
                TCPServerState = SM_DISCONNECT_CLIENT;
                return;
            }
            if(TCPIsGetReady(MySocket)<1)
                return;
            TCPGet(MySocket , &theChar);
            if(theChar == (BYTE)0xFF){
                commandIndex = 0;
                TCPServerState = SM_PROCESS_COMMAND;
            }
            else{
                command[commandIndex] = theChar;
                commandIndex++;
            }
            break;
 
        case SM_PROCESS_COMMAND: // Display the menu
            if (TCPIsConnected(MySocket) == FALSE){
                TCPServerState = SM_DISCONNECT_CLIENT;
                return;
            } else if(command[0] == 'd'){
                TCPPutString(MySocket,(BYTE*)"Disconnect Ack:");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPServerState = SM_DISCONNECT_CLIENT;
                break;
            } else if(command[0] == 'q'){
                TCPPutString(MySocket,(BYTE*)"Quit Ack:");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='1' && command[2]=='o' && command[3]=='n'){
                LED0_IO = 1;
                TCPPutString(MySocket,(BYTE*)"L1on");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='1' && command[2]=='o' && command[3]=='f'){
                LED0_IO = 0;
                TCPPutString(MySocket,(BYTE*)"L1off");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='2' && command[2]=='o' && command[3]=='n'){
                LED1_IO = 1;
                TCPPutString(MySocket,(BYTE*)"L2on");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='2' && command[2]=='o' && command[3]=='f'){
                LED1_IO = 0;
                TCPPutString(MySocket,(BYTE*)"L2off");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='3' && command[2]=='o' && command[3]=='n'){
                LED2_IO = 1;
                TCPPutString(MySocket,(BYTE*)"L3on");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='3' && command[2]=='o' && command[3]=='f'){
                LED2_IO = 0;
                TCPPutString(MySocket,(BYTE*)"L3off");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='4' && command[2]=='o' && command[3]=='n'){
                LED3_IO = 1;
                TCPPutString(MySocket,(BYTE*)"L4on");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='L' && command[1]=='4' && command[2]=='o' && command[3]=='f'){
                LED3_IO = 0;
                TCPPutString(MySocket,(BYTE*)"L4off");
                TCPPut(MySocket,(BYTE)0xFF);
                TCPFlush(MySocket);
            } else if(command[0]=='g' && command[1]=='p' && command[2]=='b' && command[3]=='1'){
                myPB = BUTTON0_IO;
                if(myPB == BUTTON_DOWN) {
                    TCPPutString(MySocket,(BYTE*)"PB1Down");
                    TCPPut(MySocket,(BYTE)0xFF);
                    TCPFlush(MySocket);
                } else {
                    TCPPutString(MySocket,(BYTE*)"PB1Up");
                    TCPPut(MySocket,(BYTE)0xFF);
                    TCPFlush(MySocket);
                }
            } else if(command[0]=='g' && command[1]=='p' && command[2]=='b' && command[3]=='2'){
                myPB = BUTTON1_IO;
                if(myPB == BUTTON_DOWN) {
                    TCPPutString(MySocket,(BYTE*)"PB2Down");
                    TCPPut(MySocket,(BYTE)0xFF);
                    TCPFlush(MySocket);
                } else {
                    TCPPutString(MySocket,(BYTE*)"PB2Up");
                    TCPPut(MySocket,(BYTE)0xFF);
                    TCPFlush(MySocket);
                }
            } else if(command[0]=='g' && command[1]=='p' && command[2]=='b' && command[3]=='3'){
                myPB = BUTTON2_IO;
                if(myPB == BUTTON_DOWN) {
                    TCPPutString(MySocket,(BYTE*)"PB3Down");
                    TCPPut(MySocket,(BYTE)0xFF);
                    TCPFlush(MySocket);
                } else {
                    TCPPutString(MySocket,(BYTE*)"PB3Up");
                    TCPPut(MySocket,(BYTE)0xFF);
                    TCPFlush(MySocket);
                }
            } 
            TCPServerState = SM_BUILD_MESSAGE_INCREMENTALLY;
            break;
            
        case SM_DISCONNECT_CLIENT:
            TCPClose(MySocket);
            TCPServerState = SM_LISTEN_FOR_CLIENT_CONNECTION;
        break;
    }
}

#endif //#if defined(STACK_USE_GENERIC_TCP_SERVER_EXAMPLE)


