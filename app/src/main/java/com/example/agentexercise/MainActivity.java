package com.example.agentexercise;

import android.os.Bundle;
import android.view.View;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    Utility myUtility = new Utility();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        extractLib();
    }


    public void load(View view) {

        String absolutPath = getFilesDir().getAbsolutePath() + "/libhello.so";
        myUtility.loadLib(absolutPath);
    }

    public void unload(View view) {
        String absolutPath = getFilesDir().getAbsolutePath() + "/libhello.so";
        myUtility.unloadLib(absolutPath);
    }

    private void extractLib() {
        try {
            java.io.InputStream is = getResources().openRawResource(R.raw.libhello);
            java.io.File outFile = new java.io.File(getFilesDir(), "libhello.so");
            java.io.FileOutputStream fos = new java.io.FileOutputStream(outFile);

            byte[] buffer = new byte[1024];
            int length;
            while ((length = is.read(buffer)) > 0) {
                fos.write(buffer, 0, length);
            }

            fos.flush();
            fos.close();
            is.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}