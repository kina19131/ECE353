#include <assert.h>
#include "common.h"
#include "point.h"
#include <math.h>

void
point_translate(struct point *p, double x, double y)
{
	double update_x = point_X(p) + x;
	double update_y = point_Y(p) + y;

	point_set(p, update_x, update_y);
	//p->x = update_x; 
	//p->y = update_y;
}

double
point_distance(const struct point *p1, const struct point *p2)
{
	double x1 = point_X(p1);
	double y1 = point_Y(p1);
	
	double x2 = point_X(p2); 
	double y2 = point_Y(p2);

	double cart_dist = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	return(cart_dist);
	//return -1.0;
}

int
point_compare(const struct point *p1, const struct point *p2)
{
	struct point origin;
	point_set(&origin, 0, 0);

	double euc_p1 = point_distance(&origin, p1); 
	double euc_p2 = point_distance(&origin, p2); 

	if (euc_p1 < euc_p2){
		return -1;
	}
	else if (euc_p1 == euc_p2){
		return 0; 
	}
	else{
		return 1; 
	}
}
