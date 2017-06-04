package com.example.ordering_system;

import java.util.Timer;
import java.util.TimerTask;

public class MenuList {
	private String TableNumber;
    private String Menu;
	private Timer timer;
	private int Time;
	
	public MenuList(String tn, String menu, int time) {
        this.TableNumber = tn;
        this.Menu = menu;
        this.Time = time;
        this.timer = new Timer(true);
    }
	
	public String getTableNumber() {
        return TableNumber;
    }
    public void setTableNumber(String tn) {
        this.TableNumber = tn;
    }
    public String getMenu() {
        return Menu;
    }
    public void setMenu(String menu) {
        this.Menu = menu;
    }
    public int getTime() {
        return Time;
    }
    public void setTime(int time) {
        this.Time = time;
    }
	
    public void start(TimerTask job){
    	timer.schedule(job,Time);
    }
}
