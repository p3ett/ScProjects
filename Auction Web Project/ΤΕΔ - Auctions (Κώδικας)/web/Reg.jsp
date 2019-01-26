<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <script>
        function Validate()
        {
            var form = document.forms['regfrm'];
            var uname = form.username.value;
            var psw = form.password.value;
            var psw2 = form.password2.value;
            var fname = form.firstname.value;
            var lname = form.lastname.value;
            var email = form.email.value;
            var location = form.location.value;
            var country = form.country.value;
            if (uname.length < 3 || uname.length > 60)
            {
                alert("Username must be 3-60 characters long!");
                return false;
            }
            if (psw.length < 5 || psw.length > 60)
            {
                alert("Password must be 5-60 characters long!");
                return false;
            }
            if (psw !== psw2)
            {
                alert("The two passwords are not the same!");
                return false;
            }
            if (fname.length < 3 || fname.length > 60)
            {
                alert("First Name must be 3-60 characters long!");
                return false;
            }
            if (lname.length < 3 || lname.length > 60)
            {
                alert("Last Name must be 3-60 characters long!");
                return false;
            }
            if (email.length < 3 || email.length > 100)
            {
                alert("Email Name must be 3-100 characters long!");
                return false;
            }
            if (location.length < 3 || location.length > 60)
            {
                alert("Location must be 3-60 characters long!");
                return false;
            }
            if (country.length < 3 || country.length > 60)
            {
                alert("Country must be 3-60 characters long!");
                return false;
            }
            return true;
        }
    </script>
    <title>Registration</title>
    <link rel="stylesheet" type="text/css" href="style.css" >
</head>
<body>
    <%@include file="Top.jsp" %>
    <form name="regfrm" method="POST" onsubmit="return Validate();">
        <table style="margin: 5px; padding: 5px; background-color: #ddd; ">
            <tr>
                <th colspan="2">
                    New Account
                </th>
            </tr>
            <tr>
                <td>
                    Username
                </td>
                <td>
                    <input type="text" name="username">
                </td>
                <%
                Boolean exists = (Boolean)request.getAttribute("existsusername");
                if (exists != null && exists)
                    out.print("<td>Username Exists!</td>");
                %>
            </tr>
            <tr>
                <td>
                    Password
                </td>
                <td>
                    <input type="password" name="password">
                </td>
            </tr>
            <tr>
                <td>
                    Re-enter Password
                </td>
                <td>
                    <input type="password" name="password2">
                </td>
            </tr>
            <tr>
                <td>
                    First Name
                </td>
                <td>
                    <input type="text" name="firstname">
                </td>
            </tr>
            <tr>
                <td>
                    Last Name
                </td>
                <td>
                    <input type="text" name="lastname">
                </td>
            </tr>
            <tr>
                <td>
                    E-mail
                </td>
                <td>
                    <input type="text" name="email">
                </td>
            </tr>
            <tr>
                <td>
                    Phone
                </td>
                <td>
                    <input type="text" name="phone">
                </td>
            </tr>
            <tr>
                <td>
                    Location
                </td>
                <td>
                    <input type="text" name="location">
                </td>
            </tr>
            <tr>
                <td>
                    Country
                </td>
                <td>
                    <input type="text" name="country">
                </td>
            </tr>
            <tr>
                <td>
                    Tax Number
                </td>
                <td>
                    <input type="text" name="taxnumber">
                </td>
            </tr>
            <tr>
                <th colspan="2">
                    <input type="submit" value="Complete Registration">
                </th>
            </tr>
        </table>
    </form>
</body>
</html>

