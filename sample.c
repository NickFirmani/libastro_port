/* example program to use libastro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astro.h"
#include "preferences.h"

static void reportPosition (Now *np, Obj *op);

int
main (int ac, char *av[])
{
	Now now, *np = &now;			/* Now and handy pointer */
	Obj obj, *op = &obj;			/* Obj and handy pointer */

	/* define local circumstances */
	
	memset (np, 0, sizeof(*np));		/* zero everything initially */
	cal_mjd (1, 1.0, 2014, &(np->n_mjd));		/* mjd (modified julian day) from calendar date and time */
	np->n_lat = degrad(37.8717);			/* latitude, +n rads */
	np->n_lng = degrad (-122.2728);			/* longitude, +e rads */
	np->n_tz = 7.0;				/* time zone, hrs behind UTC */
	np->n_temp = 27;				/* ambient air temp, C */
	np->n_pressure = 1013.25;				/* ambient air pressure, mBar */
	np->n_elev = 0;			/* local height above MSL (mean sea level), earth equatorial radii */
	//dip = degrad(18);			/* sun horizon altitude at twilight, rads down */
	np->n_epoch = J2000;				/* time of computed coords, MJD or EOD */
	pref_set (PREF_EQUATORIAL, PREF_TOPO);	/* set topocentric place, else GEO */

	/* define target object, example star */

	memset (op, 0, sizeof(*op));		/* zero everything initially */
	op->o_type = FIXED;			/* fixed, except possibly for proper motion */
	op->f_RA = hrrad(1.0);			/* RA, rads */
	op->f_dec = degrad(20.0);		/* dec, +n rads */
	op->f_epoch = J2000;			/* coord system */
	(void) strcpy (op->o_name, "Star");	/* name */
	(void) obj_cir (np, op);		/* compute position @ now */
	reportPosition (np, op);		/* print */

	/* define target object, example Moon */

	memset (op, 0, sizeof(*op));		/* zero everything initially */
	op->o_type = PLANET;			/* core type */
	op->pl_code = MOON;			/* ID code if PLANET */
	(void) strcpy (op->o_name, "Moon");	/* name */
	(void) obj_cir (np, op);		/* compute position @ now */
	reportPosition (np, op);		/* print */

	return (0);
}

static void
reportPosition (Now *np, Obj *op)
{
	int mon, year;
	double day;
	char sexa[32];

	printf ("Circumstances:\n");
	mjd_cal (mjd, &mon, &day, &year);
	printf ("UTC:       %d/%g/%d\n", mon, day, year);
	fs_sexa (sexa, raddeg(lat), 3, 3600);
	printf ("Latitude:  %s D:M:S +N\n", sexa);
	fs_sexa (sexa, raddeg(lng), 3, 3600);
	printf ("Longitude: %s D:M:S +E\n", sexa);
	printf ("\n");

	printf ("%s:\n", op->o_name);
	fs_sexa (sexa, radhr(op->s_ra), 3, 3600);
	printf ("RA:        %s H:M:S\n", sexa);
	fs_sexa (sexa, raddeg(op->s_dec), 3, 3600);
	printf ("Dec:       %s D:M:S\n", sexa);
	fs_sexa (sexa, raddeg(op->s_alt), 3, 3600);
	printf ("Altitude:  %s D:M:S\n", sexa);
	fs_sexa (sexa, raddeg(op->s_az), 3, 3600);
	printf ("Azimuth:   %s D:M:S\n", sexa);
    printf("\n");
    printf("Azimuth:   %f Degrees\n", raddeg(op->s_az));
    printf("Altitude (Elevation):   %f Degrees\n", raddeg(op->s_alt));
    
	printf ("\n");

	printf ("\n");
}
