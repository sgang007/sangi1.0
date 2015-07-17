package com.sangi.sangicommands;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.speech.RecognizerIntent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;




public class MainActivity extends Activity implements OnClickListener{

	private Button recordButton;
	private EditText translatedText;
	private static final int REQUEST_CODE = 1234;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        //Assign the views to objects
        recordButton = (Button) findViewById(R.id.speechStart);
        translatedText = (EditText) findViewById(R.id.translatedText);
        
        //Set listener
        recordButton.setOnClickListener(this);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


	@Override
	public void onClick(View v) {
		if ( recordButton.isPressed() )
		{
			startVoiceRecognitionActivity();
		}
		
	}
	
	 public void startVoiceRecognitionActivity() {
				    
		 	Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
			intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL,
			                RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
			intent.putExtra(RecognizerIntent.EXTRA_PROMPT, "Voice recognition Demo...");
			startActivityForResult(intent, REQUEST_CODE);
			        
	 }
	    
	 protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		    if (requestCode == REQUEST_CODE && resultCode== -1) {
		      ArrayList matches = data
		          .getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
		      translatedText.setText((String) matches.get(0));
		    }
		    super.onActivityResult(requestCode, resultCode, data);
	 }
}

