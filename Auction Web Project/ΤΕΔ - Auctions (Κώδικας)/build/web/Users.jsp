<%@page import="java.sql.ResultSet"%>
<%@page import="Database.DB"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%
    if (!session.getAttribute("user").equals("admin"))
        return;
%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Users</title>
        <link rel="stylesheet" type="text/css" href="style.css" >
    </head>
    <body>
        <%@include file="Top.jsp" %>
        <%
            String query = "SELECT id, username, firstname, lastname, approved FROM user";
            ResultSet rs = DB.SelectQuery(query);
            if (rs != null)
            {
                out.println("<table>\n<tr><th colspan=\"4\">\nUsers\n</th>\n<tr>");
                out.println("<th>Username</th><th>Real Name</th><th>Approved</th>");
                if (rs.next())
                {
                    int id;
                    String username, realname;
                    boolean approved;
                    do
                    {
                        id = rs.getInt("id");
                        username = rs.getString("username");
                        realname = rs.getString("lastname");
                        realname += " " + rs.getString("firstname");
                        approved = rs.getBoolean("approved");
                        out.println("<tr><td>");
                        out.println(username);
                        out.println("</td><td>");
                        out.println(realname);
                        out.println("</td><td>");
                        out.println(approved ? "Yes" : "No");
                        out.println("</td><td>");
                        out.println("<a href=\"Users?userId=" + id + "\">"); 
                        out.println("<button type=\"Button\">View/Edit</button></a></td></tr>"); 
                    }
                    while (rs.next());
                    out.println("</table><br>");
                }
                DB.CloseResources(rs);
            }
        %>
    </body>
</html>
