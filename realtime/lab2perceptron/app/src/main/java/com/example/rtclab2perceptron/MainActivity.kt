package com.example.rtclab2perceptron

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.view.View.SYSTEM_UI_FLAG_FULLSCREEN
import kotlinx.android.synthetic.main.activity_main.*

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

    }

    public fun onStartClick(v: View?) {
        /*
        spaceView.AddPoint(100f, 100f, 1)
        spaceView.AddPoint(300f, 100f, 0)
        spaceView.AddPoint(300f, 300f, 1)
        spaceView.AddPoint(100f, 300f, 1)
        */
    }

    public fun onAddPosClick(v: View?) {
        val h = spaceView.height
        val w = spaceView.width
        spaceView.AddPoint(w*0.5f, h*0.5f, 1)
    }

    public fun onAddNegClick(v: View?) {
        val h = spaceView.height
        val w = spaceView.width
        spaceView.AddPoint(w*0.5f, h*0.5f, 0)
    }

    public fun onClearClick(v: View?) {
        spaceView.ClearPoints()
    }
}
