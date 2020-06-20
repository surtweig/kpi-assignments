package com.example.rtclab2perceptron


import android.content.Context
import android.content.res.Resources
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import android.view.ViewConfiguration
import androidx.core.content.res.ResourcesCompat
import kotlin.math.abs

// Stroke width for the the paint.
private const val STROKE_WIDTH = 6f

/**
 * Custom view that follows touch events to draw on a canvas.
 */
class MyCanvasView(context: Context, attrs: AttributeSet) : View(context, attrs) {

    data class Point(var posX: Float = 0f, var posY: Float = 0f, var color: Int = 0)
    private val pointRadius: Float = 48f
    public val spaceWidth: Float = 10f
    public val spaceHeight: Float = 10f
    private val points: MutableList<Point> = mutableListOf<Point>()

    public fun AddPoint(X: Float, Y: Float, C: Int, redraw: Boolean = true) {
        points.add(Point(X, Y, C))
        if (redraw) {
            drawPoints()
            invalidate()
        }
    }

    public fun ClearPoints() {
        points.clear()
        drawPoints()
        invalidate()
    }

    public fun ViewToSpaceX(X: Float): Float {
        return (X/width)*spaceWidth
    }

    public fun ViewToSpaceY(Y: Float): Float {
        return (1f - Y/height)*spaceHeight
    }

    public fun SpaceToViewX(X: Float): Float {
        return (X/spaceWidth)*width
    }

    public fun SpaceToViewY(Y: Float): Float {
        return (1f - Y/spaceHeight)*height
    }

    private fun drawPoints() {
        for (p in points.iterator()) {
            val tpx = SpaceToViewX(p.posX)
            val tpy = SpaceToViewY(p.posY)
            if (p.color == 0)
                extraCanvas.drawCircle(tpx, tpy, pointRadius, pointNegPaint)
            else
                extraCanvas.drawCircle(tpx, tpy, pointRadius, pointPosPaint)
            //val pf = perceptf(p)
            //extraCanvas.drawText("%.0f".format(pf), tpx, tpy, textPaint)
        }
    }

    private fun pickPoint(X: Float, Y: Float): Point? {
        val pr = (pointRadius/width)*spaceWidth
        val pr2 = pr*pr
        for (p in points.iterator()) {
            val dX = p.posX - X
            val dY = p.posY - Y
            if (dX*dX <= pr2 && dY*dY <= pr2)
                return p
        }
        return null
    }

    public var AutoRedraw: Boolean = false
    public var P: Float = 4f
    public var Sigma: Float = 0.1f
    public var Iters: Int = 100
    public var Weight: Point = Point(0f, 0f, 0)
    public var LearnDuration: Long = 0
    public val MaxLearnDuration: Long = 1200

    private fun perceptf(inp: Point): Float {
        val dot = inp.posX*Weight.posX + inp.posY*Weight.posY
        if (dot > P)
            return 1f
        else
            return 0f
    }

    private fun learnStep(inp: Point) {
        val yc = perceptf(inp)
        val yd = inp.color
        val d = Sigma*(yd-yc)
        Weight.posX += d*inp.posX
        Weight.posY += d*inp.posY
    }

    public fun startLearning() {
        Weight = Point(0f, 0f, 0)
        val learnStartTimestamp = System.currentTimeMillis()
        for (i in 1..Iters) {
            if (System.currentTimeMillis() - learnStartTimestamp > MaxLearnDuration)
                break
            for (p in points.iterator()) {
                learnStep(p)
            }
        }
        LearnDuration = System.currentTimeMillis() - learnStartTimestamp
        invalidate()
    }

    // Holds the path you are currently drawing.
    private var path = Path()
    private var selectedPoint: Point? = null
    private val drawColor = ResourcesCompat.getColor(resources, R.color.colorPaint, null)
    private val pointPosColor = ResourcesCompat.getColor(resources, R.color.colorPositivePoint, null)
    private val pointNegColor = ResourcesCompat.getColor(resources, R.color.colorNegativePoint, null)
    private val backgroundColor = ResourcesCompat.getColor(resources, R.color.colorBackground, null)
    private lateinit var extraCanvas: Canvas
    private lateinit var extraBitmap: Bitmap
    private lateinit var frame: Rect

    // Set up the paint with which to draw.
    private val textPaint = Paint().apply {
        color = drawColor
        // Smooths out edges of what is drawn without affecting shape.
        isAntiAlias = true
        // Dithering affects how colors with higher-precision than the device are down-sampled.
        isDither = true
        //style = Paint.Style.STROKE // default: FILL
        //strokeJoin = Paint.Join.ROUND // default: MITER
        //strokeCap = Paint.Cap.ROUND // default: BUTT
        strokeWidth = STROKE_WIDTH // default: Hairline-width (really thin)
        textSize = 64f
    }

    private val outlinePaint = Paint().apply {
        color = drawColor
        // Smooths out edges of what is drawn without affecting shape.
        isAntiAlias = true
        // Dithering affects how colors with higher-precision than the device are down-sampled.
        isDither = true
        style = Paint.Style.STROKE // default: FILL
        //strokeJoin = Paint.Join.ROUND // default: MITER
        //strokeCap = Paint.Cap.ROUND // default: BUTT
        strokeWidth = STROKE_WIDTH // default: Hairline-width (really thin)

    }

    private val pointPosPaint = Paint().apply {
        color = pointPosColor
        isAntiAlias = true
        isDither = true
        //alpha = 128
    }

    private val pointNegPaint = Paint().apply {
        color = pointNegColor
        isAntiAlias = true
        isDither = true
        //alpha = 128
    }

    /**
     * Don't draw every single pixel.
     * If the finger has has moved less than this distance, don't draw. scaledTouchSlop, returns
     * the distance in pixels a touch can wander before we think the user is scrolling.
     */
    private val touchTolerance = ViewConfiguration.get(context).scaledTouchSlop*0.25f

    private var currentX = 0f
    private var currentY = 0f

    private var motionTouchEventX = 0f
    private var motionTouchEventY = 0f

    /**
     * Called whenever the view changes size.
     * Since the view starts out with no size, this is also called after
     * the view has been inflated and has a valid size.
     */
    override fun onSizeChanged(width: Int, height: Int, oldWidth: Int, oldHeight: Int) {
        super.onSizeChanged(width, height, oldWidth, oldHeight)

        if (::extraBitmap.isInitialized) extraBitmap.recycle()
        extraBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        extraCanvas = Canvas(extraBitmap)
        extraCanvas.drawColor(backgroundColor)

        // Calculate a rectangular frame around the picture.
        val inset = 40
        frame = Rect(inset, inset, width - inset, height - inset)
    }

    private fun drawExtra() {
        extraCanvas.drawColor(backgroundColor)
        if (selectedPoint != null) {
            selectedPoint?.posX = ViewToSpaceX(currentX)
            selectedPoint?.posY = ViewToSpaceY(currentY)
            drawPoints()
            val label = "(%.2f, %.2f)".format(selectedPoint?.posX, selectedPoint?.posY)//selectedPoint?.posX.toString() + ", " + selectedPoint?.posY.toString()
            var toffx: Float = 64f
            var toffy: Float = -64f
            var tbounds: Rect = Rect(0, 0, 0, 0)
            textPaint.getTextBounds(label, 0, label.length, tbounds)
            if (currentX + toffx + tbounds.width() > width-4f)
                toffx = width-4f-currentX-tbounds.width()
            if (currentY + toffy - tbounds.height() < 4f)
                toffy = 64f+tbounds.height()
            extraCanvas.drawText(label, currentX+toffx, currentY+toffy, textPaint)
            val tpx = SpaceToViewX(selectedPoint!!.posX)
            val tpy = SpaceToViewY(selectedPoint!!.posY)
            extraCanvas.drawCircle(tpx, tpy, pointRadius, outlinePaint)

            // w1x + w2y + b = 0
            // x = (- w2y - b)/w1
            // y = (- w1x - b)/w2
            /*

                x = 0:
                    y >= 0 && y <= h
                    y = -b/w2

             */


            /*
            val wdx = Weight.posY
            val wdy = Weight.posX
            val wcx = P*Weight.posX
            val wcy = P*Weight.posY
            val hl = P+spaceWidth
            extraCanvas.drawLine(wcx-wdx*hl, wcy-wdy*hl, wcx+wdx*hl, wcy+wdy*hl, outlinePaint)
            */
        }
        else
            drawPoints()

        var wp0: Point? = null
        var wp1: Point? = null
        if (abs(Weight.posY) > 0.00001f) {
            val y0 = P/Weight.posY
            val yw = (-Weight.posX*spaceWidth + P)/Weight.posY
            if (y0 >= 0f && y0 <= spaceHeight) {
                wp0 = Point(0f, y0, 0)
            }
            if (yw >= 0f && yw <= spaceHeight) {
                if (wp0 == null)
                    wp0 = Point(spaceWidth, yw, 0)
                else
                    wp1 = Point(spaceWidth, yw, 0)
            }
        }
        if (wp1 == null && abs(Weight.posX) > 0.00001f) {
            val x0 = P/Weight.posX
            val xh = (-Weight.posY*spaceHeight + P)/Weight.posX
            if (x0 >= 0f && x0 <= spaceWidth) {
                if (wp0 == null)
                    wp0 = Point(x0, 0f, 0)
                else
                    wp1 = Point(x0, 0f, 0)
            }
            if (xh >= 0f && xh <= spaceWidth) {
                if (wp0 == null)
                    wp0 = Point(xh, spaceHeight, 0)
                else
                    wp1 = Point(xh, spaceHeight, 0)
            }
        }

        if (wp0 != null && wp1 != null) {
            extraCanvas.drawLine(SpaceToViewX(wp0.posX), SpaceToViewY(wp0.posY),
                SpaceToViewX(wp1.posX), SpaceToViewY(wp1.posY), outlinePaint)
        }
    }

    override fun onDraw(canvas: Canvas) {
        // Draw the bitmap that has the saved path.
        drawExtra()
        canvas.drawBitmap(extraBitmap, 0f, 0f, null)
        // Draw a frame around the canvas.
        //extraCanvas.drawRect(frame, paint)
    }

    /**
     * No need to call and implement MyCanvasView#performClick, because MyCanvasView custom view
     * does not handle click actions.
     */
    override fun onTouchEvent(event: MotionEvent): Boolean {
        motionTouchEventX = event.x
        motionTouchEventY = event.y

        when (event.action) {
            MotionEvent.ACTION_DOWN -> touchStart()
            MotionEvent.ACTION_MOVE -> touchMove()
            MotionEvent.ACTION_UP -> touchUp()
        }
        return true
    }

    /**
     * The following methods factor out what happens for different touch events,
     * as determined by the onTouchEvent() when statement.
     * This keeps the when conditional block
     * concise and makes it easier to change what happens for each event.
     * No need to call invalidate because we are not drawing anything.
     */
    private fun touchStart() {
        path.reset()
        path.moveTo(motionTouchEventX, motionTouchEventY)
        currentX = motionTouchEventX
        currentY = motionTouchEventY
        selectedPoint = pickPoint(ViewToSpaceX(currentX), ViewToSpaceY(currentY))
    }

    private fun touchMove() {
        val dx = Math.abs(motionTouchEventX - currentX)
        val dy = Math.abs(motionTouchEventY - currentY)
        if (dx >= touchTolerance || dy >= touchTolerance) {
            // QuadTo() adds a quadratic bezier from the last point,
            // approaching control point (x1,y1), and ending at (x2,y2).
            path.quadTo(currentX, currentY, (motionTouchEventX + currentX) / 2, (motionTouchEventY + currentY) / 2)
            currentX = motionTouchEventX
            currentY = motionTouchEventY
            // Draw the path in the extra bitmap to save it.
            //extraCanvas.drawPath(path, paint)
        }

        if (AutoRedraw)
            startLearning()

        drawExtra()

        // Invalidate() is inside the touchMove() under ACTION_MOVE because there are many other
        // types of motion events passed into this listener, and we don't want to invalidate the
        // view for those.
        invalidate()
    }

    private fun touchUp() {
        // Reset the path so it doesn't get drawn again.
        selectedPoint = null
        path.reset()
    }
}