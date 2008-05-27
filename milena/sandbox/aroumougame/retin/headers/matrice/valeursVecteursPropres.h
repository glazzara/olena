//g++ essaisLibGSL.cpp -I/utilisateurs/venkiah/include -L/utilisateurs/venkiah/lib -lgsl -lgslcblas -lm -o essaisLibGSL
// librairie GSL (source august) voir doc : /utilisateurs/venkiah/documents/info/C/GNU_ScientificLibrary_gsl1.6-ref.pdf
// ou http://www.gnu.org/software/gsl/manual/gsl-ref_toc.html
// http://www.gnu.org/software/gsl/manual/html_node/index.html
#include <iostream>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

using namespace std;


void valeursVecteursPropres(int nbLignes, int nbColonnes, float *valeurs, float *valeursPropres, float *vecteursPropres);
