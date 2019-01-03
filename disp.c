/*
* ------------------------------------------------------------------------------------------------ 
* Stupid OS Project Copyright 3DMinus3 
* Display Functions 
* Started December 7 2001 : Updated December 8 2001
*------------------------------------------------------------------------------------------------- 
*/
#include <print.h>
#include <time.h>

int scr_xpos = 9 ;
int scr_ypos = 0 ;


// Clear screen function
void cls()
{
	char *d = (char*) 0xb8000 ;  // Display pointer
	int i ;

	for(i=0 ; i<4000 ; i+=2)     // Move blank to all locations
		*(d+i)=' ' ;

	scr_xpos=0 ;
	scr_ypos=0 ;
}
		


// Print string  
void print(char *p, char attr)
{
	char *z=(char*) (0xb8000) ;    		// Display pointer  
	char c ;					
	int i=scr_xpos*160+scr_ypos*2 ;		// Offset, according to cursor position
	int tab;
	
	while((c=*p)!='\0')					// Scan string, printing each
	{							// character		
		switch(c)
		{
			case '\n' :
			        i+=(160-i%160) ;
				scr_xpos++ ;
				scr_ypos=0 ;
				break ;
			
			case '\t' :
				tab = (i % 160) % 16;
				i += 16 - tab;
				scr_ypos += (16 - tab) / 2 ;
				break ;

			case '\b' :
				i-=2 ;
				scr_ypos-- ;
				break ;
			
			default:				
				*(z+i)=c ;
				*(z+i+1)=attr ;
				scr_ypos++ ;
				i=i+2 ;
		}
		
		p++ ;
		
		if(scr_ypos>80)					// If character moves to the next
		{						// line, update cursor position
			scr_ypos=0 ;				
			scr_xpos++ ;
		}
		
		if(scr_xpos>24)
		{
			i-=160 ;
			scroll() ;
			scr_xpos=24 ;
		}
	}
	
}	



// Print string at row x and column y 
void printxy(char *p, int x, int y,  char attr)
{
	char *z = (char*) (0xb8000+x*160+(y-1)*2) ;			// Diplay pointer
	char c ;
	int i=0 ;
	
	while((c=*p)!='\0')
	{		
		switch(c)
		{
			case '\n' :
			        i+=(160-i%160) ;
				break ;
			
			case '\t' :
				i+=10 ;
				break ;

			case '\b' :
				i-=2 ;
				break ;
			
			default:				
				*(z+i)=c ;
				*(z+i+1)=attr ;
				i=i+2 ;
		}
		p++ ;
	}
}	




// Print integer function
void printi(long x)
{
	char s[20] ;
	char res[20] ;
	int i,c ;
	
	c=0 ; 
	
	do			
	{
		s[c]='0'+(int)x%10 ;		// Fill the s[] array with the character form
		x=x/10 ;			// of the number, stripped one digit at the time
		c++ ;		
	}
	while(x!=0) ;

	res[c]='\0' ;				// Terminate the string with a new-line character
						// and a null character
	
	for(i=c-1 ; i>=0 ; i--)			// Store the contents of the s[] array in the 
		res[c-i-1]=s[i] ;		// res[] array in the correct order (reversed)
	
	print(res,0xf) ;			// Print the result string using the print function
}	



// Scrolling Function
void scroll()
{
	char *d = (char*) 0xb8000 ;
	char *s = (char*) (0xb8000+160) ;
	int i ;

	for(i=0 ; i<4000 ; i+=2) 		// Copy the next line contents to the previous
	{					// line along with the attribute byte
		*(d+i)=*(s+i) ;
		*(d+i+1)=*(s+i+1) ;
	}

	for(i=24*160 ; i<4000 ; i+=2)
		*(d+i)=' ' ;
}		



// Cursor position function
void gotoxy(int x, int y)
{
	scr_xpos=x-1 ;				// D-U-H
	scr_ypos=y-1 ;
}	



// Print a single character to the display at the current cursor position
void printc(char c)
{
	char *d = (char*) (0xb8000) ;	// Display pointer
	int i = scr_xpos*160 + scr_ypos*2 ;
	
	switch(c)
	{
		case '\n' :
		        scr_ypos=0 ;
			scr_xpos++ ;
			i+=(160-i%160) ;
			break ;
			
		case '\t' :
			scr_ypos+=5 ;
			i+=10 ;
			break ;

		case '\b' :
			*(d+i)=' ' ;
			scr_ypos-- ;
			i-=2 ;
			break ;
			
		default:				
			*(d+i)=c ;
			scr_ypos++ ;
			i+=2 ;
	}
	
	if(scr_ypos>80) 					// Cursor next-line
	{
		scr_xpos++ ;
		scr_ypos=0 ;
	}

	if(scr_xpos>24)						// Cursor off-screen
	{
		scroll() ;
		scr_xpos=24 ;
		i-=160 ;
	}

	if(scr_ypos<0) 					// Cursor next-line
	{
		scr_xpos-- ;
		scr_ypos=80 ;
	}
}


// Prints a newline character
void newline()
{
	scr_xpos++ ;

	if(scr_xpos>24) 
	{
		scroll() ;
		scr_xpos=24 ;
	}
	scr_ypos=0 ;
}

void print_time()
{
	time_st_t time;
	date_st_t date;

	char days[7][5] = {"SUN "  , "MON ", "TUE ", "WED ", "THU ", "FRI ", "SAT "};
	char months[12][5] = {"JAN ", "FEB ", "MAR ", "APR ", "MAY ", "JUN ",
			      "JUL ", "AUG ", "SEP ", "OCT ", "NOV ", "DEC "};

	get_time(&time);
	get_date(&date);
        print("Local Time : ", 0xf);
	print(days[date.day], 0xf);
        print(months[date.month - 1], 0xF);
        printi(date.date);
	print(" ", 0xF);
	if (time.hour % 12 == 0)
		printi(12);
	else
	        printi(time.hour % 12);
        print(":", 0xF);
        printi(time.minute);
        print(":", 0xF);
	printi(time.second);
	if (time.hour >= 12)
		print(" PM ", 0xF);
	else
		print(" AM ", 0xF);
	printi(date.year + 2000);
	print("\t[ DONE ]\n", 0xF);
}

