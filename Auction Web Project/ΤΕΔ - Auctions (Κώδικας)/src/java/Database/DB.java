package Database;
import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.sql.DataSource;
import javax.naming.InitialContext;
import javax.naming.Context;
import javax.naming.NamingException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

public class DB
{  
    private static DataSource ds;
    
    public static boolean createDataSource()
    {
        InitialContext initialContext;
        try {
            initialContext = new InitialContext();
        } catch (NamingException ex) {
            Logger.getLogger(DB.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
       Context context;
        try {
            context = (Context) initialContext.lookup("java:comp/env");
        } catch (NamingException ex) {
            Logger.getLogger(DB.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
        try {
            ds = (DataSource) context.lookup("connpool");
        } catch (NamingException ex) {
            Logger.getLogger(DB.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
        
        return true;
    }
    
    private static Connection getConnection()
    {
        createDataSource();
        Connection connection = null;
        try {
            connection = ds.getConnection();
        } catch (SQLException ex) {
            Logger.getLogger(DB.class.getName()).log(Level.SEVERE, null, ex);
        }
        return connection;
    }
    
    public static ResultSet SelectQuery(String query)
    {
        Connection connection = getConnection();
        if (connection == null) {
            return null;
        }
        PreparedStatement st = null;
        ResultSet rs = null;
        try {
            st = connection.prepareStatement(query);
        } catch (SQLException ex) {
            try {
                if (!connection.isClosed()) {
                    connection.close();
                }
            } catch (SQLException exc) {
            }
        }
        try {
            if (st != null) {
                rs = st.executeQuery(query);
            }
        } catch (SQLException ex) {
            try {
                if (st != null) {
                    st.close();
                }
            } catch (SQLException exs) {
            } finally {
                try {
                    if (!connection.isClosed()) {
                        connection.close();
                    }
                } catch (SQLException exc) {
                }
            }
        }
        return rs;
    }
    
    public static boolean DoQuery(String query)
    {
        Boolean Success = false;
        Connection connection = getConnection();
        if (connection == null)
            return Success;
        PreparedStatement st = null;
        try
        {
            st = connection.prepareStatement(query);
            if (st != null)
            {
                st.executeUpdate(query);
                Success = true;
            }
        } 
        catch (SQLException ex) { System.out.printf("1", ex.getMessage());
        }
        finally
        {
            try
            {
                if (!connection.isClosed())
                    connection.close();
            }
            catch (SQLException ex) {}
            try
            {
                if (st != null)
                    st.close();
            }
            catch (SQLException ex) {}
        }
        return Success;
    }
     
    public static boolean DoTransaction(ArrayList<String> queries)
    {
        Connection connection = null;
        PreparedStatement[] st = new PreparedStatement[queries.size()];
        try
        {
            connection = getConnection();
            connection.setAutoCommit(false);

            for (int i = 0; i < queries.size(); i++) {
                st[i] = connection.prepareStatement(queries.get(i));
                st[i].executeUpdate();
            }
            
            connection.commit();
        }
        catch (SQLException e)
        {
            if (connection != null)
            {
                try {
                    connection.rollback();
                } catch (SQLException ex) {
                    Logger.getLogger(DB.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        } 
        finally
        {
            try 
            {
                if (connection != null && !connection.isClosed())
                  connection.close();
            } 
            catch (SQLException ex) {}
            for (PreparedStatement s : st) {
                try {
                    s.close();
                } catch (Exception ex) { }
            }
        }
        return true;
    }

    public static void CloseResources(ResultSet rs)
    {
        try
        {
            rs.getStatement().getConnection().close();
        }
        catch (Exception ex) {}
        try
        {
            rs.getStatement().close();
        }
        catch (Exception ex) {}
        try
        {
            rs.close();
        }
        catch (Exception ex) {}
    }
    
    public static String Log(HttpServletRequest request)
    {
        // Logout
        if (request.getParameter("logout") != null)
        {
            request.getSession().setAttribute("user", null);
            return "Logged out Successfully!";
        }
        // Login
        String Username = request.getParameter("uname");
        String Password = request.getParameter("pass");
        if (Username == null || Password == null)
            return null;
        HttpSession session = request.getSession();
        
        if (Username.equals("admin"))
        {
            if (Password.equals("admin"))
                session.setAttribute("user", "admin");
            else
                return null;
        }
        
        String Query = "SELECT * FROM user WHERE username = '" + Username + "' AND password = '" + Password + "';";
        ResultSet rs = SelectQuery(Query);
        if (rs == null)
            return "An error occured while trying to process your request";
        try
        {
            if (rs.next())
            {
                if (rs.getBoolean("approved"))
                {
                    session.setAttribute("userId", rs.getInt("id"));
                    session.setAttribute("user", rs.getString("username"));
                    return "Success";
                }
                else
                    return "Your account is not approved by the administrator";
            }
        }
        catch (SQLException ex) {}
        finally { CloseResources(rs); }
        return "Invalid username or password";
    }

}
