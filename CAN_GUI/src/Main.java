import gnu.io.SerialPort;

import javax.swing.*;
import java.io.OutputStream;

public class Main {
    public static void main(String[] args) throws Exception {
//        System.out.println("why me");
//
//        SwingUtilities.invokeLater(new Runnable() {
//            @Override
//            public void run() {
//                final MainWindow mainWindow = new MainWindow();
//                mainWindow.setVisible(true);
//            }
//        });

        CANDataTransmission COM3 = new CANDataTransmission();

//        //connect
//        COM3.Connect("COM3");

//        heartbeat (0/1)
        COM3.Connect("COM3","0");

        //sourceID (max: 0x1FF)
        COM3.Connect("COM3","000");

        //autonomous (0/1)
        COM3.Connect("COM3","0");

        //message type (max: 0x7)
        COM3.Connect("COM3","0");

        //extraID (max: 0x7FFF)
        COM3.Connect("COM3","0002");

        //data length (max: 8)
        COM3.Connect("COM3","8");

        //Data (max: 0xFF)
        COM3.Connect("COM3","00"); // data[0]
        COM3.Connect("COM3","01"); // data[1]
        COM3.Connect("COM3","02"); // data[2]
        COM3.Connect("COM3","03"); // data[3]
        COM3.Connect("COM3","04"); // data[4]
        COM3.Connect("COM3","05"); // data[5]
        COM3.Connect("COM3","06"); // data[6]
        COM3.Connect("COM3","07"); // data[7]


//        CANDataTransmission CANDT = new CANDataTransmission();
//
//        SerialPort com3 = (new CANDataTransmission()).openPort("COM3");
//        OutputStream os = CANDT.openOutputStream(com3);
//
//
//        CANDT.sendData(os, "0".getBytes());
//
//        CANDT.sendData(os, "000".getBytes());
//
//        CANDT.sendData(os, "0".getBytes());
//
//        CANDT.sendData(os, "0".getBytes());
//
//        CANDT.sendData(os, "0002".getBytes());
//
//        CANDT.sendData(os, "8".getBytes());
//
//        CANDT.sendData(os, "02".getBytes());
//        CANDT.sendData(os, "02".getBytes());
//        CANDT.sendData(os, "02".getBytes());
//        CANDT.sendData(os, "02".getBytes());
//        CANDT.sendData(os, "02".getBytes());
//        CANDT.sendData(os, "02".getBytes());
//        CANDT.sendData(os, "02".getBytes());
//        CANDT.sendData(os, "02".getBytes());
//
//        CANDT.disconnect(com3, os);


    }


}
