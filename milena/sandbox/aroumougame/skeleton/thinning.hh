#include "neighborhood1.hh"
#include <mln/core/p_set.hh>
#include <mln/core/graph_image.hh>
#include <mln/core/graph_elt_neighborhood.hh>
#include <mln/math/min.hh>
#include <mln/math/sqrt.hh>
#include <mln/make/dpoint2d.hh>
#include "miscellenous.hh"
#include "sedt.hh"


using namespace mln;

template <typename N>
    int nb_composant_connexe(p_set< mln_point(N) > X_full,const Neighborhood<N>& nbh,const mln_point(N)& p_ref, bool local)
{
  
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

template <typename P>
    int nb_composant_connexe(graph_image<P, bool> gi, bool object, const typename graph_image<P, bool>::psite& p_ref, bool local)
{
  int T;
  p_set< graph_psite<P> > done;
  p_set< graph_psite<P> > neighbors;
  p_set< graph_psite<P> > composant;
  
  done.insert(p_ref);
  graph_elt_neighborhood< P > nbh;
  mln_niter(graph_elt_neighborhood< P >) fq(nbh, p_ref);
  for_all(fq)
  {
    if (gi(fq)==object && !done.has(fq))
    {
      neighbors.insert(fq);
    }
  }

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
    mln_niter(graph_elt_neighborhood< P >) t(nbh, neighbors[0]);
    neighbors.remove(neighbors[0]);
    for_all(t)
    {
      if (gi(t)==object && !done.has(t))
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
      
      mln_niter(graph_elt_neighborhood< P >) r(nbh, composant[0]);
      composant.remove(composant[0]);
      for_all(r)
      {
        if (gi(r)==object && !done.has(r))
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

template <typename P>
    bool simple_point(graph_image<P, bool> gi, const typename graph_image<P, bool>::psite& p_ref, bool local)
{
  int nX = nb_composant_connexe(gi,true,p_ref,local);
  int nX_complement = nb_composant_connexe(gi,false,p_ref,local);
  
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

template <typename P>
    bool extremity(graph_image<P, bool> gi, const typename graph_image<P, bool>::psite& p_ref)
{
  int count=0;
  graph_elt_neighborhood< P > nbh;
  mln_niter(graph_elt_neighborhood< P >) n(nbh, p_ref);
  for_all(n)
    if (gi(n) && count<2)
    {
      count++;
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
// to be modified in thinning
template <typename N>
    bool isthmus(p_set< mln_point(N) > X,const Neighborhood<N>& nbh,const mln_point(N)& p_ref,bool local)
{
  int n = nb_composant_connexe(X,nbh,p_ref,local);
  if(n==2)
    return true;
  return false;
}

// isthmus1d => type_isthmus = true and isthmus2d => type_isthmus = false
// to be modified in thinning
template <typename P>
    bool isthmus(graph_image<P, bool> gi, bool type_isthmus, const typename graph_image<P, bool>::psite& p_ref, bool local)
{
  int n = nb_composant_connexe(gi,type_isthmus,p_ref,local);
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

  while(P.npoints()!=0 && L>0)
  {
    Q.clear();
    // FIXME need a method to get the number of neighbor from the neighborhood instead of the 6
    for(int t=0; t<6; t++)
    {
      for(uint i=0; i <P.npoints(); i++)
      {
        // isthmus1d => X and isthmus2d => X_complement
        if(extremity(X,nbh,P[i]))
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

template <typename P>
    graph_image<P, bool> thinning(graph_image<P, bool> gi, int L, bool local)
{
  typedef graph_image<P, bool> gi_t;
  
  p_set< graph_psite<P> > N;
  p_set< graph_psite<P> > Q;
  p_set< graph_psite<P> > K;

  mln_piter(gi_t) p_ref(gi.domain());
  for_all(p_ref)
  {
    if(gi(p_ref) && simple_point(gi,p_ref,false))
      N.insert(p_ref);
  }

  while(N.npoints()!=0 && L>0)
  {
    Q.clear();
    
    for(uint i=0; i <N.npoints(); i++)
    {
        // isthmus1d => type_isthmus = true and isthmus2d => type_isthmus = false
      if(extremity(gi,N[i]))
      {
        K.insert(N[i]);
      }
        
      if(!K.has(N[i]) && simple_point(gi,N[i],false))
      {
        gi(N[i]) = false;
          
        graph_elt_neighborhood< P > nbh;
        mln_niter(graph_elt_neighborhood< P >) r(nbh, N[i]);
        for_all(r)
        {
          if (gi(r) && !K.has(r))
          {
            Q.insert(r);
          }
        }
      }
    }
    
    N.clear();
    L--;
    
    for(uint i=0; i<Q.npoints(); i++)
    {
      if(simple_point(gi,Q[i],false))
      {
        N.insert(Q[i]);
      }
    }
  }

  return gi;
}

template <typename P>
    int nb_composant_connexe(p_set< graph_psite<P> > X_full, const typename graph_image<P, bool>::psite& p_ref, bool local)
{
  
  p_set< graph_psite<P> > X;
  p_set< graph_psite<P> > neighbors;
  p_set< graph_psite<P> > composant;
  p_set< graph_psite<P> > done;
  
  graph_elt_neighborhood< P > nbh;
  if(local)
  {
    mln_niter(graph_elt_neighborhood< P >) n(nbh, p_ref);
    for_all(n)
    {
      if (X_full.has(n)&& !done.has(n))
      {
        neighbors.insert(n);
        X.insert(n);
      }
    }
    
    for(int i=0; i < neighbors.npoints(); i++)
    {
      mln_niter(graph_elt_neighborhood< P >) n(nbh, neighbors[i]);
    
      for_all(n)
      {
        if (X_full.has(n) && !X.has(n))
        {
          composant.insert(n);
          X.insert(n);
        }
      }
    }
    
    for(int i=0; i < composant.npoints(); i++)
    {
      mln_niter(graph_elt_neighborhood< P >) n(nbh, composant[i]);
    
      for_all(n)
      {
        if (X_full.has(n) && !X.has(n))
        {
          X.insert(n);
        }
      }
    }
    
    neighbors.clear();
    composant.clear();
  }
  else
  {
    X = X_full;
  }
  
  int T;
  
  done.insert(p_ref);
  
  mln_niter(graph_elt_neighborhood< P >) fq(nbh, p_ref);
  for_all(fq)
  {
    if (X.has(fq) && !done.has(fq))
    {
      neighbors.insert(fq);
    }
  }

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
    mln_niter(graph_elt_neighborhood< P >) t(nbh, neighbors[0]);
    neighbors.remove(neighbors[0]);
    for_all(t)
    {
      if (X.has(t) && !done.has(t))
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
      
      mln_niter(graph_elt_neighborhood< P >) r(nbh, composant[0]);
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

template <typename P>
    bool simple_point(p_set< graph_psite<P> > X, p_set< graph_psite<P> > X_complement, const typename graph_image<P, bool>::psite& p_ref, bool local)
{
  int nX = nb_composant_connexe(X,p_ref,local);
  int nX_complement = nb_composant_connexe(X_complement,p_ref,local);
  
  if((nX_complement == 1)&&(nX == 1))
    return true;
  return false;
}


// isthmus1d => X and isthmus2d => X_complement
// to be modified in thinning
template <typename P>
    bool isthmus(p_set< graph_psite<P> > X, const typename graph_image<P, bool>::psite& p_ref, bool local)
{
  int n = nb_composant_connexe(X,p_ref,local);
  if(n==2)
    return true;
  return false;
}


template <typename N>
    p_set< graph_psite<N> > thinning(p_set< graph_psite<N> > X, p_set< graph_psite<N> > X_complement, int L, bool local)
{
  p_set< graph_psite<N> > P;
  p_set< graph_psite<N> > Q;
  p_set< graph_psite<N> > K;

  for(uint i=0; i<X.npoints(); i++)
  {
    if(simple_point(X,X_complement,X[i],local))
      P.insert(X[i]);
  }

  while(P.npoints()!=0 && L>0)
  {
    Q.clear();
    
    for(uint i=0; i <P.npoints(); i++)
    {
        // isthmus1d => X and isthmus2d => X_complement
//       if(isthmus(X,P[i],local))
//       {
//         K.insert(P[i]);
//       }
        
      if(!K.has(P[i]) && simple_point(X,X_complement,P[i],local))
      {
        X.remove(P[i]);
        X_complement.insert(P[i]);
          
        graph_elt_neighborhood< N > nbh;
        mln_niter(graph_elt_neighborhood< N >) r(nbh, P[i]);
        for_all(r)
        {
          if (X.has(r) && !K.has(r))
          {
            Q.insert(r);
          }
        }
      }
    }
    
    P.clear();
    L--;
    
    for(uint i=0; i<Q.npoints(); i++)
    {
      if(simple_point(X,X_complement,Q[i],local))
      {
        P.insert(Q[i]);
      }
    }
  }

  return X;
}


template <typename N>
    p_set<mln_point(N)> euclideanSkeleton(p_set<mln_point(N)> X, p_set<mln_point(N)> X_complement, image2d<value::int_u8> dt, p_set<mln_point(N)>& Y, const Neighborhood<N>& nbh, bool local)
{
  std::vector< std::pair< double, mln::point2d> > Q;
  std::vector< std::pair< double, mln::point2d> > R;
  
  // fill Q
  for(uint i = 0; i < X.npoints(); i++)
  {
    if (!Y.has(X[i]))
    {
      std::pair<double, mln_point(N)> p(math::sqrt(double(dt(X[i]))),X[i]);
      Q = insertDicho(Q,p);
    }
  }
  
  // fill R
  for(uint i = 0; i < X.npoints(); i++)
  {
    if (!Y.has(X[i]))
    {
      double min = 1023.99;
      mln_niter(N) r(nbh, X[i]);
      for_all(r)
      {
        if(Y.has(r))
        {
          double tmp = distance(r[0], r[1], X[i][0], X[i][1]);
          double d = math::sqrt(double(dt(r)))+(math::sqrt(double(dt(X[i])))-math::sqrt(double(dt(r))))/tmp;
          min = math::min(min,d);
        }
      }
      if(min!=1023.99)
      {
        std::pair<double, mln_point(N)> p(min,X[i]);
        R = insertDicho(R, p);
      }
    }
  }
  
  while(!Q.empty() || !R.empty())
  {
    mln_point(N) tmp;
    if(Q[0].first < R[0].first)
    {
      tmp = Q[0].second;
    }
    else
    {
      tmp = R[0].second;
    }
    
    Q = remove(Q, tmp);
    R = remove(R, tmp);
    
    if(!Y.has(tmp) && X.has(tmp))
    {
      if(simple_point(X, nbh, X_complement, tmp, local))
      {
        X.remove(tmp);
        X_complement.insert(tmp);
      }
      else
      {
        Y.insert(tmp);
        mln_niter(N) r(nbh, tmp);
        for_all(r)
        {
          if(!Y.has(r) && X.has(r))
          {
            double dist = distance(r[0], r[1], tmp[0], tmp[1]);
            double d = math::sqrt(double(dt(tmp)))+(math::sqrt(double(dt(r)))-math::sqrt(double(dt(tmp))))/dist;
            std::pair<double, mln_point(N)> p(d,r);
            R = insertDicho(R, p);
          }
        }
        
      }
    }
    
  }
  return X;
}


    p_set<point2d> EP(image2d<value::int_u8> dt, point2d x, std::vector< std::vector<std::pair< int, int> > > lut, const Neighborhood<neighb2d>& nbh)
{
  p_set<point2d> EP;
  p_set<point2d> tmp;
  int w = geom::ncols(dt);
  int h = geom::nrows(dt);
  
  
  mln_niter_(neighb2d) r(nbh, x);
  for_all(r)
  {
    if(dt(r) <= dt(x))
    {
      for(uint i=0; i<lut[dt(r)].size(); i++)
      {
        if((r[0]+lut[dt(r)][i].first < h) && (r[1]+lut[dt(r)][i].second < w))
        {
          if(!dt(r+dpoint2d(lut[dt(r)][i].first, lut[dt(r)][i].second)))
            EP.insert(r+dpoint2d(lut[dt(r)][i].first, lut[dt(r)][i].second));
        }
        if((r[0]-lut[dt(r)][i].first >= 0) && (r[1]-lut[dt(r)][i].second >= 0))
        {
          if(!dt(r+dpoint2d(-lut[dt(r)][i].first, -lut[dt(r)][i].second)))
            EP.insert(r+dpoint2d(-lut[dt(r)][i].first, -lut[dt(r)][i].second));
        }
        if((r[0]+lut[dt(r)][i].first < h) && (r[1]-lut[dt(r)][i].second >= 0))
        {
          if(!dt(r+dpoint2d(lut[dt(r)][i].first, -lut[dt(r)][i].second)))
            EP.insert(r+dpoint2d(lut[dt(r)][i].first, -lut[dt(r)][i].second));
        }
        if((r[0]-lut[dt(r)][i].first >= 0) && (r[1]+lut[dt(r)][i].second < w))
        {
        if(!dt(r+dpoint2d(-lut[dt(r)][i].first, lut[dt(r)][i].second)))
          EP.insert(r+dpoint2d(-lut[dt(r)][i].first, lut[dt(r)][i].second));
        }
        if((r[0]+lut[dt(r)][i].second < h) && (r[1]+lut[dt(r)][i].first < w))
        {
          if(!dt(r+dpoint2d(lut[dt(r)][i].second, lut[dt(r)][i].first)))
            EP.insert(r+dpoint2d(lut[dt(r)][i].second, lut[dt(r)][i].first));
        }
        if((r[0]-lut[dt(r)][i].second >= 0) && (r[1]-lut[dt(r)][i].first >= 0))
        {
          if(!dt(r+dpoint2d(-lut[dt(r)][i].second, -lut[dt(r)][i].first)))
            EP.insert(r+dpoint2d(-lut[dt(r)][i].second, -lut[dt(r)][i].first));
        }
        if((r[0]+lut[dt(r)][i].second < h) && (r[1]-lut[dt(r)][i].first >= 0))
        {
          if(!dt(r+dpoint2d(lut[dt(r)][i].second, -lut[dt(r)][i].first)))
            EP.insert(r+dpoint2d(lut[dt(r)][i].second, -lut[dt(r)][i].first));
        }
        if((r[0]-lut[dt(r)][i].second >= 0) && (r[1]+lut[dt(r)][i].first < w))
        {
          if(!dt(r+dpoint2d(-lut[dt(r)][i].second, lut[dt(r)][i].first)))
            EP.insert(r+dpoint2d(-lut[dt(r)][i].second, lut[dt(r)][i].first));
        }
      }
    }
  }
  
  return EP;
}

image2d<value::int_u8> DiscreteBisector(image2d<value::int_u8> dt, p_set<point2d> Y, const Neighborhood<neighb2d>& nbh, int N)
{
  int w = geom::ncols(dt);
  int h = geom::nrows(dt);
  
  int ux,uy,vx,vy, produit, angle, max;
  double cos, normu, normv;
  
  std::vector< std::vector<std::pair< int, int> > > lut;
  lut = Lut2d(N);
  
  p_set<point2d> proj;
  
  image2d<value::int_u8> bisector(h, w);
  level::fill(bisector, 0);
  
  for(uint i=0; i<Y.npoints(); i++)
  {
    proj = EP(dt, Y[i], lut, nbh);
    
    int n=proj.npoints();

    if(n>1)
    {
      max = 0;
      for(int y=0; y<n; y++)
      {
        for(int z=0; z<y; z++)
        {
          ux = proj[y][0]-Y[i][0];
          uy = proj[y][1]-Y[i][1];
          vx = proj[z][0]-Y[i][0];
          vy = proj[z][1]-Y[i][1];
          
          produit = ux * vx + uy * vy;
          
          normu = sqrt(ux*ux + uy*uy);
          normv = sqrt(vx*vx + vy*vy);
          
          cos = produit/(normu*normv);
          angle = int(acos(cos)*180.0/3.1415);

          max = math::max(max, angle);
        }
      }
      bisector(Y[i]) = max;
    }
    
  }
  
  return bisector;
  
}


template <typename N>
  p_set<mln_point(N)> ultimateSkeleton(p_set<mln_point(N)> X, p_set<mln_point(N)> X_complement, image2d<value::int_u8> dt, p_set<mln_point(N)> Y, const Neighborhood<N>& nbh, bool local)
{
  std::vector< std::pair< double, mln::point2d> > Q;
  // fill Q
  for(uint i = 0; i < X.npoints(); i++)
  {
    if (!Y.has(X[i]))
    {
      std::pair<double, mln_point(N)> p(dt(X[i]),X[i]);
      Q = insertDicho(Q,p);
    }
  }
  
  
  while(!Q.empty())
  {
    mln_point(N) tmp = Q[0].second;
    
    Q = remove(Q, tmp);
    
    if(simple_point(X, nbh, X_complement, tmp, local))
    {
      X.remove(tmp);
      X_complement.insert(tmp);
      mln_niter(N) r(nbh, tmp);
      for_all(r)
      {
        if(!Y.has(r) && X.has(r))
        {
          std::pair<double, mln_point(N)> p(dt(r),r);
          Q = insertDicho(Q, p);
        }
      }
    }
    
  }
  return X;
}

image2d<bool> filteredSkeleton(image2d<bool> pic, const Neighborhood<neighb2d>& nbh, uint r, uint alpha, bool local)
{
  
  typedef mln::image2d<bool> I;
  typedef mln::p_set<point2d> S;
  
  image2d<value::int_u8> pic1,dt;
  
  pic1 = intImage(pic);
  dt = sedt(pic1);
  
  mln::io::pgm::save(dt, "dt.pgm");
  
  int w = geom::ncols(pic);
  int h = geom::nrows(pic);
  int l= math::min(w, h);
  uint rmax = getRMax(dt);
  uint rknown = 0;
  p_set<point2d> X,Y,Z;
  p_set<point2d> X_complement, Z_complement;
  
  image2d<value::int_u8> DTg(l,l,0);
  std::vector< std::vector<int> > Mgl;
  std::vector< std::vector<int> > Lut;
    
  Mgl = CompLutMask (DTg,Mgl,Lut,l,0,rmax);
  
  rknown =rmax;
  
  mln_fwd_piter_(image2d<bool>) p(pic.domain());
  
  for_all(p)
  {
    if(pic(p)==1)
    {
      X.insert(p);
    }
    else
    {
      X_complement.insert(p);
    }
  }
  std::cout << " medial axis " << std::endl;
  pic = MA(pic, Mgl, dt, Lut);
  
  mln::io::pbm::save(pic, "ma.pbm");
  
  for_all(p)
  {
    if(pic(p)==1)
    {
      Y.insert(p);
    }
  }
  
  std::cout << " euclidean skeleton " << std::endl;
  Z = euclideanSkeleton(X, X_complement, dt, Y, nbh, local);
  
  sub_image<I, S> es = pic | Z;
  I es1(pic.domain());
  level::fill(es1, false);

  level::paste(es, es1);
  
  mln::io::pbm::save(es1, "euclidean.pbm");
  
  for_all(p)
  {
    if(!Z.has(p))
    {
      Z_complement.insert(p);
    }
  }
  std::cout << " discrete bisector " << std::endl;
  pic1 = DiscreteBisector(dt, Y, nbh, rmax);
  
  
  mln::io::pgm::save(pic1, "bisector.pgm");
  
  uint cpt=0;
  while(cpt!=Y.npoints())
  {
    if(dt(Y[cpt])>=r && pic1(Y[cpt])>=alpha)
    {
      cpt++;
    }
    else
    {
      Y.remove(Y[cpt]);
    }
  }
  
  
  sub_image<I, S> skel = pic | Y;
  I test(pic.domain());
  level::fill(test, false);

  level::paste(skel, test);
  
  mln::io::pbm::save(test, "Y.pbm");
  
  std::cout << " ultimate skeleton " << std::endl;
  Z = ultimateSkeleton(Z, Z_complement, dt, Y, nbh, local);
  
  
  
  sub_image<I, S> skeleton = pic | Z;
  I output(pic.domain());
  level::fill(output, false);

  level::paste(skeleton, output);
  
  return output;
}

template <typename P>
    p_set< graph_psite<P> > ultimateSkeleton(p_set< graph_psite<P> > X, p_set< graph_psite<P> > X_complement, graph_image< P, int> gi, p_set< graph_psite<P> > Y, bool local)
{
  typedef graph_psite<P> pt_t;
  
  std::vector< std::pair< double, pt_t > > Q;
  graph_elt_neighborhood<P> nbh;
  // fill Q
  for(uint i = 0; i < X.npoints(); i++)
  {
    if (!Y.has(X[i]))
    {
      std::pair<double, pt_t > p(gi(X[i]), X[i]);
      Q = insertDicho(Q,p);
    }
  }
  
  
  while(!Q.empty())
  {
    pt_t tmp = Q[0].second;
    
    Q = remove(Q, tmp);
    
    if(simple_point(X, X_complement, tmp, local))
    {
      X.remove(tmp);
      X_complement.insert(tmp);
//       std::cout <<  Y.npoints() << std::endl;
      mln_niter(graph_elt_neighborhood< P >) r(nbh, tmp);
      for_all(r)
      {
        if(!Y.has(r) && X.has(r))
        {
          std::pair<double, pt_t > p(gi(r),r);
          Q = insertDicho(Q, p);
        }
      }
    }
    
  }
  return X;
}

