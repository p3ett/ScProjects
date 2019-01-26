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

<body>
    <%@include file="Top.jsp" %>
    <%
        int userId = (int)session.getAttribute("userId");
        String mode = request.getParameter("mode");
            if (mode == null || "".equals(mode)) {
                mode = "inbox";
            }
        ArrayList<Message> messageList = Message.load(userId, mode.equals("sent"));
    %>
    <div>
        <div style="float: left; background-color: #ddd; padding: 5px; margin: 5px;">
                <table style="width:150px;">
                    <%
                        if (mode.equals("inbox"))
                        {
                            %>
                <tr>
                    <td><b>Inbox</b></td>
                </tr>
                <tr>
                    <td><a href="Message?mode=sent">Sent</a></td>
                </tr>
                            <%
                        }
                        else
                        {
                            %>
                            <tr>
                                <td><a href="Message?mode=inbox">Inbox</a></td>
                            </tr>
                            <tr>
                                <td><b>Sent</b></td>
                            </tr>
                            <%
                        }
                        %>
            </table>
        </div>
        <div style="float: left; margin: 0px 30px;">
            <%
                if (messageList.size() == 0) {
                    %>
                    <p>No messages were found!</p>
                    <%
                }
                else
                {
                for (Message message : messageList)
                {
                    boolean unread = !message.read && message.to.id == userId;
                    if (unread) {
                        %>
                        <b>
                        <%
                    }
                    %>
                    <a href="Message?id=<% out.print(message.id); %>&mode=<% out.print(mode); %>">
                    <table style="background-color: #ddd; margin:10px;">
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
                                <% out.print(message.text); %>
                            </td>
                        </tr>
                    </table>
                    </a>
                    <%
                        if (unread) {
                    %>
                    </b>
                    <%
                    }
                }}
                %>
        </div>
    </div>
</body>
</html>

