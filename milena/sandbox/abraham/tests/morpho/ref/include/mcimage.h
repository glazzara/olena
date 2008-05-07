/* $Id: mcimage.h,v 1.9 2006/02/28 07:49:12 michel Exp $ */
/* ============== */
/* prototypes for mcimage.c    */
/* ============== */

extern struct xvimage *allocimage(char * name, int32_t rs, int32_t cs, int32_t d, int32_t t);
extern void razimage(struct xvimage *f);
extern struct xvimage *allocheader(char * name, int32_t rs, int32_t cs, int32_t d, int32_t t);
extern int32_t showheader(char * name);
extern void freeimage(struct xvimage *image);
extern struct xvimage *copyimage(struct xvimage *f);
extern int32_t copy2image(struct xvimage *dest, struct xvimage *source);
extern int32_t equalimages(struct xvimage *im1, struct xvimage *im2);
extern void list2image(struct xvimage * image, double *P, int32_t n);
extern double * image2list(struct xvimage * image, int32_t *n);

extern void writeimage(
  struct xvimage * image,
  char *filename
);

extern void writese(
  struct xvimage * image,
  char *filename,
  int32_t x, int32_t y, int32_t z
);

extern void writelongimage(
  struct xvimage * image,
  char *filename
);

extern void writerawimage(
  struct xvimage * image,
  char *filename
);

extern void writeascimage(
  struct xvimage * image,
  char *filename
);

extern void printimage(
  struct xvimage * image
);

extern void writergbimage(
  struct xvimage * redimage,
  struct xvimage * greenimage,
  struct xvimage * blueimage,
  char *filename
);

extern struct xvimage * readimage(
  char *filename
);

extern struct xvimage * readheader(
  char *filename
);

extern struct xvimage * readse(char *filename, int32_t *x, int32_t *y, int32_t*z);

extern struct xvimage * readlongimage(
  char *filename
);

extern int32_t readrgbimage(
  char *filename,
  struct xvimage ** r,
  struct xvimage ** g,
  struct xvimage ** b
);

extern int32_t readbmp(
  char *filename, 
  struct xvimage ** r, 
  struct xvimage ** g, 
  struct xvimage ** b
);

extern void writebmp(
  struct xvimage * redimage,
  struct xvimage * greenimage,
  struct xvimage * blueimage,
  char *filename
);

extern int32_t readrgb(
  char *filename, 
  struct xvimage ** r, 
  struct xvimage ** g, 
  struct xvimage ** b
);

extern int32_t convertgen(struct xvimage **f1, struct xvimage **f2);
extern int32_t convertlong(struct xvimage **f1);
extern int32_t convertfloat(struct xvimage **f1);
