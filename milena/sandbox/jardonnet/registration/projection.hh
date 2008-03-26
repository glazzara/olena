
#ifndef PROJECTION_HH
# define PROJECTION_HH

# include <assert.h>

namespace mln
{

  namespace projection
  {

    template <typename P>
    void de_base(// input
                 const p_array<P>& Ck,
                 const p_array<P>& X,
                 // inout
                 p_array<P>& Xk,
                 float& err)
    {
      //      assert(Ck.npoints() == Xk.npoints());

      err = 0.f;

      for (size_t i = 0; i < Ck.npoints(); ++i)
        {
          algebra::vec<P::dim,float> Cki = Ck[i];
          algebra::vec<P::dim,float> best_x = X[0];
          float best_d = norm::l2(Cki - best_x);
          for (size_t j = 1; j < X.npoints(); ++j)
            {
              algebra::vec<P::dim,float> Xj = X[j];
              float d = norm::l2(Cki - Xj);
              if (d < best_d)
                {
                  best_d = d;
                  best_x = Xj;
                }
            }
          if (i < Xk.npoints()) // FIXME:double hack
            Xk.hook_()[i] = algebra::to_point<P>(best_x);
          err += best_d;
        }
      err /= Ck.npoints();
    }

  }
}

#endif // ndef PROJECTION_HH
