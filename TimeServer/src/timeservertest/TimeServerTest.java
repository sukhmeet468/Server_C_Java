package timeservertest;

public class TimeServerTest {

    public static void main(String[] args) {

        final gui.GUI myUI = new gui.GUI();

        timeserver.TimeServer myServer = new timeserver.TimeServer(7777, 100, myUI);
        timeserverclientmessagehandler.ClientMessageHandler myClientMessageHandler
                = new timeserverclientmessagehandler.ClientMessageHandler(myServer);
        myServer.setClientMessageHandler(myClientMessageHandler);
        timeserverusercommandhandler.TimeServerUserCommandHandler myUserCommandHandler
                = new timeserverusercommandhandler.TimeServerUserCommandHandler(myUI, myServer);
        myUI.setUserCommandHandler(myUserCommandHandler);
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                myUI.setVisible(true);
            }
        });
    }

//    public static void main(String[] args) {
//
//        standardiouserinterface.StandardIO myUI = 
//                new standardiouserinterface.StandardIO();
//        timeserver.TimeServer myServer = new timeserver.TimeServer(7777, 100, myUI);
//        timeserverclientmessagehandler.ClientMessageHandler 
//                myClientMessageHandler = 
//                new timeserverclientmessagehandler.ClientMessageHandler(myServer);
//        myServer.setClientMessageHandler(myClientMessageHandler);
//        timeserverusercommandhandler.TimeServerUserCommandHandler 
//                myUserCommandHandler = 
//                new timeserverusercommandhandler.TimeServerUserCommandHandler
//                (myUI, myServer);
//        myUI.setUserCommandHandler(myUserCommandHandler);
//        Thread myUIthread = new Thread(myUI);
//        myUIthread.start();     
//        myUI.update("1:\tQuit\n2:\tlisten\n3:\tSet Port\n4:\tGet " + "Port\n5:\tStop listening\n6:\tStart Server Socket\n");
//    }
}
