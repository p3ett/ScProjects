// resource : http://www.java2s.com/Tutorials/Java/JavaFX/0610__JavaFX_Pagination.htm

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Pagination;
import javafx.scene.control.Label;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.stage.Stage;
//
//public class Main extends Application {
//
//    private Pagination pagination;
//    String[] fonts = new String[]{};
//
//    public static void main(String[] args) throws Exception {
//        launch(args);
//    }
//
//    public int itemsPerPage() {
//        return 15;
//    }
//
//    public VBox createPage(int pageIndex) {
//        VBox box = new VBox(5);
//        int page = pageIndex * itemsPerPage();
//        for (int i = page; i < page + itemsPerPage(); i++) {
//            Label font = new Label(fonts[i]);
//            box.getChildren().add(font);
//        }
//        return box;
//    }
//
//    @Override
//    public void start(final Stage stage) throws Exception {
//        fonts = Font.getFamilies().toArray(fonts);
//        pagination = new Pagination(fonts.length/itemsPerPage(), 0);
//        pagination.setPageFactory((Integer pageIndex) -> createPage(pageIndex));
//
//        AnchorPane anchor = new AnchorPane();
//        AnchorPane.setTopAnchor(pagination, 10.0);
//        AnchorPane.setRightAnchor(pagination, 10.0);
//        AnchorPane.setBottomAnchor(pagination, 10.0);
//        AnchorPane.setLeftAnchor(pagination, 10.0);
//        anchor.getChildren().addAll(pagination);
//        Scene scene = new Scene(anchor, 400, 450);
//        stage.setScene(scene);
//        stage.show();
//    }
//}