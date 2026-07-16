package com.ashu;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.view.View;

import java.util.Date;

public class DrawView extends View implements Runnable {
    Paint mStrokePaint;
    Paint mFilledPaint;
    Paint mTextPaint;
    Paint mBitMapPaint;
    Paint mRectPaint1;
    Paint mRectPaint2;

    Thread mThread;
    int FPS = 240;
    long sleepTime;
    Date time;

    public DrawView(Context context)
    {
        super(context, null, 0);
        InitializePaints();
        setFocusableInTouchMode(false);
        setBackgroundColor(0);
        time = new Date();
        sleepTime = (long)(1000 / FPS);
        mThread = new Thread(this);
        mThread.start();
    }

    @Override
    protected void onDraw(Canvas canvas)
    {
        if (canvas != null && getVisibility() == VISIBLE)
        {
            ClearCanvas(canvas);
            time.setTime(System.currentTimeMillis());
            Menu.OnDrawLoad(this, canvas);
        }
    }

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);
        while (mThread.isAlive() && !mThread.isInterrupted())
        {
            try
            {
                long t1 = System.currentTimeMillis();
                postInvalidate();
                long td = System.currentTimeMillis() - t1;
                Thread.sleep(Math.max(Math.min(0, sleepTime - td), sleepTime));
            }
            catch (InterruptedException it)
            {
                return;
            }
        }
    }

    public void InitializePaints()
    {
        mStrokePaint = new Paint();
        mStrokePaint.setStyle(Paint.Style.STROKE);
        mStrokePaint.setAntiAlias(true);

        mFilledPaint = new Paint();
        mFilledPaint.setStyle(Paint.Style.FILL);
        mFilledPaint.setAntiAlias(true);

        mTextPaint = new Paint();
        mTextPaint.setTypeface(Typeface.MONOSPACE);
        mTextPaint.setAntiAlias(true);
        mTextPaint.setTextAlign(Paint.Align.CENTER);

        mRectPaint1 = new Paint();
        mRectPaint1.setTypeface(Typeface.MONOSPACE);
        mRectPaint1.setAntiAlias(true);
        mRectPaint1.setTextAlign(Paint.Align.CENTER);

        mRectPaint2 = new Paint();
        mRectPaint2.setTypeface(Typeface.MONOSPACE);
        mRectPaint2.setAntiAlias(true);
        mRectPaint2.setTextAlign(Paint.Align.CENTER);

        mBitMapPaint = new Paint();
        mBitMapPaint.setAntiAlias(true);
    }

    public void ClearCanvas(Canvas cvs) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
    }

    public void DrawLogo(Canvas cvs, float posX, float posY, float width, float height, float alpha) {
        if (Menu.logoBitmap != null) {
            Paint p = new Paint(mBitMapPaint);
            p.setAlpha((int) alpha);
            android.graphics.RectF dst = new android.graphics.RectF(posX - width / 2, posY - height / 2, posX + width / 2, posY + height / 2);
            cvs.drawBitmap(Menu.logoBitmap, null, dst, p);
        }
    }

    public void DrawLine(Canvas cvs, int a, int r, int g, int b, float lineWidth, float fromX, float fromY, float toX, float toY) {
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        mStrokePaint.setStrokeWidth(lineWidth);
        cvs.drawLine(fromX, fromY, toX, toY, mStrokePaint);
    }

    public void DrawGradientRect(Canvas canvas, int startA, int startR, int startG, int startB,
                                 int endA, int endR, int endG, int endB,
                                 float x, float y, float width, float height) {
        Paint paint = new Paint();
        LinearGradient gradient = new LinearGradient(
                x, y, x + width, y + height,
                Color.argb(startA, startR, startG, startB),
                Color.argb(endA, endR, endG, endB),
                Shader.TileMode.CLAMP
        );
        paint.setShader(gradient);
        canvas.drawRect(x, y, x + width, y + height, paint);
    }


    public void DrawText(Canvas cvs, int a, int r, int g, int b, float stroke, String txt, float posX, float posY, float size)
    {
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        mTextPaint.setTextSize(size);

        if (getRight() > 1920 || getBottom() > 1920) {
            mTextPaint.setTextSize(4.0f + size);
        }
        else if (getRight() == 1920 || getBottom() == 1920) {
            mTextPaint.setTextSize(size + 2.0f);
        }
        else
            mTextPaint.setTextSize(size);

        cvs.drawText(txt, posX, posY, mTextPaint);
    }



    public void DrawText2(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.clearShadowLayer();
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        if (getRight() > 1920 || getBottom() > 1920) {
            mTextPaint.setShadowLayer(12.0f, 0.0f, 0.0f, Color.rgb(r, g, b));
            mTextPaint.setTextSize(4 + size);
        }
        else if (getRight() == 1920 || getBottom() == 1920) {
            mTextPaint.setTextSize(2 + size);
            mTextPaint.setShadowLayer(12.0f, 0.0f, 0.0f, Color.rgb(r, g, b));
        }
        else
            mTextPaint.setTextSize(size);
        mTextPaint.setShadowLayer(12.0f, 0.0f, 0.0f, Color.rgb(r, g, b));
        cvs.drawText(txt, posX, posY, mTextPaint);
    }

    public void DrawCircle(Canvas cvs, int a, int r, int g, int b, float stroke, float posX, float posY, float radius)
    {
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        mStrokePaint.setStrokeWidth(stroke);
        cvs.drawCircle(posX, posY, radius, mStrokePaint);
    }

    public void DrawFilledCircle(Canvas cvs, int a, int r, int g, int b, float posX, float posY, float radius)
    {
        mFilledPaint.setColor(Color.rgb(r, g, b));
        mFilledPaint.setAlpha(a);
        cvs.drawCircle(posX, posY, radius, mFilledPaint);
    }

    public void DrawRoundRect(Canvas cvs, int a, int r, int g, int b, float stroke, int rx, int ry, float x, float y, float width, float height) {
        mStrokePaint.setStrokeWidth(stroke);
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        cvs.drawRoundRect(x, y, x + width, y + height, rx,ry, mStrokePaint);
    }

    public void DrawRect(Canvas cvs, int a, int r, int g, int b, int stroke, float x, float y, float width, float height)
    {
        mStrokePaint.setStrokeWidth(stroke);
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        cvs.drawRect(x, y, x + width, y + height, mStrokePaint);
    }

    public void DrawTextRect(Canvas cvs, int a, int r, int g, int b, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, float x, float y)
    {
        mRectPaint1.setColor(Color.rgb(r, g, b));
        mRectPaint1.setStrokeWidth(1.1f);
        mRectPaint1.setAlpha(a);

        mRectPaint2.setColor(Color.rgb(0, 0, 255));
        mRectPaint2.setStrokeWidth(1.1f);
        mRectPaint2.setAlpha(150);

        cvs.drawRect(x - a1, y - a2, x + a3, y + a4, mRectPaint1);
        cvs.drawRect(x - a5, y - a6, x - a7, y + a8, mRectPaint2);
    }

    public void DrawFilledRect(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height)
    {
        mFilledPaint.setColor(Color.rgb(r, g, b));

        mFilledPaint.setAlpha(70);
        cvs.drawRect(x, y, x + width, y + height, mFilledPaint);
    }

    public void DrawFilledRectInfo(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height)
    {
        mFilledPaint.setColor(Color.rgb(r, g, b));

        mFilledPaint.setAlpha(a);
        cvs.drawRect(x, y, x + width, y + height, mFilledPaint);
    }
}
