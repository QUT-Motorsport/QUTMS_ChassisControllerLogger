import gnu.io.*;

import java.io.IOException;
import java.io.OutputStream;

public class CANDataTransmission {

    public CANDataTransmission() {
        super();
    }

    private OutputStream os;
    private CommPort cp;

    public void Connect(String portName, String asciiData) throws Exception {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);

        if (portIdentifier.isCurrentlyOwned()) { //the port is currently in use
            System.out.println("Error: Port is currently in use!");
        } else {
            CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);

            cp = commPort;
            if (commPort instanceof SerialPort) {
                SerialPort serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

                OutputStream out = serialPort.getOutputStream();
                os = out;
                out.write(asciiData.getBytes());

                cp.close();

            } else {
                System.out.println("Error: Only Serial ports are handled by this example!!");
            }
        }
    }

//    public class SerialWriter implements Runnable{
//        OutputStream out;
//
//        public SerialWriter(OutputStream out){
//            this.out = out;
//        }
//
//        public void run(){
//
//        }
//    }

    public SerialPort openPort(String portName) throws UnsupportedCommOperationException, PortInUseException, NoSuchPortException {
        SerialPort serialPort = null;
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);

        if (portIdentifier.isCurrentlyOwned()) { //the port is currently in use
            System.out.println("Error: Port is currently in use!");
        } else {
            CommPort commPort = portIdentifier.open(portName, 2000);

            if (commPort instanceof SerialPort) {
                serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
            }
        }
        return serialPort;
    }

    public OutputStream openOutputStream(SerialPort serialPort){
        OutputStream os = null;
        try{
            os = serialPort.getOutputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return os;
    }
    public void sendData(OutputStream os, byte[] asciiData) {
        try {
            os.write(asciiData);

        } catch (IOException e){
            System.out.println(e);
        }
    }

    public void disconnect(SerialPort serialPort, OutputStream os) throws IOException {

        os.close();
        serialPort.close();
    }
}

