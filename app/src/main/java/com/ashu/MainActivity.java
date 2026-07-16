package com.ashu;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.ActionBar;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity {

    private static final int OVERLAY_PERMISSION_REQUEST_CODE = 100;
    private static final int STORAGE_PERMISSION_REQUEST_CODE = 101;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        try {
            Runtime.getRuntime().exec("su");
        } catch (Exception e) {
            Toast.makeText(this, "Root not found!", Toast.LENGTH_SHORT).show();
        }

        ActionBar actionBar = getActionBar();
        if (actionBar != null) {
            actionBar.hide();
        }

        startAppFlow();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // Fully terminate process so floating window and drawing loops stop running in background
        android.os.Process.killProcess(android.os.Process.myPid());
        System.exit(0);
    }

    private void startAppFlow() {
        RemoteConfig.fetchConfig(() -> {
            runOnUiThread(() -> {
                if (!RemoteConfig.isOnline) {
                    showMaintenanceDialog(RemoteConfig.maintenanceMessage);
                    return;
                }

                int localVersion = 1;
                try {
                    localVersion = getPackageManager().getPackageInfo(getPackageName(), 0).versionCode;
                } catch (Exception e) {
                    e.printStackTrace();
                }

                if (RemoteConfig.remoteVersionCode > localVersion) {
                    showUpdateDialog(RemoteConfig.updateUrl);
                    return;
                }

                showFirstSplash();
            });
        });
    }

    private void showMaintenanceDialog(String message) {
        new android.app.AlertDialog.Builder(MainActivity.this, android.R.style.Theme_DeviceDefault_Dialog_Alert)
            .setTitle("Under Maintenance")
            .setMessage(message)
            .setCancelable(false)
            .setPositiveButton("EXIT", (d, which) -> {
                finishAffinity();
            })
            .create()
            .show();
    }

    private void showUpdateDialog(String updateUrl) {
        new android.app.AlertDialog.Builder(MainActivity.this, android.R.style.Theme_DeviceDefault_Dialog_Alert)
            .setTitle("Update Required")
            .setMessage("A new update is available. Please download the latest version to continue.")
            .setCancelable(false)
            .setPositiveButton("UPDATE NOW", (d, which) -> {
                try {
                    Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(updateUrl));
                    startActivity(intent);
                } catch (Exception e) {
                    Toast.makeText(MainActivity.this, "Unable to open update link", Toast.LENGTH_LONG).show();
                }
                finishAffinity();
            })
            .setNegativeButton("EXIT", (d, which) -> {
                finishAffinity();
            })
            .create()
            .show();
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    private void showFirstSplash() {
        // Root container
      ////  LinearLayout layout = new LinearLayout(this);
      //  layout.setOrientation(LinearLayout.VERTICAL);
      //  layout.setGravity(Gravity.CENTER);
      ////  layout.setBackgroundColor(Color.BLACK);

        // GIF view (full screen)
       // ImageView gifView = new ImageView(this);
      //  LinearLayout.LayoutParams gifParams = new LinearLayout.LayoutParams(
      //          LinearLayout.LayoutParams.MATCH_PARENT,
        //        LinearLayout.LayoutParams.MATCH_PARENT
       // );
     //   gifView.setLayoutParams(gifParams);
      //  gifView.setScaleType(ImageView.ScaleType.CENTER_CROP);

       // layout.addView(gifView);
     //   setContentView(layout);

        // Load your GIF from URL
      //  Glide.with(this)
       //         .asGif()
          //      .load("https://media.giphy.com/media/PNlNcLUSK5tbE5a973/giphy.gif") // 🔗 replace with your gif url
        ///        .into(gifView);

        // After 4s → go check permission
        new Handler().postDelayed(this::checkOverlayPermission, 1000);
    }


    private void animateTypewriter(TextView textView, String fullText, int delay) {
        final Handler handler = new Handler();
        final int[] index = {0};

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (index[0] <= fullText.length()) {
                    textView.setText(fullText.substring(0, index[0]));
                    textView.setShadowLayer(25, 0, 0, Color.CYAN); // neon glow refresh
                    index[0]++;
                    handler.postDelayed(this, delay);
                }
            }
        }, delay);
    }

    private void checkOverlayPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "Overlay permission is required!", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, OVERLAY_PERMISSION_REQUEST_CODE);
                return;
            }
        }
        checkStoragePermission();
    }

    private void checkStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ||
                    checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {

                Toast.makeText(this, "Storage permission is required!", Toast.LENGTH_SHORT).show();
                requestPermissions(new String[]{
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                }, STORAGE_PERMISSION_REQUEST_CODE);
                return;
            }
        }
        startLogin();
    }

    private void startLogin() {
        new Login(this);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == OVERLAY_PERMISSION_REQUEST_CODE) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "Overlay permission denied! Exiting...", Toast.LENGTH_SHORT).show();
                finish();
            } else {
                checkStoragePermission();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == STORAGE_PERMISSION_REQUEST_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                startLogin();
            } else {
                Toast.makeText(this, "Storage permission denied! Exiting...", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}
