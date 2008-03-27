
#ifndef JACOBI_HH
# define JACOBI_HH


#include <mln/algebra/mat.hh>

// from num. rec. in C


namespace mln
{


#define rotateJacobi(a,i,j,k,l) g=a(i,j);h=a(k,l);a(i,j)=g-s*(h+g*tau); \
  a(k,l)=h+s*(g-h*tau);

  void jacobi(algebra::mat<4,4,float> a, algebra::quat& q)
  {
    float dd, d[4];
    algebra::mat<4,4,float> v;
    int j,iq,ip,i = 0;
    float tresh,theta,tau,t,sm,s,h,g,c,b[4],z[4];
    for (ip=0;ip<4;ip++) {
      for (iq=0;iq<4;iq++) v(ip,iq)=0.0;
      v(ip,ip)=1.0;
    }
    for (ip=0;ip<4;ip++) {
      b[ip]=d[ip]=a(ip,ip);
      z[ip]=0.0;
    }
    while (1) {
      sm=0.0;
      for (ip=0;ip<3;ip++) {
        for (iq=ip+1;iq<4;iq++)
          sm += fabs(a(ip,iq));
      }
      if (sm < 1e-12) {
        dd = d[0];
        iq = 0;
        for (ip=1;ip<4;ip++)
          if (d[ip]>dd) {
            iq = ip;
            dd = d[ip];
          }
        q = algebra::quat(v(0,iq),
                          v(1,iq),
                          v(2,iq),
                          v(3,iq));
        q.set_unit();
        return;
      }
      if (i < 4) {
        i++;
        tresh=0.0125*sm;
      }
      else
        tresh=0.0;
      for (ip=0;ip<3;ip++) {
        for (iq=ip+1;iq<4;iq++) {

          /* unusefull */
          g=100.0*fabs(a(ip,iq));
          if (i > 4 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip])
              && (float)(fabs(d[iq])+g) == (float)fabs(d[iq]))
            a(ip,iq)=0.0;
          /* unusefull */

          else if (fabs(a(ip,iq)) > tresh) {
            h=d[iq]-d[ip];
            if ((float)(fabs(h)+g) == (float)fabs(h))
              t=(a(ip,iq))/h;
            else {
              theta=0.5*h/(a(ip,iq));
              t=1.0/(fabs(theta)+sqrt(1.0+theta*theta));
              if (theta < 0.0) t = -t;
            }
            c=1.0/sqrt(1+t*t);
            s=t*c;
            tau=s/(1.0+c);
            h=t*a(ip,iq);
            z[ip] -= h;
            z[iq] += h;
            d[ip] -= h;
            d[iq] += h;
            a(ip,iq)=0.0;
            for (j=0;j<=ip-1;j++) {
              rotateJacobi(a,j,ip,j,iq)
                }
            for (j=ip+1;j<=iq-1;j++) {
              rotateJacobi(a,ip,j,j,iq)
                }
            for (j=iq+1;j<4;j++) {
              rotateJacobi(a,ip,j,iq,j)
                }
            for (j=0;j<4;j++) {
              rotateJacobi(v,j,ip,j,iq)
                }
          }
        }
      }
      for (ip=0;ip<4;ip++) {
        b[ip] += z[ip];
        d[ip]=b[ip];
        z[ip]=0.0;
      }
    }
  }

}

#endif // ndef JACOBI_HH
