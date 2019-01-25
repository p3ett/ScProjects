package state;

import java.awt.*;
import java.io.Serializable;

public class CurrentState implements Serializable {
    private float sensor1_thres = 0.5f;
    private float sensor2_thres_x = -1;
    private float sensor2_thres_y = -1;
    private float sensor2_thres_z = -10;

    private String topic1 = "iddev1";
    private String topic2 = "iddev2";
    private String topic3 = "results";

    private String server_ip = "127.0.0.1";
    private String server_port = "1883";

    private int ID;
    private String Mobile;
    private double Longitude;
    private double Latitude;
    private String Sensor1Type;
    private String Sensor2Type;
    private String DateTime;
    private float X1Value;
    private float Y1Value;
    private float Z1Value;
    private float X2Value;
    private float Y2Value;
    private float Z2Value;
    private boolean Confirmed;

    public CurrentState() {

    }

    public float getSensor1_thres() {
        return sensor1_thres;
    }

    public void setSensor1_thres(float sensor1_thres) {
        this.sensor1_thres = sensor1_thres;
    }

    public float getSensor2_thres_x() {
        return sensor2_thres_x;
    }

    public void setSensor2_thres_x(float sensor2_thres_x) {
        this.sensor2_thres_x = sensor2_thres_x;
    }

    public float getSensor2_thres_y() {
        return sensor2_thres_y;
    }

    public void setSensor2_thres_y(float sensor2_thres_y) {
        this.sensor2_thres_y = sensor2_thres_y;
    }

    public float getSensor2_thres_z() {
        return sensor2_thres_z;
    }

    public void setSensor2_thres_z(float sensor2_thres_z) {
        this.sensor2_thres_z = sensor2_thres_z;
    }

    public String getServer_ip() {
        return server_ip;
    }

    public void setsServer_ip(String server_ip) {
        this.server_ip = server_ip;
    }

    public String getServer_port() {
        return server_port;
    }

    public void setServer_port(String server_port) {
        this.server_port = server_port;
    }

    public String getTopic1() {
        return topic1;
    }

    public void setTopic1(String topic1) {
        this.topic1 = topic1;
    }

    public String getTopic2() {
        return topic2;
    }

    public void setTopic2(String topic2) {
        this.topic2 = topic2;
    }

    public String getTopic3() {
        return topic3;
    }

    public void setTopic3(String topic3) {
        this.topic3 = topic3;
    }

    public int getID() {return ID;}

    public void setID(int ID) {this.ID = ID;}

    public String getMobile() {return Mobile;}

    public void setMobile(String mobile) {Mobile = mobile;}

    public double getLongitude() {return Longitude;}

    public void setLongitude(double longitude) {Longitude = longitude;}

    public double getLatitude() {return Latitude;}

    public void setLatitude(double latitude) {Latitude = latitude;}

    public String getSensor1Type() {return Sensor1Type;}

    public void setSensor1Type(String sensor1Type) {Sensor1Type = sensor1Type;}

    public String getSensor2Type() {return Sensor2Type;}

    public void setSensor2Type(String sensor2Type) {Sensor2Type = sensor2Type;}

    public String getDateTime() {return DateTime;}

    public void setDateTime(String dateTime) {DateTime = dateTime;}

    public float getX1Value() {return X1Value;}

    public void setX1Value(float x1Value) {X1Value = x1Value;}

    public float getY1Value() {return Y1Value;}

    public void setY1Value(float y1Value) {Y1Value = y1Value;}

    public float getZ1Value() {return Z1Value;}

    public void setZ1Value(float z1Value) {Z1Value = z1Value;}

    public float getX2Value() {return X2Value;}

    public void setX2Value(float x2Value) {X2Value = x2Value;}

    public float getY2Value() {return Y2Value;}

    public void setY2Value(float y2Value) {Y2Value = y2Value;}

    public float getZ2Value() {return Z2Value;}

    public void setZ2Value(float z2Value) {Z2Value = z2Value;}

    public boolean isConfirmed() {return Confirmed;}

    public void setConfirmed(boolean confirmed) {Confirmed = confirmed;}
}
