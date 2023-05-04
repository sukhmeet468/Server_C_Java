/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package timeserverusercommandhandler;

/**
 *
 * @author ferens
 */
public class TimeServerUserCommandHandler implements Runnable{
    frameworkuserinterface.FrameworkUserInterface myUI;
    frameworkserver.FrameworkServer myServer;
    String theCommand = "";

    public TimeServerUserCommandHandler(
            frameworkuserinterface.FrameworkUserInterface myUI, 
            frameworkserver.FrameworkServer myServer) {
        this.myUI = myUI;
        this.myServer = myServer;
    }

    public void handleUserCommand(String theCommand) {
        this.theCommand = theCommand;
        Thread myCommandThread = new Thread(this);
        myCommandThread.start();
    }
    
    public void run() {
        int switchCase = Integer.parseInt(theCommand.split(" ")[0]);
        switch (switchCase) {
            case 1: //QUIT
                myServer.stopServer();
                myUI.update("Quiting program by User command.");
                System.exit(0);
                break;
            case 2: //LISTEN
                myServer.listen();
                break;
            case 3: //SET PORT
                String portString = theCommand.split(" ")[1];
                int portNumber = Integer.parseInt(portString);
                myServer.setPort(portNumber);
                break;
            case 4: //GET PORT
                myUI.update("The port number is: " +String.valueOf(myServer.getPort()));
                break;
            case 5: //Stop Listening
                myUI.update("Stop listening command has not been implemented yet.\n");
                //myServer.stopListening();
                break;
            case 6: //START SERVER SOCKET
                myServer.startServer();
                break;
            default:
                break;
        }
    }
}
