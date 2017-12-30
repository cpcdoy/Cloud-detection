#pragma once
#include <gtk/gtk.h>

#define LargeurImage 360

void ComputeImage(guchar *pucImaOrig,
		  guint uiNbCol,
		  guint uiNbLine,
		  guchar *pucImaRes);

#define MAX_VECT 5

guchar get_pixel_radiometry(guchar *img, int curr_loc);
void fill_lin_space(guchar (*arr)[MAX_VECT], guchar max_val, guchar min_val, int nb_classes);
