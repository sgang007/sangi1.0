package com.sangi.sangicommands;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.speech.RecognizerIntent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;




public class MainActivity extends Activity implements OnClickListener{

	private Button recordButton;
	private Button bluetoothButton;
	private EditText translatedText;
	private static final int REQUEST_CODE = 1234;
	private static final int DISCOVER_DURATION = 300;
	private static final int REQUEST_BLU = 1;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        //Assign the views to objects
        recordButton = (Button) findViewById(R.id.speechStart);
        translatedText = (EditText) findViewById(R.id.translatedText);
        bluetoothButton = (Button) findViewById(R.id.sendDataBluetooth);
        
        //Set listener
        recordButton.setOnClickListener(this);
        bluetoothButton.setOnClickListener(this);
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
		if(bluetoothButton.isPressed())
		{
			sendViaBluetooth();
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
		 if(resultCode == DISCOVER_DURATION && requestCode == REQUEST_BLU) {
	    		
	    		Intent intent = new Intent();
	    		intent.setAction(Intent.ACTION_SEND);
	    		intent.setType("text/plain");
	    		File sdcard = Environment.getExternalStorageDirectory();
	    		//Get the text file
	    		File file = new File(sdcard,"Commands.txt");
	    		FileOutputStream fos;
	            try {
	            	fos = new FileOutputStream(file);
					fos.write(translatedText.getText().toString().getBytes());
					fos.flush();
		            fos.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	            
	    		intent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(file));
	    		
	    		PackageManager pm = getPackageManager();
	    		List<ResolveInfo> appsList = pm.queryIntentActivities(intent, 0);
	    		
	    		if(appsList.size() > 0) {
	    			String packageName = null;
	    			String className = null;
	    			boolean found = false;
	    			
	    			for(ResolveInfo info : appsList) {
	    				packageName = info.activityInfo.packageName;
	    				if(packageName.equals("com.android.bluetooth")) {
	    					className = info.activityInfo.name;
	    					found = true;
	    					break;
	    				}
	    			}
	    			
					if (!found) {
						Toast.makeText(this, "Bluetooth havn't been found",
								Toast.LENGTH_LONG).show();
					} else {
						intent.setClassName(packageName, className);
						startActivity(intent);
					}
	    		}
			} else {
				Toast.makeText(this, "Bluetooth is cancelled", Toast.LENGTH_LONG)
						.show();
			}
		 
		 
		 if (requestCode == REQUEST_CODE && resultCode== -1) {
		      ArrayList matches = data
		          .getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
		      translatedText.setText((String) matches.get(0));
		    }
		    super.onActivityResult(requestCode, resultCode, data);
		    
		    
	 }
	 
	 public void sendViaBluetooth(){
		 	BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
	    	
	    	if(btAdapter == null) {
	    		Toast.makeText(this, "Bluetooth is not supported on this device", Toast.LENGTH_LONG).show();
	    	} else {
	    		enableBluetooth();
	    	}
	 }
	 
	 public void enableBluetooth() {
	    	
	    	Intent discoveryIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
	    	
	    	discoveryIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, DISCOVER_DURATION);
	    	
	    	startActivityForResult(discoveryIntent, REQUEST_BLU);
	    }
}

