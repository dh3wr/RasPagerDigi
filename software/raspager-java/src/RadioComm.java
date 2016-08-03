package rasPager;

import java.util.ArrayList;

import com.sun.jna.Library;
import com.sun.jna.Native;

public class RadioComm {
	
	private Log log = null;
	private RadioLink radioLink = null;
	private com.sun.jna.Pointer radioLinkPointer = null;

    public interface RadioLink extends Library {
        public com.sun.jna.Pointer setup();
        public boolean pttOn(com.sun.jna.Pointer radioLinkPointer);
        public void pttOff(com.sun.jna.Pointer radioLinkPointer);
        public void sendByte(int data);
    }

	// write message into log file (log level normal)
	private void log(String message, int type) {
		log(message, type, Log.DEBUG_SENDING);
	}

	// write message with given log level into log file
	private void log(String message, int type, int level) {
		// is there a log file?
		if(log != null) {
			// write message with given log level into log file
			log.println(message, type, level);
		}
	}
	
	public RadioComm(Log log)  throws FailedRadioSetupException {
		// set current settings
		this.log = log;
        this.radioLink = (RadioLink) Native.loadLibrary("radiolink", RadioLink.class);
		
		if ((this.radioLinkPointer = this.radioLink.setup()) == null) {
			throw new FailedRadioSetupException("Radio Setup failed!");
		} else {
			log("Radio Setup done.", Log.INFO);
		}
	}
	
	// set pin on
	public void setOn() {
		if(!this.radioLink.pttOn(this.radioLinkPointer)){
			log("Could not power up radio!", Log.ERROR);
		} else {
			log("Powered up radio!", Log.INFO);
		}
	}

	// set pin off
	public void setOff() {
		this.radioLink.pttOff(this.radioLinkPointer);
		log("Radio off!", Log.INFO);
	}
	
	public void sendByte(final ArrayList<Integer> data) {
		new Thread(new Runnable() {
		    public void run() {
				for(int i=0;i<data.size();i++){
					radioLink.sendByte(data.get(i));
				}
		    }
		}).start();
	}
}
