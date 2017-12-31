#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "compute.h"

/*******************************************************
  IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
  DES FONCTIONS
 *******************************************************/


/*---------------------------------------
Proto:


But:

Entrees:
--->le tableau des valeurs des pixels de l'image d'origine
(les lignes sont mises les unes à la suite des autres)
--->le nombre de lignes de l'image,
--->le nombre de colonnes de l'image,
--->le tableau des valeurs des pixels de l'image resultat
(les lignes sont mises les unes à la suite des autres)


Sortie:

Rem:

Voir aussi:

---------------------------------------*/
int isequal(guchar (*centers)[RAD_VECT_SIZE],  guchar (*last_centers)[RAD_VECT_SIZE], int nb_classes)
{
    for (int i = 0; i < nb_classes; i++)
        for (int j = 0; j < MAX_VECT; j++)
        {
            if (centers[i][j] != last_centers[i][j])
                return 0;
        }
    return 1;
}

void ComputeImage(guchar *img_orig,
        guint nb_lines,
        guint nb_cols,
        guchar *img_res)
{
    int img_size, i;
    int channel_i, nb_channels=3; /* on travaille sur des images couleurs*/

    printf("Img: l:%d c:%d\n", nb_lines, nb_cols);

    // Find min/max
    img_size = nb_cols * nb_lines;
    guchar max_val = 0;
    guchar min_val = 255;
    guchar img_rad[img_size];
    int j = 0;
    for (i=0; i<img_size*nb_channels; i=i+nb_channels, j++)
        for (channel_i=0; channel_i<nb_channels; channel_i++)
        {
            guchar c = get_pixel_radiometry(img_orig, i);
            img_res[i + channel_i] = c;
            img_rad[j] = c;

            max_val = MAX(max_val, c);
            min_val = MIN(min_val, c);
        }

    //printf("Max: %d\n", max_val);
    //printf("Min: %d\n", min_val);

    //const int nb_classes_p = 8;
    const int nb_classes_xs = 9;
    int nb_classes = nb_classes_xs;

    // Init of mass centers
    guchar centers[nb_classes][MAX_VECT];
    guchar last_centers[nb_classes][MAX_VECT];
    fill_lin_space(centers, max_val, min_val, nb_classes);

    guchar radiometry_img[img_size][RAD_VECT_SIZE];
    guchar class_img[img_size];

    compute_radiometric_vectors(radiometry_img, img_rad, nb_cols, nb_lines, RAD_VECT_SIZE);

    assign_classes(centers, radiometry_img, class_img, nb_classes, nb_cols, nb_lines);

    compute_centers(centers, radiometry_img, class_img, nb_cols, nb_lines, nb_classes);

    for (int i = 0; i < nb_classes; i++)
        for (int j = 0; j < MAX_VECT; j++)
            last_centers[i][j] = 0;

    while (!isequal(centers, last_centers, nb_classes))
    {
        for (int i = 0; i < nb_classes; i++)
            for (int j = 0; j < MAX_VECT; j++)
                last_centers[i][j] = centers[i][j];
        // Compute radiometric vectors
        compute_radiometric_vectors(radiometry_img, img_rad, nb_cols, nb_lines, RAD_VECT_SIZE);

        assign_classes(centers, radiometry_img, class_img, nb_classes, nb_cols, nb_lines);

        compute_centers(centers, radiometry_img, class_img, nb_cols, nb_lines, nb_classes);
        //printf("----------------- It %d\n", i);
    }

    j = 0;
    int cloud_nb = 0;
    for (i=0; i<img_size*nb_channels; i=i+nb_channels, j++)
        for (channel_i=0; channel_i<nb_channels; channel_i++)
        {
            guchar c = class_img[j];
            if (c < nb_classes - 3)
                img_res[i + channel_i] = 0;
            else
                cloud_nb++;
        }
    cloud_nb /= 3;
    printf("Cloud percent : %f\n", ((float)cloud_nb / (float)img_size) * 100.f);
}

// Utils
double euclidean_dist(guchar p1[RAD_VECT_SIZE], guchar p2[RAD_VECT_SIZE])
{
    double dist = 0.0;
    for (int i = 0; i < RAD_VECT_SIZE; i++)
    {
        double p = p2[i] - p1[i];
        dist += p * p;
    }

    return sqrt(dist);
}

int get_closest_center(guchar v[RAD_VECT_SIZE], guchar (*centers)[RAD_VECT_SIZE], int nb_classes)
{
    int min_class = 0;
    double min_dist = INT_MAX;
    for (int i = 0; i < nb_classes; i++)
    {
        double dist = euclidean_dist(v, centers[i]);
        if (dist < min_dist)
        {
            min_class = i;
            min_dist = dist;
        }
    }

    return min_class;
}

void assign_classes(guchar (*centers)[RAD_VECT_SIZE], guchar (*radiometry_img)[RAD_VECT_SIZE], guchar *class_img, int nb_classes, int nb_cols, int nb_lines)
{
    for (int i = 0; i < nb_cols; i++)
        for (int j = 0; j < nb_lines; j++)
        {
            int loc = i + j * nb_cols;
            class_img[loc] = get_closest_center(radiometry_img[loc], centers, nb_classes);
        }
}

int compare_guchar(const void *a, const void *b)
{
    return (*(guchar*)b - *(guchar*)a);
}

void compute_centers(guchar (*centers_out)[MAX_VECT], guchar (*radiometry_img)[RAD_VECT_SIZE], guchar *clf_img, int nb_cols, int nb_lines, int nb_classes)
{
    int center_occurences[nb_classes];
    int centers[nb_classes][MAX_VECT];

    for (int i = 0; i < nb_classes; i++)
    {
        center_occurences[i] = 0;
        for (int j = 0; j < MAX_VECT; j++)
            centers[i][j] = 0;
    }

    for (int i = 0; i < nb_cols; i++)
    {
        for (int j = 0; j < nb_lines; j++)
        {
            int class = clf_img[i + j * nb_cols];
            center_occurences[class]++;
            if (class == nb_classes - 1)
                continue;
            for (int u = 0; u < MAX_VECT; u++)
                centers[class][u] += radiometry_img[i + j * nb_cols][u];
        }
    }

    int cloud_size = center_occurences[nb_classes - 1] * MAX_VECT;
    guchar center_cloud_vals[cloud_size];
    int cloud_idx = 0;
    for (int i = 0; i < nb_cols; i++)
        for (int j = 0; j < nb_lines; j++)
        {
            int class = clf_img[i + j * nb_cols];
            if (class == nb_classes - 1)
                for (int u = 0; u < MAX_VECT; u++, cloud_idx++)
                    center_cloud_vals[cloud_idx] = radiometry_img[i + j * nb_cols][u];
        }

    qsort(&center_cloud_vals, cloud_size, sizeof (guchar), compare_guchar);

    int median_cloud_rad = center_cloud_vals[cloud_size / 2];

    for (int j = 0; j < MAX_VECT; j++)
        centers_out[nb_classes - 1][j] = median_cloud_rad;

    for (int i = 0; i < nb_classes; i++)
        for (int j = 0; j < MAX_VECT; j++)
        {
            if (center_occurences[i] > 0 && i != nb_classes - 1)
            {
                centers[i][j] /= center_occurences[i];
                centers_out[i][j] = (guchar)centers[i][j];
            }
            //printf("center: %d, value: %d\n", i, centers_out[i][j]);
        }

}

void compute_radiometric_vectors(guchar (*radiometry_img)[RAD_VECT_SIZE], guchar *img, int nb_cols, int nb_lines, int rad_vect_size)
{
    int img_size = nb_cols * nb_lines;
    int rad_idx = 0;
    for (int i = 0; i < nb_cols; i++)
    {
        for (int j = 0; j < nb_lines; j++)
        {
            radiometry_img[i + j * nb_cols][0] = img[i + j * nb_cols];
            rad_idx = 1;
            if ((i + 1) + j * nb_cols < img_size)
            {
                radiometry_img[i + j * nb_cols][rad_idx] = img[(i + 1) + j * nb_cols];
                rad_idx++;
            }
            if ((i - 1) + j * nb_cols > 0)
            {
                radiometry_img[i + j * nb_cols][rad_idx] = img[(i - 1) + j * nb_cols];
                rad_idx++;
            }
            if (i + (j + 1) * nb_cols < img_size)
            {
                radiometry_img[i + j * nb_cols][rad_idx] = img[i + (j + 1) * nb_cols];
                rad_idx++;
            }
            if (i + (j - 1) * nb_cols > 0)
            {
                radiometry_img[i + j * nb_cols][rad_idx] = img[i + (j - 1) * nb_cols];
                rad_idx++;
            }
            while (rad_idx < rad_vect_size)
            {
                radiometry_img[i + j * nb_cols][rad_idx] = 0;
                rad_idx++;
            }
            qsort(&radiometry_img[i + j * nb_cols], rad_vect_size, sizeof (guchar), compare_guchar);
        }
    }
}

void fill_lin_space(guchar (*arr)[MAX_VECT], guchar max_val, guchar min_val, int nb_classes)
{
    int step = (max_val - min_val) / nb_classes;
    //printf("Step: %d\n", step);
    //printf("m: %d\n", max_val);
    for (int i = min_val + step / 2, center = 0; i < max_val && center < nb_classes; i += step, center++)
    {
        //printf("%d %d\n", i, center);
        for (int j = 0; j < MAX_VECT; j++)
            arr[center][j] = i;
    }
}

guchar get_pixel_radiometry(guchar *img, int curr_loc)
{
    guchar mean_pix = (unsigned char)
        ((
          *(img + curr_loc) +
          *(img + curr_loc + 1) +
          *(img + curr_loc + 2)) / 3);

    return mean_pix;
}
