#ifndef MESSAGE_H
#define MESSAGE_H
#include <QGraphicsScene>
#include "spaceship.h"
/*专用的消息传递类*/

class GameOnce;class MyGame;class Gamecenter;class SettingScene;class MyRectButton;class ActionHolder;class GameHolder;
class MESSAGE
{
public:
    //static int Alive[11],total;
    //static Ship** allships;
    static int N,nextN,turns,computers;
    static int hScene,wScene,hDisplay,wDisplay;
    static void setScreen(double rate=1);
    static int largescreen;
    static int isonesec;
    static int stupid;
    static int showtrace;
    static int esctosave;
    static int paused;
    static QGraphicsScene *sce;
    static QGraphicsView  *vie;
    static vec npush;
    static MyGame *app;
    static GameOnce *gamenow;
    static Gamecenter *cen;
    static GameHolder *roundnow;
    static int wcom,hcom;
    static int keys[MAXN][6];
    static double ratio,rate;
    static int scores[MAXN];
    static int finished;
    static int loading;
    static int aimode;
};

class Special
{
public:
    static int onsetting;
    static SettingScene *sett;
    static MyRectButton *n_pl;
    static ActionHolder *actor;
    //static QSound *bgm;
};

#endif // MESSAGE

