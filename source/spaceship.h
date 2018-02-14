
#ifndef SPACESHIP
#define SPACESHIP

/*#include <cmath>
#define fg cout<<'\t'
#define hh cout<<endl;
#define eps (1e-9)

const double GM=1,dt=1e-5,PI=3.14159265358979;

class vec
{
    public:
    double x,y;
    vec(double x0=0,double y0=0):x(x0),y(y0){}
    vec(const vec& b){x=b.x;y=b.y;}

};
//double InvSqrt2(double x)
{
    double xhalf = 0.5f*x;
    int i = *(int*)&x; // get bits for doubleing VALUE
    i = 0x5f375a86- (i>>1); // gives initial guess y0
    x = *(double*)&i; // convert bits BACK to double
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracyx = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    return x;
}
double InvSqrt(double x)
{
    double xhalf=0.5*x,y;
    do{y=xhalf*x*x;x=x*(1.5-y);}while((y-0.5>eps)||(y-0.5<-eps));
    return x;
}
vec operator +(vec a,vec b){return vec(a.x+b.x,a.y+b.y);}
double operator ^(vec a,vec b){return a.x*b.x+a.y*b.y;}
double operator &(vec a,vec b){return a.x*b.y-a.y*b.x;}
vec operator &(vec a,double b){return vec(a.y*b,-a.x*b);}
vec operator *(vec a,double b){return vec(a.x*b,a.y*b);}
double Invabs(vec a){return InvSqrt(a.x*a.x+a.y*a.y);}
double R2(vec a){return a.x*a.x+a.y*a.y;}
class Spaceship
{
    public:
    vec r,v,LRL;double t,w,hp,L;int n;
    Spaceship(vec r0=vec(0,0),vec v0=vec(0,0),double t0=0,double w0=0,double hp0=0,int n0=20):r(r0),v(v0),t(t0),w(w0),hp(hp0),n(n0){}
    Spaceship(const Spaceship& b){r=b.r;v=b.v;t=b.t;w=b.w;hp=b.hp;n=b.n;}
    double calcL(){return L=r&v;}
    double calcE(){return (v^v)/2-GM*Invabs(r);}
    vec calcLRL(){return LRL=(v&calcL())+(r*Invabs(r)*(-GM));}
    int nextflip()
    {
        vec a=(r*Invabs(r))*(-GM/R2(r));
        r=r+v*dt;v=v+a*dt;
        t=t+w*dt;if(t>PI)t=t-PI;if(t<-PI)t=t+PI;
        calcLRL();
        return 0;
    }
};*/
#include <cmath>
#include <QGraphicsScene>
#define eps (1e-9)
/*定义了各种常量，以及物理核心的所有计算*/

const double GM=1e7,dt=1e-5,PI=3.14159255358979;
const int dts_interval=1000,interval=10,checkinterval=10,splits=1;
const double nearR=100,startR=400;

const int rBullet=10,wBullet=rBullet,hBullet=rBullet;
const int wShip=120,hShip=100,totalhp=5;
const int wScene0=1200,hScene0=1200;

const double ShipSpeed=120,BulletSpeed=300;
const double mShip=20,mBullet=1,IShip=50000;
const double maxdw=4,deadlyspin=2.5,ROF=100,ROS=100,ROL=1000;
const int MAXN=500,maxBullets=20;

const int rTracepoint=2,wTracepoint=rTracepoint,hTracepoint=rTracepoint;
const int tfadetime=1000,traceinterval=100;

class vec
{
    public:
    double x,y;
    vec(double x0=0,double y0=0);
    vec(const vec& b);

};//向量类，计算的基础
double InvSqrt(double x);
vec operator +(vec a,vec b);
double operator ^(vec a,vec b);//点乘
double operator &(vec a,vec b);//叉乘
vec operator &(vec a,double b);//实数可以被当做是垂直于平面的向量
vec operator &(double b,vec a);
vec operator *(vec a,double b);
vec operator /(vec a,double b);
vec operator -(vec a,vec b);
double Invabs(vec a);
double R2(vec a);
vec Rot(vec a,double t);
class Spaceship
{
    public:
    vec r,v,LRL;double t,w,hp,L;int n;
    Spaceship(vec r0=vec(0,0),vec v0=vec(0,0),double t0=0,double w0=0,double hp0=0,int n0=maxBullets);
    Spaceship(const Spaceship& b);
    double calcL();
    double calcE();
    vec calcLRL();
    int nextflip();
    vec front();
    vec transr(vec r_origin);
    vec transr(double x,double y);
    vec fire(vec v_relative=vec(0,0),vec r_relative=vec(0,0));
    void bulletshot(vec v_relative=vec(0,0),vec r_relative=vec(0,0));
    void bouncewithwall(vec bpoint,vec n);
};//物理内核，模拟运动和碰撞
int bouncecalc(Spaceship &ship1,Spaceship &ship2,vec bpoint,vec n);
//void bouncecalc2(Spaceship &ship,vec bpoint,int (*touch),vec n);

#endif // SPACESHIP


