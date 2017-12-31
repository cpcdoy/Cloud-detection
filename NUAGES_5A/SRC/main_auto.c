#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <dirent.h>
#include <string.h>

#include "compute.h"

int is_image(const char *str)
{
    size_t len = strlen(str);

    const char *exts[] = { ".jpg", ".bmp", ".jpeg", ".png" };

    int bools[] = { 1, 1, 1, 1 };

    size_t len_exts = 4;

    for (int i = 0; i < len_exts; i++)
    {
        size_t len_curr_ext = strlen(exts[i]);
        size_t off = len - len_curr_ext;
        if (off > 0)
            for (int j = 0; j < len_curr_ext; j++)
            {
                if (exts[i][j] != str[(len - len_curr_ext) + j])
                {
                    bools[i] = 0;
                    break;
                }

            }
    }

    int res = 0;
    for (int i = 0; i < len_exts; i++)
        res |= bools[i];

    return res;
}

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


    DIR *d;

    struct dirent *dir;

    d = opendir(".");

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (is_image(dir->d_name))
            {
                printf("Filename: %s\n", dir->d_name);

                GError *error = NULL;
                GdkPixbuf *pix = gdk_pixbuf_new_from_file(dir->d_name, &error);
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
            }
        }

        closedir(d);
    }


    return(0);

}
