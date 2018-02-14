#include <spaceship.h>
#include <cmath>
#include <QDebug>
#define eps (1e-9)
vec::vec(double x0,double y0):x(x0),y(y0){}
vec::vec(const vec& b){x=b.x;y=b.y;}
double InvSqrt(double x)
{
    //double xhalf=0.5*x,y;
    //do{y=xhalf*x*x;x=x*(1.5-y);}while((y-0.5>eps)||(y-0.5<-eps));
    //return x;
    return 1.0/sqrt(x);
}
vec operator +(vec a,vec b){return vec(a.x+b.x,a.y+b.y);}
double operator ^(vec a,vec b){return a.x*b.x+a.y*b.y;}
double operator &(vec a,vec b){return a.x*b.y-a.y*b.x;}
vec operator &(vec a,double b){return vec(a.y*b,-a.x*b);}
vec operator &(double b,vec a){return vec(-a.y*b,a.x*b);}
vec operator *(vec a,double b){return vec(a.x*b,a.y*b);}
vec operator /(vec a,double b){return vec(a.x/b,a.y/b);}
vec operator -(vec a,vec b){return vec(a.x-b.x,a.y-b.y);}
double Invabs(vec a){return InvSqrt(a.x*a.x+a.y*a.y);}
double R2(vec a){return a.x*a.x+a.y*a.y;}
vec Rot(vec a, double t){return vec(a.x*cos(t)-a.y*sin(t),a.x*sin(t)+a.y*cos(t));}

Spaceship::Spaceship(vec r0,vec v0,double t0,double w0,double hp0,int n0):r(r0),v(v0),t(t0),w(w0),hp(hp0),n(n0){}
Spaceship::Spaceship(const Spaceship& b){r=b.r;v=b.v;t=b.t;w=b.w;hp=b.hp;n=b.n;}
double Spaceship::calcL(){return L=r&v;}
double Spaceship::calcE(){return IShip*w*w/2+mShip*(R2(v)/2-GM*Invabs(r));}
vec Spaceship::calcLRL(){return LRL=(v&calcL())+(r*Invabs(r)*(-GM));}
int Spaceship::nextflip()
    {
        vec a=(r*Invabs(r))*(-GM/R2(r));
        r=r+v*dt;v=v+a*dt;
        t=t+w*dt;while(t>PI)t=t-2*PI;while(t<-PI)t=t+2*PI;
        calcLRL();
        return 0;
    }
vec Spaceship::front()
{
    return Rot(vec(1,0),t);
}
vec Spaceship::transr(vec r_origin){return Rot(r_origin,t);}
vec Spaceship::transr(double x, double y){return Rot(vec(x,y),t);}
vec Spaceship::fire(vec v_relative,vec r_relative)
{
    //double J=r_relative&v_relative;
    double M=mShip+mBullet*n;
    vec dvS=v_relative*(-mBullet/M),vB=v_relative*(1-mBullet/M)+v+(w&r_relative);
    v=v+dvS;
    //w=w+J*(-mBullet/IShip);
    return vB;
}
void Spaceship::bulletshot(vec v_relative, vec r_relative)
{
    double M=mShip+mBullet*(n+1),J=mBullet*(r_relative&v_relative);
    vec dvS=v_relative*(mBullet/M);
    v=v+dvS;
    w=w+J/IShip;
}
void Spaceship::bouncewithwall(vec bpoint,vec n)
{//qDebug()<<calcE()<<' '<<((mShip*R2(v)+IShip*w*w)/2)<<' '<<v.x<<' '<<v.y<<' '<<w;
    double m1=mShip+mBullet*this->n,I1=IShip;
    vec r1=bpoint-r,vb1=v+(w&r1);//qDebug()<<r1.x<<' '<<r1.y<<' '<<vb1.x<<' '<<vb1.y;
    double C1=r1&n;
    if((vb1^n)<0)return;
    double J=2*(vb1^n)/(1/m1+C1*C1/I1);//qDebug()<<J;
    v=v+n*(-J/m1);
    w=w+C1*(-J/I1);
    //hp-=J/10000;if(hp<0)hp=-0.01;
    //qDebug()<<w<<' '<<C1<<' '<<r1.x<<' '<<r1.y<<' '<<vb1.x<<' '<<vb1.y;
    //qDebug()<<calcE()<<' '<<((mShip*R2(v)+IShip*w*w)/2)<<' '<<v.x<<' '<<v.y<<' '<<w;
}

int  bouncecalc(Spaceship &ship1,Spaceship &ship2,vec bpoint,vec n)
{
    double m1=mShip+mBullet*ship1.n,m2=mShip+mBullet*ship2.n,I1=IShip,I2=IShip,w1=ship1.w,w2=ship2.w;
    vec r1=bpoint-ship1.r,r2=bpoint-ship2.r;
    vec vb1=ship1.v+(w1&r1),vb2=ship2.v+(w2&r2);
    if((vb1^n)<(vb2^n))return 0;
    double C1=r1&n,C2=r2&n;
    double J=2*((vb1-vb2)^n)/(1/m1+1/m2+C1*C1/I1+C2*C2/I2);
    //qDebug()<<J<<' '<<C1<<' '<<C2<<' '<<Invabs(r1)<<' '<<Invabs(r2)<<' '<<n.x<<' '<<n.y<<' '<<r1.x<<' '<<r1.y<<' '<<r2.x<<' '<<r2.y;
    ship1.v=ship1.v+n*(-J/m1);ship2.v=ship2.v+n*(J/m2);
    ship1.w+=C1*(-J/I1);ship2.w+=C2*(J/I2);
    //ship1.hp-=J/10000;ship2.hp-=J/10000;

    //w1=ship1.w;w2=ship2.w;vb1=ship1.v+(w1&r1);vb2=ship2.v+(w2&r2);
    //n=r1+r2*(-1);n=n*Invabs(n);
    //MESSAGE::npush=vb2+vb1*(-1);MESSAGE::npush=MESSAGE::npush*(MESSAGE::npush^n);MESSAGE::npush=MESSAGE::npush*Invabs(MESSAGE::npush);
    //if((vb1^n)-(vb2^n)>0){vec dv=n*((vb1+(vb2*(-1))^n*(2)));ship1.v=ship1.v+dv*(-1);ship2.v=ship2.v+dv;}
    return 1;
}
