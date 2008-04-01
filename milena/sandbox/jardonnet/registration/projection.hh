
#ifndef PROJECTION_HH
# define PROJECTION_HH

# include <assert.h>

namespace mln
{

  namespace projection
  {

    template <typename P, typename T1, typename T2>
    float fill_Xk(const p_array<P>& Ck,
                  std::pair<T1,T2>& map,
                  p_array<P>& Xk)
    {
      assert(Ck.npoints() == Xk.npoints());
      
      float err = 0.f;

      for (size_t i = 0; i < Ck.npoints(); ++i)
        {
          //FIXME: bof
          if (map.second.has(Ck[i]))
            {
              //x[i] := Ck[i] closest point in X
              Xk.hook_()[i] = map.second(Ck[i]);
              //err := Distance between Ck[i] and its closest point
              err += map.first(Ck[i]);
            }
        }
      return err /= Ck.npoints();
    }
    
    template <typename P>
    void de_base(// input
                 const p_array<P>& Ck,
                 const p_array<P>& X,
                 // inout
                 p_array<P>& Xk,
                 float& err)
    {
      assert(Ck.npoints() == Xk.npoints());

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
          Xk.hook_()[i] = algebra::to_point<P>(best_x);
          err += best_d;
        }
      err /= Ck.npoints();
    }
  
        
    template <typename P, typename T>
    void memo(const p_array<P>& Ck,
              const p_array<P>& X,
              p_array<P>& Xk,
              lazy_map<T>& map) // first: closest points, second: is_computed
    {
      assert(Ck.npoints() == Xk.npoints());

      for (size_t i = 0; i < Ck.npoints(); ++i)
        {
          float best_d;
          if (map.known(Ck[i]) == false)
            {
              algebra::vec<P::dim,float> Cki = Ck[i];
              algebra::vec<P::dim,float> best_x = X[0];
              best_d = norm::l2(Cki - best_x);
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
              Xk.hook_()[i] = algebra::to_point<P>(best_x);
              map.map(Ck[i]) = Xk[i];
              map.known(Ck[i]) = true;
            }
          else
            Xk.hook_()[i] = map.map(Ck[i]);
        }
    }
    
  } // end of namespace projection
  
} // end of namespace mln

#endif // ndef PROJECTION_HH
