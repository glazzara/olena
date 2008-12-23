// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.
#ifndef SKELETON_HH
# define SKELETON_HH

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <sandbox/aroumougame/skeleton/sedt.hh>

#include <mln/core/site_set/p_set.hh>
#include <mln/math/sqrt.hh>
#include <mln/opt/at.hh>

namespace mln
{


template <typename P>
    std::vector< std::pair< double, P > > remove(std::vector< std::pair< double, P > > Q, P p)
{
  typename std::vector<std::pair< double, P > >::iterator it;
  
  for(it = Q.begin(); it!=Q.end(); it++)
  {
    if((*it).second==p)
    {
      it = Q.erase(it);
      break;
    }
  }
  return Q;
}
template <typename N>
    double distance(N a, N b, N c, N d)
{
  double dist = sqrt((a-c)*(a-c)+(b-d)*(b-d));
  return dist;
}
template <typename P>
 std::vector< std::pair< double, P > > insertDicho(std::vector< std::pair< double, P > > Q, std::pair< double, P> p)
{
  int indMin, indMax, indMid;
  
  indMin = 0;
  indMax = Q.size();
  
  if(indMax==0 || Q[indMax-1].first <= p.first)
    Q.push_back(p);
  else
  {
    while(indMax > indMin)
    {
      indMid = int(indMin + (indMax-indMin)/2);
      if(Q[indMid].first < p.first)
      {
        indMin = indMid+1;
        if(Q[indMin].first > p.first)
        {
          indMax = indMid;
        }
      }
      else
      {
        indMax = indMid-1;
        if(Q[indMax].first < p.first)
        {
          indMin = indMid;
        }
      }
    }

    typename std::vector< std::pair< double, P > >::iterator it=Q.begin();
    it = Q.insert ( it+indMin, p);
  }

  return Q;
}


const neighb2d& complement_neighborhood(const Neighborhood<neighb2d>& nbh)
{
  if(&nbh == &c4())
    return c8();
  return c4();
}

template <typename N>
    int nb_composant_connexe(p_set< mln_psite(N) > X_full,const Neighborhood<N>& nbh_,const mln_psite(N)& p_ref, bool local)
{
  N nbh = exact(nbh_);
  p_set< mln_psite(N) > X;
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
  p_set< mln_psite(N) > done;
  p_set< mln_psite(N) > neighbors;
  p_set< mln_psite(N) > composant;
  
  done.insert(p_ref);
  mln_niter(N) q(nbh, p_ref);
  for_all(q)
  {
    if (X.has(q)&&!done.has(q))
    {
      neighbors.insert(q);
    }
  }
//   std::cout << "nb_composant_connexe: neighbors " << neighbors.nsites() <<std::endl; 
  if(neighbors.nsites()<=1)
  {
    return neighbors.nsites();
  }
  else
    T=0;
    
  while(neighbors.nsites()!=0)
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
     
    while(composant.nsites()!=0)
    {
      done.insert(composant[0]);
      if(neighbors.has(composant[0]))
      {
        neighbors.remove(composant[0]);
        if(neighbors.nsites()==0)
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
    bool simple_point(p_set< mln_psite(N) > X,const Neighborhood<N>& nbh, p_set< mln_psite(N) > X_complement, const mln_psite(N)& p_ref, bool local)
{
  int nX = nb_composant_connexe(X,exact(nbh),p_ref,local);
  int nX_complement = nb_composant_connexe(X_complement,complement_neighborhood(exact(nbh)),p_ref,local);
  
  if((nX_complement == 1)&&(nX == 1))
    return true;
  return false;
}


  template <typename N>
  p_set<mln_psite(N)> euclideanSkeleton(p_set<mln_psite(N)> X, p_set<mln_psite(N)> X_complement, image2d<value::int_u8> dt, p_set<mln_psite(N)>& Y, const Neighborhood<N>& nbh_, bool local)
  {
    std::vector< std::pair< double, mln::point2d> > Q;
    std::vector< std::pair< double, mln::point2d> > R;
    N nbh = exact(nbh_);

    // fill Q
    for (uint i = 0; i < X.nsites(); i++)
    {
      if (!Y.has(X[i]))
      {
	std::pair<double, mln_psite(N)> p(math::sqrt(double(dt(X[i]))),X[i]);
	Q = insertDicho(Q,p);
      }
    }

    // fill R
    for (uint i = 0; i < X.nsites(); i++)
    {
      if (!Y.has(X[i]))
      {
	double min = 1023.99;
	mln_niter(N) r(nbh, X[i]);
	for_all(r)
	{
	  if (Y.has(r))
	  {
	    double tmp = distance(r[0], r[1], X[i][0], X[i][1]);
	    double d = math::sqrt(double(dt(r)))+(math::sqrt(double(dt(X[i])))-math::sqrt(double(dt(r))))/tmp;
	    min = math::min(min,d);
	  }
	}
	if (min!=1023.99)
	{
	  std::pair<double, mln_psite(N)> p(min,X[i]);
	  R = insertDicho(R, p);
	}
      }
    }

    while (!Q.empty() || !R.empty())
    {
      mln_psite(N) tmp;
      if (Q[0].first < R[0].first)
      {
	tmp = Q[0].second;
      }
      else
      {
	tmp = R[0].second;
      }

      Q = remove(Q, tmp);
      R = remove(R, tmp);

      if (!Y.has(tmp) && X.has(tmp))
      {
	if (simple_point(X, nbh, X_complement, tmp, local))
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
	    if (!Y.has(r) && X.has(r))
	    {
	      double dist = distance(r[0], r[1], tmp[0], tmp[1]);
	      double d = math::sqrt(double(dt(tmp)))+(math::sqrt(double(dt(r)))-math::sqrt(double(dt(tmp))))/dist;
	      std::pair<double, mln_psite(N)> p(d,r);
	      R = insertDicho(R, p);
	    }
	  }

	}
      }

    }
    return X;
  }


  p_set<point2d> EP(image2d<value::int_u8> dt, point2d x, std::vector< std::vector<std::pair< int, int> > > lut, const neighb2d& nbh)
  {
    p_set<point2d> EP;
    p_set<point2d> tmp;
    int w = geom::ncols(dt);
    int h = geom::nrows(dt);


    mln_niter_(neighb2d) r(nbh, x);
    for_all(r)
    {
      if (dt(r) <= dt(x))
      {
	for (uint i=0; i<lut[dt(r)].size(); i++)
	{
	  if ((r[0]+lut[dt(r)][i].first < h) && (r[1]+lut[dt(r)][i].second < w))
	  {
	    if (!dt(r+dpoint2d(lut[dt(r)][i].first, lut[dt(r)][i].second)))
	      EP.insert(r+dpoint2d(lut[dt(r)][i].first, lut[dt(r)][i].second));
	  }
	  if ((r[0]-lut[dt(r)][i].first >= 0) && (r[1]-lut[dt(r)][i].second >= 0))
	  {
	    if (!dt(r+dpoint2d(-lut[dt(r)][i].first, -lut[dt(r)][i].second)))
	      EP.insert(r+dpoint2d(-lut[dt(r)][i].first, -lut[dt(r)][i].second));
	  }
	  if ((r[0]+lut[dt(r)][i].first < h) && (r[1]-lut[dt(r)][i].second >= 0))
	  {
	    if (!dt(r+dpoint2d(lut[dt(r)][i].first, -lut[dt(r)][i].second)))
	      EP.insert(r+dpoint2d(lut[dt(r)][i].first, -lut[dt(r)][i].second));
	  }
	  if ((r[0]-lut[dt(r)][i].first >= 0) && (r[1]+lut[dt(r)][i].second < w))
	  {
	    if (!dt(r+dpoint2d(-lut[dt(r)][i].first, lut[dt(r)][i].second)))
	      EP.insert(r+dpoint2d(-lut[dt(r)][i].first, lut[dt(r)][i].second));
	  }
	  if ((r[0]+lut[dt(r)][i].second < h) && (r[1]+lut[dt(r)][i].first < w))
	  {
	    if (!dt(r+dpoint2d(lut[dt(r)][i].second, lut[dt(r)][i].first)))
	      EP.insert(r+dpoint2d(lut[dt(r)][i].second, lut[dt(r)][i].first));
	  }
	  if ((r[0]-lut[dt(r)][i].second >= 0) && (r[1]-lut[dt(r)][i].first >= 0))
	  {
	    if (!dt(r+dpoint2d(-lut[dt(r)][i].second, -lut[dt(r)][i].first)))
	      EP.insert(r+dpoint2d(-lut[dt(r)][i].second, -lut[dt(r)][i].first));
	  }
	  if ((r[0]+lut[dt(r)][i].second < h) && (r[1]-lut[dt(r)][i].first >= 0))
	  {
	    if (!dt(r+dpoint2d(lut[dt(r)][i].second, -lut[dt(r)][i].first)))
	      EP.insert(r+dpoint2d(lut[dt(r)][i].second, -lut[dt(r)][i].first));
	  }
	  if ((r[0]-lut[dt(r)][i].second >= 0) && (r[1]+lut[dt(r)][i].first < w))
	  {
	    if (!dt(r+dpoint2d(-lut[dt(r)][i].second, lut[dt(r)][i].first)))
	      EP.insert(r+dpoint2d(-lut[dt(r)][i].second, lut[dt(r)][i].first));
	  }
	}
      }
    }

    return EP;
  }

  std::vector< std::vector<std::pair< int, int> > > Lut2d(int N)
  {
    int n = int(sqrt(N))+1;
    int i=0;
    std::vector< std::vector<std::pair< int, int> > > lut;

    for(i = 0; i <= N; i++)
    {
      std::vector<std::pair< int, int> > vect;
      lut.push_back(vect);
    }

    for(int x = 0; x <= n; x++)
    {
      for(int y = 0; y <= x; y++)
      {
	i=x*x+y*y;
	if(i<=N)
	{
	  std::pair<int,int> p(x,y);
	  lut[i].push_back(p);
	}
      }
    }

  return lut;
}

  image2d<value::int_u8> DiscreteBisector(image2d<value::int_u8> dt, p_set<point2d> Y, const neighb2d& nbh, int N)
  {
    int w = geom::ncols(dt);
    int h = geom::nrows(dt);

    int ux,uy,vx,vy, produit, angle, max;
    double cos, normu, normv;

    std::vector< std::vector<std::pair< int, int> > > lut;
    lut = Lut2d(N);

    p_set<point2d> proj;

    image2d<value::int_u8> bisector(h, w);
    data::fill(bisector, 0);

    for (uint i=0; i<Y.nsites(); i++)
    {
      proj = EP(dt, Y[i], lut, nbh);

      int n=proj.nsites();

      if (n>1)
      {
	max = 0;
	for (int y=0; y<n; y++)
	{
	  for (int z=0; z<y; z++)
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



const neighb2d& max_neighborhood(const Neighborhood<neighb2d>& nbh)
{
  return c8();
}
template <typename N>
  p_set<mln_psite(N)> ultimateSkeleton(p_set<mln_psite(N)> X, p_set<mln_psite(N)> X_complement, image2d<value::int_u8> dt, p_set<mln_psite(N)> Y, const Neighborhood<N>& nbh_, bool local)
{
  std::vector< std::pair< double, mln::point2d> > Q;

  N nbh = exact(nbh_);
  // fill Q
  for(uint i = 0; i < X.nsites(); i++)
  {
    if (!Y.has(X[i]))
    {
      std::pair<double, mln_psite(N)> p(dt(X[i]),X[i]);
      Q = insertDicho(Q,p);
    }
  }
  
  
  while(!Q.empty())
  {
    mln_psite(N) tmp = Q[0].second;
    
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
          std::pair<double, mln_psite(N)> p(dt(r),r);
          Q = insertDicho(Q, p);
        }
      }
    }
    
  }
  return X;
}

 image2d<value::int_u8> intImage(image2d<bool> pic)
{
  int w = geom::ncols(pic);
  int h = geom::nrows(pic);
  
  image2d<value::int_u8> out(h,w);
  for(int i=0; i<w; i++)
    for(int j=0; j<h; j++)
  {
    if(opt::at(pic, j,i))
      opt::at(out, j,i) = 1;
    else
      opt::at(out, j,i) = 0;
  }
  return out;
}
  image2d<bool> filteredSkeleton(image2d<bool> pic, const neighb2d& nbh, uint r, uint alpha, bool local)
  {
    using value::int_u8;

    typedef image2d<bool> I;
    typedef p_set<point2d> S;

    image2d<value::int_u8> pic1 = intImage(pic);
    image2d<value::int_u8> dt = sedt(pic1);

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
      if (pic(p)==1)
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
      if (pic(p)==1)
      {
	Y.insert(p);
      }
    }

    std::cout << " euclidean skeleton " << std::endl;
    Z = euclideanSkeleton(X, X_complement, dt, Y, nbh, local);

    sub_image<I, S> es = pic | Z;
    I es1(pic.domain());
    data::fill(es1, false);

    data::paste(es, es1);

    mln::io::pbm::save(es1, "euclidean.pbm");

    for_all(p)
    {
      if (!Z.has(p))
      {
	Z_complement.insert(p);
      }
    }
    std::cout << " discrete bisector " << std::endl;
    pic1 = DiscreteBisector(dt, Y, nbh, rmax);


    mln::io::pgm::save(pic1, "bisector.pgm");

    uint cpt=0;
    while (cpt!=Y.nsites())
    {
      if (dt(Y[cpt])>=r && pic1(Y[cpt])>=alpha)
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
    data::fill(test, false);

    data::paste(skel, test);

    mln::io::pbm::save(test, "Y.pbm");

    std::cout << " ultimate skeleton " << std::endl;
    Z = ultimateSkeleton(Z, Z_complement, dt, Y, nbh, local);



    sub_image<I, S> skeleton = pic | Z;
    I output(pic.domain());
    data::fill(output, false);

    data::paste(skeleton, output);

    return output;
  }

} // End of namespace mln
#endif // ! SKELETON_HH
