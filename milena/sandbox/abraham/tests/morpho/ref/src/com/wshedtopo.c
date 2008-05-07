/* $Id: wshedtopo.c,v 1.12 2006/12/04 14:33:57 michel Exp $ */
/*! \file wshedtopo.c

\brief topological grayscale watershed

<B>Usage:</B> wshedtopo in.pgm connex [i] out.pgm

<B>Description:</B>
Topological grayscale watershed as defined in [CB97,CNB05].

The parameter \b connex indicates the connectivity of the minima.
Possible choices are 4, 8 in 2d and 6, 26 in 3d.

If the parameter <B>i</B> is given, then the dual operator is applied.

References:

[CB97] M. Couprie and G. Bertrand:
``Topological Grayscale Watershed Transformation'',
SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.

[CNB05] M. Couprie and L. Najman and G. Bertrand: 
"Quasi-linear algorithms for the topological watershed",  
Journal of Mathematical Imaging and Vision, Vol. 22, No. 2-3, pp. 231-249, 2005.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lwshedtopo.h>

/* ==================================== */
static void inverse(struct xvimage * image)
/* ==================================== */
{
  int32_t i, N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *pt;
  for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
    *pt = NDG_MAX - *pt;
} // inverse

/* =============================================================== */
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 5) && strcmp(argv[3],"i"))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  
  if (argc == 4) inverse(image);
  if (! lwshedtopo(image, connex))
  {
    fprintf(stderr, "%s: lwshedtopo failed\n", argv[0]);
    exit(1);
  }
  if (argc == 4) inverse(image);

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



