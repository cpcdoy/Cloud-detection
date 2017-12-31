#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include "compute.h"

/************************************************
fonction principale
************************************************/
int main (int argc, char **argv)
{
  /* la taille par defaut des images */
  int NbCol=LargeurImage;

  /*---------------------------------------------
    actions
    ---------------------------------------------*/

  /*initialisation des appels des bibliotheques*/
  gtk_init(&argc,&argv);

  GError *error = NULL;
  GdkPixbuf *pix = gdk_pixbuf_new_from_file ("../IMAGES/20342579204091110141x.bmp", &error);
  if (pix == NULL)
  {
      g_printerr ("Error loading file: #%d %s\n", error->code, error->message);
      g_error_free (error);
      exit (1);
  }

  guchar *img = gdk_pixbuf_get_pixels(pix);

  GdkPixbuf *pGdkPixbufImaRes=gdk_pixbuf_copy(pix);

  NbCol=gdk_pixbuf_get_width(pix);
  int NbLine=gdk_pixbuf_get_height(pix);
  guchar *pucImaRes=gdk_pixbuf_get_pixels(pGdkPixbufImaRes);
  /* on realise le calcul sur l'image */
  ComputeImage(img,NbLine,NbCol,pucImaRes);

  return(0);

}
