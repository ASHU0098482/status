package com.ashu;

import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.RippleDrawable;
import android.os.Build;
import android.text.Html;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.HorizontalScrollView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.topjohnwu.superuser.Shell;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import android.animation.ArgbEvaluator;
import android.animation.ValueAnimator;
import android.view.animation.DecelerateInterpolator;

public class Menu {

    // Native Functions
    public static native void Functions();

    public static native void ChangesID(int ID, int Value);

    public static native void Init();

    private String target = "com.dts.freefiremax";
    private int injectType;

    // Variables Menu
    public static String userLicenseKey = "admin";
    private int buttonClick = 0;
    public static int PrimaryColor = 0xFFFFB800; // Golden accent
    public static int TabSelectedColor = 0xFFFFB800; // Golden accent for selected tabs
    private static Context context;
    private static Utils utils;

    private native String imageBase64();

    // Parte Do Sistema De Janela
    private WindowManager windowManager;
    private WindowManager.LayoutParams windowManagerParams;
    private FrameLayout frameLayout;

    // DrawView Global
    DrawView drawView;

    // Tab Management
    private static Map<String, LinearLayout> tabContentContainers = new HashMap<>();
    private static List<TextView> tabButtons = new ArrayList<>();
    private static String currentTab = "";

    // Parte do Draw
    WindowManager.LayoutParams windowManagerDrawViewParams;

    public static native void OnDrawLoad(DrawView drawView, Canvas canvas);

    public void DrawCanvas() {
        int LAYOUT_FLAG;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_PHONE;
        }

        drawView = new DrawView(context);
        windowManagerDrawViewParams = new WindowManager.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT,
                LAYOUT_FLAG,
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                        WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE |
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                        WindowManager.LayoutParams.FLAG_FULLSCREEN,
                PixelFormat.TRANSPARENT);
        windowManagerDrawViewParams.gravity = Gravity.CENTER;
        windowManager.addView(drawView, windowManagerDrawViewParams);
    }

    // Parte Do Template Do Menu
    private static ScrollView scrollView_center;
    private static LinearLayout tabsContainer;
    private static LinearLayout featuresScrollContainer;

    public Menu(Context globContext, int glob_injectType) {
        context = globContext;
        utils = new Utils(context);
        injectType = glob_injectType;
        if (context != null) {
            String saved = context.getSharedPreferences("ASHUPrefs", Context.MODE_PRIVATE)
                    .getString("saved_license", "");
            if (saved != null && !saved.trim().isEmpty()) {
                userLicenseKey = saved.trim();
            }
        }
        System.loadLibrary("hawdawdawdawda");
        onCreate();
    }

    public void onCreate() {
        onCreateSystemWindow();
        onCreateTemplate();
        showNoticeIfAvailable();
    }

    private void showNoticeIfAvailable() {
        if (RemoteConfig.showNotice && RemoteConfig.noticeMessage != null && !RemoteConfig.noticeMessage.isEmpty()) {
            new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(() -> {
                try {
                    String title = (RemoteConfig.noticeTitle != null && !RemoteConfig.noticeTitle.isEmpty())
                            ? RemoteConfig.noticeTitle
                            : "📢 Notice";
                    new android.app.AlertDialog.Builder(context, android.R.style.Theme_DeviceDefault_Dialog_Alert)
                            .setTitle(title)
                            .setMessage(RemoteConfig.noticeMessage)
                            .setPositiveButton("OK", (dialog, which) -> dialog.dismiss())
                            .create()
                            .show();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }, 500);
        }
    }

    public static class FontUtil {
        public static android.graphics.Typeface getAimkillFont(Context context) {
            return android.graphics.Typeface.createFromAsset(context.getAssets(), "fonts/aimkill_font.ttf");
        }
    }

    public static android.graphics.Bitmap logoBitmap;
    public static TextView statusBannerView;

    public static void showActiveToast(final String featureName) {
        if (context == null) return;
        new android.os.Handler(android.os.Looper.getMainLooper()).post(() -> {
            try {
                if (statusBannerView != null) {
                    statusBannerView.setText("⚡ ACTIVE: " + featureName.toUpperCase());
                    statusBannerView.setTextColor(Color.parseColor("#00E676"));
                }
                Toast toast = Toast.makeText(context, "🟢 " + featureName + " : ACTIVE", Toast.LENGTH_SHORT);
                toast.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL, 0, utils.FixDP(70));
                toast.show();
            } catch (Exception e) {
                Toast.makeText(context, "🟢 " + featureName + " : ACTIVE", Toast.LENGTH_SHORT).show();
            }
        });
    }

    // Criar Template - Modern Cyber Dark VIP Menu
    public void onCreateTemplate() {
        GradientDrawable gradientDrawable_container = new GradientDrawable();
        gradientDrawable_container.setColor(Color.parseColor("#F2161616")); // Dark frosted glass
        gradientDrawable_container.setCornerRadius(utils.FixDP(16));
        gradientDrawable_container.setStroke(utils.FixDP(1.8f), PrimaryColor); // Golden Neon Accent Border

        LinearLayout container = new LinearLayout(context);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
            container.setLayoutTransition(new android.animation.LayoutTransition());
        }
        container.setOrientation(LinearLayout.VERTICAL);
        container.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        // Main menu container - Sleek width
        final LinearLayout container_menu = new LinearLayout(context);
        container_menu.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(265),
                ViewGroup.LayoutParams.WRAP_CONTENT));
        container_menu.setVisibility(View.GONE);
        container_menu.setOrientation(LinearLayout.VERTICAL);
        container_menu.setBackground(gradientDrawable_container);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            container_menu.setElevation(utils.FixDP(12));
        }

        // Floating icon
        final ImageBase64 icon_cheat = new ImageBase64(context);
        icon_cheat.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(60),
                utils.FixDP(60)));
        android.graphics.drawable.Drawable placeholderDrawable = null;
        try {
            byte[] decodeImageBase64 = android.util.Base64.decode(imageBase64(), android.util.Base64.DEFAULT);
            logoBitmap = android.graphics.BitmapFactory.decodeByteArray(decodeImageBase64, 0, decodeImageBase64.length);
            placeholderDrawable = new android.graphics.drawable.BitmapDrawable(context.getResources(), logoBitmap);
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (placeholderDrawable != null) {
            icon_cheat.setImageDrawable(placeholderDrawable);
        }
        if (RemoteConfig.floatingIconUrl != null && !RemoteConfig.floatingIconUrl.isEmpty()) {
            String floatUrl = RemoteConfig.floatingIconUrl;
            if (floatUrl.contains("?")) {
                floatUrl += "&t=" + System.currentTimeMillis();
            } else {
                floatUrl += "?t=" + System.currentTimeMillis();
            }
            com.bumptech.glide.Glide.with(context)
                    .asBitmap()
                    .load(floatUrl)
                    .diskCacheStrategy(com.bumptech.glide.load.engine.DiskCacheStrategy.NONE)
                    .skipMemoryCache(true)
                    .into(new com.bumptech.glide.request.target.CustomTarget<android.graphics.Bitmap>() {
                        @Override
                        public void onResourceReady(@androidx.annotation.NonNull android.graphics.Bitmap resource,
                                @androidx.annotation.Nullable com.bumptech.glide.request.transition.Transition<? super android.graphics.Bitmap> transition) {
                            if (resource != null) {
                                logoBitmap = resource;
                                icon_cheat.setImageBitmap(logoBitmap);
                            }
                        }

                        @Override
                        public void onLoadCleared(
                                @androidx.annotation.Nullable android.graphics.drawable.Drawable placeholder) {
                        }
                    });
        }
        GradientDrawable iconBackground = new GradientDrawable();
        iconBackground.setShape(GradientDrawable.OVAL);
        iconBackground.setColor(Color.TRANSPARENT);
        icon_cheat.setBackground(iconBackground);
        icon_cheat.setPadding(utils.FixDP(5), utils.FixDP(5), utils.FixDP(5), utils.FixDP(5));
        icon_cheat.setOnTouchListener(onTouchListener());
        icon_cheat.setOnClickListener(view -> {
            icon_cheat.setVisibility(View.GONE);
            container_menu.setVisibility(View.VISIBLE);
            try {
                windowManager.updateViewLayout(frameLayout, windowManagerParams);
            } catch (Exception e) {
                e.printStackTrace();
            }
        });

        // Top section of the menu with Header branding
        LinearLayout container_top = new LinearLayout(context);
        container_top.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        container_top.setPadding(
                utils.FixDP(12),
                utils.FixDP(10),
                utils.FixDP(12),
                utils.FixDP(8));
        container_top.setGravity(Gravity.CENTER_VERTICAL);
        container_top.setOrientation(LinearLayout.HORIZONTAL);

        // Menu icon in top bar
        ImageBase64 icon_menu = new ImageBase64(context);
        icon_menu.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(36),
                utils.FixDP(36)));
        if (placeholderDrawable != null) {
            icon_menu.setImageDrawable(placeholderDrawable);
        }
        if (RemoteConfig.floatingIconUrl != null && !RemoteConfig.floatingIconUrl.isEmpty()) {
            com.bumptech.glide.Glide.with(context)
                    .load(RemoteConfig.floatingIconUrl)
                    .placeholder(placeholderDrawable)
                    .error(placeholderDrawable)
                    .into(icon_menu);
        }

        // Title layout
        LinearLayout titleCol = new LinearLayout(context);
        titleCol.setOrientation(LinearLayout.VERTICAL);
        LinearLayout.LayoutParams titleColParams = new LinearLayout.LayoutParams(
                0, ViewGroup.LayoutParams.WRAP_CONTENT, 1.0f);
        titleColParams.setMargins(utils.FixDP(8), 0, 0, 0);
        titleCol.setLayoutParams(titleColParams);

        TextView menuTitle = new TextView(context);
        String appDisplayName = (RemoteConfig.appName != null && !RemoteConfig.appName.isEmpty())
                ? RemoteConfig.appName : "JACK PANEL";
        menuTitle.setText(appDisplayName);
        menuTitle.setTextSize(13.5f);
        menuTitle.setTextColor(PrimaryColor);
        menuTitle.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
        titleCol.addView(menuTitle);

        TextView menuSub = new TextView(context);
        menuSub.setText("VIP AIMBOT & ESP");
        menuSub.setTextSize(9f);
        menuSub.setTextColor(Color.parseColor("#94A3B8"));
        menuSub.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
        titleCol.addView(menuSub);

        // Live badge on top-right
        TextView liveBadge = new TextView(context);
        liveBadge.setText("● LIVE");
        liveBadge.setTextSize(8.5f);
        liveBadge.setTextColor(Color.parseColor("#00E676"));
        liveBadge.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
        liveBadge.setPadding(utils.FixDP(6), utils.FixDP(2), utils.FixDP(6), utils.FixDP(2));
        GradientDrawable liveBadgeBg = new GradientDrawable();
        liveBadgeBg.setColor(Color.parseColor("#153322"));
        liveBadgeBg.setCornerRadius(utils.FixDP(6));
        liveBadgeBg.setStroke(utils.FixDP(1), Color.parseColor("#00E676"));
        liveBadge.setBackground(liveBadgeBg);

        container_top.addView(icon_menu);
        container_top.addView(titleCol);
        container_top.addView(liveBadge);

        // Glowing divider line
        View headerDivider = new View(context);
        headerDivider.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(1)));
        headerDivider.setBackgroundColor(Color.parseColor("#2E2E2E"));

        // Center section where features will be displayed
        final LinearLayout container_center = new LinearLayout(context);
        container_center.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                utils.FixDP(230)));
        container_center.setGravity(Gravity.CENTER);
        container_center.setPadding(utils.FixDP(6), utils.FixDP(4), utils.FixDP(6), utils.FixDP(4));

        // Scroll view for features
        scrollView_center = new ScrollView(context);
        scrollView_center.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        scrollView_center.setVerticalScrollBarEnabled(false);
        scrollView_center.setPadding(0, utils.FixDP(2), 0, utils.FixDP(2));

        // Container for all feature tabs
        featuresScrollContainer = new LinearLayout(context);
        featuresScrollContainer.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        featuresScrollContainer.setOrientation(LinearLayout.VERTICAL);

        scrollView_center.addView(featuresScrollContainer);

        // Progress bar
        final ProgressBar progressBar = new ProgressBar(context);
        progressBar.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(50),
                utils.FixDP(50)));
        progressBar.getIndeterminateDrawable().setColorFilter(PrimaryColor, PorterDuff.Mode.SRC_IN);

        // Bottom section with status banner and close button
        LinearLayout container_bottom = new LinearLayout(context);
        container_bottom.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        container_bottom.setPadding(
                utils.FixDP(10),
                utils.FixDP(4),
                utils.FixDP(10),
                utils.FixDP(10));
        container_bottom.setOrientation(LinearLayout.VERTICAL);
        container_bottom.setGravity(Gravity.CENTER);

        // Active Status Chip
        statusBannerView = new TextView(context);
        statusBannerView.setText("⚡ SYSTEM: READY");
        statusBannerView.setTextSize(10f);
        statusBannerView.setTextColor(Color.parseColor("#94A3B8"));
        statusBannerView.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
        statusBannerView.setGravity(Gravity.CENTER);
        statusBannerView.setPadding(0, 0, 0, utils.FixDP(6));
        container_bottom.addView(statusBannerView);

        // Button styling - Golden Pill Button
        GradientDrawable gradientDrawable_inject_close = new GradientDrawable();
        gradientDrawable_inject_close.setColor(PrimaryColor);
        gradientDrawable_inject_close.setCornerRadius(utils.FixDP(25));
        RippleDrawable rippleDrawable = new RippleDrawable(
                ColorStateList.valueOf(0xFF444444),
                gradientDrawable_inject_close,
                null);

        // Inject/Close button
        final Button inject_close = new Button(context);
        inject_close.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                utils.FixDP(38)));
        inject_close.setPadding(0, 0, 0, 0);
        inject_close.setText("INJECT");
        inject_close.setTextSize(13);
        inject_close.setTextColor(0xFFFFFFFF);
        inject_close.setBackground(rippleDrawable);
        inject_close.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);

        inject_close.setOnClickListener(view -> {
            if (buttonClick == 0) {
                Toast.makeText(context, "Processing injection...", Toast.LENGTH_SHORT).show();

                new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(() -> {
                    try {
                        Init();
                        Functions();
                    } catch (UnsatisfiedLinkError e) {
                        e.printStackTrace();
                    }
                    progressBar.setVisibility(View.GONE);
                    inject_close.setText("CLOSE");
                    container_center.removeAllViews();
                    container_center.addView(scrollView_center);
                    buttonClick++;
                    Toast.makeText(context, "✅ Injection successful!", Toast.LENGTH_SHORT).show();
                }, 800);

            } else if (buttonClick == 1) {
                icon_cheat.setVisibility(View.VISIBLE);
                container_menu.setVisibility(View.GONE);
                try {
                    windowManager.updateViewLayout(frameLayout, windowManagerParams);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        // Add all views to their respective containers
        frameLayout.addView(container);
        container.addView(icon_cheat);
        container.addView(container_menu);

        container_menu.addView(container_top);
        container_menu.addView(headerDivider);

        container_menu.addView(container_center);
        container_center.addView(progressBar);

        container_menu.addView(container_bottom);
        container_bottom.addView(inject_close);
    }

    // Create System Window
    public void onCreateSystemWindow() {
        int LAYOUT_FLAG;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_PHONE;
        }

        frameLayout = new FrameLayout(context);
        frameLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        frameLayout.setOnTouchListener(onTouchListener());
        frameLayout.setAlpha(0.95f);

        windowManagerParams = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.WRAP_CONTENT,
                LAYOUT_FLAG,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_OVERSCAN |
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN |
                        WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM |
                        WindowManager.LayoutParams.FLAG_SPLIT_TOUCH,
                PixelFormat.TRANSPARENT);
        windowManagerParams.gravity = Gravity.TOP | Gravity.LEFT;
        windowManagerParams.x = 50;
        windowManagerParams.y = 100;

        windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DrawCanvas();
        windowManager.addView(frameLayout, windowManagerParams);
    }

    // OnTouchListener for menu
    private View.OnTouchListener onTouchListener() {
        return new View.OnTouchListener() {
            private static final int TOUCH_MOVE_THRESHOLD = 8;
            private int x;
            private int y;
            private int initialX;
            private int initialY;
            private boolean isMoving = false;

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        x = (int) event.getRawX();
                        y = (int) event.getRawY();
                        initialX = x;
                        initialY = y;
                        isMoving = false;
                        frameLayout.setAlpha(0.8f);
                        return true;

                    case MotionEvent.ACTION_MOVE:
                        int nowX = (int) event.getRawX();
                        int nowY = (int) event.getRawY();

                        int totalMoveX = Math.abs(nowX - initialX);
                        int totalMoveY = Math.abs(nowY - initialY);

                        if (!isMoving && (totalMoveX > TOUCH_MOVE_THRESHOLD || totalMoveY > TOUCH_MOVE_THRESHOLD)) {
                            isMoving = true;
                        }

                        if (isMoving) {
                            int movedX = nowX - x;
                            int movedY = nowY - y;
                            x = nowX;
                            y = nowY;
                            windowManagerParams.x = windowManagerParams.x + movedX;
                            windowManagerParams.y = windowManagerParams.y + movedY;
                            windowManager.updateViewLayout(frameLayout, windowManagerParams);
                        }
                        return true;

                    case MotionEvent.ACTION_UP:
                        if (!isMoving) {
                            v.performClick();
                        }
                        frameLayout.setAlpha(0.95f);
                        return true;

                    default:
                        break;
                }
                return false;
            }
        };
    }

    // -------------------- NEW TAB METHODS --------------------

    /**
     * Create a new tab and its content container (hidden buttons)
     * 
     * @param tabName name of the tab
     */
    public static void addTab(final String tabName) {
        final boolean isFirstTab = tabButtons.isEmpty();

        // Tab button (hidden by default)
        final TextView tabButton = new TextView(context);
        tabButton.setVisibility(View.GONE); // ⬅️ HIDE IT
        tabButtons.add(tabButton);

        if (tabsContainer != null) {
            // Still add to container but invisible
            tabsContainer.addView(tabButton);
        }

        // Create content for this tab
        LinearLayout tabContent = new LinearLayout(context);
        tabContent.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        tabContent.setOrientation(LinearLayout.VERTICAL);
        tabContent.setPadding(utils.FixDP(10), utils.FixDP(5), utils.FixDP(10), utils.FixDP(5));
        tabContent.setVisibility(isFirstTab ? View.VISIBLE : View.GONE);

        // Store & add it
        tabContentContainers.put(tabName, tabContent);
        featuresScrollContainer.addView(tabContent);

        if (isFirstTab)
            currentTab = tabName;
    }

    /**
     * Select a tab and show its content
     */
    private static void selectTab(String tabName) {
        if (tabName.equals(currentTab))
            return;

        for (Map.Entry<String, LinearLayout> entry : tabContentContainers.entrySet()) {
            entry.getValue().setVisibility(entry.getKey().equals(tabName) ? View.VISIBLE : View.GONE);
        }

        currentTab = tabName;
    }

    /**
     * Add a category heading within the current tab
     */
    /**
     * Add a category heading within the current tab
     */
    public static void addCategory(String name) {
        if (currentTab.isEmpty() || !tabContentContainers.containsKey(currentTab)) {
            return; // No tab selected
        }

        GradientDrawable gradientDrawable = new GradientDrawable();
        gradientDrawable.setColor(Color.parseColor("#221A0F")); // Amber tinted dark badge
        gradientDrawable.setCornerRadius(utils.FixDP(6));
        gradientDrawable.setStroke(utils.FixDP(1), PrimaryColor);

        LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                utils.FixDP(26)));
        linearLayout.setBackground(gradientDrawable);
        linearLayout.setGravity(Gravity.CENTER_VERTICAL);
        linearLayout.setPadding(utils.FixDP(10), 0, utils.FixDP(10), 0);

        LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) linearLayout.getLayoutParams();
        params.setMargins(0, utils.FixDP(8), 0, utils.FixDP(4));
        linearLayout.setLayoutParams(params);

        TextView textView = new TextView(context);
        textView.setText("🎯  " + name.toUpperCase());
        textView.setTextSize(10.5f);
        textView.setTextColor(PrimaryColor);
        textView.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);

        linearLayout.addView(textView);
        tabContentContainers.get(currentTab).addView(linearLayout);
    }

    /**
     * Add a switch to the current tab
     */
    public static void addSwitch(String name, final int ID) {
        LinearLayout rowCard = new LinearLayout(context);
        LinearLayout.LayoutParams rowParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        rowParams.setMargins(0, utils.FixDP(2.5f), 0, utils.FixDP(2.5f));
        rowCard.setLayoutParams(rowParams);
        rowCard.setPadding(utils.FixDP(10), utils.FixDP(6), utils.FixDP(8), utils.FixDP(6));
        rowCard.setOrientation(LinearLayout.HORIZONTAL);
        rowCard.setGravity(Gravity.CENTER_VERTICAL);

        GradientDrawable rowBg = new GradientDrawable();
        rowBg.setColor(Color.parseColor("#1C1C1C"));
        rowBg.setCornerRadius(utils.FixDP(8));
        rowBg.setStroke(utils.FixDP(1), Color.parseColor("#2C2C2C"));
        rowCard.setBackground(rowBg);

        final TextView textView = new TextView(context);
        textView.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1));
        textView.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        textView.setText(name);
        textView.setTextSize(11.5f);
        textView.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);

        final SwitchStyle switchStyle = new SwitchStyle(context);
        switchStyle.setLayoutParams(new LinearLayout.LayoutParams(utils.FixDP(38), utils.FixDP(20)));

        final int colorOff = 0xFF888888;
        final int colorOn = 0xFFFFFFFF;

        textView.setTextColor(switchStyle.isChecked() ? colorOn : colorOff);

        switchStyle.setOnCheckedChangeListener((view, isChecked) -> {
            ChangesID(ID, 0);

            if (isChecked) {
                showActiveToast(name);
            }

            int startColor = textView.getCurrentTextColor();
            int endColor = isChecked ? colorOn : colorOff;

            ValueAnimator colorAnimation = ValueAnimator.ofObject(new ArgbEvaluator(), startColor, endColor);
            colorAnimation.setDuration(200);
            colorAnimation.setInterpolator(new DecelerateInterpolator());
            colorAnimation.addUpdateListener(animator -> textView.setTextColor((int) animator.getAnimatedValue()));
            colorAnimation.start();
        });

        rowCard.setOnClickListener(view -> switchStyle.setChecked(!switchStyle.isChecked()));

        rowCard.addView(textView);
        rowCard.addView(switchStyle);
        tabContentContainers.get(currentTab).addView(rowCard);
    }

    public static void addSeekBar(final String name, int value, int max, final String type, final int ID) {
        LinearLayout rowCard = new LinearLayout(context);
        LinearLayout.LayoutParams rowParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        rowParams.setMargins(0, utils.FixDP(2.5f), 0, utils.FixDP(2.5f));
        rowCard.setLayoutParams(rowParams);
        rowCard.setPadding(utils.FixDP(10), utils.FixDP(6), utils.FixDP(10), utils.FixDP(6));
        rowCard.setOrientation(LinearLayout.VERTICAL);

        GradientDrawable rowBg = new GradientDrawable();
        rowBg.setColor(Color.parseColor("#1C1C1C"));
        rowBg.setCornerRadius(utils.FixDP(8));
        rowBg.setStroke(utils.FixDP(1), Color.parseColor("#2C2C2C"));
        rowCard.setBackground(rowBg);

        final TextView textView = new TextView(context);
        textView.setText(name + ": " + value + type);
        textView.setTextSize(11f);
        textView.setTextColor(0xFFFFFFFF);
        textView.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
        if (type.equals("Color")) {
            if (value == 0) {
                textView.setText(Html.fromHtml(name + ": <font color='#ffffff'>" + "White" + "</font>"));
            } else if (value == 1) {
                textView.setText(Html.fromHtml(name + ": <font color='#00FF00'>" + "Green" + "</font>"));
            } else if (value == 2) {
                textView.setText(Html.fromHtml(name + ": <font color='#0000FF'>" + "Blue" + "</font>"));
            } else if (value == 3) {
                textView.setText(Html.fromHtml(name + ": <font color='#FF0000'>" + "Red" + "</font>"));
            } else if (value == 4) {
                textView.setText(Html.fromHtml(name + ": <font color='#000000'>" + "Black" + "</font>"));
            } else if (value == 5) {
                textView.setText(Html.fromHtml(name + ": <font color='#FFFF00'>" + "Yellow" + "</font>"));
            } else if (value == 6) {
                textView.setText(Html.fromHtml(name + ": <font color='#00FFFF'>" + "Cyan" + "</font>"));
            } else if (value == 7) {
                textView.setText(Html.fromHtml(name + ": <font color='#FF00FF'>" + "Magenta" + "</font>"));
            } else if (value == 8) {
                textView.setText(Html.fromHtml(name + ": <font color='#808080'>" + "Gray" + "</font>"));
            } else if (value == 9) {
                textView.setText(Html.fromHtml(name + ": <font color='#A020F0'>" + "Purple" + "</font>"));
            }
        } else if (type.equals("BoxType")) {
            if (value == 0) {
                textView.setText(name.concat(": Stroke"));
            } else if (value == 1) {
                textView.setText(name.concat(": Filled"));
            } else if (value == 2) {
                textView.setText(name.concat(": Rounded"));
            }
        } else if (type.equals("LineType")) {
            if (value == 0) {
                textView.setText(name.concat(": Top"));
            } else if (value == 1) {
                textView.setText(name.concat(": Center"));
            } else if (value == 2) {
                textView.setText(name.concat(": Bottom"));
            }
        }

        SeekBar seekBar = new SeekBar(context);
        seekBar.getThumb().setColorFilter(PrimaryColor, PorterDuff.Mode.SRC_IN);
        seekBar.getProgressDrawable().setColorFilter(PrimaryColor, PorterDuff.Mode.SRC_IN);
        seekBar.setProgress(value);
        seekBar.setMax(max);
        if (type.equals("Color")) {
            seekBar.setMax(9);
        } else if (type.equals("BoxType")) {
            seekBar.setMax(2);
        } else if (type.equals("LineType")) {
            seekBar.setMax(2);
        }

        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                if (type.equals("Color")) {
                    if (i == 0) {
                        textView.setText(Html.fromHtml(name + ": <font color='#ffffff'>" + "White" + "</font>"));
                    } else if (i == 1) {
                        textView.setText(Html.fromHtml(name + ": <font color='#00FF00'>" + "Green" + "</font>"));
                    } else if (i == 2) {
                        textView.setText(Html.fromHtml(name + ": <font color='#0000FF'>" + "Blue" + "</font>"));
                    } else if (i == 3) {
                        textView.setText(Html.fromHtml(name + ": <font color='#FF0000'>" + "Red" + "</font>"));
                    } else if (i == 4) {
                        textView.setText(Html.fromHtml(name + ": <font color='#000000'>" + "Black" + "</font>"));
                    } else if (i == 5) {
                        textView.setText(Html.fromHtml(name + ": <font color='#FFFF00'>" + "Yellow" + "</font>"));
                    } else if (i == 6) {
                        textView.setText(Html.fromHtml(name + ": <font color='#00FFFF'>" + "Cyan" + "</font>"));
                    } else if (i == 7) {
                        textView.setText(Html.fromHtml(name + ": <font color='#FF00FF'>" + "Magenta" + "</font>"));
                    } else if (i == 8) {
                        textView.setText(Html.fromHtml(name + ": <font color='#808080'>" + "Gray" + "</font>"));
                    } else if (i == 9) {
                        textView.setText(Html.fromHtml(name + ": <font color='#A020F0'>" + "Purple" + "</font>"));
                    }
                } else if (type.equals("BoxType")) {
                    if (i == 0) {
                        textView.setText(name.concat(": Stroke"));
                    } else if (i == 1) {
                        textView.setText(name.concat(": Filled"));
                    } else if (i == 2) {
                        textView.setText(name.concat(": Corner"));
                    }
                } else if (type.equals("LineType")) {
                    if (i == 0) {
                        textView.setText(name.concat(": Top"));
                    } else if (i == 1) {
                        textView.setText(name.concat(": Center"));
                    } else if (i == 2) {
                        textView.setText(name.concat(": Bottom"));
                    }
                } else {
                    textView.setText(name.concat(": ") + i + type);
                }

                ChangesID(ID, i);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        rowCard.addView(textView);
        rowCard.addView(seekBar);
        tabContentContainers.get(currentTab).addView(rowCard);
    }

    // Injection methods
    private boolean InjectX86(String Lib) {
        try {
            String injector = context.getApplicationInfo().nativeLibraryDir + File.separator + "libupakul.so";
            String payload_source = context.getApplicationInfo().nativeLibraryDir + File.separator + Lib;
            String payload_dest = "/data/local/" + Lib;
            String payload_dest2 = "/data/local/libifuhiufoi.so";

            Shell.su("cp " + payload_source + " " + payload_dest).exec();
            Shell.su("cp " + injector + " " + payload_dest2).exec();
            Shell.su("su -c chmod 777 " + payload_dest).exec();
            Shell.su("su -c chmod 777 " + payload_dest2).exec();
            Shell.su("su -c " + payload_dest2 + " " + target + " " + payload_dest).exec();
            Shell.su("rm -f " + payload_dest).exec();
            Shell.su("rm -f " + payload_dest2).exec();
            Functions();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(context, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
            return false;
        }
    }

    private boolean InjectX32(String Lib) {
        try {
            String injector = context.getApplicationInfo().nativeLibraryDir + File.separator + "libinjectMobile.so";
            String payload_source = context.getApplicationInfo().nativeLibraryDir + File.separator + Lib;
            String payload_dest = "/data/local/" + Lib;
            String payload_dest2 = "/data/local/libinject.so";

            Shell.su("cp " + payload_source + " " + payload_dest).exec();
            Shell.su("cp " + injector + " " + payload_dest2).exec();
            Shell.su("su -c chmod 755 " + payload_dest).exec();
            Shell.su("su -c chmod 777 " + payload_dest2).exec();
            Shell.su("su -c " + payload_dest2 + " -f -n " + target + " -so " + payload_dest + " --hide-memory").exec();
            Shell.su("rm -f " + payload_dest).exec();
            Shell.su("rm -f " + payload_dest2).exec();
            Functions();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(context, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
            return false;
        }
    }
}