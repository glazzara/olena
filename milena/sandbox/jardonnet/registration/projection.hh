
#ifndef PROJECTION_HH
# define PROJECTION_HH

# include <assert.h>

namespace mln
{

  namespace projection
  {

    template <class Pk_t, class X_t, class Xk_t>
    void de_base(// input
                 const Pk_t& Pk,
                 const X_t& X,
                 // inout
                 Xk_t& Xk,
                 // output
                 algebra::vec<3, float>& mu_Xk,
                 float& err)
    {
      assert(Pk.size() == Xk.size());

      err = 0.f;
      mu_Xk = make::vec(0,0,0);

      for (size_t i = 0; i < Pk.size(); ++i)
        {
          algebra::vec<3,float> best_x = X[0];
          float best_d = norm::l2(Pk[i] - best_x);
          for (size_t j = 1; j < X.size(); ++j)
            {
              float d = norm::l2(Pk[i] - X[j]);
              if (d < best_d)
                {
                  best_d = d;
                  best_x = X[j];
                }
            }
          Xk[i] = best_x;
          mu_Xk += Xk[i];
          err += best_d;
        }
      mu_Xk /= Pk.size();
      err /= Pk.size();
    }

  }
}

#endif // ndef PROJECTION_HH
