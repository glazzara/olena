/* ============================================================================== */
/* 
  Structure de donnees pour la construction de l'arbre des composantes.

  Les sommets de cet arbre representent les composantes des coupes de F,  
  a l'exception de celles qui sont egales a une composante d'un niveau inferieur.
  Il y a donc moins de N sommets (N = nombre de pixels) et de N-1 arcs.

  Une composante (sommet) est representee par une structure ctreenode.
*/
/* ============================================================================== */

#define ATTRIB_AREA
#define ATTRIB_VOL

typedef struct soncell   // cell structure for the lists of sons
{
  int32_t son;               // index of the son in table tabnodes [struct ctree]
  struct soncell *next;  // points to next cell
} soncell;

typedef struct           // structure for one node in the component tree
{
  u_int8_t data;    // node's level
  int32_t father;            // index of the father node. value -1 indicates the root
  int32_t nbsons;            // number or sons. value -1 indicates a deleted node
#ifdef ATTRIB_AREA
  int32_t area;              // number of pixels in the component
#endif
#ifdef ATTRIB_VOL
  int32_t vol;               // volume of the component
#endif
  soncell *sonlist;      // list of sons (points to the first son cell)
  soncell *lastson;      // direct pointer to the last son cell
} ctreenode;

typedef struct           // structure for a component tree
{
  int32_t nbnodes;           // total number of nodes
  int32_t nbleafs;           // total number of leafs
  int32_t nbsoncells;        // number of avaliable son cells
  int32_t root;              // index of the root node in table tabnodes
  ctreenode * tabnodes;  // table which contains all the nodes
  soncell * tabsoncells; // table which contains all the son cells
  u_int8_t *flags;  // each flag is associated to the node with the same index
} ctree;

/* ==================================== */
/* PROTOTYPES */
/* ==================================== */
#define IMGCHAR
//#define IMGLONG

#ifdef IMGCHAR
#define MAXGREY 256
#else
#define MAXGREY 65536
#endif

extern ctree * ComponentTreeAlloc(int32_t N);
extern void ComponentTreeFree(ctree * CT);
#ifdef IMGCHAR
extern int32_t ComponentTree( u_int8_t *F, int32_t rs, int32_t N, int32_t connex, // inputs
#endif
#ifdef IMGLONG
extern int32_t ComponentTree( u_int32_t *F, int32_t rs, int32_t N, int32_t connex, // inputs
#endif
                           ctree **CompTree, // output
                           int32_t **CompMap     // output
			 );
#ifdef IMGCHAR
extern int32_t ComponentTree3d( u_int8_t *F, int32_t rs, int32_t ps, int32_t N, int32_t connex, // inputs
#endif
#ifdef IMGLONG
extern int32_t ComponentTree3d( u_int32_t *F, int32_t rs, int32_t ps, int32_t N, int32_t connex, // inputs
#endif
                           ctree **CompTree, // output
                           int32_t **CompMap     // output
			 );
