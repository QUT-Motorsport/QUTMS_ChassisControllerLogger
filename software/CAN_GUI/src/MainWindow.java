import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.NumberFormatter;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.text.NumberFormat;

public class MainWindow extends JFrame {
    private final String windowTitle = "QUTMS CAN Logger";
    private final int MAX_DATA = 8;

    JSpinner spinnerDataLength;
    JSpinner[] spinnerDataBytes;


    public MainWindow() {
        setupGUI();
    }

    private void setupGUI() {
        this.setTitle(windowTitle);

        // set close operation
        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                MainWindow.this.setVisible(false);
                MainWindow.this.dispose();
            }
        });

        this.setPreferredSize(new Dimension(800,600));
        this.setLayout(new GridLayout(2,1));

        JPanel sendMessagePanel = new JPanel();
        JPanel recievedMessagePanel = new JPanel();

        // send message panel
        JPanel messageHeaderPanel = new JPanel();
        JPanel messageContentsPanel = new JPanel();

        // message header panel
        messageHeaderPanel.setBackground(Color.orange);

        // message contents panel
        messageContentsPanel.setBackground(Color.pink);
        messageContentsPanel.setLayout(new GridLayout(1, MAX_DATA+2));

        spinnerDataLength = new JSpinner(new SpinnerNumberModel(0,0,8, 1));
        JPanel dataLengthWrapper = new JPanel();
        dataLengthWrapper.setBorder(BorderFactory.createEmptyBorder(50,20,50,20));
        dataLengthWrapper.setLayout(new BorderLayout());
        dataLengthWrapper.add(spinnerDataLength, BorderLayout.CENTER);
        messageContentsPanel.add(dataLengthWrapper);

        spinnerDataBytes = new JSpinner[MAX_DATA];

        for (int i = 0; i < MAX_DATA; i++) {
            SpinnerNumberModel dataModel = new SpinnerNumberModel(0,0,0xFF, 1);
            spinnerDataBytes[i] = new JSpinner(dataModel);

            // pack data field into a panel for formatting
            JPanel dataWrapper = new JPanel();
            dataWrapper.setBorder(BorderFactory.createEmptyBorder(50,20,50,20));
            dataWrapper.setLayout(new BorderLayout());
            dataWrapper.add(spinnerDataBytes[i], BorderLayout.CENTER);

            messageContentsPanel.add(dataWrapper);
        }

        spinnerDataLength.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                int dataLength = (int)spinnerDataLength.getValue();
                for (int i = 0; i < MAX_DATA; i++) {
                    spinnerDataBytes[i].setEnabled(i < dataLength);
                }
            }
        });

        JButton btnSendMessage = new JButton("Send");
        messageContentsPanel.add(btnSendMessage);



        sendMessagePanel.setLayout(new GridLayout(2,1));
        sendMessagePanel.add(messageHeaderPanel);
        sendMessagePanel.add(messageContentsPanel);



        // recieved message panel
        JPanel messageTablePanel = new JPanel();
        JPanel messageInfoPanel = new JPanel();

        messageTablePanel.setBackground(Color.red);
        messageInfoPanel.setBackground(Color.green);

        recievedMessagePanel.setLayout(new GridLayout(1,2));
        recievedMessagePanel.add(messageTablePanel);
        recievedMessagePanel.add(messageInfoPanel);

        // add panels to GUI and pack
        this.add(sendMessagePanel);
        this.add(recievedMessagePanel);
        this.pack();
    }

    public void RecieveMessage(CANMessage message) {

    }

    public void SendMessage(CANMessage message) {

    }
}
