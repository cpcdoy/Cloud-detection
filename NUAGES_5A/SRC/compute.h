#pragma once
#include <gtk/gtk.h>

#define LargeurImage 360

void ComputeImage(guchar *pucImaOrig,
		  guint uiNbCol,
		  guint uiNbLine,
		  guchar *pucImaRes);

enum neighbor_loc
{
    SELF,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ALL
};

guchar get_pixel(guchar *img, enum neighbor_loc loc, int curr_loc);
guchar get_pixel_radiometry(guchar *img, int curr_loc);
