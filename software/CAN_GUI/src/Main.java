import javax.swing.*;

public class Main {
    public static void main(String[] args) {
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
