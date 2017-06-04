package com.example.ordering_system;

public class ListviewItem {
    private String TableNumber;
    private String Menu;
    private int Time;
    private int Perm;

    public ListviewItem() {
    }

    public ListviewItem(String tn, int perm) {
        this.TableNumber = tn;
        this.Menu = null;
        this.Time = 0;
        this.Perm = perm;
    }
    
    public ListviewItem(String tn, String menu, int time) {
        this.TableNumber = tn;
        this.Menu = menu;
        this.Time = time;
        this.Perm = 0;
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
    public int getPerm() {
        return Perm;
    }

    public void setPerm(int perm) {
        this.Perm = perm;
    }
}
