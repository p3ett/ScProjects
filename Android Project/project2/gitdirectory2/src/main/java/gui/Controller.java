package gui;

import connectdb.Crash;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import state.CurrentState;

import java.awt.*;
import java.util.ArrayList;

public class Controller {
    private CurrentState state = new CurrentState();

    public TextField idTopic1;
    public TextField idTopic2;
    public TextField idTopic3;

    public TextField idTH1;
    public TextField idTH2x;
    public TextField idTH2y;
    public TextField idTH2z;

    public TextField textfieldID;
    public TextField textfieldMobile;
    public TextField textfieldLongitude;
    public TextField textfieldLatitude;
    public TextField textfieldSensor1Type;
    public TextField textfieldSensor2Type;
    public TextField textfieldDateTime;
    public TextField textfieldX1Value;
    public TextField textfieldY1Value;
    public TextField textfieldZ1Value;
    public TextField textfieldX2Value;
    public TextField textfieldY2Value;
    public TextField textfieldZ2Value;
    public TextField textfieldConfirmed;

   // public Button buttonSearch;
   // public Button buttonRefresh;
   // public Button buttonClear;
   // public Button idApplyChanges;
   // public Button idMqttApplyChanges;

    @FXML
    public void initialize() {
        idTopic1.setText(state.getTopic1());
        idTopic2.setText(state.getTopic2());
        idTopic3.setText(state.getTopic3());

        idTH1.setText(state.getSensor1_thres() + "");
        idTH2x.setText(state.getSensor2_thres_x() + "");
        idTH2y.setText(state.getSensor2_thres_y() + "");
        idTH2z.setText(state.getSensor2_thres_z() + "");
    }


    public void handleSubmitSearchAction(ActionEvent actionEvent) {
        ArrayList<Crash> data = Crash.load(textfieldID, textfieldMobile, textfieldLongitude, textfieldLatitude, textfieldDateTime,
                textfieldSensor1Type,textfieldSensor2Type textfieldX1Value, textfieldY1Value, textfieldZ1Value, textfieldX2Value,
                textfieldY2Value, textfieldZ2Value, textfieldConfirmed);

        // pagination
        for (Crash line : data) {
            System.out.println(line.getId());
        }

        textfieldConfirmed.setText("hello");
    }

    public void handleSubmitApplySettingChangesAction(ActionEvent actionEvent) {
        String th1 = idTH1.getText();
        String th2x = idTH2x.getText();
        String th2y = idTH2y.getText();
        String th2z = idTH2z.getText();

        try {
            state.setSensor1_thres(Float.parseFloat(th1));
            state.setSensor2_thres_x(Float.parseFloat(th2x));
            state.setSensor2_thres_y(Float.parseFloat(th2y));
            state.setSensor2_thres_z(Float.parseFloat(th2z));
        } catch(Exception e) {

        }

    }

    public void handleSubmitApplyMqttSettingChangesAction(ActionEvent actionEvent) {
        state.setTopic1(idTopic1.getText());
        state.setTopic2(idTopic2.getText());
        state.setTopic3(idTopic3.getText());
    }



}
