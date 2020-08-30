import javax.swing.table.AbstractTableModel;
import java.time.LocalDateTime;
import java.util.ArrayList;

public class CANMessageTableModel extends AbstractTableModel {
    private String[] columnNames = {"Time", "Priority", "Source ID", "Autonomous", "Message Type", "Extra ID",
            "Data Length", "Byte0", "Byte1","Byte2","Byte3","Byte4","Byte5","Byte6","Byte7"};
    private ArrayList<CANMessage> messages;

    private Class[] columnClasses = {LocalDateTime.class, String.class, String.class, String.class, String.class, int.class, // header
            int.class, // data length
            int.class, int.class, int.class, int.class, int.class, int.class, int.class, int.class}; // data

    @Override
    public int getRowCount() {
        if (messages == null) {
            return 0;
        } else {
            return messages.size();
        }
    }

    @Override
    public String getColumnName(int column) {
        return columnNames[column];
    }

    @Override
    public Class<?> getColumnClass(int columnIndex) {
        return columnClasses[columnIndex];
    }

    @Override
    public int getColumnCount() {
        return columnNames.length;
    }

    @Override
    public Object getValueAt(int rowIndex, int columnIndex) {
        CANMessage message = messages.get(rowIndex);

        if (columnIndex < 7) {
            switch(columnIndex) {
                case 0:
                    return message.timeStamp.toLocalTime().toString();
                case 1:
                    return message.priority;
                case 2:
                    return message.sourceID;
                case 3:
                    return message.autonomous;
                case 4:
                    return message.messageType;
                case 5:
                    return message.extraID;
                case 6:
                    return message.dataLength;
                default:
                    return null;
            }
        } else {
            int dataIdx = columnIndex - 7;
            if (dataIdx >= message.dataLength) {
                return "";
            } else {
                return message.data[columnIndex - 7];
            }
        }
    }

    public void AddMessage(CANMessage message) {
        if (messages == null) {
            messages = new ArrayList<>();
        }

        messages.add(message);

        this.fireTableDataChanged();
    }

    public void ClearMessages() {

    }
}
