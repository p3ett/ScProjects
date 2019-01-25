package state;

import java.io.Serializable;

public class CurrentState implements Serializable {
    private int milliseconds = 1000;

    private float sensor1_thres = 0.5f;
    private float sensor2_thres_x = -1;
    private float sensor2_thres_y = -1;
    private float sensor2_thres_z = -10;

    private String server_ip = "127.0.0.1";
    private String server_port = "1883";

    public CurrentState() {

    }

    public int getMilliseconds() {
        return milliseconds;
    }

    public void setMilliseconds(int milliseconds) {
        this.milliseconds = milliseconds;
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

    public void setServer_ip(String server_ip) {
        this.server_ip = server_ip;
    }

    public String getServer_port() {
        return server_port;
    }

    public void setServer_port(String server_port) {
        this.server_port = server_port;
    }
}
