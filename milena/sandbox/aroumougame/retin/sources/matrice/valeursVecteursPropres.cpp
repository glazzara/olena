#include "matrice/valeursVecteursPropres.h"

void valeursVecteursPropres(int nbLignes, int nbColonnes, float *valeurs, float *valeursPropres, float *vecteursPropres)
{
 
	gsl_matrix * matrice = gsl_matrix_alloc (nbLignes, nbColonnes);

	for (int i = 0; i < nbLignes; i++)
    		for (int j = 0; j < nbColonnes; j++)
      			gsl_matrix_set (matrice, i, j, valeurs[i*nbColonnes+j]);
	

	/*cout << "affichage de la matrice" << endl;
	for (size_t i = 0; i < nbLignes; i++)
		{
    		for (size_t j = 0; j < nbColonnes; j++)
      			cout << gsl_matrix_get (matrice, i, j) << "\t";
		cout << endl;
		} */
		
	// calcul des valeurs propres
	gsl_matrix_view matrixTemp =  gsl_matrix_submatrix (matrice, 0, 0, nbLignes, nbColonnes);  	// matrice temporaire pour calcul
  	gsl_vector *eval = gsl_vector_alloc (nbLignes);
  	gsl_matrix *evec = gsl_matrix_alloc (nbLignes, nbColonnes);
	gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (nbLignes);
  	gsl_eigen_symmv (&matrixTemp.matrix, eval, evec, w);
  	gsl_eigen_symmv_free (w);
  	gsl_eigen_symmv_sort (eval, evec, GSL_EIGEN_SORT_ABS_ASC);

	{
	size_t l=0;
	size_t k=0;
    	for (int i = 0; i < nbLignes; i++)
      {
        double eval_i = gsl_vector_get (eval, l);
	//cout << "eigenvalue : " << eval_i << endl;
	valeursPropres[i]=(float) eval_i;
	//cout << "eigenvalue : " << valeursPropres[i] << endl;
	
	k=0;
	for(int j=0; j < nbColonnes ; j++)
	{
		vecteursPropres[j*nbLignes+i]= gsl_matrix_get (evec, k, l);
		//cout << " eigenvector" << gsl_matrix_get (evec, k, l) << "\t" ;
	k++;
	}
	//cout << endl;
	l++;
	}
	}

	gsl_matrix_free (matrice);
	gsl_vector_free	(eval);
	gsl_matrix_free (evec);
}



