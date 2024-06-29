package ok.computer;

import android.app.NativeActivity;
import android.os.Bundle;

public class MainActivity extends NativeActivity {

    // Used to load the 'computer' library on application startup.
    static {
        System.loadLibrary("main");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
}