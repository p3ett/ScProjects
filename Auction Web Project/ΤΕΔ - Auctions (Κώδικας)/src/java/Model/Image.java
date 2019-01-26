package Model;

import Database.DB;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Image {
    public int id;
    public String filename;
    
    public static ArrayList<Image> load(int auctionId) {
        ArrayList<Image> itemList = new ArrayList<Image>();
        String query = "SELECT * FROM image WHERE auction_id = " + auctionId + ";";

        ResultSet rs = DB.SelectQuery(query);
        if (rs != null) {
            try {
                while (rs.next()) {
                    try {
                        Image image = new Image();
                        image.id = rs.getInt("id");
                        image.filename = rs.getString("filename");
                        itemList.add(image);
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
}
