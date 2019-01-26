package Servlets;

import Database.DB;
import Model.Message;
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

public class MessageServlet extends HttpServlet {

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
        
        response.setContentType("text/html;charset=UTF-8");
        String page = "Messages";
        String sendTo = request.getParameter("sendTo");
        String id = request.getParameter("id");
        if (sendTo != null) {
            page = "MessageEdit";
        } else if (id != null) {
            if ("true".equals(request.getParameter("delete")))
            {
                PrintWriter out = response.getWriter();
                if (Message.delete(Integer.parseInt(id), (int) request.getSession().getAttribute("userId"))) {
                    out.write("<meta http-equiv=\"refresh\" content=\"3; url=Message\">");
                    out.write("Message deleted!");
                } else {
                    out.write("<meta http-equiv=\"refresh\" content=\"3; url=Message\">");
                    out.write("Error deleting message!");
                }
                out.close();
            }
            else
                page = "MessageView";
        }
        request.getRequestDispatcher("/" + page + ".jsp").forward(request, response);
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
        
        StringBuilder sb = new StringBuilder();
        sb.append("INSERT INTO message ");
        sb.append("(message.from, message.to, message.text)");
        sb.append(" VALUES (");
        sb.append(userId + ",");
        sb.append(request.getParameter("sendToId") + ",");
        sb.append("'" + request.getParameter("text") + "'");
        sb.append(");");
        PrintWriter out = response.getWriter();
        if (DB.DoQuery(sb.toString())) {
            out.write("<meta http-equiv=\"refresh\" content=\"3; url=Message?mode=sent\">");
            out.write("Message sent!");
        } else {
            out.write("<meta http-equiv=\"refresh\" content=\"3; url=Message?mode=sent\">");
            out.write("Error creating message!");
        }
        out.close();
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
