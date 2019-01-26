<%@page import="Model.User"%>
<%@page import="java.sql.ResultSet"%>
<%@page import="Database.DB"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>User</title>
        <link rel="stylesheet" type="text/css" href="style.css" >
        <script>
            function getUrlVars() { 
                var vars = {}; 
                var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value) { 
                    vars[key] = value;
                }); 
                return vars;
            }
            function Approve() {
                document.location.href = "Users?approved=1&userId=" + getUrlVars()["userId"];
            }
        </script>
    </head>
    <body>
        <%@include file="Top.jsp" %>
        <a href="/Auctions/Users">Go Back</a><br>
        <%
            int userId = Integer.parseInt(request.getParameter("userId"));
            User usr = User.load(userId);
            if (usr.id > 0)
            {
                %>
                <table border="1">
                    <tr>
                        <th colspan="2"><% out.println(usr.username + (usr.approved ? "" : " (Not Activated)")); %></th>
                    </tr>
                    <tr>
                        <td>
                            Name
                        </td>
                        <td>
                            <% out.println(usr.firstname + " " + usr.lastname); %>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            E-mail
                        </td>
                        <td>
                            <% out.println(usr.email); %>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            Phone
                        </td>
                        <td>
                            <% out.println(usr.phone); %>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            Location
                        </td>
                        <td>
                            <% out.println(usr.location); %>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            Country
                        </td>
                        <td>
                            <% out.println(usr.country); %>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            Tax Number
                        </td>
                        <td>
                            <% out.println(usr.taxnumber); %>
                        </td>
                    </tr>
                </table>
                <% if (!usr.approved)
                    out.println("<button onclick=\"Approve();\">Approve</button>");

            }
            else
            {
                out.println("User was not found!");
            }
        %>
              
    </body>
</html>
