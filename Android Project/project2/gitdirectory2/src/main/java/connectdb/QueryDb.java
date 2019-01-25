package connectdb;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class QueryDb {
    public static ResultSet SelectQuery(String query)
    {
        Connection connection = ConnectDb.getConnection();
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
        Connection connection = ConnectDb.getConnection();
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
}
