
import com.fazecast.jSerialComm.*;

import java.io.*;

public class CANDataTransmission {
    public interface IReceiveCANCallback{

        void ReceiveMessage();
    }

    public CANDataTransmission(IReceiveCANCallback receiveCANCallback) {
        this.receiveCANCallback = receiveCANCallback;

    }

    private IReceiveCANCallback receiveCANCallback;
    private SerialPort selectedSerialPort;
    private SerialPort[] serialPortsList;
    private byte[] readBuffer;
    private Integer BAUDSpeed;
    private long extID;
    private byte[] data;
    private boolean hasMessage = false;


    /**
     *
     * @return a list of description of the device that connects to this device via USB port
     */
    public String[] getPortDescriptionLists(){

        this.serialPortsList = SerialPort.getCommPorts();

        String[] results = new String[serialPortsList.length];

        int i = 0;

        while (i < serialPortsList.length){
            results[i] = serialPortsList[i].getDescriptivePortName();
            i++;
        }

        return results;
    }

    /**
     * setup the selected port.
     * @param selectedSerialPort
     * @param BAUDSpeed BAUDSpeed
     */
    public void setupSelectedPort(SerialPort selectedSerialPort, Integer BAUDSpeed){
        if (this.selectedSerialPort != null) {
            if (this.selectedSerialPort.isOpen()) {
                this.selectedSerialPort.closePort();
            }
        }


        this.selectedSerialPort = selectedSerialPort;
        this.BAUDSpeed = BAUDSpeed;

        selectedSerialPort.openPort();
        this.selectedSerialPort.setComPortParameters(this.BAUDSpeed,
                8, 1,
                SerialPort.NO_PARITY);

        selectedSerialPort.addDataListener(new SerialPortDataListener() {
            int called = 0;
            @Override
            public int getListeningEvents() {
                return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
            }

            @Override
            public void serialEvent(SerialPortEvent event) {

                System.out.println("serial event " + called);
                called++;
                System.out.println(event.toString());
                if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
                    return;

                int bytesAvailable = selectedSerialPort.bytesAvailable();

                while (bytesAvailable > 0) {

                    while (bytesAvailable < 5) {
                        bytesAvailable = selectedSerialPort.bytesAvailable();
                    }
                    byte[] header = new byte[5];
                    selectedSerialPort.readBytes(header, 5);
                    int numData = header[4];
                    bytesAvailable = selectedSerialPort.bytesAvailable();
                    while (bytesAvailable < numData) {
                        bytesAvailable = selectedSerialPort.bytesAvailable();
                    }

                    byte[] dataBuffer = new byte[numData];
                    selectedSerialPort.readBytes(dataBuffer, numData);


                    long t_extID = header[0] | (header[1] << 8) | (header[2] << 16) | (header[3] << 24);
                    int extIDRead = (int) (t_extID & 0x1FFFFFFF);

                    int[] dataRead = new int[numData];
                    data = new byte[numData];
                    for (int i = 0; i < numData; i++) {
                        dataRead[i] = (int) dataBuffer[i] & 0xFF;
                        data[i] = (byte) dataRead[i];
                    }


                    extID = extIDRead;

                    hasMessage = true;
                    receiveCANCallback.ReceiveMessage();

                    bytesAvailable = selectedSerialPort.bytesAvailable();
                    System.out.println("remaining bytes: " + bytesAvailable);
                }



                return;
            }
        });
    }

    public long getExtID(){
        if(!hasMessage){
            return 0;
        }
        return extID;
    }

    public int getLength() {
        if (data == null || !hasMessage) {
            return 0;
        }
        return data.length;
    }

    public byte[] getData(){
        if(!hasMessage){
            return null;
        }
        return data;
    }
    /**
     * send data within the buffer
     *
     * @param canMessage the CAN message
     * @throws Exception
     */
    public void sendBuffer(CANMessage canMessage) {
        try {

            byte[] buffer = new byte[6 + canMessage.dataLength];
            buffer[0] = (byte) canMessage.priority;
            buffer[1] = (byte) canMessage.sourceID;
            buffer[2] = (byte) canMessage.autonomous;
            buffer[3] = (byte) canMessage.messageType;
            buffer[4] = (byte) canMessage.extraID;
            buffer[5] = (byte) canMessage.dataLength;
            for (int i = 0; i < canMessage.dataLength; i++){
                buffer[i + 6] = (byte) canMessage.data[i];
            }

            for (int i = 0; i < buffer.length; i++){

                OutputStream o = selectedSerialPort.getOutputStream();
                o.write(buffer[i]);


            }

        } catch (Exception e ){
            System.out.println(e);
        }
    }

}


