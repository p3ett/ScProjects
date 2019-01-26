package Model;

import Database.DB;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Bid {
    public double amount;
    public User user;
    public int auctionId;
    public Date time;
    
    public static ArrayList<Bid> load(int auctionId) {
        ArrayList<Bid> itemList = new ArrayList<Bid>();
        String query = "SELECT bid.*, user.id, user.username, user.location, user.country FROM bid INNER JOIN user ON user.id = bid.user_id WHERE auction_id = " + auctionId + " ORDER BY bid.id DESC;";

        ResultSet rs = DB.SelectQuery(query);
        if (rs != null) {
            try {
                while (rs.next()) {
                    try {
                        Bid bid = new Bid();
                        bid.amount = rs.getDouble("bid.amount");
                        bid.auctionId = auctionId;
                        bid.time = rs.getDate("bid.time");
                        bid.user = new User();
                        bid.user.id = rs.getInt("user.id");
                        bid.user.username = rs.getString("user.username");
                        bid.user.location = rs.getString("user.location");
                        bid.user.country = rs.getString("user.country");
                        itemList.add(bid);
                    } catch (Exception ex) { }
                }
            } catch (SQLException ex) {
                Logger.getLogger(Item.class.getName()).log(Level.SEVERE, null, ex);
            } finally {
                DB.CloseResources(rs);
            }
        }
        return itemList;
    }
    
    public void insert() {
        String query = "INSERT INTO bid (user_id, auction_id, time, amount) ";
        query += "SELECT " + user.id + ", " + auctionId + ", NOW(), " + amount + " FROM (SELECT 1) tmp ";
        query += "WHERE NOT EXISTS (SELECT * FROM bid WHERE auction_id = " + auctionId + " AND amount >= " + amount + ");";
        DB.DoQuery(query);
    }

}
