import gnu.io.*; // rxtx library. Need to add library manually. The library is in RxTx Library folder

//import com.fazecast.jSerialComm.*;
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
    private gnu.io.SerialPort serialPort;
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
    public void sendData(int asciiData) throws Exception {

        openPort();

        OutputStream os = commPort.getOutputStream();
        os.write(asciiData);

        disconnect();

//        com.fazecast.jSerialComm.SerialPort sp = com.fazecast.jSerialComm.SerialPort.getCommPort("COM4");
//        sp.openPort();
//        sp.setComPortParameters(115200,8,1,0);
//        OutputStream o = sp.getOutputStream();
//        o.write(asciiData);
//
//        sp.closePort();
    }


    /**
     * open the port
     * @throws Exception
     */
    private void openPort() throws Exception {

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
    private void disconnect() throws IOException {
        commPort.close();
    }

    /**
     * read the input data via the serial port
     * @throws Exception
     */
    public void readData() throws Exception {

        com.fazecast.jSerialComm.SerialPort sp = com.fazecast.jSerialComm.SerialPort.getCommPort("COM3");
        sp.openPort();
        sp.setComPortParameters(115200, 8,1,0);
//        sp.setComPortTimeouts(com.fazecast.jSerialComm.SerialPort.TIMEOUT_READ_BLOCKING, 1000, 0);
        InputStream in = sp.getInputStream();
        System.out.println(sp.getBaudRate());
        try {
            while (true)
            {
                while(sp.bytesAvailable() == 0)
                    Thread.sleep(20);
                byte[] readBuffer = new byte[20];
                int numRead = sp.readBytes(readBuffer, readBuffer.length);
                System.out.println("numRead: " + numRead);
                if (numRead < 5) {
                    System.out.println("invalid message, bad header");
                    continue;
                }
                long t_extID = readBuffer[0] | (readBuffer[1] << 8) | (readBuffer[2] << 16) | (readBuffer[3] << 24);
                int extID = (int)(t_extID & 0x1FFFFFFF);
                int dlc = readBuffer[4];
                if (numRead < 5 + dlc) {
                    System.out.println("invalid message, bad data");
                    System.out.println("dlc: " + dlc);
                    continue;
                }

                int[] data = new int[dlc];
                for (int i = 0; i < dlc; i++) {
                    data[i] = (int)readBuffer[5+i] & 0xFF;
                }
                System.out.println("<------------------------------------------------>");
                System.out.println("ExtId: " + String.format("0x%08X", extID));
                System.out.println("dlc: " + dlc);
                for (int i = 0; i < dlc; i++) {
                    System.out.print("data["+i+"]: " + data[i] + " ");
                }
                System.out.print("\n");
            }
        } catch (Exception e) { e.printStackTrace(); }
        sp.closePort();
    }
}

