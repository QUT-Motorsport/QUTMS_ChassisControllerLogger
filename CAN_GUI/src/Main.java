import gnu.io.SerialPort; // rxtx library. Need to add library manually. The library is in RxTx Library folder

import javax.swing.*;
import java.io.OutputStream;

public class Main {
    public static void main(String[] args) throws Exception {

        // String PORT = "COM3"; // Windows
        String PORT = "/dev/ttyACM0"; // Linux

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
//        COM3.Connect(PORT);

//        heartbeat (0/1)
        COM3.Connect(PORT,"0");

        //sourceID (max: 0x1FF)
        COM3.Connect(PORT,"000");

        //autonomous (0/1)
        COM3.Connect(PORT,"0");

        //message type (max: 0x7)
        COM3.Connect(PORT,"0");

        //extraID (max: 0x7FFF)
        COM3.Connect(PORT,"0002");

        //data length (max: 8)
        COM3.Connect(PORT,"8");

        //Data (max: 0xFF)
        COM3.Connect(PORT,"00"); // data[0]
        COM3.Connect(PORT,"01"); // data[1]
        COM3.Connect(PORT,"02"); // data[2]
        COM3.Connect(PORT,"03"); // data[3]
        COM3.Connect(PORT,"04"); // data[4]
        COM3.Connect(PORT,"05"); {
            "java.project.referencedLibraries": [
                "lib/**/*.jar",
                "c:\\path\\to\\jarfile\\commons-logging-1.1.1.jar"
            ]
        }// data[5]
        COM3.Connect(PORT,"06"); // data[6]
        COM3.Connect(PORT,"07"); // data[7]


//        CANDataTransmission CANDT = new CANDataTransmission();
//
//        SerialPort com3 = (new CANDataTransmission()).openPort(PORT);
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
