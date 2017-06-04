#define TEXTLCD_RS         lcd_gpios[8]
#define TEXTLCD_RW         lcd_gpios[9]
#define TEXTLCD_EN         lcd_gpios[10]

#define TEXTLCD_DATA0         lcd_gpios[0]
#define TEXTLCD_DATA1         lcd_gpios[1]
#define TEXTLCD_DATA2         lcd_gpios[2]
#define TEXTLCD_DATA3         lcd_gpios[3]
#define TEXTLCD_DATA4         lcd_gpios[4]
#define TEXTLCD_DATA5         lcd_gpios[5]
#define TEXTLCD_DATA6         lcd_gpios[6]
#define TEXTLCD_DATA7         lcd_gpios[7]

#define TEXTLCD_BASE         0x56
#define TEXTLCD_FUNCTION_SET      _IO(TEXTLCD_BASE, 0x31)   

#define TEXTLCD_DISPLAY_ON      _IO(TEXTLCD_BASE, 0x32) 
#define TEXTLCD_DISPLAY_OFF      _IO(TEXTLCD_BASE, 0x33) 
#define TEXTLCD_DISPLAY_CURSOR_ON   _IO(TEXTLCD_BASE, 0x34) 
#define TEXTLCD_DISPLAY_CURSOR_OFF   _IO(TEXTLCD_BASE, 0x35) 

#define TEXTLCD_CURSOR_SHIFT_RIGHT   _IO(TEXTLCD_BASE, 0x36) 
#define TEXTLCD_CURSOR_SHIFT_LEFT   _IO(TEXTLCD_BASE, 0x37) 

#define TEXTLCD_ENTRY_MODE_SET      _IO(TEXTLCD_BASE, 0x38) 
#define TEXTLCD_RETURN_HOME      _IO(TEXTLCD_BASE, 0x39) 
#define TEXTLCD_CLEAR         _IO(TEXTLCD_BASE, 0x3a) 

#define TEXTLCD_DD_ADDRESS_1      _IO(TEXTLCD_BASE, 0x3b) 
#define TEXTLCD_DD_ADDRESS_2      _IO(TEXTLCD_BASE, 0x3c) 
#define TEXTLCD_WRITE_BYTE      _IO(TEXTLCD_BASE, 0x3d) 
