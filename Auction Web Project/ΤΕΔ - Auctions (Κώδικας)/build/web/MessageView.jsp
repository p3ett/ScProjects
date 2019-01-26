<%@page import="Model.Category"%>
<%@page import="java.util.ArrayList"%>
<%@page import="Model.Message"%>
<%@page import="java.sql.ResultSet"%>
<%@page import="Database.DB"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <script>
    </script>
    <title>Messages</title>
    <link rel="stylesheet" type="text/css" href="style.css" >
</head>

<%
    int userId = (int) session.getAttribute("userId");
    String mode = request.getParameter("mode");
    if (mode == null || "".equals(mode)) {
        mode = "inbox";
    }
    Message message = Message.load(Integer.parseInt(request.getParameter("id")));
    if (message.to.id == userId) {
        message.markAsRead();
    }
%>

<body>
    <%@include file="Top.jsp" %>
    <div>
        <div style="float: left; background-color: #ddd; padding: 5px; margin: 5px;">
            <a href="Message?mode=<% out.print(mode); %>">Back</a>
        </div>
        <div style="float: left; margin: 0px 30px;">
            <table style="background-color: #ddd;">
                <tr>
                    <td style="width: 250px;">
                        From: <% out.print(message.from.username); %>
                    </td>
                    <td style="width: 250px;">
                        To: <% out.print(message.to.username); %>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <% out.print(message.text);%>
                    </td>
                </tr>
            </table>
            <a href="Message?id=<% out.print(message.id); %>&delete=true">Delete</a>
            <a href="Message?sendTo=<% out.print(mode.equals("inbox") ? message.from.id : message.to.id); %>">Reply</a>
        </div>
    </div>
</body>
</html>

