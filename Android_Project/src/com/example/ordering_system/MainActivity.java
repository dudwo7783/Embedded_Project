package com.example.ordering_system;


import java.util.ArrayList;
import java.util.TimerTask;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;



public class MainActivity extends Activity implements View.OnClickListener{
	public native int Print7Segment(int status, String table_number);
	public native int PrintLed(int status);
	public native int GetRes();
	public native int GetTouch();
	static {
		System.loadLibrary("Ordering_System");
    }
	
	final int CUSTOMER_IN = 0;
	final int CUSTOMER_OUT = 1;
	final int ADMIT_WAITLIST = 2;
	final int DENY_WAITLIST = 3;
	
	
	private EditText table;
	private EditText menu;
	private LinkedList CustomerList;
	private LinkedList WaitList;
	private ListView customerview ;
	private ListView waitlistview ;
	private ListViewAdapter adapter1;
	private ListViewAdapter adapter2;
	private MenuAdapter menulist;
	
	private int btn;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        table = (EditText)findViewById(R.id.edit_table);
        menu = (EditText)findViewById(R.id.edit_menu);
        findViewById(R.id.reservation).setOnClickListener(this);
        findViewById(R.id.order).setOnClickListener(this);
        
        CustomerList = new LinkedList();
        WaitList = new LinkedList();
        

        // Adapter 생성
        adapter1 = new ListViewAdapter();
        adapter2 = new ListViewAdapter();
        menulist = new MenuAdapter() ;

        // 리스트뷰 참조 및 Adapter달기
        customerview = (ListView) findViewById(R.id.customerlist);
        customerview.setAdapter(adapter1);
        waitlistview = (ListView) findViewById(R.id.waitlist);
        waitlistview.setAdapter(adapter2);
        
        BtnThread.setDaemon(true);
        BtnThread.start();

    }
    
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		int i = v.getId();

        if (i == R.id.reservation) {
        	reservation();        	
        }
        else if (i == R.id.order) {
        }
	}
	
	public int reservation(){
		String wait_first;
		int max;
		String TableNumber = table.getText().toString();
		
		max = GetRes();
		
		if(TableNumber.getBytes().length <= 0){	//빈값이 넘어올때의 처리
			Toast.makeText(MainActivity.this, "값을 입력하세요." ,Toast.LENGTH_SHORT).show();
			return -1;
		}else{
			int error = 0;
			
			if(CustomerList.size() == 0){
				error = Print7Segment(CUSTOMER_IN,TableNumber);
				PrintLed(CUSTOMER_IN);
				Toast.makeText(MainActivity.this, "고객 입장." ,Toast.LENGTH_SHORT).show();
				
				CustomerList.addLast(TableNumber);
				adapter1.addItem(TableNumber, 0);
	            adapter1.notifyDataSetChanged();
				return 1;
			}
			
			if(CustomerList.indexOf(TableNumber) != -1){
				error = Print7Segment(CUSTOMER_OUT,TableNumber);
				PrintLed(CUSTOMER_OUT);
				Toast.makeText(MainActivity.this, "고객이 나갑니다." ,Toast.LENGTH_SHORT).show();
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				CustomerList.remove(CustomerList.indexOf(TableNumber));
				adapter1.RemoveItem(TableNumber);
				adapter1.notifyDataSetChanged();
				
				if(WaitList.size()!=0){
					wait_first = (String)WaitList.removeFirst();
					error = Print7Segment(CUSTOMER_IN,wait_first);
					PrintLed(CUSTOMER_IN);
					Toast.makeText(MainActivity.this, "대기고객 입장" ,Toast.LENGTH_SHORT).show();
					
					CustomerList.addLast(wait_first);
					
					adapter1.addItem(wait_first, 0);
		            adapter2.RemoveItem(wait_first);
		            adapter2.notifyDataSetChanged();
		            
				}
				return 1;
			}
			if(WaitList.size() != 0 && WaitList.indexOf(TableNumber) != -1){
				
				error = Print7Segment(CUSTOMER_OUT,TableNumber);
				PrintLed(CUSTOMER_OUT);
				Toast.makeText(MainActivity.this, "대기고객이 나갑니다." ,Toast.LENGTH_SHORT).show();
				
				WaitList.remove(WaitList.indexOf(TableNumber));
				adapter2.RemoveItem(TableNumber);
	            adapter2.notifyDataSetChanged();
				
				return 1;
			}
					
			if(CustomerList.size()<max){
				error = Print7Segment(CUSTOMER_IN,TableNumber);
				PrintLed(CUSTOMER_IN);
				Toast.makeText(MainActivity.this, "고객 입장." ,Toast.LENGTH_SHORT).show();
				
				CustomerList.addLast(TableNumber);
				adapter1.addItem(TableNumber, 0);
	            adapter1.notifyDataSetChanged();
				return 1;
			}else{
				if(WaitList.size()<max){
					error = Print7Segment(ADMIT_WAITLIST,TableNumber);
					PrintLed(ADMIT_WAITLIST);
					Toast.makeText(MainActivity.this, "만석입니다. 예약 허가" ,Toast.LENGTH_SHORT).show();
					
					WaitList.addLast(TableNumber);
					adapter2.addItem(TableNumber, 1);
		            adapter2.notifyDataSetChanged();
					
					return 1;
				}
				else{
					error = Print7Segment(DENY_WAITLIST,TableNumber);
					PrintLed(DENY_WAITLIST);
					
					Toast.makeText(MainActivity.this, "만석입니다. 예약 거부" ,Toast.LENGTH_SHORT).show();
					
					return -1;
				}
			}			
		}
	}
	
	public int order(){
		String TableNumber = table.getText().toString();
		String Menu = menu.getText().toString();
		int time=0;
		
		if(TableNumber.getBytes().length <= 0
				|| Menu.getBytes().length <= 0){	//빈값이 넘어올때의 처리
			Toast.makeText(MainActivity.this, "값을 입력하세요." ,Toast.LENGTH_SHORT).show();
			return -1;
		}
		else{
			
			ScheduleJob job = new ScheduleJob();
			if(Menu == "BBQ")
				time = 5;
			else
				time = 10;
			
			menulist.addItem(TableNumber, Menu, time, job);			
		}
				
		return 0;
	}
	
	Thread BtnThread = new Thread(new Runnable() {

        @Override
        public void run() {
            while (true) {
                try {
                    BtnThread.sleep(1000);
                    btn = GetTouch();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                    	if(btn==1){
                    		while(CustomerList.size() != 0)
                    			CustomerList.removeFirst();
                    		while(WaitList.size() != 0)
                    			WaitList.removeFirst();
                    		adapter1.RemoveAllItem();
                    		adapter2.RemoveAllItem();
                    		adapter1.notifyDataSetChanged();
                    		adapter2.notifyDataSetChanged();
                    		Toast.makeText(MainActivity.this, "손님 내쫓기." ,Toast.LENGTH_SHORT).show();
                    	}
                        //timerview.setText(String.valueOf(time));
                        //handler.sendEmptyMessage(time);
                    }
                });
            }
        }
    });
	
	class ScheduleJob extends TimerTask{
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			// function is implemented by song!!!
			// vibrate
			// print textlcd
			// ...
		}
	}

}
