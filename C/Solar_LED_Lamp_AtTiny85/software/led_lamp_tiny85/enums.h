enum lamp_status
{
	CHARGE = 0,
	WAIT = 1,
	LIGHT = 2
	
	};
	
enum battery_status
{
	OK = 0,
	DISCHARGED = 1
	
	};
	
enum battery_cycle
{
	MAX_CHARGING = 0,
	NORMAL_WORK = 1,
	MAX_DISCHARGING = 2
	};
	
typedef int bool;
enum {FALSE, TRUE };