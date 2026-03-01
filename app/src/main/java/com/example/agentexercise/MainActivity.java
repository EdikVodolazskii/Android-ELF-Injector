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

        extractLib("libhello.so", R.raw.libhello);
        extractLib("libgadget.so", R.raw.libgadget);
    }


    public void load(View view) {

        String gadgetPath = getFilesDir().getAbsolutePath() + "/libgadget.so";
        myUtility.loadLib(gadgetPath);

        String absolutPath = getFilesDir().getAbsolutePath() + "/libhello.so";
        myUtility.loadLib(absolutPath);
    }

    public void unload(View view) {
        String absolutPath = getFilesDir().getAbsolutePath() + "/libhello.so";
        myUtility.unloadLib(absolutPath);
    }

    public void patch(View view) {
        String absolutPath = getFilesDir().getAbsolutePath() + "/libhello.so";
        myUtility.patchLib(absolutPath);
    }

    private void extractLib(String libname, int resourceId) {
        try {

            java.io.InputStream is = getResources().openRawResource(resourceId);
            java.io.File outFile = new java.io.File(getFilesDir(), libname);
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