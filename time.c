/*---------------------------------------------------------------------------------------------------
Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 08 2001	Updated : Dec 08 2001
File Name : time.c      Purpose : Functions for obtaining system time
---------------------------------------------------------------------------------------------------*/

#include <cmos.h>
#include <time.h>
#include <print.h>

void get_time(time_st_t *time)
{
	time -> second = cmos_read(SECOND);
	time -> minute = cmos_read(MINUTE);
	time -> hour   = cmos_read(HOUR);
}

void get_date(date_st_t *date)
{
	date -> year  = cmos_read(YEAR);
	date -> month = cmos_read(MONTH);
	date -> date  = cmos_read(DATE);
	date -> day   = cmos_read(DAY);
}

