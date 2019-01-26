<%@page import="Model.Category"%>
<%@page import="java.util.ArrayList"%>
<%@page import="Model.Item"%>
<%@page import="java.sql.ResultSet"%>
<%@page import="Database.DB"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <script>
    </script>
    <title>Browse Auctions</title>
    <link rel="stylesheet" type="text/css" href="style.css" >
</head>

<body>
    <%@include file="Top.jsp" %>
    
    <%
        boolean showMyAuctions = request.getParameter("my") != null;
        int userId = session.getAttribute("userId") == null ? 0 : (int)session.getAttribute("userId");
        
        String fDescription = (String)session.getAttribute("fDescription");
        String fCategory = (String) session.getAttribute("fCategory");
        String fPriceFrom = (String) session.getAttribute("fPriceFrom");
        String fPriceTo = (String) session.getAttribute("fPriceTo");
        String fLocation = (String) session.getAttribute("fLocation");
        
        int curPage = request.getParameter("page") == null ? 0 : Integer.parseInt(request.getParameter("page"));
        ArrayList<Item> itemList = Item.load(showMyAuctions ? userId : 0, fDescription, fCategory, fPriceFrom, fPriceTo, fLocation, curPage);
    %>
    <div>
        <div style="float: left; background-color: #ddd; padding: 5px; margin: 5px;">
            <form name="frm" method="POST" onsubmit="return true;">
                <table>
                <tr>
                    <th colspan="2">Filters</th>
                </tr>
                <tr>
                    <td>Description</td>
                    <td><input name="description" type="text" value="<% out.print(fDescription == null ? "" : fDescription); %>"/></td>
                </tr>
                <tr>
                    <td>Category</td>
                    <td>
                        <select name="category">
                            <option id=""></option>
                            <%
                                String selectedCategory = (String)session.getAttribute("fCategory");
                                if (selectedCategory == null)
                                    selectedCategory = "";
                                for (Category category : Category.load()) {
                                    String selected = category.name.equals(selectedCategory) ? " selected" : "";
                                    out.println("<option value=\"" +  category.name + "\"" + selected + ">" + category.name + "</option>");
                                }
                                %>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td>Price From</td>
                    <td><input name="priceFrom" type="number" step="0.01" min="0" value="<% out.print(fPriceFrom); %>"/></td>
                </tr>
                <tr>
                    <td>Price To</td>
                    <td><input name="priceTo" type="number" step="0.01" min="0"  value="<% out.print(fPriceTo); %>"/></td>
                </tr>
                <tr>
                    <td>Location</td>
                    <td><input name="location" type="text" value="<% out.print(fLocation == null ? "" : fLocation); %>"/></td>
                </tr>
                <tr>
                    <th colspan="2">
                        <input type="submit" value="Apply Filters" />
                    </th>
                </tr>
            </table>
        </form>
        </div>
        <div style="float: left; margin: 0px 30px;">
            <% if (itemList.size() == 0) {
                out.print("<p><b>No Auctions were found!</b></p>");
            }
            else { %>
            <p><b><% if (showMyAuctions && userId > 0) {
                    out.println("<b>My Auctions - </b>");
                } else { out.println("<b>Browse - </b>"); } %>Page</b>
            <%
                for (int i = 1; i <= Item.getPagesCount(showMyAuctions ? userId : 0, fDescription, fCategory, fPriceFrom, fPriceTo, fLocation); i++)
                    if (curPage == i - 1) { out.print("<b>" + i + "</b>"); }
                    else {
                    %>
                    <a href="/Auctions/Browse?page=<% out.print(i-1);if (showMyAuctions) out.print("&my=true"); %>"/><% out.print(i); %></a>
                     <%       
                    }
                %>
                </p>
                <div>
                <%
                    for (Item item : itemList) { %>
                    <a href="Item?id=<% out.print(item.id); %>">
                    <div style="background-color: #eee; float:left; width: 100%; margin: 5px;">
                        <div style="width: 120px; height: 120px; float:left; margin: 10px;">
                            <% if (item.images.size() > 0) { %>
                            <image src="img/<% out.print(item.images.get(0).filename); %>" style="width: 120px; height: 120px; object-fit: contain;"></image> <% } %>
                        </div>
                        <div style=" height: 120px; float:left; margin: 10px;">

                            <b><% out.print(item.name); %></b><br/>
                            <% out.print(item.userName); %> - <% out.println(item.bids.size()); %> Bids<br/>
                            <% out.println(item.description); %>
                        </div>
                    </div></a>
                    <%}
                %> 

                </div>
            <% } %>
        </div>
    </div>
</body>
</html>

