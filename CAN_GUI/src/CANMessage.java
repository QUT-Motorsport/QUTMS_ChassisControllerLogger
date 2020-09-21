import java.time.LocalDateTime;

public class CANMessage {

    // header
    public int priority;
    public int sourceID;
    public int autonomous;
    public int messageType;
    public int extraID;

    // contents
    public int dataLength;
    public int[] data;

    public LocalDateTime timeStamp;

    // Transfer the string inputs to a corresponding integer to be send from the serial port
    public CANMessage(LocalDateTime timeStamp, String priority, String sourceID, String autonomous, String messageType, int extraID, int dataLength, int[] data) {
        this.timeStamp = timeStamp;

        // For priority
        if (priority == "Error") {
            this.priority = 0;
        }
        else if (priority == "Heartbeat")
        {
            this.priority = 1;
        }
        else if (priority == "Normal")
        {
            this.priority = 2;
        }

        // For sourceID (The first possible value was chosen)
        if (sourceID == "External")
        {
            this.sourceID = 0;
        }
        else if (sourceID == "Chassis Controller")
        {
            this.sourceID = 4;
        }
        else if (sourceID == "AMS")
        {
            this.sourceID = 16;
        }
        else if (sourceID == "BMS")
        {
            this.sourceID = 18;
        }
        else if (sourceID == "Shutdown")
        {
            this.sourceID = 6;
        }
        else if (sourceID == "Shutdown - BPSD")
        {
            this.sourceID = 10;
        }
        else if (sourceID == "Shutdown - Current")
        {
            this.sourceID = 12;
        }
        else if (sourceID == "Shutdown - IMD")
        {
            this.sourceID = 14;
        }
        else if (sourceID == "PDM")
        {
            this.sourceID = 20;
        }
        else if (sourceID == "Steering Wheel")
        {
            this.sourceID = 22;
        }
        else if (sourceID == "Charger")
        {
            this.sourceID = 24;
        }
        else if (sourceID == "Sensors")
        {
            this.sourceID = 32;
        }

        // For autonomous
        if (autonomous == "No")
        {
            this.autonomous = 0;
        }
        else if (autonomous == "Yes")
        {
            this.autonomous = 1;
        }

        // For messageType
        if (messageType == "Heartbeat")
        {
            this.messageType = 1;
        }
        else if (messageType == "Error Detected")
        {
            this.messageType = 0;
        }
        else if (messageType == "Data Receive")
        {
            this.messageType = 2;
        }
        else if (messageType == "Data Transmit")
        {
            this.messageType = 3;
        }

        // Already integer inputs
        this.extraID = extraID;
        this.dataLength = dataLength;
        this.data = data;
    }

}
