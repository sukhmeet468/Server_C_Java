/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frameworkclientconnection;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author ferens
 */
public class FrameworkClientConnection implements Runnable {

    InputStream input;
    OutputStream output;
    ServerSocket serverSocket = null;
    Socket clientSocket = null;
    frameworkclientmessageinterface.FrameworkClientMessageInterface 
            myClientMessageHandler;
    frameworkserver.FrameworkServer myServer;
    boolean stopThisThread = false;

    public FrameworkClientConnection(Socket clientSocket, 
            frameworkclientmessageinterface.FrameworkClientMessageInterface 
                    myClientMessageHandler, 
            frameworkserver.FrameworkServer myServer) {
        this.clientSocket = clientSocket;
        this.myClientMessageHandler = myClientMessageHandler;
        this.myServer = myServer;
        try {
            input = clientSocket.getInputStream();
            output = clientSocket.getOutputStream();
        } catch (IOException ex) {
            Logger.getLogger(FrameworkClientConnection.class.getName()).log(Level.SEVERE, null, ex);
            myServer.sendMessageToUI("Cannot create IO streams; ; disconnecting client.");
            disconnectClient();
        }
    }

    @Override
    public void run() {
        byte msg;
        String theClientMessage = "";
        while (stopThisThread == false) {
            try {
                msg = (byte) input.read();
                theClientMessage = Character.toString((char)msg);
                myClientMessageHandler.handleClientMessage(this, 
                        theClientMessage);
            } catch (IOException e) {
                myClientMessageHandler.handleClientMessage("IOException: "
                        + e.toString()
                        + ". Stopping thread and disconnecting client: "
                        + clientSocket.getRemoteSocketAddress());
                disconnectClient();
                stopThisThread = true;
            }
        }
    }

    public void sendMessageToClient(byte msg) {
        try {
            output.write(msg);
            output.flush();
        } catch (IOException e) {
            myServer.sendMessageToUI("cannot send to socket; disconnecting client.");
            disconnectClient();
        } finally {
        }
    }

    public void sendStringMessageToClient(String theMessage) {
        for (int i = 0; i < theMessage.length(); i++) {
            byte msg = (byte) theMessage.charAt(i);
            sendMessageToClient(msg);
        }
    }

    public void clientQuit() {
        disconnectClient();
    }

    public void clientDisconnect() {
        disconnectClient();
    }

    public void disconnectClient() {
        try {
            String clientIPAddress = clientSocket.getRemoteSocketAddress().toString();
            stopThisThread = true;
            clientSocket.close();
            clientSocket = null;
            input = null;
            output = null;
            myServer.clientDisconnected(clientIPAddress);
        } catch (IOException e) {
            myServer.sendMessageToUI("cannot close client socket; continuing program.");
            //System.exit(0);
        } finally {
        }
    }

    public Socket getClientSocket() {
        return clientSocket;
    }
     
}
