package com.example.rtclab2perceptron

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.view.View.SYSTEM_UI_FLAG_FULLSCREEN
import kotlinx.android.synthetic.main.activity_main.*
import kotlin.random.Random

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        /*
        val perceptronSpaceView = PerceptronSpaceView(this)
        perceptronSpaceView.systemUiVisibility = SYSTEM_UI_FLAG_FULLSCREEN;
        perceptronSpaceView.contentDescription = getString(R.string.canvasContentDescription)
        setContentView(perceptronSpaceView)
        */
        /*
        val myCanvasView = MyCanvasView(this)
        // No XML file; just one custom view created programmatically.
        // Request the full available screen for layout.
        myCanvasView.systemUiVisibility = SYSTEM_UI_FLAG_FULLSCREEN
        myCanvasView.contentDescription = getString(R.string.canvasContentDescription)
        setContentView(myCanvasView)
        */

        spaceView.AddPoint(1f, 5f, 1, false)
        spaceView.AddPoint(2f, 4f, 0, false)
    }

    public fun onAutoClick(v: View?) {
        spaceView.AutoRedraw = AutoCheckBox.isChecked
    }

    public fun onStartClick(v: View?) {
        /*
        spaceView.AddPoint(100f, 100f, 1)
        spaceView.AddPoint(300f, 100f, 0)
        spaceView.AddPoint(300f, 300f, 1)
        spaceView.AddPoint(100f, 300f, 1)
        */
        spaceView.Sigma = LearningRateEdit.text.toString().toFloat()
        spaceView.Iters = IterationsCountEdit.text.toString().toInt()
        spaceView.P = ThresholdEdit.text.toString().toFloat()
        spaceView.startLearning()
        LearnResultText.setText("W=(%.2f, %.2f)   Time=%dms".format(spaceView.Weight.posX, spaceView.Weight.posY, spaceView.LearnDuration))
    }

    public fun onAddPosClick(v: View?) {
        val h = spaceView.height
        val w = spaceView.width
        spaceView.AddPoint(Random.nextFloat()*spaceView.spaceWidth*0.8f+0.1f,
                           Random.nextFloat()*spaceView.spaceHeight*0.8f+0.1f, 1)
    }

    public fun onAddNegClick(v: View?) {
        val h = spaceView.height
        val w = spaceView.width
        spaceView.AddPoint(Random.nextFloat()*spaceView.spaceWidth*0.8f+0.1f,
            Random.nextFloat()*spaceView.spaceHeight*0.8f+0.1f, 0)
    }

    public fun onClearClick(v: View?) {
        spaceView.ClearPoints()
    }
}
