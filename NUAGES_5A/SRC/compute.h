#pragma once
# include <gtk/gtk.h>

# include <math.h>
# include <limits.h>


#define RAD_VECT_SIZE 5
#define LargeurImage 360

void ComputeImage(guchar *pucImaOrig,
		  guint uiNbCol,
		  guint uiNbLine,
		  guchar *pucImaRes);

#define MAX_VECT 5

guchar get_pixel_radiometry(guchar *img, int curr_loc);

double euclidean_dist(guchar p1[RAD_VECT_SIZE], guchar p2[RAD_VECT_SIZE]);
int get_closest_center(guchar v[RAD_VECT_SIZE], guchar (*centers)[RAD_VECT_SIZE], int nb_classes);
void assign_classes(guchar (*centers)[RAD_VECT_SIZE], guchar (*radiometry_img)[RAD_VECT_SIZE], guchar *class_img, int nb_classes, int nb_cols, int nb_lines);

void fill_lin_space(guchar (*arr)[MAX_VECT], guchar max_val, guchar min_val, int nb_classes);

void compute_radiometric_vectors(guchar (*radiometry_img)[RAD_VECT_SIZE], guchar *img, int nb_cols, int nb_lines, int rad_vect_size);

void compute_centers(guchar (*centers)[MAX_VECT], guchar (*radiometry_img)[RAD_VECT_SIZE], guchar *clf_img, int nb_cols, int nb_lines, int nb_classes);
