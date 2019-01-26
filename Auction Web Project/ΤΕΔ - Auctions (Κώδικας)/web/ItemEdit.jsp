<%@page import="Model.Image"%>
<%@page import="Model.Category"%>
<%@page import="Model.Item"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <script>
        function Validate()
        {
            var form = document.forms['regfrm'];
            var name = form.name.value;
            var location = form.location.value;
            var country = form.country.value;
            var description = form.description.value;
            var firstBid = form.firstBid.value;
            var buyPrice = form.buyPrice.value;
            if (name.length < 3 || name.length > 45)
            {
                alert("Name must be 3-45 characters long!");
                return false;
            }
            if (location.length < 3 || location.length > 200)
            {
                alert("Location must be 3-200 characters long!");
                return false;
            }
            if (country.length < 3 || country.length > 100)
            {
                alert("Country must be 3-100 characters long!");
                return false;
            }
            if (description.length < 10 || description.length > 10000)
            {
                alert("Description must be 10-10000 characters long!");
                return false;
            }
            if (buyPrice > 0 && buyPrice <= firstBid)
            {
                alert("First Bid Price should not exceed Buy Price!");
                return false;
            }
            return true;
        }
        var categoryId = 0;
        function addCategory() {
            var categoryElement = document.getElementById('category');
            if (categoryElement.value === null || categoryElement.value.match(/^ *$/) !== null) {
                return; }
            var div = document.getElementById('categories');
            categoryId++;
            div.innerHTML = div.innerHTML + '<div id="cat' + categoryId + '"><input type="hidden" name="category" value="' + categoryElement.value + '" />' + categoryElement.value + '<button type="button" onclick="remove(\'cat' + categoryId + '\');">Remove</button></div>';
            categoryElement.value = null;
        }
        var imageId = 0;
        function addImage() {
            var div = document.getElementById('images');
            imageId++;

            var el = document.createElement('div');
            el.id = imageId;
            el.innerHTML = '<input type="file" accept="image/*" name="image'  + imageId + '"/><button type="button" onclick="remove(' + imageId + ');">Remove</button>';
            div.appendChild(el);
        }
        function remove(id) {
            var el = document.getElementById(id);
            el.parentNode.removeChild( el );
        }
        function removeExistingImage(id) {
            var div = document.getElementById('images');
            var el = document.createElement('input');
            el.setAttribute("type", "hidden");
            el.setAttribute("name", 'deleteimage');
            el.setAttribute("value", id);
            div.appendChild(el);
        }
        
    </script>
    <title>Auction</title>
    <link rel="stylesheet" type="text/css" href="style.css" >
</head>
<body>
    <%@include file="Top.jsp" %>
    <%
        Item item = (Item) request.getAttribute("item");
        %>
        <form name="regfrm" method="POST" enctype="multipart/form-data" onsubmit="return Validate();">
            <hidden name="id" value="<% out.print(item.id); %>"></hidden>
        <table>
            <tr>
                <th colspan="2">
                    Auction
                </th>
            </tr>
            <tr>
                <td>
                    Name
                </td>
                <td>
                    <input type="text" name="name" value="<% out.print(item.name == null ? "" : item.name);%>" />
                </td>
            </tr>
            <tr>
                <td>
                    Categories
                </td>
                <td>
                    <input type="text" id="category" list="cats">
                    <datalist id="cats">
                        <%
                            for (Category category : Category.load())
                                out.println("<option value=\"" + category.name + "\"/>");
                            %>
                    </datalist>
                    <button type="button" onclick="addCategory();">Add</button>
                    <div id="categories">
                        <% for (int i = 0; i < item.categories.size() ; i++) { %>
                            <div id="ecat<% out.print(i); %>">
                                <input type="hidden" name="category" value="<% out.print(item.categories.get(i).name);%>"/><% out.print(item.categories.get(i).name);%>
                            <button type="button" onclick="remove('ecat<% out.print(i); %>');">Remove</button></div>
                    <%    } %>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    Location
                </td>
                <td>
                    <input type="text" name="location" value="<% out.print(item.location == null ? "" : item.location);%>" />
                </td>
            </tr>
            <tr>
                <td>
                    Country
                </td>
                <td>
                    <input type="text" name="country" value="<% out.print(item.country == null ? "" : item.country);%>" />
                </td>
            </tr>
            <tr>
                <td>
                    Description
                </td>
                <td>
                    <textarea name="description"><% out.print(item.description == null ? "" : item.description);%></textarea>
                </td>
            </tr>
            <tr>
                <td>
                    First Bid Price
                </td>
                <td>
                    <input type="number" step="0.01" min="0.00" name="firstBid" value="<% out.print(item.firstBid);%>"></input>
                </td>
            </tr>
            <tr>
                <td>
                    Buy Price
                </td>
                <td>
                    <input type="number" step="0.01" min="0.00" name="buyPrice" value="<% out.print(item.buyPrice);%>"></input>
                </td>
            </tr>
        </table>
        <p>
            Images
        <div id="images">
            <%
                int imageIndex = 0;
                for (Image image : item.images) {
                    imageIndex++;
            %>
            <table id="image<% out.print(imageIndex); %>" style="background-color: #ddd; padding: 5px;">
                <tr>
                    <td>
                        <img src="img/<% out.print(image.filename); %>" style="width: 120px; height: 120px; object-fit: contain; "/>
                    </td>
                </tr>
                <tr>
                    <td>
                        <button type="button" onclick="remove('image<% out.print(imageIndex); %>'); removeExistingImage(<% out.print(image.id); %>);" style="width: 100%">Remove Image</button>
                    </td>
                </tr>
            </table>
            <%
                }
            %>
            <button type="button" onclick="addImage();">Add Image</button><br/>
        </div>
        </p>
            <br>
            <input type="submit" value="Save" style="width:100px;" />
    </form>
</body>
</html>

