package Model;

import Database.DB;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.OutputStream;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class Item {
    public User buyer;
    public int id, userId;
    public String name, description, userName, location, country;
    public double firstBid, buyPrice;
    public boolean expired;
    public Date started, ended;
    public ArrayList<Category> categories = new ArrayList<Category>(0);
    public ArrayList<Bid> bids = new ArrayList<Bid>(0);
    public ArrayList<Image> images = new ArrayList<Image>(0);
    
    public static final int ItemCountInSinglePage = 3;
    
    public double currently() {
        double max = firstBid;
        for (Bid bid : bids)
            if (bid.amount > max)
                max = bid.amount;
        return max;
    }
    
    public boolean active() {
        return started != null && !expired;
    }
    
    public User getWinner() {
        if (expired) {
            if (buyer.id > 0)
                return buyer;
            Bid maxBid = null;
            for (Bid bid : bids)
                if (maxBid == null || maxBid.amount < bid.amount)
                    maxBid = bid;
            if (maxBid != null)
                return maxBid.user;
        }
        return new User();
    }
    
    public static Item load(int id) {
        Item item = new Item();
        String query = "SELECT auction.*, seller.username, buyer.username, (auction.ended IS NOT NULL AND NOW() > auction.ended OR sold_to IS NOT NULL) expired ";
        query += "FROM auction INNER JOIN user seller ON seller.id = auction.user_id LEFT JOIN user buyer ON buyer.id = auction.sold_to WHERE auction.id = " + id + ";";
        ResultSet rs = DB.SelectQuery(query);
        try {
            if (rs != null && rs.next()) {
                item.id = rs.getInt("id");
                item.userId = rs.getInt("user_id");
                item.userName = rs.getString("seller.username");
                item.name = rs.getString("name");
                item.description = rs.getString("description");
                item.location = rs.getString("location");
                item.country = rs.getString("country");
                item.firstBid = rs.getDouble("first_bid");
                item.buyPrice = rs.getDouble("buy_price");
                item.started = rs.getDate("started");
                item.ended = rs.getDate("ended");
                item.buyer = new User();
                item.buyer.id = rs.getInt("sold_to");
                item.buyer.username = rs.getString("buyer.username");
                item.expired = rs.getBoolean("expired");
                item.categories = Category.load(id);
                item.images = Image.load(id);
                item.bids = Bid.load(id);
            }
        }
        catch (Exception ex) { }
        finally { DB.CloseResources(rs); }
        return item;
    }
    
    public static int getPagesCount(int userId, String description, String category, String priceFrom, String priceTo, String location) {
        
        int count = 0;
        StringBuilder sb = new StringBuilder();
        sb.append("SELECT COUNT(*) cnt FROM auction WHERE ");
        fillWhereClause(sb, userId, description, category, priceFrom, priceTo, location);
        
        ResultSet rs = DB.SelectQuery(sb.toString());
        if (rs != null) {
            try {
                if (rs.next())
                    try {
                        count = (rs.getInt("cnt") + ItemCountInSinglePage - 1) / ItemCountInSinglePage;
                    } catch (Exception ex) {
                    } 
            } catch (SQLException ex) {
                Logger.getLogger(Item.class.getName()).log(Level.SEVERE, null, ex);
            } finally { DB.CloseResources(rs); }
        }
        return count;
    }
    
    public static ArrayList<Item> load(int userId, String description, String category, String priceFrom, String priceTo, String location, int page) {
        ArrayList<Item> itemList = new ArrayList<Item>();
        StringBuilder sb = new StringBuilder();
        sb.append("SELECT auction.*, seller.username, buyer.username, (auction.ended IS NOT NULL AND NOW() > auction.ended OR sold_to IS NOT NULL) expired ");
        sb.append("FROM auction INNER JOIN user seller ON seller.id = auction.user_id LEFT JOIN user buyer ON buyer.id = auction.sold_to WHERE ");

        fillWhereClause(sb, userId, description, category, priceFrom, priceTo, location);

        sb.append(" ORDER BY name LIMIT " + (page * ItemCountInSinglePage) + "," + ItemCountInSinglePage + ";");
        ResultSet rs = DB.SelectQuery(sb.toString());
        if (rs != null) {
            try {
                while (rs.next()) {
                    try {
                        Item item = new Item();
                        item.id = rs.getInt("id");
                        item.userId = rs.getInt("user_id");
                        item.userName = rs.getString("seller.username");
                        item.name = rs.getString("name");
                        item.description = rs.getString("description");
                        item.location = rs.getString("location");
                        item.country = rs.getString("country");
                        item.firstBid = rs.getDouble("first_bid");
                        item.buyPrice = rs.getDouble("buy_price");
                        item.started = rs.getDate("started");
                        item.ended = rs.getDate("ended");
                        item.buyer = new User();
                        item.buyer.id = rs.getInt("sold_to");
                        item.buyer.username = rs.getString("buyer.username");
                        item.expired = rs.getBoolean("expired");
                        item.images = Image.load(item.id);
                        item.bids = Bid.load(item.id);
                        itemList.add(item);
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
    
    private static void fillWhereClause(StringBuilder sb, int userId, String description, String category, String priceFrom, String priceTo, String location)
    {
        if (userId > 0) {
            sb.append("user_id = " + userId);
        } else {
            sb.append("started IS NOT NULL AND ended > NOW() AND sold_to IS NULL");
        }

        final String empty = "";
        if (description != null && !description.equals(empty)) {
            sb.append(" AND description LIKE '%" + description + "%'");
        }
        if (category != null && !category.equals(empty)) {
            sb.append(" AND (SELECT COUNT(*) FROM category WHERE auction_id = auction.id AND name = '" + category + "') > 0");
        }
        if (priceFrom != null && !priceFrom.equals(empty)) {
            sb.append(" AND buy_price > " + priceFrom);
        }
        if (priceTo != null && !priceTo.equals(empty)) {
            sb.append(" AND buy_price < " + priceTo);
        }
        if (location != null && !location.equals(empty)) {
            sb.append(" AND location LIKE '%" + location + "%'");
        }
    }

    public ByteArrayInputStream exportToXML()
    {
        try {

            DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
            Document doc = docBuilder.newDocument();
            
            // Item
            Element rootElement = doc.createElement("Item");
            Attr attr = doc.createAttribute("ItemId");
            attr.setValue(String.valueOf(this.id));
            rootElement.setAttributeNode(attr);
            doc.appendChild(rootElement);

            // Name
            Element node = doc.createElement("Name");
            node.setTextContent(this.name);
            rootElement.appendChild(node);

            // Categories
            for (Category category : categories) {
                node = doc.createElement("Category");
                node.setTextContent(category.name);
                rootElement.appendChild(node);
            }
            
            // Currently
            node = doc.createElement("Currently");
            node.setTextContent(String.valueOf(this.currently()));
            rootElement.appendChild(node);

            // First_Bid
            node = doc.createElement("First_Bid");
            node.setTextContent(String.valueOf(this.firstBid));
            rootElement.appendChild(node);
            
            // Number_Of_Bids
            node = doc.createElement("Number_Of_Bids");
            node.setTextContent(String.valueOf(this.bids.size()));
            rootElement.appendChild(node);

            // Bids
            for (Bid bid : bids) {
                node = doc.createElement("Bid");
                Element bidder = doc.createElement("Bidder");
                attr = doc.createAttribute("Rating");
                attr.setValue(String.valueOf(bid.user.bidderRating));
                bidder.setAttributeNode(attr);
                attr = doc.createAttribute("UserID");
                attr.setValue(bid.user.username);
                bidder.setAttributeNode(attr);
                Element subNode = doc.createElement("Location");
                subNode.setTextContent(bid.user.location);
                bidder.appendChild(subNode);
                subNode = doc.createElement("Country");
                subNode.setTextContent(bid.user.country);
                bidder.appendChild(subNode);
                node.appendChild(bidder);
                Element time = doc.createElement("Time");
                time.setTextContent(bid.time.toString());
                node.appendChild(time);
                Element amount = doc.createElement("Amount");
                amount.setTextContent(String.valueOf(bid.amount));
                node.appendChild(amount);
                rootElement.appendChild(node);
            }
            
            // Location
            node = doc.createElement("Location");
            node.setTextContent(this.location);
            rootElement.appendChild(node);
            
            // Country
            node = doc.createElement("Country");
            node.setTextContent(this.country);
            rootElement.appendChild(node);
            
            // Ends
            node = doc.createElement("Ends");
            node.setTextContent(this.ended.toString());
            rootElement.appendChild(node);
            
            // Started
            node = doc.createElement("Started");
            node.setTextContent(this.started.toString());
            rootElement.appendChild(node);
            
            // Seller
            node = doc.createElement("Seller");
            attr = doc.createAttribute("Rating");
            attr.setValue(String.valueOf(this.userId));
            node.setAttributeNode(attr);
            attr = doc.createAttribute("UserID");
            attr.setValue(this.userName);
            node.setAttributeNode(attr);
            rootElement.appendChild(node);
            
            // Description
            node = doc.createElement("Description");
            node.setTextContent(this.description);
            rootElement.appendChild(node);
            
            // write the content into xml file
            TransformerFactory transformerFactory = TransformerFactory.newInstance();
            Transformer transformer = transformerFactory.newTransformer();
            DOMSource source = new DOMSource(doc);
            
            ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
            StreamResult result = new StreamResult(outputStream);

            // Output to console for testing
            // StreamResult result = new StreamResult(System.out);
            transformer.transform(source, result);

            return new ByteArrayInputStream(outputStream.toByteArray());
        } catch (ParserConfigurationException pce) {
            pce.printStackTrace();
        } catch (TransformerException tfe) {
            tfe.printStackTrace();
        }
        return null;
    }
    
}
