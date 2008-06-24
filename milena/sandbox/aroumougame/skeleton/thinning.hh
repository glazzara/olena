#include "neighborhood1.hh"
#include <mln/core/p_set.hh>


using namespace mln;

template <typename N>
    int nb_composant_connexe(p_set< mln_point(N) > X_full,const Neighborhood<N>& nbh,const mln_point(N)& p_ref, bool local)
{
  
  // FIXME: Optimization valid on 2-D grids only.
  p_set< mln_point(N) > X;
  if(local)
  {
    mln_niter(N) n(max_neighborhood(nbh), p_ref);
    
    for_all(n)
    {
      if (X_full.has(n))
        X.insert(n);
    }
  }
  else
  {
    X = X_full;
  }

  int T;
  p_set< mln_point(N) > done;
  p_set< mln_point(N) > neighbors;
  p_set< mln_point(N) > composant;
  
  done.insert(p_ref);
  mln_niter(N) q(nbh, p_ref);
  for_all(q)
  {
    if (X.has(q)&&!done.has(q))
    {
      neighbors.insert(q);
    }
  }
//   std::cout << "nb_composant_connexe: neighbors " << neighbors.npoints() <<std::endl; 
  if(neighbors.npoints()<=1)
  {
    return neighbors.npoints();
  }
  else
    T=0;
    
  while(neighbors.npoints()!=0)
  {
    T++;
    done.insert(neighbors[0]);
    mln_niter(N) t(nbh, neighbors[0]);
    neighbors.remove(neighbors[0]);
    for_all(t)
    {
      if (X.has(t)&&!done.has(t))
      {
        composant.insert(t);
      }
    }
     
    while(composant.npoints()!=0)
    {
      done.insert(composant[0]);
      if(neighbors.has(composant[0]))
      {
        neighbors.remove(composant[0]);
        if(neighbors.npoints()==0)
          return T;
      }
      
      mln_niter(N) r(nbh, composant[0]);
      composant.remove(composant[0]);
      for_all(r)
      {
        if (X.has(r) && !done.has(r))
        {
          composant.insert(r);
        }
      }
    }
  }
  return T;
}

template <typename N>
    bool simple_point(p_set< mln_point(N) > X,const Neighborhood<N>& nbh, p_set< mln_point(N) > X_complement, const mln_point(N)& p_ref, bool local)
{
  int nX = nb_composant_connexe(X,nbh,p_ref,local);
  int nX_complement = nb_composant_connexe(X_complement,complement_neighborhood(nbh),p_ref,local);
  
  if((nX_complement == 1)&&(nX == 1))
    return true;
  return false;
}


template <typename N>
    bool extremity(p_set< mln_point(N) > X,const Neighborhood<N>& nbh,const mln_point(N)& p_ref)
{
  int count=0;
  mln_niter(N) q(nbh, p_ref);
  for_all(q)
  {
    if (X.has(q) && count<2)
    {
      count++;
    }
  }
  if(count == 1)
    return true;
  return false;
}

template <typename N>
    int type(p_set< mln_point(N) > X,const Neighborhood<N>& nbh,const mln_point(N)& p_ref)
{
  int count=0;
  mln_niter(N) q(nbh, p_ref);
  for_all(q)
  {
    if (X.has(q))
    {
      count++;
    }
    else
    {
      return count;
    }
  }
  return -1;
}

// isthmus1d => X and isthmus2d => X_complement
template <typename N>
    bool isthmus(p_set< mln_point(N) > X,const Neighborhood<N>& nbh,const mln_point(N)& p_ref,bool local)
{
  int n = nb_composant_connexe(X,nbh,p_ref,local);
  if(n==2)
    return true;
  return false;
}

template <typename N>
    p_set< mln_point(N) > thinning(p_set< mln_point(N) > X, const Neighborhood<N>& nbh, p_set< mln_point(N) > X_complement, int L, bool local)
{
  p_set<mln_point(N)> P;
  p_set<mln_point(N)> Q;
  p_set<mln_point(N)> K;

  for(uint i=0; i<X.npoints(); i++)
  {
    if(simple_point(X,nbh,X_complement,X[i],local))
      P.insert(X[i]);
  }
//   std::cout << "thinning: P " << P.npoints() <<std::endl;
  while(P.npoints()!=0 && L>0)
  {
    Q.clear();
    for(int t=0; t<6; t++)
    {
      for(uint i=0; i <P.npoints(); i++)
      {
        if(isthmus(X_complement,complement_neighborhood(nbh),P[i],local))
        {
          K.insert(P[i]);
        }
        
        if(!K.has(P[i]) && simple_point(X,nbh,X_complement,P[i],local) && t==type(X,nbh,P[i]))
        {
          X.remove(P[i]);
          X_complement.insert(P[i]);
          
          mln_niter(N) r(nbh, P[i]);
          for_all(r)
          {
            if (X.has(r) && !K.has(r))
            {
              Q.insert(r);
            }
          }
        }
      }
    }
    P.clear();
    L--;
    
    for(uint i=0; i<Q.npoints(); i++)
    {
      if(simple_point(X,nbh,X_complement,Q[i],local))
      {
        P.insert(Q[i]);
      }
    }
  }

  return X;
}

