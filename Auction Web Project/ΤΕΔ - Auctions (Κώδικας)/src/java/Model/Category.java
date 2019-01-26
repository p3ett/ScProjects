package Model;

import Database.DB;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Category {
    public int auctionId;
    public String name;
    
    
    public static ArrayList<Category> load() {
        return load(-1);
    }
    
    public static ArrayList<Category> load(int auctionId) {
        ArrayList<Category> itemList = new ArrayList<Category>();
        String query;
        if (auctionId < 0)
            query = "SELECT DISTINCT name FROM category;";
        else
            query = "SELECT * FROM category WHERE auction_id = " + auctionId + ";";
        ResultSet rs = DB.SelectQuery(query);
        if (rs != null) {
            try {
                while (rs.next())
                    try {
                        Category item = new Category();
                        if (auctionId >= 0)
                            item.auctionId = rs.getInt("auction_id");
                        item.name = rs.getString("name");
                        itemList.add(item);
                    } catch (Exception ex) {
                    } 
            } catch (SQLException ex) {
                Logger.getLogger(Category.class.getName()).log(Level.SEVERE, null, ex);
            } finally { DB.CloseResources(rs); }
        }
        return itemList;
    }
    
    public void insert() {
        String query = "INSERT INTO category (name, auction_id) VALUES ('" + this.name + "', " + auctionId + "');";
        DB.DoQuery(query);
    }
    
    public void delete() {
        String query = "DELETE FROM category WHERE name = '" + this.name + "' AND auction_id = " + this.auctionId + ";";
        DB.DoQuery(query);
    }
}
