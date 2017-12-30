#include <stdio.h>
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
void ComputeImage(guchar *img_orig,
		  guint nb_lines,
		  guint nb_cols,
		  guchar *img_res)
{
  int img_size, i;
  int channel_i, nb_channels=3; /* on travaille sur des images couleurs*/

  printf("Img: l:%d c:%d\n", nb_lines, nb_cols);

  img_size=nb_cols*nb_lines;
  guchar max_val = 0;
  guchar min_val = 255;
  for(i=0; i<img_size*nb_channels; i=i+nb_channels)
    for(channel_i=0; channel_i<nb_channels; channel_i++)
    {
      guchar c = get_pixel_radiometry(img_orig, i);
      img_res[i + channel_i] = c;

      max_val = MAX(max_val, c);
      min_val = MIN(min_val, c);
    }

  printf("Max: %d\n", max_val);
  printf("Min: %d\n", min_val);

  //const int nb_classes_p = 8;
  const int nb_classes_xs = 9;
  int nb_classes = nb_classes_xs;

  guchar centers[nb_classes][MAX_VECT];
  fill_lin_space(centers, max_val, min_val, nb_classes);
}

void fill_lin_space(guchar (*arr)[MAX_VECT], guchar max_val, guchar min_val, int nb_classes)
{
  int step = (max_val - min_val) / nb_classes;
  printf("Step: %d\n", step);
  printf("m: %d\n", max_val);
  for (int i = min_val + step / 2, center = 0; i < max_val && center < nb_classes; i += step, center++)
  {
    printf("%d %d\n", i, center);
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
