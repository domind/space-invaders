// space inviders

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include<sstream>
#include <vector>
#include <cmath>
#define rows 3
#define screenx 800
#define screeny 600;
double scale=0.5;
const int przeciwnikx=50;
//------------------------
double timermaxprzeciwnicy=800;
bool prawo=1,wyjdz=0;
double deltax;
sf::Clock timerstrzal;
sf::Clock timertextura;
sf::Clock ruchwbok;  //global
int bok=1; //global
sf::Clock ruchwdol;  // global
sf::CircleShape mojstatek(25, 3);
bool zmientexture=true;
bool zmiana;
const int iloscprzeciwnikow=36;

sf::SoundBuffer buffers;
sf::Sound strzaldzwiek;
sf::SoundBuffer buffwybuch;
sf::Sound statekwybuch;
sf::SoundBuffer buffkill;
sf::Sound killsound;
sf::SoundBuffer buffer[4];
sf::Sound invm[4];
/*sf::SoundBuffer buffer2;
sf::Sound invm2;
sf::SoundBuffer buffer3;
sf::Sound invm3;
sf::SoundBuffer buffer4;
sf::Sound invm4;*/
int music=0;
struct Statek
{
double x;
double y;
double life;
};
struct Strzal
{
double x;
double y;

sf:: RectangleShape pocisk;
public: Strzal(double f_x =0.0, double f_y=0.0)
:x(f_x)
,y(f_y)
{}
};

struct Bomba
{
double x;
double y;

sf:: RectangleShape bombshape;
public: Bomba(double f_x , double f_y)
:x(f_x)
,y(f_y)
{}
};

struct przeciwnik
{
    double x;
    double y;
  //  double time;
    int life;
  //  double ruchX;
  //  double ruchY;
};





przeciwnik przeciwnik1[iloscprzeciwnikow];
//typedef std::vector < Strzal > Strzelanie;
 //   Strzelanie strzal;
    std::vector < Strzal > strzal;
//typedef std::vector < Bomba > Bomby;
   // Bomby bomby;
    std::vector < Bomba > bomby;
Statek statek;
using namespace std;
string s;
sf::Event event;
sf::Text text;
int przeciwnicy=iloscprzeciwnikow, zycie=3, punkty=0;
sf::RenderWindow window;
sf::Texture texture;
sf::Texture texture2;
sf::Texture statektexture;
sf::Sprite sprite;
sf::Sprite statekspr;
//------------------------------------------------------------
 namespace std {
    std::string to_string(size_t n) {
        std::ostringstream s;
        s << n;
        return s.str();
    }
}
//------------------------------------------------------------
void stratazycia(void)
{
    zycie--; statek.x=400;statek.y=580;  strzal.clear(); statekwybuch.play();
timerstrzal.restart(); while (timerstrzal.getElapsedTime().asMilliseconds()<1000){}

}
void generujprzeciwnikow(void)
{
    scale=(((2.0*screenx/3.0)/(iloscprzeciwnikow/rows))*2.0/3.0)/przeciwnikx;
deltax= przeciwnikx*scale/2.0;
for (int i=0;i<iloscprzeciwnikow;i++)
{

przeciwnik1[i].x = 2*deltax+(i/rows)*3*deltax;

przeciwnik1[i].y = 2*deltax+(i%rows)*3*deltax;
przeciwnik1[i].life=1;
}

}
//------------------------------------------------------------

void sprawdzzycie(void)
{
for (int i=0;i<iloscprzeciwnikow;i++)
if (przeciwnik1[i].y>(600-2*deltax)) {zycie=0; stratazycia();}

for (int i=0;i < bomby.size(); i++)
    if (statekspr.getGlobalBounds().intersects(bomby[i].bombshape.getGlobalBounds()))
    {
        stratazycia(); bomby.erase(bomby.begin()+i); i--;
    }
}

//------------------------------------------------------------
void rysujsprite(void)
{
statekspr.setTexture(statektexture);
statekspr.setOrigin(110,220);//statekspr.getGlobalBounds(),statekspr.getGlobalBounds());
statekspr.setScale(0.20,0.2);
statekspr.setPosition(statek.x, statek.y);
window.draw(statekspr);
}
//------------------------------------------------------------
void rysujprzeciwnikow(void)
{
//sf::CircleShape przeciwniks(25, 8);
for (int i=0;i<iloscprzeciwnikow;i++)
{
    if (przeciwnik1[i].life!=0)
        {
        if (timertextura.getElapsedTime().asMilliseconds()>500) { zmientexture=!zmientexture; timertextura.restart();}
        if (zmientexture) sprite.setTexture(texture); else sprite.setTexture(texture2);
        sprite.setOrigin(25,25);
        sprite.setScale(0.5,0.5);
        sprite.setPosition(sf::Vector2f(przeciwnik1[i].x, przeciwnik1[i].y));
        window.draw(sprite);
        if ( statekspr.getGlobalBounds().intersects(sprite.getGlobalBounds()) )
                {stratazycia();}
        for (int ii=0;ii<strzal.size();ii++)
            {
            if (sprite.getGlobalBounds().contains(strzal[ii].x,strzal[ii].y))
                    {przeciwnik1[i].life=0; killsound.play(); punkty=punkty+10; strzal.erase(strzal.begin()+ii); ii--;}
            }
        }
}
            przeciwnicy=0;
 for (int ii=0; ii<iloscprzeciwnikow; ii++)
    if (przeciwnik1[ii].life!=0) przeciwnicy++;

timermaxprzeciwnicy= 700-(iloscprzeciwnikow-przeciwnicy)*20+150;
}
//------------------------------------------------------------
void rysujstrzaly(void)
{
for (int i=0;i < strzal.size(); i++)
    for (int ii=0;ii < bomby.size(); ii++)
    {
        //if ((strzal[i].x==bomby[i].x)&&(strzal[i].y<=bomby[i].y))
        if (strzal[i].pocisk.getGlobalBounds().intersects(bomby[ii].bombshape.getGlobalBounds()))
        {
            strzal.erase(strzal.begin()+i);
            bomby.erase(bomby.begin()+ii);
            i--;
            ii--;
        }
    }
for (int i=0;i < strzal.size(); i++)
 {
 strzal[i].pocisk.setSize(sf::Vector2f(2, 10));
 strzal[i].pocisk.setOrigin(1, 0);
 strzal[i].pocisk.setPosition(strzal[i].x , strzal[i].y );
 strzal[i].pocisk.setFillColor(sf::Color::Red);
 window.draw(strzal[i].pocisk);
 strzal[i].y=strzal[i].y-5;
 if (strzal[i].y<10) {strzal.erase(strzal.begin()+i); i--;}
 }
 for (int i=0;i < bomby.size(); i++)
    {
    bomby[i].bombshape.setSize(sf::Vector2f(15,15));
    bomby[i].bombshape.setOrigin(2.5,2.5);
    bomby[i].bombshape.setPosition(bomby[i].x,bomby[i].y);
    window.draw(bomby[i].bombshape);
    bomby[i].y+=5;
    }
}
//------------------------------------------------------------
void wypiszpunkty(void)
{
//    text.setString("Punkty: "+to_string(scale*100)+" t  "+to_string(deltax));
text.setString("Punkty: "+to_string(punkty));
text.setCharacterSize(15);
text.setStyle(sf::Text::Bold);
text.setColor(sf::Color::White);
text.setPosition (100,10);
window.draw(text);
text.setString("Zycie: "+to_string(zycie));
text.setPosition (600,10);
window.draw(text);
}
//------------------------------
void koniec(void)
{bool again=0;
    while(!again)
    {
   window.clear(sf::Color::Black);
  text.setString("Koniec gry");
  text.setCharacterSize(80);
  text.setStyle(sf::Text::Bold);
  text.setColor(sf::Color::White);
  text.setPosition (20,200);
  window.draw(text);
  text.setString("Twoje punkty: "+to_string(punkty));
  text.setPosition (20,300);
   window.draw(text);
  text.setCharacterSize(30);
    text.setString("Wyjscie: X    Jeszcze raz: 1");
  text.setPosition (20,500);
  window.draw(text);

               while (window.pollEvent(event))
         {
             // "close requested" event: we close the window
                 if (event.type == sf::Event::Closed)
             {window.close(); exit(0);}

              if (event.type == sf::Event::KeyPressed)
              {
                  if (event.key.code==sf::Keyboard::X)
                  {
                      window.close(); exit(0);
                  }
                  if (event.key.code==sf::Keyboard::Num1)
                  {
                      again=1; punkty=0; zycie=3;
                      statek.x=400;
                        statek.y=580;
                        ruchwbok.restart();
                        timerstrzal.restart();
                        strzal.clear();
                        bomby.clear();
                        generujprzeciwnikow();
                        timermaxprzeciwnicy=800;
                        prawo=1;
                    }
              }
         }


   window.display();
    }
 }
 void przesun_przeciwnikow(void)
{bool flag=false;
int bomb;
 if (ruchwbok.getElapsedTime().asMilliseconds()>timermaxprzeciwnicy)
 {
     ruchwbok.restart();

invm[music].play();
music++; if (music>3) music=0;
if (!zmiana)
 { flag=0;
for (int i=0;i<iloscprzeciwnikow;i++)
 {
 if (prawo) {
            przeciwnik1[i].x=przeciwnik1[i].x+  deltax;
            if ((przeciwnik1[i].x > (screenx-2*deltax))&&(przeciwnik1[i].life==1))flag=1;
            }
    else
    {
    przeciwnik1[i].x=przeciwnik1[i].x-  deltax;
            if ((przeciwnik1[i].x<2*deltax)&&(przeciwnik1[i].life==1))flag=1;

    }

 }
 if (flag) zmiana=1;
 }
else
 {
 for (int i=0;i<iloscprzeciwnikow;i++)
 przeciwnik1[i].y=przeciwnik1[i].y+abs(deltax)*3;
 if (prawo) prawo=0; else prawo=1; //deltax=0-deltax;

 zmiana=0;
 }
bomb=rand() % iloscprzeciwnikow;

    if (przeciwnik1[bomb].life!=0) bomby.push_back(Bomba(przeciwnik1[bomb].x,przeciwnik1[bomb].y));
 }

}
//-----------------------------------------------------------
int main()
{
    int punkty=0;
    srand (time(NULL));
    window.create(sf::VideoMode(800, 600), "My window");
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

statek.x=400;
statek.y=580;
//sf::SoundBuffer buffer;
   if (!buffers.loadFromFile("shoot.wav"))
        return -1;
strzaldzwiek.setBuffer(buffers);
   if (!buffkill.loadFromFile("invaderkilled.wav"))
        return -1;
killsound.setBuffer(buffkill);
   if (!buffwybuch.loadFromFile("explosion.wav"))
        return -1;
statekwybuch.setBuffer(buffwybuch);
   if (!buffer[0].loadFromFile("fastinvader1.wav"))
        return -1;
invm[0].setBuffer(buffer[0]);
   if (!buffer[1].loadFromFile("fastinvader2.wav"))
        return -1;
invm[1].setBuffer(buffer[1]);
   if (!buffer[2].loadFromFile("fastinvader3.wav"))
        return -1;
invm[2].setBuffer(buffer[2]);
   if (!buffer[3].loadFromFile("fastinvader4.wav"))
        return -1;
invm[3].setBuffer(buffer[3]);
  /* initialize random seed: */
  srand (time(NULL));
texture.loadFromFile("invader.png");
texture2.loadFromFile("invader2.png");
statektexture.loadFromFile("ship.png");
sf::Font font;
font.loadFromFile("Arial.ttf");




text.setFont(font);
text.setString("Hello");
sf::Clock zegar;
ruchwbok.restart();
ruchwdol.restart();
timerstrzal.restart();
generujprzeciwnikow();
koniec();
  while (window.isOpen())
    {



        // check all the window's events that were triggered since the last iteration of the loop

        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

        }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  {statek.x=statek.x-0.02; if (statek.x<20) statek.x=20; }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {statek.x=statek.x+0.02; if (statek.x>780) statek.x=780; }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    if (strzal.size()>=1)
                            {if (timerstrzal.getElapsedTime().asMilliseconds()>500)
                               {
                                strzal.push_back( Strzal( statek.x, statek.y-50 ) );
                                strzaldzwiek.play();
                                timerstrzal.restart();
                                }
                            }
                        else
                        {
                            strzal.push_back( Strzal( statek.x, statek.y-50 ) );
                                strzaldzwiek.play();
                                timerstrzal.restart();
                        }
                            }

 if (zegar.getElapsedTime().asMilliseconds() > 20 )
 {
 window.clear(sf::Color::Black);

// sprawdzstrzaly();
if (przeciwnicy==0)
        {generujprzeciwnikow();
        timermaxprzeciwnicy-=100;
        statek.x=400; statek.y=580;
        strzal.clear();
        bomby.clear();
        prawo=1;}

 sprawdzzycie();
 rysujsprite();
 przesun_przeciwnikow();
 rysujprzeciwnikow();
 rysujstrzaly();
 wypiszpunkty();
 zegar.restart();
 window.display();
 if (punkty==1000) zycie++;
 if (punkty==5000)zycie++;
 if (!zycie) koniec();
 }


    }

 return 0;
}
