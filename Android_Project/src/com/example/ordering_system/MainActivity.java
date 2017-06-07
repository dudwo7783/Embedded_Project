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

public class MainActivity extends Activity implements View.OnClickListener {
	public native int Print7Segment(int status, String table_number);

	public native int PrintLed(int status);

	public native int GetRes();

	public native int GetTouch();

	public native int GetSwitch();

	public native int Piezo(int status);

	public native void Vibrator(int status);

	public native int PrintTextlcd(String menu_name);

	static {
		System.loadLibrary("Ordering_System");
	}

	final int CUSTOMER_IN = 0;
	final int CUSTOMER_OUT = 1;
	final int ADMIT_WAITLIST = 2;
	final int DENY_WAITLIST = 3;
	final int MENU_IN = 4;
	final int MENU_OUT = 5;

	private EditText table;
	private EditText menu;
	private LinkedList CustomerList;
	private LinkedList WaitList;
	private LinkedList MenuList;
	private ListView customerview;
	private ListView waitlistview;
	private ListView menulistview;
	private ListViewAdapter adapter1;
	private ListViewAdapter adapter2;
	private MenuAdapter MenuAdapter;

	private int btn;
	private int touch;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		table = (EditText) findViewById(R.id.edit_table);
		menu = (EditText) findViewById(R.id.edit_menu);
		findViewById(R.id.reservation).setOnClickListener(this);
		findViewById(R.id.order).setOnClickListener(this);

		CustomerList = new LinkedList();
		WaitList = new LinkedList();
		MenuList = new LinkedList();

		// Adapter 생성
		adapter1 = new ListViewAdapter();
		adapter2 = new ListViewAdapter();
		MenuAdapter = new MenuAdapter();

		// 리스트뷰 참조 및 Adapter달기
		customerview = (ListView) findViewById(R.id.customerlist);
		customerview.setAdapter(adapter1);
		waitlistview = (ListView) findViewById(R.id.waitlist);
		waitlistview.setAdapter(adapter2);
		menulistview = (ListView) findViewById(R.id.menulist);
		menulistview.setAdapter(MenuAdapter);

		BtnThread.setDaemon(true);
		TouchThread.setDaemon(true);
		BtnThread.start();
		TouchThread.start();
		

	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		int i = v.getId();

		if (i == R.id.reservation) {
			reservation();
		} else if (i == R.id.order) {
			order();
		}
	}

	public int reservation() {
		String wait_first;
		int max;
		String TableNumber = table.getText().toString();

		max = GetRes();

		if (TableNumber.getBytes().length <= 0) { // 빈값이 넘어올때의 처리
			Toast.makeText(MainActivity.this, "값을 입력하세요.", Toast.LENGTH_SHORT).show();
			return -1;
		} else {
			int error = 0;

			if (CustomerList.size() == 0) {
				error = Print7Segment(CUSTOMER_IN, TableNumber);
				PrintLed(CUSTOMER_IN);
				Toast.makeText(MainActivity.this, "고객 입장.", Toast.LENGTH_SHORT)
						.show();

				CustomerList.addLast(TableNumber);
				adapter1.addItem(TableNumber, 0);
				adapter1.notifyDataSetChanged();
				return 1;
			}

			if (CustomerList.indexOf(TableNumber) != -1) {
				error = Print7Segment(CUSTOMER_OUT, TableNumber);
				PrintLed(CUSTOMER_OUT);
				Toast.makeText(MainActivity.this, "고객이 나갑니다.",
						Toast.LENGTH_SHORT).show();
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				CustomerList.remove(CustomerList.indexOf(TableNumber));
				adapter1.RemoveItem(TableNumber);
				adapter1.notifyDataSetChanged();

				if (WaitList.size() != 0) {
					if (CustomerList.size() >= max) {
						return 0;
					}
					wait_first = (String) WaitList.removeFirst();
					error = Print7Segment(CUSTOMER_IN, wait_first);
					PrintLed(CUSTOMER_IN);
					Toast.makeText(MainActivity.this, "대기고객 입장",
							Toast.LENGTH_SHORT).show();

					CustomerList.addLast(wait_first);

					adapter1.addItem(wait_first, 0);
					adapter2.RemoveItem(wait_first);
					adapter2.notifyDataSetChanged();

				}
				return 1;
			}
			if (WaitList.size() != 0 && WaitList.indexOf(TableNumber) != -1) {
				// function is implemented by song!!!

				error = Print7Segment(CUSTOMER_OUT, TableNumber);
				PrintLed(CUSTOMER_OUT);
				Toast.makeText(MainActivity.this, "대기고객이 나갑니다.",
						Toast.LENGTH_SHORT).show();

				WaitList.remove(WaitList.indexOf(TableNumber));
				adapter2.RemoveItem(TableNumber);
				adapter2.notifyDataSetChanged();

				return 1;
			}

			if (CustomerList.size() < max) {
				error = Print7Segment(CUSTOMER_IN, TableNumber);
				PrintLed(CUSTOMER_IN);
				Toast.makeText(MainActivity.this, "고객 입장.", Toast.LENGTH_SHORT)
						.show();

				CustomerList.addLast(TableNumber);
				adapter1.addItem(TableNumber, 0);
				adapter1.notifyDataSetChanged();
				return 1;
			} else {
				if (WaitList.size() < max) {
					error = Print7Segment(ADMIT_WAITLIST, TableNumber);
					PrintLed(ADMIT_WAITLIST);
					Toast.makeText(MainActivity.this, "만석입니다. 예약 허가",
							Toast.LENGTH_SHORT).show();

					WaitList.addLast(TableNumber);
					adapter2.addItem(TableNumber, 1);
					adapter2.notifyDataSetChanged();

					return 1;
				} else {
					error = Print7Segment(DENY_WAITLIST, TableNumber);
					PrintLed(DENY_WAITLIST);

					Toast.makeText(MainActivity.this, "만석입니다. 예약 거부",
							Toast.LENGTH_SHORT).show();

					return -1;
				}
			}
		}
	}

	public int order() {
		String TableNumber = table.getText().toString();
		String Menu = menu.getText().toString();
		int time = 0;

		if (TableNumber.getBytes().length <= 0 || Menu.getBytes().length <= 0) { // 빈값이 넘어올때의 처리
			Toast.makeText(MainActivity.this, "값을 입력하세요.", Toast.LENGTH_SHORT).show();
			return -1;
		} else {
			if(CustomerList.size() == 0){
				Toast.makeText(MainActivity.this, "없는 좌석입니다.", Toast.LENGTH_SHORT).show();
				return -1;
			}
			if(CustomerList.indexOf(TableNumber) == -1){
				Toast.makeText(MainActivity.this, "없는 좌석입니다.", Toast.LENGTH_SHORT).show();
				return -1;
			}
			if(Menu.equals("RICE"))
				time = 10000;
			else if(Menu.equals("NOODLE"))
				time = 15000;
			else if(Menu.equals("BBQ"))
				time = 20000;
			else{
				Toast.makeText(MainActivity.this, "없는 메뉴입니다.", Toast.LENGTH_SHORT).show();
				return -1;
			}
			
			ScheduleJob job = new ScheduleJob(TableNumber, Menu, MenuAdapter);
			
			MenuAdapter.addItem(TableNumber, Menu, time, job);
			Piezo(MENU_IN);
			Vibrator(MENU_IN);
			PrintTextlcd(Menu);
			Print7Segment(CUSTOMER_IN, TableNumber);
			MenuAdapter.notifyDataSetChanged();
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
						if (btn == 1) {
							while (CustomerList.size() != 0)
								CustomerList.removeFirst();
							while (WaitList.size() != 0)
								WaitList.removeFirst();
							adapter1.RemoveAllItem();
							adapter2.RemoveAllItem();
							MenuAdapter.RemoveAllItem();
							adapter1.notifyDataSetChanged();
							adapter2.notifyDataSetChanged();
							MenuAdapter.notifyDataSetChanged();
							Toast.makeText(MainActivity.this, "손님 내쫓기.",
									Toast.LENGTH_SHORT).show();
						}
						// timerview.setText(String.valueOf(time));
						// handler.sendEmptyMessage(time);
					}
				});
			}
		}
	});
	
	Thread TouchThread = new Thread(new Runnable() {

		@Override
		public void run() {
			while (true) {
				try {
					TouchThread.sleep(1000);
					touch = GetSwitch();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						if (touch == 1) {
							MenuAdapter.RemoveAllItem();
							MenuAdapter.notifyDataSetChanged();
							Toast.makeText(MainActivity.this, "FOOD 내쫓기.",
									Toast.LENGTH_SHORT).show();
						}
					}
				});
			}
		}
	});

	class ScheduleJob extends TimerTask {
		private String TableNumber;
		private String Menu;
		int err = 0;

		public ScheduleJob(String tn, String menu, MenuAdapter menuadapter) {
			this.TableNumber = tn;
			this.Menu = menu;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			// function is implemented by song!!!
			// vibrate
			// print textlcd
			// ...
			MenuAdapter.RemoveItem(TableNumber, Menu);
			// Toast.makeText(MainActivity.this, "Complete menu"
			// ,Toast.LENGTH_SHORT).show();
			err = Piezo(MENU_OUT);
			Log.d("PIEZO", String.valueOf(err));
			Vibrator(MENU_OUT);
			err = PrintTextlcd(Menu);
			Log.d("LCD", String.valueOf(err));
			Print7Segment(CUSTOMER_IN, TableNumber);

			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					MenuAdapter.notifyDataSetChanged();
				}
			});
			try {
				this.finalize();
			} catch (Throwable e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
