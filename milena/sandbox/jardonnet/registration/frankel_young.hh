#ifndef _FRANKEL_YOUNG_HH
# define _FRANKEL_YOUNG_HH

//Successive over-relaxation (SOR) is a numerical method used to speed up
//convergence of the Gauss-Seidel method for solving a linear system of
//equations. 

namespace mln
{
  
  algebra::quat frankel_young(algebra::mat<4,4,float> a, float w)
  {
    //Inputs: A , b, w
    //Output: phi

    algebra::vec<4,float> phi(literal::zero);
    algebra::vec<4,float> old_phi(literal::zero);

    algebra::vec<4,float> b;
    for (int i = 0; i < 4; i++)
      b[i] = phi[i] = a(i,i);
    
    //Choose an initial guess phi to the solution
    while (fabs(norm::l2(old_phi) - norm::l2(phi)) > 1e-6)
      {
        old_phi = phi;
        for (int i = 1; i < 4; i++)
          {
            float sigma = 0;
            
            for (int j = 1; j < i-1; j++)
              sigma += a(i,j) * phi[i];
            
            for (int j = i + 1; j < 4; j++)
              sigma += a(i,j) * phi[i];
            
            phi[i] = (1 - w) * phi[i] + w / a(i,i) * (b[i] - sigma);
          }
      }
    algebra::quat q(phi);
    q.set_unit();
    return q;
  } 
}

#endif /* _FRANKEL_YOUNG_HH */
