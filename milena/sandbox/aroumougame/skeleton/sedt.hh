#include <string>
#include <mln/core/image/image2d.hh>
#include <mln/core/clone.hh>
#include <mln/math/max.hh>
#include <mln/math/min.hh>
#include <mln/math/sqr.hh>
# include <mln/value/int_u.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pbm/load.hh>
#include <mln/level/paste.hh>
#include <mln/level/fill.hh>
#include <mln/geom/max_row.hh>
#include <mln/geom/max_col.hh>
#include <mln/geom/max_ind.hh>
#include <mln/debug/println.hh>

 using namespace mln;
 
 
 image2d<value::int_u8> sedt(image2d<value::int_u8> pic)
{
  
  int w = geom::ncols(pic);
  int h = geom::nrows(pic);
  
  image2d<value::int_u16 > g(h,w);
  
  /////////// phase 1 /////////////////
  
  for(int j=0; j<h; j++)
  {
    if(!pic.at(j,0))
    {
      g.at(j,0)=0;
    }
    else
    {
      g.at(j,0)=1023;
    }
    
    for(int i=1; i<w; i++)
    {
      if(!pic.at(j,i))
      {
        g.at(j,i)=0;
      }
      else
      {
        g.at(j,i)=g.at(j,i-1)+1;
      }
    }
    
    for(int i=w-2; i>=0; i--)
    {
      if(g.at(j,i+1)<g.at(j,i))
        g.at(j,i)=1+g.at(j,i+1);
    }
  }
  
  
  /////////// phase 2 /////////////////
  
  int q,v,tmp1,tmp2;
  int length = math::max(h,w);
  image2d<value::int_u8> dt(h,w);
  int t[length];
  int s[length];
  
  for(int i=0; i<w; i++)
  {
    q = 0;
    s[0]=0;
    t[0]=0;
    
    for(int j=1; j<h; j++)
    {
      tmp1 = g.at(s[q],i)*g.at(s[q],i)+(t[q]-s[q])*(t[q]-s[q]);
      tmp2 = g.at(j,i)*g.at(j,i)+(t[q]-j)*(t[q]-j);
      while((q>=0)&&( tmp1>tmp2 ))
      {
        q--;
        tmp1 = g.at(s[q],i)*g.at(s[q],i)+(t[q]-s[q])*(t[q]-s[q]);
        tmp2 = g.at(j,i)*g.at(j,i)+(t[q]-j)*(t[q]-j);
      }
      if(q<0)
      {
        q=0;
        s[0]=j;
      }
      else
      {
        v=1+(j*j-s[q]*s[q]+g.at(j,i)*g.at(j,i)-g.at(s[q],i)*g.at(s[q],i))/(2*(j-s[q]));
        if(v<h)
        {
          q++;
          s[q]=j;
          t[q]=v;
        }
      }
    }
    
    for(int j= h-1; j>=0; j--)
    {
      dt.at(j,i)= g.at(s[q],i)*g.at(s[q],i)+(j-s[q])*(j-s[q]);
      if(j==t[q])
        q--;
    }
  }
  return dt;
}
 
 
 double D_Intersec (int u, int gu, int v, int gv)
{
  return (u+v + ((double)gu-gv)/(u-v)) / 2;
}
 
 image2d<value::int_u8> sedt_octant(image2d<value::int_u<32> >& CTg, int L, uint R)
{
  int x, xM, y, dp, k, propag;
  int Si[L], Sv[L], Sn;  /* index, values, number */
  double Sr[L];          /* intersections */
  image2d<int> DTg(L, L);
 
  /* compute bound xM[ and verify that xM < L */
  for (xM = 0; xM < L; xM++) if (CTg.at(0,xM) > R) break;
  if (xM >= L) printf ("WARNING xM is not < L\n");
    
  /* First scan: x++, y-- */
  for (x = 0; x <= xM; x++)
  {
    k = 0; propag = 0;
    for (y = x; y >= 0; y--)
    {
      if (CTg.at(y,x) > R)   /* outside the ball : background */
        propag = 1;
      else if (propag)  /* inside the ball, mark to dist. k*k from bg */
      { k++; DTg.at(y,x) = k*k; }
      else DTg.at(y,x) = -1; /* inside the ball, no distance propagated */
    }
  }
    
  /* Second and final scan: y++, x++ */
  for (y = 0; y <= xM; y++)
  {
    Sn = 0;  /* empty stacks */
        
    /* Compute stacks indices Si[Sn]=x and values Sv[Sn]=DTg[x,y] */
    for (x = y; x <= xM; x++)
    {
      dp = DTg.at(y,x);
      if (dp < 0) continue;  /* Non propagated value */
            
      /* To speedup algorithm, stop at the second consecutive 0 */
      if (dp == 0 && x > y && DTg.at(y,x-1)==0) break;
            
      while (Sn >= 2 && D_Intersec (Si[Sn-1], Sv[Sn-1], x, dp) < Sr[Sn-1])
        Sn--;  /* pop */
                  
      /* push */    
      Si[Sn] = x; Sv[Sn] = dp; 
      if (Sn >= 1) Sr[Sn] = D_Intersec (Si[Sn-1], Sv[Sn-1], Si[Sn], Sv[Sn]);
      Sn++;
    }
        
    if (Sn == 0) continue;  /* Empty stack */
        
    /* Compute new DTg values using stacks */
    for (x = xM; x >= y; x--)
    {
      if (DTg.at(y,x)==0) continue;
            
      while (Sn >= 2 && x < Sr[Sn-1]) Sn--;  /* pop */
                  
      DTg.at(y,x) = (x-Si[Sn-1])*(x-Si[Sn-1]) + Sv[Sn-1];
    } 
  }
  
  image2d<value::int_u8> dt(L,L);
  for(int i=0; i<L; i++)
  {
    for( int j=0; j<L; j++)
    {
      dt.at(i,j) = DTg.at(i,j);
    }
  }
  return dt;
}
 
 
 image2d<value::int_u<32> > CTg(int L)
{
  image2d<value::int_u<32> > CTg(L,L);
  for(int i=0; i<L; i++)
  {
    for(int j=0; j <= i; j++)
    {
      CTg.at(j,i) = i*i + j*j;
    }
  }
  
  return CTg;
}
 
 
 int getRMax(image2d<value::int_u8> dt)
{
  uint max = 0;
  mln_fwd_piter_(image2d<value::int_u8> ) p(dt.domain());
  for_all(p)
  {
    if(dt(p) > max)
      max = dt(p);
  }
  return max;
}
 
 
std::vector< std::vector<int> > lutCol(image2d<value::int_u<32> >& CTg, int L, std::vector< std::vector<int> >& Lut, std::vector< std::vector<int> >& Mlut, int ind, int Rmax)
{
  int x, y, r1, r2, ra, rb;
  
  std::vector< int > LutCol(Rmax+1,0);
  Lut.push_back(LutCol);
  
  
  for (x = 0; x < L - Mlut[ind][0]; x++)
    for (y = 0; y <= x; y++)
{

    r1 = CTg.at(y,x) +1;

    r2 = CTg.at(y+Mlut[ind][1], x+Mlut[ind][0]) +1;

    if (r1 <= Rmax && r2 > Lut[ind][r1]) Lut[ind][r1] = r2;
}
  
  rb = 0;
  for (ra = 0; ra <= Rmax; ra++)
{
    if (Lut[ind][ra] > rb)
      rb = Lut[ind][ra];
    else Lut[ind][ra] = rb;
}
  return Lut;
}


bool isMAg(int x, int y, std::vector< std::vector<int> >& Mlut, image2d<value::int_u8>& DTg, std::vector< std::vector<int> >& Lut)
{
  int xx, yy, val;

  val = DTg.at(y, x);

  for (uint i = 0; i < Mlut.size(); i++)
  {
    xx = x - Mlut[i][0];
    yy = y - Mlut[i][1];

    if (0 <= yy && yy <= xx)
    {
      if ( DTg.at(yy, xx) >= Lut[i][val] )
        return false;
    }
  }
  return true;
}


std::vector< std::vector<int> > CompLutMask (image2d<value::int_u8>& DTg, std::vector< std::vector<int> >& Mgl, std::vector< std::vector<int> >& Lut, int L,
  int Rknown, uint Rtarget)
{
  
  image2d< value::int_u<32> > CT = CTg(L);
  
  for (uint i = 0; i < Mgl.size(); i++)
  {
    Lut = lutCol(CT, L, Lut, Mgl, i, Rtarget);
  }
  
  for(uint r=Rknown+1; r<=Rtarget; r++)
  {
    DTg=sedt_octant(CT,L,r);
    
    for(int x=1; x<L; x++)
    {
      for(int y=0; y<=x; y++)
      {
        if(DTg.at(y,x) > 0 && isMAg(x, y, Mgl, DTg, Lut))
        {
          std::vector<int> tmp(3);
          tmp[0] = x;
          tmp[1] = y;
          tmp[2] = r;
          
          Mgl.push_back(tmp);

          Lut =  lutCol(CT, L, Lut, Mgl, Mgl.size()-1, Rtarget);
          if(isMAg(x, y, Mgl, DTg, Lut))
          {
            std::cout << "   -----erreur r: " << r << " | " << x << ", " << y << std::endl;
          }
        }
      }
    }
  }
  return Mgl;
  
}

bool isMA(int x, int y, std::vector< std::vector<int> >& Mlut, image2d<value::int_u8>& DTg, std::vector< std::vector<int> >& Lut)
{
  int Vx, Vy, val, h, w;

  val = DTg.at(y, x);
  h = geom::nrows(DTg);
  w = geom::ncols(DTg);

  for (uint i = 0; i < Mlut.size(); i++)
  {
    Vx = Mlut[i][0];
    Vy = Mlut[i][1];

    if((0 <= y-Vy) && (0 <= x-Vx))
    {
      if ( DTg.at(y-Vy, x-Vx) >= Lut[i][val] )
        return false;
    }
    if((y+Vy < h) && (x+Vx < w))
    {
      if ( DTg.at(y+Vy, x+Vx) >= Lut[i][val] )
        return false;
    }
    if((0 <= y-Vy) && (x+Vx < w))
    {
      if ( DTg.at(y-Vy, x+Vx) >= Lut[i][val] )
        return false;
    }
    if((y+Vy < h) && (0 <= x-Vx))
    {
      if ( DTg.at(y+Vy, x-Vx) >= Lut[i][val] )
        return false;
    }
    if((0 <= x-Vy) && (0 <= y-Vx))
    {
      if ( DTg.at(y-Vx, x-Vy) >= Lut[i][val] )
        return false;
    }
    if((x+Vy < w) && (y+Vx < h))
    {
      if ( DTg.at(y+Vx, x+Vy) >= Lut[i][val] )
        return false;
    }
    if((0 <= x-Vy) && (y+Vx < h))
    {
      if ( DTg.at(y+Vx, x-Vy) >= Lut[i][val] )
        return false;
    }
    if((x+Vy < w) && (0 <= y-Vx))
    {
      if ( DTg.at(y-Vx, x+Vy) >= Lut[i][val] )
        return false;
    }

  }
  return true;
}
 
 
 image2d<bool> MA(image2d<bool> pic, std::vector< std::vector<int> > Mgl, image2d<value::int_u8> dt, std::vector< std::vector<int> > Lut)
{
  int w = geom::ncols(pic);
  int h = geom::nrows(pic);
  
  for(int i=0; i<w; i++)
  {
    for(int j=0; j<h; j++)
    {
      if(pic.at(j,i))
      {
        if(!isMA(i, j, Mgl, dt, Lut))
        {
          pic.at(j,i) = false;
        }
      }
    }
  }
  return pic;
}
 
//  int main(int argc, char* argv[])
// {
//   if(argc!=2)
//   {
//     std::cout << "filename missing " << std::endl;
//     exit(1);
//   }
//   
//   image2d<value::int_u8> pic1,dt;
//   
//   std::string filename = argv[1];
//   
//   image2d<bool> pic = io::pbm::load(filename);
//   
//   int w = geom::ncols(pic);
//   int h = geom::nrows(pic);
//   
//   int l= math::min(w, h);
//   
//   pic1 = intImage(pic);
//   dt = sedt(pic1);
//   std::string pgm = filename;
//   pgm[pgm.length()-2]='g';
//   
//   io::pgm::save(dt,"DT-"+pgm);
//   uint rmax = getRMax(dt);
//   
//   image2d<value::int_u8> DTg(l,l,0);
//   std::vector< std::vector<int> > Mgl;
//   std::vector< std::vector<int> > Lut;
//   
//   Mgl = CompLutMask (DTg,Mgl,Lut,l,0,rmax);
//   
//   pic = MA(pic, Mgl, dt, Lut);
//   
//   io::pbm::save(pic,"MA-"+filename);
//   
// }
