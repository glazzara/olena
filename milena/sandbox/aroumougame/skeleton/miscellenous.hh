#include <mln/core/site_set/p_set.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u.hh>
#include <mln/opt/at.hh>
#include <vector>

using namespace mln;

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

template <typename N>
    double distance(N a, N b, N c, N d)
{
  double dist = sqrt((a-c)*(a-c)+(b-d)*(b-d));
  return dist;
}


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

/*

int main()
{
  std::vector< std::pair< double, mln::point2d> > vect;
  std::pair< double, mln::point2d> p(3.25, mln::point2d(2,2));
  std::pair< double, mln::point2d> q(5.25, mln::point2d(1,2));
  std::pair< double, mln::point2d> r(4.55, mln::point2d(2,3));
  std::pair< double, mln::point2d> s(4.55, mln::point2d(5,3));
  std::pair< double, mln::point2d> t(5.11, mln::point2d(2,9));
  std::pair< double, mln::point2d> u(7.11, mln::point2d(6,1));
  vect = insertDicho(vect, p);
  vect = insertDicho(vect, q);
  vect = insertDicho(vect, r);
  vect = insertDicho(vect, s);
  vect = insertDicho(vect, t);
  vect = insertDicho(vect, u);
  
  vect = remove(vect, mln::point2d(2,3));
  
  for(uint i=0; i< vect.size(); i++)
  {
    std::cout << vect[i].first << std::endl;
  }
}*/
