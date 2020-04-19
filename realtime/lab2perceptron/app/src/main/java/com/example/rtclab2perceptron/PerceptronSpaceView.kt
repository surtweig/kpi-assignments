package com.example.rtclab2perceptron

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Rect
import android.util.AttributeSet
import android.view.View

class PerceptronSpaceView(context: Context) : View(context) {
    init {
        setWillNotDraw(false)
    }

    private val backgroundPaint = Paint(0).apply {
        style = Paint.Style.FILL
        color = 0x00ffff
    }

    private val axisPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        style = Paint.Style.FILL_AND_STROKE
        color = 0xffffff
        strokeWidth = 10f
    }

    private val textPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = 0xffffff
        textSize = 50f
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        // Try for a width based on our minimum
        val minw: Int = paddingLeft + paddingRight + suggestedMinimumWidth
        val w: Int = View.resolveSizeAndState(minw, widthMeasureSpec, 1)

        // Whatever the width ends up being, ask for a height that would let the pie
        // get as big as it can
        val minh: Int = View.MeasureSpec.getSize(w) + paddingBottom + paddingTop
        val h: Int = View.resolveSizeAndState(
            View.MeasureSpec.getSize(w),
            heightMeasureSpec,
            0
        )

        setMeasuredDimension(w, h)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        setBackgroundColor(0x00ff00);
        canvas.drawColor(0xffffff);
        
        canvas.apply {
            //drawRect(Rect(0,0,100,100), backgroundPaint)
            //drawCircle(50f, 50f,100f, axisPaint)
            drawText("Hello, world!", 0f, 0f, textPaint)
        }

    }
}