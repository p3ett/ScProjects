<%@page import="Model.Message"%>
<%@page import="java.sql.ResultSet"%>
<%@page import="Database.DB"%>
<div style="background-color: #e3e3e3; margin: 14px 6px 14px 6px; padding: 6px;">
    <%
        String user = (String) session.getAttribute("user");
        if (user != null)
        {
            out.println("<a href=\"Browse\"><button>Browse Auctions</button></a>");
            if (user.equals("admin"))
                out.println("<a href=\"Users\"><button>View Users</button></a>");
            else
            {
                out.println("<a href=\"Browse?my=true\"><button>My Auctions</button></a>");
                out.println("<a href=\"Item?edit=true\"><button>Create New Auction</button></a>");
                int newMessages = Message.getUnreadMessagesCount((int)session.getAttribute("userId"));
                out.println("<a href=\"Message\"><button>Messages (" + newMessages + ")</button></a>");
            }
            out.println("<a href=\"Log?logout=true\"><button>Logout</button></a>");
        }
        else
        {
            out.println("<a href=\"Browse\"><button>Browse Auctions</button></a>");
            out.println("<a href=\"Register\"><button>Register</button></a>");
            out.println("<a href=\"Log\"><button>Login</button></a>");
        }
        out.println(user == null ? "(Guest Mode)" : "<b>" + user + "</b>");
    %>
</div>
