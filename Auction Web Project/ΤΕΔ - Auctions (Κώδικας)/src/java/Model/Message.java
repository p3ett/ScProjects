package Model;

import Database.DB;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Message {
    public int id;
    public User from, to;
    public String text;
    public boolean read, deletefrom, deleteto;
    
    public boolean markAsRead()
    {
        String query = "UPDATE message SET message.read = 1 WHERE id = " + this.id + ";";
        return DB.DoQuery(query);
    }
    
    public static ArrayList<Message> load(int userId, boolean from) {
        ArrayList<Message> itemList = new ArrayList<Message>();
        String query = "SELECT * FROM message msg INNER JOIN user userfrom ON msg.from = userfrom.id INNER JOIN user userto ON msg.to = userto.id ";
        query += "WHERE " + (from ? "msg.from" : "msg.to") + " = " + userId + " AND " + (from ? "deletefrom" : "deleteto") + " = 0;";

        ResultSet rs = DB.SelectQuery(query);
        if (rs != null) {
            try {
                while (rs.next()) {
                    try {
                        Message message = new Message();
                        message.id = rs.getInt("msg.id");
                        message.text = rs.getString("msg.text");
                        message.read = rs.getBoolean("msg.read");
                        message.deletefrom = rs.getBoolean("msg.deletefrom");
                        message.deleteto = rs.getBoolean("msg.deleteto");
                        message.from = new User();
                        message.from.id = rs.getInt("msg.from");
                        message.from.username = rs.getString("userfrom.username");
                        message.to = new User();
                        message.to.id = rs.getInt("msg.to");
                        message.to.username = rs.getString("userto.username");
                        itemList.add(message);
                    } catch (Exception ex) {
                    }
                }
            } catch (SQLException ex) {
                Logger.getLogger(Item.class.getName()).log(Level.SEVERE, null, ex);
            } finally {
                DB.CloseResources(rs);
            }
        }
        return itemList;
    }
    
    public static Message load(int id){
        Message message = new Message();
        String query = "SELECT * FROM message msg INNER JOIN user userfrom ON msg.from = userfrom.id INNER JOIN user userto ON msg.to = userto.id WHERE msg.id = " + id + ";"; 
        ResultSet rs = DB.SelectQuery(query);
        try {
            if (rs != null && rs.next()) {
                message.id = rs.getInt("msg.id");
                message.text = rs.getString("msg.text");
                message.read = rs.getBoolean("msg.read");
                message.deletefrom = rs.getBoolean("msg.deletefrom");
                message.deleteto = rs.getBoolean("msg.deleteto");
                message.from = new User();
                message.from.id = rs.getInt("msg.from");
                message.from.username = rs.getString("userfrom.username");
                message.to = new User();
                message.to.id = rs.getInt("msg.to");
                message.to.username = rs.getString("userto.username");
            }
        } catch (Exception ex) {
        } finally {
            DB.CloseResources(rs);
        }
        return message;
    }
    
    public static boolean delete(int id, int userId)
    {
        String query = "UPDATE message SET deletefrom = (message.from = " + userId + " OR deletefrom), deleteto = (message.to = " + userId + " OR deleteto) WHERE id = " + id + ";";
        return DB.DoQuery(query);
    }
    
    public static int getUnreadMessagesCount(int userId)
    {
        int unreadMessages = 0;
        String query = "SELECT COUNT(*) cnt FROM message WHERE message.to = " + userId + " AND message.read = 0";
        ResultSet rs = DB.SelectQuery(query);
        if (rs != null) {
            try {
                if (rs.next()) {
                    unreadMessages = rs.getInt("cnt");
                }
            } catch (SQLException ex) {
                Logger.getLogger(Message.class.getName()).log(Level.SEVERE, null, ex);
            }
            finally {
                DB.CloseResources(rs);
            }
        }
        return unreadMessages;
    }
}
