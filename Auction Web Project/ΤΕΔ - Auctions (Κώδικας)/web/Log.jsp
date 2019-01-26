<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <title>Login</title>
    <link rel="stylesheet" type="text/css" href="style.css" >
</head>
<body>
    <%@include file="Top.jsp" %>
    <form method="POST" action="/Auctions/Log">
        <table style="margin: 5px; padding: 5px; background-color: #ddd;">
            <tr>
                <th colspan="3">
                    Auctions Login
                </th>
            </tr>
            <tr>
                <td>
                    <input type="text" placeholder="Username" name="uname">
                </td>
            </tr>
            <tr>
                <td>
                    <input type="password" placeholder="Password" name="pass">
                </td>
            </tr>
            <tr>
                <td colspan="2">
                    <input type="submit" value="Sign In" style="width:100%;">
                </td>
            </tr>
        </table>

        <p style="color: red;">
            <%
                String msg = (String) request.getAttribute("msg");
                if (msg != null) {
                    out.print(msg);
                }
            %>
        </p>
    </form>
</body>
</html>

