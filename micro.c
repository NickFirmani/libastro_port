#include <stdio.h>
#include <string.h>

#include "astro.h"
#include "preferences.h"

/* Undo the astro.h #defines of common body attribute names. */
#undef mjd
#undef lat
#undef lng
#undef tz
#undef temp
#undef pressure
#undef elev
#undef dip
#undef epoch
#undef tznm
#undef mjed

void initNow(Now *np, int year, int month, int day, int hour, int minute, int second, double timezon,
		double latitude, double longitude, double temperature, double press, double elevation);
void setNowTime(Now *np, int year, int month, int day, int hour, int minute, int second);
void setObj(Obj *op, char s[]);
void unitTest();
static void reportNowPosition(Now *np);
static void reportObjPosition(Obj *op);

/**
	Function that sets up our Now variable with the proper fields
	year, month, day, h, m, s = current time at NOW.
	latitude, degrees +north (standard)
	longitude, degrees +east (standard)
	timezone, hours behind UTC (reverse of standard eg PST = 7)
	temp, degrees C
	pressure, milibars
	elevation, above Mean Sea Level in Meters
**/
void initNow(Now *np, int year, int month, int day, int hour, int minute, int second, double timezon,
		double latitude, double longitude, double temperature, double press, double elevation) {
	memset (np, 0, sizeof(*np));			/* zero everything initially */
	setNowTime(np, year, month, day, hour, minute, second);
	np->n_lat = degrad(latitude);			/* latitude, +n rads */
	np->n_lng = degrad (longitude);			/* longitude, +e rads */
	np->n_tz = timezon;						/* time zone, hrs behind UTC */
	np->n_temp = temperature;				/* ambient air temp, C */
	np->n_pressure = press;					/* ambient air pressure, mBar */
	np->n_elev = elevation / ERAD;			/* local height above MSL (mean sea level), earth equatorial radii */
	np->n_epoch = J2000;					/* time of computed coords, MJD or EOD */
	pref_set (PREF_EQUATORIAL, PREF_TOPO);	/* set topocentric place, else GEO */
}

/**
	MJD is silly, this lets us set the field for a now from typical ymd,hms -> decimal days (!?)
**/
void setNowTime(Now *np, int year, int month, int day, int hour, int minute, int second) {
	/* mjd (modified julian day) from calendar date and time. */
	cal_mjd (month, day, year, &(np->n_mjd));
	np->n_mjd += hour / 24.;
	np->n_mjd += minute / (24. * 60.);
	np->n_mjd += second / (24. * 60. * 60.);
}

/**
	Sets the correct object fields given a EDB line.
	Prints an error if it can't figure out what to do
*/
void setObj(Obj *op, char s[]) {
	char buf[1024];
	memset (op, 0, sizeof(*op));
	if (db_crack_line (s, op, 0, 0, buf) < 0) {
		//Error.
		printf("%s", buf);
	}
}

void unitTest() {
	/* Now and handy pointer */
	Now now, *np = &now;
	Obj obj, *op = &obj;

	/* Set up our Now struct */
	//y, m, d, h, m, s, tz, lat, lon, temp, pressure, elevation
	initNow(np, 2014, 4, 2, 13, 30, 0, 7, 37.8717, -122.2728, 27, 1013.25, 0);
	
	/* Set up our target earth sat via known good EDB (3/30/15 loaded) */
	char s[] = "ISS (ZARYA)-999,E,1/089.52353579/2015, 51.6452,117.0009,0.0006864,154.4167,296.5045,15.55407364, .00015021,93579, 0.00022297";
	setObj(op, s);
	(void) obj_cir (np, op);		/* compute position at now */
	reportNowPosition(np);
	
	/* Do test for moon first */
	memset (op, 0, sizeof(*op));	/* zero everything initially */
	op->o_type = PLANET;			/* core type */
	op->pl_code = MOON;				/* ID code if PLANET */
	(void) strcpy (op->o_name, "Moon");	/* name */
	(void) obj_cir (np, op);		/* compute position @ now */
	reportObjPosition (op);			/* print */
	
	/* Do test for sun */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = SUN;
	(void) strcpy (op->o_name, "Sun");
	(void) obj_cir (np, op);
	reportObjPosition (op);
	
	/* Do test for Mercury */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = MERCURY;
	(void) strcpy (op->o_name, "Mercury");
	(void) obj_cir (np, op);
	reportObjPosition (op);
	
	/* Do test for Mars */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = MARS;	
	(void) strcpy (op->o_name, "Mars");
	(void) obj_cir (np, op);
	reportObjPosition (op);
	
	/* Do test for Jupiter */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = JUPITER;	
	(void) strcpy (op->o_name, "Jupiter");
	(void) obj_cir (np, op);
	reportObjPosition (op);
	
	/* Do test for Saturn */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = SATURN;	
	(void) strcpy (op->o_name, "Saturn");
	(void) obj_cir (np, op);
	reportObjPosition (op);
	
	/* Do test for Uranus */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = URANUS;	
	(void) strcpy (op->o_name, "Uranus");
	(void) obj_cir (np, op);
	reportObjPosition (op);
	
	/* Do test for Neptune */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = NEPTUNE;	
	(void) strcpy (op->o_name, "Neptune");
	(void) obj_cir (np, op);
	reportObjPosition (op);
	
	/* Do test for Pluto */
	memset (op, 0, sizeof(*op));
	op->o_type = PLANET;
	op->pl_code = PLUTO;	
	(void) strcpy (op->o_name, "Pluto");
	(void) obj_cir (np, op);
	reportObjPosition (op);
}

int main(int ac, char *av[]) {
	unitTest();
}
/**
	Prints out some helpful info from our now struct
*/
static void reportNowPosition(Now *np) {
	char sexa[32];
	int mon, year;
	double day;
	printf ("Observer (Now):\n");
	mjd_cal (np->n_mjd, &mon, &day, &year);
	printf ("UTC:       %d/%g/%d\n", mon, day, year);
	fs_sexa (sexa, raddeg(np->n_lat), 3, 3600);
	printf ("Latitude:    %s D:M:S +N\n", sexa);
	printf ("Latitude:    %f Degrees +N\n", raddeg(np->n_lat));
	fs_sexa (sexa, raddeg(np->n_lng), 3, 3600);
	printf ("Longitude:  %s D:M:S +E\n", sexa);
	printf ("Longitude: %f Degrees +N\n", raddeg(np->n_lng));
	printf ("\n");
}
/**
	Print out some helpful info from our obj struct
*/
static void reportObjPosition (Obj *op) {
	char sexa[32];
	printf ("%s:\n", op->o_name);
	fs_sexa (sexa, radhr(op->s_ra), 3, 3600);
	printf ("RA:          %s H:M:S\n", sexa);
	fs_sexa (sexa, raddeg(op->s_dec), 3, 3600);
	printf ("Dec:         %s D:M:S\n", sexa);
	fs_sexa (sexa, raddeg(op->s_az), 3, 3600);
	printf ("Azimuth:     %s D:M:S\n", sexa);
	printf ("Azimuth:     %f Degrees\n", raddeg(op->s_az));
	fs_sexa (sexa, raddeg(op->s_alt), 3, 3600);
	printf ("Elevation:   %s D:M:S\n", sexa);
	printf ("Elevation:  %f Degrees\n", raddeg(op->s_alt));
	printf("\n");
}
