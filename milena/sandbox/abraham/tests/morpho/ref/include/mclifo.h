/* $Id: mclifo.h,v 1.4 2006/02/28 07:49:12 michel Exp $ */

typedef struct {
  int32_t Max;          /* taille max de la Lifo */
  int32_t Sp;           /* index de pile (pointe la 1ere case libre) */
  int32_t Pts[1];
} Lifo;

/* ============== */
/* prototypes     */
/* ============== */

extern Lifo * CreeLifoVide(
  int32_t taillemax
);

extern void LifoFlush(
  Lifo * L
);

extern int32_t LifoVide(
  Lifo * L
);

extern int32_t LifoPop(
  Lifo * L
);

extern int32_t LifoHead(
  Lifo * L
);

extern void LifoPush(
  Lifo * L,
  int32_t V
);

extern void LifoPrint(
  Lifo * L
);

extern void LifoPrintLine(
  Lifo * L
);

extern void LifoTermine(
  Lifo * L
);
