import gnu.io.*; // rxtx library. Need to add library manually. The library is in RxTx Library folder

import java.io.IOException;
import java.io.OutputStream;

public class CANDataTransmission {

    public CANDataTransmission() {
        super();
    }

//    private OutputStream os;
//    private CommPort cp;
//
//    public void Connect(String portName, String asciiData) throws Exception {
//        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
//
//        if (portIdentifier.isCurrentlyOwned()) { //the port is currently in use
//            System.out.println("Error: Port is currently in use!");
//        } else {
//            CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);
//
//            cp = commPort;
//            if (commPort instanceof SerialPort) {
//                SerialPort serialPort = (SerialPort) commPort;
//                serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
//
//                OutputStream out = serialPort.getOutputStream();
//                os = out;
//                out.write(asciiData.getBytes());
//
//                cp.close();
//
//            } else {
//                System.out.println("Error: Only Serial ports are handled by this example!!");
//            }
//        }
//    }


    private CommPortIdentifier portIdentifier;
    private Integer BAUDSpeed;
    private String portName;
    private SerialPort serialPort;
    private CommPort commPort;

    /**
     * Indentify the port
     * @param portName the port name
     * @param BAUDSpeed the baud speed
     * @throws UnsupportedCommOperationException
     * @throws PortInUseException
     * @throws NoSuchPortException
     * @throws IOException
     */
    public void identifyPort(String portName, Integer BAUDSpeed) throws UnsupportedCommOperationException, PortInUseException, NoSuchPortException, IOException {
        this.portName = portName;
        this.BAUDSpeed = BAUDSpeed;
        this.portIdentifier = CommPortIdentifier.getPortIdentifier(portName);

    }

    /**
     * send data as string. Every time it sends data via port, open the port again.
     * @param asciiData
     * @throws Exception
     */
    public void sendData(String asciiData) throws Exception {

        operPort();

        OutputStream os = commPort.getOutputStream();
        os.write(asciiData.getBytes());

        disconnect();
    }


    /**
     * open the port
     * @throws Exception
     */
    private void operPort() throws Exception {

        if (portIdentifier.isCurrentlyOwned()) { //the port is currently in use
            System.out.println("Error: Port is currently in use!");
        } else {
            this.commPort = portIdentifier.open(portName, 2000);

            if (commPort instanceof SerialPort) {
                serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(BAUDSpeed, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
            }
        }
    }

    /**
     * close the port.
     * @throws IOException
     */
    public void disconnect() throws IOException {
        commPort.close();
    }
}

