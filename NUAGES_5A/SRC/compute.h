#pragma once
#include <gtk/gtk.h>
#define RAD_VECT_SIZE 5
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
void compute_radiometry(guchar (*radiometry_img)[RAD_VECT_SIZE], guchar *img, int nb_cols, int nb_lines, int rad_vect_size);
