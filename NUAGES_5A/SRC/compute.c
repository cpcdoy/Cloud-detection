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
  for(i=0; i<img_size*nb_channels; i=i+nb_channels)
    for(channel_i=0; channel_i<nb_channels; channel_i++)
      img_res[i + channel_i] = get_pixel_radiometry(img_orig, i);
}

guchar get_pixel(guchar *img, enum neighbor_loc loc, int curr_loc)
{
    return 0;
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
