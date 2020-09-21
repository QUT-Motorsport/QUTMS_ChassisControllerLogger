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


    }


}
