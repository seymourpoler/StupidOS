#include <gdt.h>
#include <idt.h>
#include <pic.h>
#include <timer.h>
#include <print.h>
#include <time.h>
#include <cmos.h>
#include <config.h>
#include <assem.h>
#include <keyboard.h>

int main()
{
	print("Stupid OS Version ", 0xf);
	printi((long)VERSION);
	print(".", 0xf);
	printi(SUBVERSION);
	print(".", 0xf);
	printi(RELEASE);
	print("\n", 0xf);
	gdt_init();
	idt_init();
	pic_init();
	keyboard_init();
	timer_init();
	print_time();
	while(1);
	return 0 ;
}
	
