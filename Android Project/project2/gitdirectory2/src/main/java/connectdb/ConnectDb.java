package connectdb;

import java.sql.*;

public class ConnectDb {

    public static Connection getConnection() {
        String JDBC_DRIVER = "com.mysql.jdbc.Driver";
        String DB_URL = "jdbc:mysql://localhost/projectdb";

        String USER = "root";
        String PASS = "root";

        Connection conn = null;
        Statement stmt = null;
        try {
            Class.forName(JDBC_DRIVER);

            System.out.println("Connecting to database...");
            conn = DriverManager.getConnection(DB_URL, USER, PASS);

            return conn;
        } catch (Exception e) {

        }
        return conn;
    }
}
