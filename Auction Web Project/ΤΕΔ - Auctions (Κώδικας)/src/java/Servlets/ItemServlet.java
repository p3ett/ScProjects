package Servlets;

import Database.DB;
import Model.Bid;
import Model.Category;
import Model.Image;
import Model.Item;
import Model.User;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.nio.file.CopyOption;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.UUID;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.Part;

public class ItemServlet extends HttpServlet {

    private static final int DEFAULT_BUFFER_SIZE = 10240; // 10KB.
    
    /**
     *
     * @param request
     * @param response
     * @throws ServletException
     * @throws IOException
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        String user = (String) request.getSession().getAttribute("user");
        boolean isEdit = request.getParameter("edit") != null;
        if (isEdit && user == null) {
            PrintWriter out = response.getWriter();
            out.println("<meta http-equiv=\"refresh\" content=\"0; url=/Auctions/Browse?my=true\">");
            out.close();
            return;
        }
        String page;
        String itemId = request.getParameter("id");
        int id = 0;
        try {
            id = Integer.parseInt(itemId);
        } catch (Exception ex) {
        }
        String activate = request.getParameter("activate");
        if ("true".equals(activate))
        {
            String query = "UPDATE auction SET started = NOW(), ended = ADDDATE(NOW(), " + request.getParameter("days") + ") WHERE id = '" + id + "';";
            DB.DoQuery(query);
        }
        Item item = Item.load(id);
        String delete = request.getParameter("delete");
        if ("true".equals(delete) && item.currently() == 0 && id != 0 && item.userId == (int) request.getSession().getAttribute("userId")) {
            String query = "DELETE FROM auction WHERE id = '" + id + "';";
            DB.DoQuery(query);
            PrintWriter out = response.getWriter();
            out.println("<meta http-equiv=\"refresh\" content=\"0; url=/Auctions/Browse?my=true\">");
            out.close();
            return;
        }
        String offer = request.getParameter("offer");
        if (offer != null && item.userId != (int) request.getSession().getAttribute("userId")) {
            Bid bid = new Bid();
            bid.auctionId = id;
            bid.user = new User();
            bid.user.id = (int) request.getSession().getAttribute("userId");
            bid.amount = Double.parseDouble(offer);
            bid.insert();
            PrintWriter out = response.getWriter();
            out.println("<meta http-equiv=\"refresh\" content=\"0; url=/Auctions/Item?id=" + id + "\">");
            out.close();
            return;
        }
        String buy = request.getParameter("buy");
        if (buy != null && buy.equals("true")) {
            String query = "UPDATE auction SET sold_to = " + request.getSession().getAttribute("userId") + " WHERE id = " + id + ";";
            DB.DoQuery(query);
            PrintWriter out = response.getWriter();
            out.println("<meta http-equiv=\"refresh\" content=\"0; url=/Auctions/Item?id=" + id + "\">");
            out.close();
            return;
        }
        String xml = request.getParameter("xml");
        if (xml != null && xml.equals("true")) // && request.getSession().getAttribute("user").equals("admin")) 
        {
            ByteArrayInputStream input = item.exportToXML();
            if (input != null)
            {
                // Init servlet response.
                response.reset();
                response.setBufferSize(DEFAULT_BUFFER_SIZE);
                response.setContentType("XML");
                response.setHeader("Content-Disposition", "attachment; filename=\"" + id + ".xml\"");

                // Prepare streams.
                BufferedOutputStream output = null;

                try {
                    // Open streams.
                    output = new BufferedOutputStream(response.getOutputStream(), DEFAULT_BUFFER_SIZE);

                    // Write file contents to response.
                    byte[] buffer = new byte[DEFAULT_BUFFER_SIZE];
                    int length;
                    while ((length = input.read(buffer)) > 0) {
                        output.write(buffer, 0, length);
                    }
                } finally {
                    // Gently close streams.
                    output.close();
                    input.close();
                }
            }
            return;
        } 
        if (isEdit)
        {
            if (id != 0 && item.userId != (int) request.getSession().getAttribute("userId"))
            {
                PrintWriter out = response.getWriter();
                out.println("<meta http-equiv=\"refresh\" content=\"0; url=/Auctions\">");
                out.close();
                return;
            }
            page = "ItemEdit";
        }
        else
        {
            page = "ItemView";
        }
        request.setAttribute("item", item);
        request.getRequestDispatcher("/" + page + ".jsp").forward(request, response);
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        
        response.setContentType("text/html;charset=UTF-8");
        int userId = (int) request.getSession().getAttribute("userId");
        int id = -1;
        String paramId = request.getParameter("id");
        if (paramId != null && !paramId.equals(""))
            id = Integer.parseInt(paramId);
        String query = "SELECT * FROM auction WHERE id = '" + id + "';";
        ResultSet rs = DB.SelectQuery(query);
        if (rs != null) {
            
            //New Images
            ArrayList<String> images = new ArrayList<String>();
            new File("/img").mkdirs();
            for (Part part : request.getParts()) {
                if (part.getName().startsWith("image") && part.getSize() > 0) {
                    File file;
                    String image;
                    do {
                        image = userId + UUID.randomUUID().toString() ;//+ "." + FilenameUtils.getExtension("");
                        file = new File("/img", image);
                    }
                    while (Files.exists(file.toPath()));
                    try (InputStream input = part.getInputStream()) {
                        Files.copy(input, file.toPath(), StandardCopyOption.REPLACE_EXISTING);
                        images.add(image);
                    }
                }
            }
            
            String[] categ = new String[0];
            Map<String, String[]> params = request.getParameterMap();
            if (params.containsKey("category"))
                categ = params.get("category");
            
            
            ArrayList<String> queries = new ArrayList<String>();
            PrintWriter out = response.getWriter();
            try {
                
                ArrayList<Category> categories = Category.load(id);
                
                if (rs.next()) // Update Auction
                {
                    int auctionUserId = rs.getInt("user_id");
                    if (auctionUserId != userId)
                        return;
                    
                    for (Category category : categories)
                    {
                        boolean included = false;
                        for (String cat : categ)
                            if (cat.equals(category.name)) {
                                included = true;
                                break;
                            }
                        if (!included) {
                            queries.add("DELETE FROM category WHERE name = '" + category.name + "' AND auction_id = " + category.auctionId + ";");
                        }
                    }
                    
                    String[] deleteImages = new String[0];
                    params = request.getParameterMap();
                    if (params.containsKey("deleteimage")) {
                        deleteImages = params.get("deleteimage");
                    }
                    for (String deleteImage : deleteImages) {
                        queries.add("DELETE FROM image WHERE auction_id = " + id + " AND id = " + deleteImage);
                    }
                    
                    StringBuilder sb = new StringBuilder();
                    sb.append("UPDATE auction SET ");
                    sb.append("name = '" + request.getParameter("name") + "', ");
                    sb.append("location = '" + request.getParameter("location") + "', ");
                    sb.append("country = '" + request.getParameter("country") + "', ");
                    sb.append("first_bid = '" + request.getParameter("firstBid") + "', ");
                    sb.append("buy_price = '" + request.getParameter("buyPrice") + "', ");
                    sb.append("description = '" + request.getParameter("description") + "' ");
                    sb.append("WHERE id = '" + id + "';");
                    queries.add(sb.toString());
                    
                    for (String cat : categ) {
                        boolean isNew = true;
                        for (Category category : categories) {
                            if (cat.equals(category.name)) {
                                isNew = false;
                                break;
                            }
                        }
                        if (isNew)
                            queries.add("INSERT INTO category (name, auction_id) VALUES ('" + cat + "', " + id + ");");
                    }
                    
                    for (String image : images) {
                        queries.add("INSERT INTO image (filename, auction_id) VALUES ('" + image + "', " + id + ");");
                    }
                    
                    if (DB.DoTransaction(queries)) {
                        out.write("<meta http-equiv=\"refresh\" content=\"2; url=/Auctions/Browse?my=true\">");
                        out.write("Auction updated successfully!");
                    } else {
                        out.write("Error updating auction!");
                        processRequest(request, response);
                    }
                    
                } else { // Insert Auction
                    
                    StringBuilder sb = new StringBuilder();
                    sb.append("INSERT INTO auction ");
                    sb.append("(user_id, name, location, country, first_bid, buy_price, description)");
                    sb.append(" VALUES (");
                    sb.append("'" + userId + "',");
                    sb.append("'" + request.getParameter("name") + "',");
                    sb.append("'" + request.getParameter("location") + "',");
                    sb.append("'" + request.getParameter("country") + "',");
                    sb.append("'" + request.getParameter("firstBid") + "',");
                    sb.append("'" + request.getParameter("buyPrice") + "',");
                    sb.append("'" + request.getParameter("description") + "'");
                    sb.append(");");
                    queries.add(sb.toString());
                    queries.add("SET @last_id = LAST_INSERT_ID();");
                    
                    for (String cat : categ)
                        queries.add("INSERT INTO category (name, auction_id) VALUES ('" + cat + "', @last_id);");
                    
                    for (String image : images)
                        queries.add("INSERT INTO image (filename, auction_id) VALUES ('" + image + "', @last_id);");
                    
                    if (DB.DoTransaction(queries)) {
                        out.write("<meta http-equiv=\"refresh\" content=\"2; url=/Auctions/Browse?my=true\">");
                        out.write("Auction created successfully!");
                    } else {
                        out.write("Error creating auction!");
                        processRequest(request, response);
                    }
                }
                
                
            } catch (SQLException ex) {
                Logger.getLogger(RegisterServlet.class.getName()).log(Level.SEVERE, null, ex);
            }
            finally {
                out.close();
                DB.CloseResources(rs);
            }
        }
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Short description";
    }// </editor-fold>
}
