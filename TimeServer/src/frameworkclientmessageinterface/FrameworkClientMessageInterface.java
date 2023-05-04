package frameworkclientmessageinterface;

/**
 *
 * @author ferens
 */
public interface FrameworkClientMessageInterface {
    public abstract void handleClientMessage(
        frameworkclientconnection.FrameworkClientConnection 
            myClientConnection, String msg);
    public abstract void handleClientMessage(
            String theExceptionalEvent);
    
}
