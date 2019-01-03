/*        			Stupid OS Project 				*/
/*     			Keyboard Driver  12/12/01				*/

#include <types.h>
#include <keyboard.h>
#include <assem.h>
#include <idt.h>
#include <print.h>
#include <pic.h>

// Break codes for lock keys
 
#define CAPS_LOCK  0xBA
#define NUM_LOCK  0xC5
#define SCROLL_LOCK 0xC6

// Make codes
#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36



// Status codes for the leds as in port  64h
 
#define NUM_LED  0x02
#define CAPS_LED 0x04
#define SCROLL_LED 0x01

byte key_map[] ={ '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , '0' , '0',   'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '[' , ']' , '0', '0', 'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , ';' , 0x27 , '`' , '0' , 0x5C, 'z' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , ',' , '.' , '/' };

/*byte shift_key_map[]={}
static boolean shift_flag;*/

byte led_status =0x02;   // Used to preserve the led status (initially set Num Lock On)

/* When a key is pressed series of bytes is sent by the keyboard controller 8042 
*  containing make code followed by break code, raising intterrupt for each byte 
*  sent. Usually all common keys send only two byte ( 1 make and  1 break) but some special 
*  keys send more than 1 byte of make and break code					*/




void keyboard_init ()
{
	load_idt_entry(33,(uint)&keyboard_handler,CS_SELECTOR,INT_GATE|PRESENT|BITS_32);
	enable_irq(KEYBOARD);
        print("Keyboard Initialization\t\t\t\t[ DONE ]\n", 0xf);
}

void keyboard_handler()
{
	
	byte key;
	char str[2];
	char ch;
	key=inb(0x60);   //The scan code is obtained from port 60h
	 	
	if(key & 0x80)	 // If the scan code is a break code	
	{
		
		switch(key)
		{
	
			case CAPS_LOCK:
			
				while ((inb(0x64) & 0x02));        //Reading the status
				outb(0x60,0xED);		   // Command code for setting LEDS	
				while ((inb(0x64) & 0x02));	   //Reading the status to send data
				
				/* The current status of LED is toggled */				
				if(led_status & CAPS_LED)
					led_status = led_status & ~CAPS_LED;
				else
					led_status = led_status | CAPS_LED;
	 			
				outb(0x60,(byte)led_status);      //Setting the LEDs
				break;
			
			case NUM_LOCK:
			
				while ((inb(0x64) & 0x02));        //Reading the status
				outb(0x60,0xED);
				while ((inb(0x64) & 0x02));
				
				if(led_status & NUM_LED)
					led_status = led_status & ~NUM_LED;
				else
					led_status = led_status | NUM_LED;
				outb(0x60,(byte)led_status);
				break;
			

			case SCROLL_LOCK:
			

				while ((inb(0x64) & 0x02));        //Reading the status
				outb(0x60,0xED);
				while ((inb(0x64) & 0x02));


				if(led_status & SCROLL_LED)
					led_status = led_status & ~SCROLL_LED;
				else
					led_status = led_status | SCROLL_LED;

				outb(0x60,(byte)led_status);
				break;
			
				
				
						
			default :
			
				key = key - 130;
				ch = key_map[key];
				if ( led_status & CAPS_LED )
				{

					if (ch >= 97 && ch <= 122)
					{
						ch = ch -32;
					}
				}
				str[0] = ch;
				str[1] = '\0';				
				print(str,0xf);
				
					
			
		}

	}
	else
	{
		switch(key)
		{
			case LEFT_SHIFT:
			case RIGHT_SHIFT:
		}				
	}
		




	outb(0x20,0x20);
        asm("mov %ebp,%esp");
        asm("pop %ebp");
        asm("iret");
}



