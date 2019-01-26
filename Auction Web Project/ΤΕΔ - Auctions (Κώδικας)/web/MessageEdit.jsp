<%@page import="Model.User"%>
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
        String sendToId = request.getParameter("sendTo");
        User sendToUser = User.load(Integer.parseInt(sendToId));
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
            <p>
                <b>New Message to <% out.print(sendToUser.username); %></b>
            </p>
            <form name="msgfrm" method="POST">
                <input type="hidden" name="sendToId" value="<% out.print(sendToId); %>" />
                <textarea type="text" name="text"></textarea>
                <p>
                    <input type="submit" value="Send Message" />
                </p>
            </form>
        </div>
    </div>
</body>
</html>

