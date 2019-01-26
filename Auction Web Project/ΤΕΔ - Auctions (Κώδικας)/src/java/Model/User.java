package Model;

import Database.DB;
import java.sql.ResultSet;

public class User {
    public int id, sellerRating, bidderRating;
    public String username, firstname, lastname, email, phone, location, country, taxnumber;
    public boolean approved;
    
    public static boolean insert(String username, String password, String firstname, String lastname, String email, String phone, String country, String location, String taxnumber) {
        StringBuilder sb = new StringBuilder();
        sb.append("INSERT INTO user ");
        sb.append("(username, password, firstname, lastname, email, phone, country, location, taxnumber)");
        sb.append(" VALUES (");
        sb.append("'" + username + "',");
        sb.append("'" + password + "',");
        sb.append("'" + firstname + "',");
        sb.append("'" + lastname + "',");
        sb.append("'" + email + "',");
        sb.append("'" + phone + "',");
        sb.append("'" + country + "',");
        sb.append("'" + location + "',");
        sb.append("'" + taxnumber + "'");
        sb.append(");");
        return DB.DoQuery(sb.toString());
    }
    
    public static User load(int id) {
        User user = new User();
        String query = "SELECT * FROM user WHERE id = " + id + ";";
        ResultSet rs = DB.SelectQuery(query);
        try {
            if (rs != null && rs.next()) {
                user.id = rs.getInt("id");
                user.username = rs.getString("username");
                user.firstname = rs.getString("firstname");
                user.lastname = rs.getString("lastname");
                user.email = rs.getString("email");
                user.phone = rs.getString("phone");
                user.location = rs.getString("location");
                user.country = rs.getString("country");
                user.taxnumber = rs.getString("taxnumber");
                user.sellerRating = rs.getInt("seller_rating");
                user.bidderRating = rs.getInt("bidder_rating");
                user.approved = rs.getBoolean("approved");
            }
        } catch (Exception ex) {
        } finally {
            DB.CloseResources(rs);
        }
        return user;
    }
}
