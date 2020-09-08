import java.time.LocalDateTime;

public class CANMessage {
    // header
    public String priority;
    public String sourceID;
    public String autonomous;
    public String messageType;
    public int extraID;

    // contents
    public int dataLength;
    public int[] data;

    public LocalDateTime timeStamp;

    public CANMessage(LocalDateTime timeStamp, String priority, String sourceID, String autonomous, String messageType, int extraID, int dataLength, int[] data) {
        this.timeStamp = timeStamp;
        this.priority = priority;
        this.sourceID = sourceID;
        this.autonomous = autonomous;
        this.messageType = messageType;
        this.extraID = extraID;
        this.dataLength = dataLength;
        this.data = data;
    }

}
