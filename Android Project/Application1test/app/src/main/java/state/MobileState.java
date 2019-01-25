package state;

import java.io.Serializable;

/**
 * Created by alkiviadis on 21/12/16.
 */

public class MobileState implements Serializable {
    private String id;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }
}
