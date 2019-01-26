<%@page import="Model.Image"%>
<%@page import="Model.Category"%>
<%@page import="Model.Item"%>
<%@page import="java.sql.ResultSet"%>
<%@page import="Database.DB"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>

<%
    Item item = (Item) request.getAttribute("item");
%>
<!DOCTYPE html>
<html>
    
<head>
    <script>
        function makeOffer() {
            var newOfferElement = document.getElementById('newOffer');
            
            if (newOfferElement.value === null || newOfferElement.value.match(/^ *$/) !== null) {
                return; }
            if (confirm("Really make a new offer of " + newOfferElement.value + "?")) {
                window.location="/Auctions/Item?id=<% out.print(item.id); %>&offer=" + newOfferElement.value;
            }
        }
        function buy() {
            if (confirm("Really buy this item?")) {
                window.location="/Auctions/Item?id=<% out.print(item.id); %>&buy=true"; }
        }
        function activate() {
            var days = document.getElementById('days').value;
            if (days > 0 && days < 60)
                window.location="Item?id=<% out.print(item.id);%>&activate=true&days=" + days;
            else
                alert("Please provide a valid number of days!");
        }
    </script>
    <script src="files/OpenLayers.js"></script>
    <script>
        window.onload = function y() {
            
        var map = new OpenLayers.Map("openMap");
        var mapnik = new OpenLayers.Layer.OSM("OpenStreetMap");
        var fromProjection = new OpenLayers.Projection("EPSG:4326");   // Transform from WGS 1984
        var toProjection   = new OpenLayers.Projection("EPSG:900913"); // to Spherical Mercator Projection
        var position       = new OpenLayers.LonLat(13.41,52.52).transform(fromProjection, toProjection);
        var zoom = 15;
        map.addLayer(mapnik);
        map.setCenter(position, zoom);
        }
    </script>
    <title>Auction</title>
    <link rel="stylesheet" type="text/css" href="style.css" >
</head>

<body>
    <%@include file="Top.jsp" %>
    <table>
        <tr>
            <th colspan="2">
                Auction by <% out.print(item.userName); %> 
            </th>
        </tr>
        <tr>
            <td>
                Name
            </td>
            <td>
                <% out.print(item.name); %>
            </td>
        </tr>
        <tr>
            <td>
                Categories
            </td>
            <td>
                <% for (int i = 0; i < item.categories.size(); i++) {
                    out.print((i == 0 ? "" : ", ") + item.categories.get(i).name);
                } %>
            </td>
        </tr>
        <tr>
            <td>
                Location
            </td>
            <td>
                <% out.print(item.location); %>
            </td>
        </tr>
        <tr>
            <td>
                Country
            </td>
            <td>
                <% out.print(item.country); %>
            </td>
        </tr>
        <tr>
            <td>
                Description
            </td>
            <td>
                <% out.print(item.description);%>
            </td>
        </tr>
        <% if (item.started != null && item.ended != null) { %>
        <tr>
            <td>
                Start Date
            </td>
            <td>
                <% out.print(item.started.toString());%>
            </td>
        </tr>
        <tr>
            <td>
                End Date
            </td>
            <td>
                <% out.print(item.ended.toString());%>
            </td>
        </tr>
        <% } %>
        <tr>
            <td>
                Total Bids
            </td>
            <td>
                <% out.print(item.bids.size()); %>
            </td>
        </tr>
        <tr>
            <td>
                Current Bid
            </td>
            <td>
                <% out.print(item.currently()); %>
            </td>
        </tr>
        <% if (session.getAttribute("userId") != null && item.userId != (int) session.getAttribute("userId") && item.active()) { %>
        <tr>
            <td>
            </td>
            <td>
                <input id="newOffer" type="number" value="<% out.print(item.currently()); %>" step="0.01" min="<% out.print(item.currently()); %>"/>
                <button type="button" onclick="makeOffer();">Make Offer</button>
            </td>
        </tr>
        <% } if (item.buyPrice > 0) { %>
        <tr>
            <td>
                Buy Price
            </td>
            <td>
                <% out.print(item.buyPrice);
                    if (session.getAttribute("userId") != null && item.userId != (int) session.getAttribute("userId") && item.active()) { %>
                    <button type="button" onclick="buy();">Buy</button>
                    <% } %>
            </td>
        </tr>
        <% } %>
    </table>
        <% for (Image image : item.images) { %>
        <img src="img/<% out.print(image.filename); %>" style="width: 200px; height: 200px; object-fit: contain; "/>
        <% } 
        if (session.getAttribute("userId") != null && item.getWinner().id == ((int)session.getAttribute("userId"))) { %>
        <p>
            You are the winner of this auction!
            <a href="/Auctions/Message?sendTo=<% out.print(item.userId); %>">Contact</a>
        </p>  
        <% }
else if (item.expired && session.getAttribute("userId") != null && item.userId == (int)session.getAttribute("userId")) { %>
<p>
    <% if (item.getWinner().id > 0) out.print("Winner is " + item.getWinner().username); else out.print("Expired without winner!"); %>
</p>
<% } %>
<br/>
<div id="openMap" style="height:250px"></div>
                <br/>   
    <% 
        if (session.getAttribute("userId") != null && item.userId == (int)session.getAttribute("userId")) {
            if (!item.active() && !item.expired) { %>
       Duration(in Days)
       <input type="number" step="1"  min="1" max="60" id="days" value="10" style="width: 50px;"></input>
       <button onclick="activate();">Activate Auction</button></a><br><br>
                   <%
                    }
            if (!item.expired && item.bids.size() == 0) {
                out.print("<a href=\"/Auctions/Item?id=" + item.id + "&delete=true\"><button>Delete Auction</button></a>");
                out.print("<a href=\"/Auctions/Item?id=" + item.id + "&edit=true\"><button>Edit Auction</button></a>");
            }
        }
        if ("admin".equals(session.getAttribute("user"))) {
            out.print("<a href=\"/Auctions/Item?id=" + item.id + "&xml=true\"><button>Export to XML</button></a>");
        }
    %>
</body>
</html>

