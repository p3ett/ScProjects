package Servlets;

import Database.DB;
import Model.User;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class RegisterServlet extends HttpServlet {

    /**
     * Processes requests for both HTTP
     * <code>GET</code> and
     * <code>POST</code> methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        request.getRequestDispatcher("/Reg.jsp").include(request, response);
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP
     * <code>GET</code> method.
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
     * Handles the HTTP
     * <code>POST</code> method.
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
        String username = request.getParameter("username");
        if ("admin".equals(username))
        {
            request.setAttribute("existsusername", true);
            processRequest(request, response);
            return;
        }
        String Query = "SELECT * FROM user WHERE username = '" + username + "';";
        ResultSet rs = DB.SelectQuery(Query);
        if (rs != null)
        {
            PrintWriter out = response.getWriter();
            try 
            {
                if (rs.next())
                {
                    request.setAttribute("existsusername", true);
                    processRequest(request, response);
                }
                else
                {
                    String password = request.getParameter("password");
                    String firstname = request.getParameter("firstname");
                    String lastname = request.getParameter("lastname");
                    String email = request.getParameter("email");
                    String phone = request.getParameter("phone");
                    String country = request.getParameter("country");
                    String location = request.getParameter("location");
                    String taxnumber = request.getParameter("taxnumber");
                    if (User.insert(username, password, firstname, lastname, email, phone, country, location, taxnumber))
                    {
                        out.write("<meta http-equiv=\"refresh\" content=\"5; url=/Auctions\">");
                        out.write("Registration was successful!<br>Your account is pending activation by the administrator!");
                    }
                    else
                    {
                        out.write("Error creating account!");
                        processRequest(request, response);
                    }
                }
            } 
            catch (SQLException ex) 
            {
                Logger.getLogger(RegisterServlet.class.getName()).log(Level.SEVERE, null, ex);
            }
            out.close();
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
