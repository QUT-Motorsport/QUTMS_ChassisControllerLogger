import gnu.io.SerialPort; // rxtx library. Need to add library manually. The library is in RxTx Library folder

import javax.swing.*;
import java.io.OutputStream;

public class Main {
    public static void main(String[] args) throws Exception {
        System.out.println("why me");

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                final MainWindow mainWindow = new MainWindow();
                mainWindow.setVisible(true);
            }
        });

//        CANDataTransmission COM5 = new CANDataTransmission();
//
//        COM5.identifyPort("COM3", 115200);
//
//        //heartbeat (0/1)
//        COM5.sendData("1");
//
//        //sourceID (max: 0x1FF)
//        COM5.sendData("000");
//
//        //autonomous (0/1)
//        COM5.sendData("0");
//
//        //message type (max: 0x7)
//        COM5.sendData("0");
//
//        //extraID (max: 0x7FFF)
//        COM5.sendData("0002");
//
//        //data length (max: 8)
//        COM5.sendData("8");
//
//        //Data (max: 0xFF)
//        COM5.sendData("00"); // data[0]
//
//        COM5.sendData("01"); // data[1]
//
//        COM5.sendData("02"); // data[2]
//
//        COM5.sendData("03"); // data[3]
//
//        COM5.sendData("04"); // data[4]
//
//        COM5.sendData("05"); // data[5]
//
//        COM5.sendData("06"); // data[6]
//
//        COM5.sendData("07"); // data[7]

    }


}
