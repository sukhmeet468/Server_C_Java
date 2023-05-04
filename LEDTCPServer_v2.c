#define __LEDTCPSERVER_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_LED_TCP_SERVER_EXAMPLE)

#include "TCPIP Stack/TCPIP.h"


// Defines which port the server will listen on
#define SERVER_PORT	4444

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
void LEDTCPServer(void) {
    WORD isReady;
    BYTE keyboardCommand;
    char menuString[24];
    unsigned int size = 0;
    static TCP_SOCKET MySocket;

    static enum _TCPServerState {
        SM_OPEN_SERVER_SOCKET = 0,
        SM_DISPLAY_MENU,
        SM_PROCESS,
        SM_CLOSE_SOCKET
    } TCPServerState = SM_OPEN_SERVER_SOCKET;

    switch (TCPServerState) {
        case SM_OPEN_SERVER_SOCKET: // Allocate a socket for this server to listen and accept connections on
            MySocket = TCPOpen(0, TCP_OPEN_SERVER, SERVER_PORT, TCP_PURPOSE_LED_SERVER);
            if (MySocket == INVALID_SOCKET) return;
            TCPServerState = SM_DISPLAY_MENU;
            break;

        case SM_DISPLAY_MENU: // Display the menu
            if (!TCPIsConnected(MySocket)) return;
            else {
                size = sizeof ("\x1b[2J \x1b[37m \x1b[1m") + \
						sizeof ("   LED SERVER MENU\n\r") + \
						sizeof ("1: LED0 ON\t2: LED0 OFF\n\r") + \
						sizeof ("3: LED1 ON\t4: LED1 OFF\n\r") + \
						sizeof ("5: LED2 ON\t6: LED2 OFF\n\r") + \
						sizeof ("7: LED3 ON\t8: LED3 OFF\n\r") + \
						sizeof ("a: PB0\ts: PB1\td: PB2\t\n\r\n\x1b[31m");
                if ((TCPIsPutReady(MySocket)) < size)
                    return;
                else {
                    TCPPutArray(MySocket, (BYTE*) "\x1b[2J \x1b[37m \x1b[1m", sizeof ("\x1b[2J \x1b[37m \x1b[1m"));
                    TCPPutArray(MySocket, (BYTE*) "   LED SERVER MENU\n\r", sizeof ("   LED SERVER MENU\n\r"));
                    TCPPutArray(MySocket, (BYTE*) "1: LED0 ON\t2: LED0 OFF\n\r", sizeof ("1: LED0 ON\t2: LED0 OFF\n\r"));
                    TCPPutArray(MySocket, (BYTE*) "3: LED1 ON\t4: LED1 OFF\n\r", sizeof ("3: LED1 ON\t4: LED1 OFF\n\r"));
                    TCPPutArray(MySocket, (BYTE*) "5: LED2 ON\t6: LED2 OFF\n\r", sizeof ("5: LED2 ON\t6: LED2 OFF\n\r"));
                    TCPPutArray(MySocket, (BYTE*) "7: LED3 ON\t8: LED3 OFF\n\r", sizeof ("7: LED3 ON\t8: LED3 OFF\n\r"));
                    TCPPutArray(MySocket, (BYTE*) "a: PB0\ts: PB1\td: PB2\t\n\r\n\x1b[31m", sizeof ("a: PB0\ts: PB1\td: PB2\t\n\r\n\x1b[31m"));
                    TCPFlush(MySocket);
                    TCPServerState = SM_PROCESS;
                }
            }
            break;

        case SM_PROCESS: // Display the menu
            if (!TCPIsConnected(MySocket)) return;
            isReady = TCPIsGetReady(MySocket);
            if (isReady == 0)
                return;
            TCPGet(MySocket, & keyboardCommand);
            if (keyboardCommand <= '8' && keyboardCommand >= '1') {
                if (keyboardCommand == '1') {
                    LED0_IO = 1;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED0  on");
                }
                else if (keyboardCommand == '2') {
                    LED0_IO = 0;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED0 off");
                }
                else if (keyboardCommand == '3') {
                    LED1_IO = 1;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED1  on");
                }
                else if (keyboardCommand == '4') {
                    LED1_IO = 0;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED1 off");
                }
                else if (keyboardCommand == '5') {
                    LED2_IO = 1;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED2  on");
                }
                else if (keyboardCommand == '6') {
                    LED2_IO = 0;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED2 off");
                }
                else if (keyboardCommand == '7') {
                    LED3_IO = 1;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED3  on");
                }
                else if (keyboardCommand == '8') {
                    LED3_IO = 0;
                    strcpy(menuString, (const char *) "\x1b[2K\rLED3 off");
                }
                while ((TCPIsPutReady(MySocket)) < sizeof ("\x1b[2K\rLED3 off"));
                TCPPutArray(MySocket, (BYTE*) menuString, sizeof ("\x1b[2K\rLED3 off"));
                TCPFlush(MySocket);
            }
            else if (keyboardCommand == 'a') {
                if (BUTTON0_IO == BUTTON_DOWN) {
                    while ((TCPIsPutReady(MySocket)) < sizeof ("\x1b\r[2K\rButton 0 is Down"));
                    TCPPutArray(MySocket, (BYTE*) "\x1b[2K\rButton 0 is Down", sizeof ("\x1b[2K\rButton 0 is Down"));
                    TCPFlush(MySocket);
                }
                else {
                    while ((TCPIsPutReady(MySocket)) < sizeof ("\x1b[2K\rButton 0 is Up"));
                    TCPPutArray(MySocket, (BYTE*) "\x1b[2K\rButton 0 is Up", sizeof ("\x1b[2K\rButton 0 is Up"));
                    TCPFlush(MySocket);
                }
                return;
            }
            else if (keyboardCommand == 's') {
                if (BUTTON1_IO == BUTTON_DOWN) {
                    while ((TCPIsPutReady(MySocket)) < sizeof ("\x1b[2K\rButton 1 is Down"));
                    TCPPutArray(MySocket, (BYTE*) "\x1b[2K\rButton 1 is Down", sizeof ("\x1b[2K\rButton 1 is Down"));
                    TCPFlush(MySocket);
                }
                else {
                    while ((TCPIsPutReady(MySocket)) < sizeof ("\x1b[2K\rButton 1 is Up"));
                    TCPPutArray(MySocket, (BYTE*) "\x1b[2K\rButton 1 is Up", sizeof ("\x1b[2K\rButton 1 is Up"));
                    TCPFlush(MySocket);
                }
                return;
            }
            else if (keyboardCommand == 'd') {
                if (BUTTON2_IO == BUTTON_DOWN) {
                    while ((TCPIsPutReady(MySocket)) < sizeof ("\x1b[2K\rButton 2 is Down"));
                    TCPPutArray(MySocket, (BYTE*) "\x1b[2K\rButton 2 is Down", sizeof ("\x1b[2K\rButton 2 is Down"));
                    TCPFlush(MySocket);
                }
                else {
                    while ((TCPIsPutReady(MySocket)) < sizeof ("\x1b[2K\rButton 2 is Up\r"));
                    TCPPutArray(MySocket, (BYTE*) "\x1b[2K\rButton 2 is Up", sizeof ("\x1b[2K\rButton 2 is Up"));
                    TCPFlush(MySocket);
                }
                return;
            }
            else if (keyboardCommand == 0x1B) {//when escape key is presses 1B is ASCII char for Escape
                TCPServerState = SM_CLOSE_SOCKET;
                return;
            }
            else if (keyboardCommand == 'q') {//this is the code modification done to exit when q is pressed 
                TCPServerState = SM_CLOSE_SOCKET;
                return;
            }
            break;

        case SM_CLOSE_SOCKET:
            TCPClose(MySocket);
            TCPServerState = SM_OPEN_SERVER_SOCKET;
            break;
    }
}

#endif //#if defined(STACK_USE_GENERIC_TCP_SERVER_EXAMPLE)
