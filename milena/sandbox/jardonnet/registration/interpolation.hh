#ifndef MLN_INTERPOLATION_HH
# define MLN_INTERPOLATION_HH


namespace mln
{
  
  void polcoe(float x[], float y[], int n,
              float cof[])
  {
    int k,j,i;
    float phi,ff,b;
    std::vector<float> s(n,0);

    for (i = 0; i <= n; i++)
      s[i] = cof[i] = 0.0;
    s[n] = -x[0];
    for (i = 1; i <= n; i++)
      {
        for (j = n - i; j <= n - 1; j++)
          s[j] -= x[i] * s[j+1];
        s[n] -= x[i];
      }
    for (j = 0; j <= n; j++)
      {
        phi = n + 1;
        for (k = n; k >= 1; k--)
          phi = k * s[k] + x[j] * phi;
        ff = y[j] / phi;
        b = 1.0; 
        for (k = n; k >= 0; k--)
          {
            cof[k] += b * ff;
            b = s[k] + x[j] * b;
          }
      }
  }
  
}

#endif // ! MLN_INTERPOLATION_HH
