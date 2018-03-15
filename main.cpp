// space inviders

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include<sstream>
#include <vector>

sf::Clock timerstrzal;
sf::Clock timertextura;
sf::Clock ruchwbok;  //global
int bok=1; //global
sf::Clock ruchwdol;  // global
sf::CircleShape mojstatek(25, 3);
bool zmientexture=true;
const int iloscprzeciwnikow=30;
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
double v;
public: Strzal(double f_x =0.0, double f_y=0.0,double f_v=0.0)
:x(f_x)
,y(f_y)
,v(f_v){}
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
typedef std::vector < Strzal > Strzelanie;
    Strzelanie strzal;
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
int generujprzeciwnikow(void)
{
for (int i=0;i<iloscprzeciwnikow;i++)
{
przeciwnik1[i].x=50+(i/3)*60;
przeciwnik1[i].y=20+(i%3)*40;
przeciwnik1[i].life=1;
}

}
//------------------------------------------------------------
int sprawdzstrzaly(void)
{


for (int i=0; i<strzal.size();i++)
{
for (int ii=0; ii<iloscprzeciwnikow; ii++)
 {
 if (przeciwnik1[ii].life!=0)
  {if ( (strzal[i].x>=(przeciwnik1[ii].x-25))&&(strzal[i].x<=(przeciwnik1[ii].x+25))
   &&(strzal[i].y>=(przeciwnik1[ii].y-10))&&(strzal[i].y<=(przeciwnik1[ii].y+10)) )
    {
    przeciwnik1[ii].life=0;
    punkty=punkty+10;
    strzal.erase(strzal.begin()+i);
    i--;
    }
 }

}
}
for (int i=0; i<strzal.size();i++)
 {
strzal[i].y=strzal[i].y-5;
if (strzal[i].y<10) {strzal.erase(strzal.begin()+i); i--;}
 }
 przeciwnicy=0;
 for (int ii=0; ii<iloscprzeciwnikow; ii++)
    if (przeciwnik1[ii].life!=0) przeciwnicy++;

}
//    strzal.push_back( Strzal( mojstatek.x, mojstatek.y-10, 1.0 ) );
//------------------------------------------------------------
void sprawdzzycie(void)
{
for (int i=0;i<iloscprzeciwnikow;i++)
if (przeciwnik1[i].y>550) {zycie--; statek.x=400;statek.y=550;}
}

//------------------------------------------------------------
void rysujsprite(void)
{
/*
mojstatek.setFillColor(sf::Color::Yellow);
mojstatek.setOrigin(mojstatek.getRadius(), mojstatek.getRadius());
mojstatek.setPosition(statek.x, statek.y);
window.draw(mojstatek);*/

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
    if (przeciwnik1[i].life!=0){
         if (timertextura.getElapsedTime().asMilliseconds()>500) { zmientexture=!zmientexture; timertextura.restart();}
         if (zmientexture) sprite.setTexture(texture); else sprite.setTexture(texture2);
            sprite.setOrigin(25,25);
            sprite.setScale(0.5,0.5);
            sprite.setPosition(sf::Vector2f(przeciwnik1[i].x, przeciwnik1[i].y));
            window.draw(sprite);
/*przeciwniks.setFillColor(sf::Color::Magenta);
przeciwniks.setOrigin(przeciwniks.getRadius(), przeciwniks.getRadius());
przeciwniks.setPosition(przeciwnik1[i].x, przeciwnik1[i].y);
window.draw(przeciwniks);*/
if ( statekspr.getGlobalBounds().intersects(sprite.getGlobalBounds()) ) {zycie--; statek.x=400;statek.y=550;}
                                }
}
}
//------------------------------------------------------------
void rysujstrzaly(void)
{
sf::RectangleShape rectangle;

for (int i=0;i < strzal.size(); i++)
 {
 rectangle.setSize(sf::Vector2f(2, 10));
 rectangle.setOrigin(1, 0);
 rectangle.setPosition(strzal[i].x , strzal[i].y );
// rectangle.setPosition(100 , 100 );
 rectangle.setFillColor(sf::Color::Red);
 window.draw(rectangle);
 }
}
//------------------------------------------------------------
void wypiszpunkty(void)
{
text.setString("Punkty: "+to_string(punkty));
text.setCharacterSize(10);
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
{
   window.clear(sf::Color::Black);
  text.setString("Koniec gry");
  text.setCharacterSize(100);
  text.setStyle(sf::Text::Bold);
  text.setColor(sf::Color::White);
  text.setPosition (20,200);
  window.draw(text);
  text.setString("Twoje punkty: "+to_string(punkty));
  text.setPosition (20,300);
  window.draw(text);

               while (window.pollEvent(event))
         {
             // "close requested" event: we close the window
                 if (event.type == sf::Event::Closed)
             window.close();
         }


   window.display();

 }
 void przesun_przeciwnikow(void)
{
if (ruchwbok.getElapsedTime().asMilliseconds()>500)
 {if (bok>0) bok=-1; else bok=1;
 for (int i=0;i<iloscprzeciwnikow;i++)
  przeciwnik1[i].x=przeciwnik1[i].x+bok*10;
 ruchwbok.restart();
 }
if (ruchwdol.getElapsedTime().asMilliseconds()>5000)
 {for (int i=0;i<iloscprzeciwnikow;i++)
  przeciwnik1[i].y=przeciwnik1[i].y+50;
 ruchwdol.restart();
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
statek.y=550;

//sf::Sprite sprite;

//sf::Texture texture;
//texture.loadFromFile("invader.png"));
//sprite.setTexture(texture);
//window.draw(sprite);

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
przeciwnicy=generujprzeciwnikow();
  while (window.isOpen())
    {

if (przeciwnicy&&zycie)
   {
        // check all the window's events that were triggered since the last iteration of the loop

        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

    if (event.type == sf::Event::KeyPressed)

  switch (event.key.code)
 {
 case sf::Keyboard::Right : {statek.x=statek.x+10; if (statek.x>780) statek.x=780; }break;
 case sf::Keyboard::Left : {statek.x=statek.x-10; if (statek.x<20) statek.x=20; } break;
 case sf::Keyboard::Up : {statek.y=statek.y-10; if (statek.y<200) statek.y=200; } break;
 case sf::Keyboard::Down : {statek.y=statek.y+10; if (statek.y>650) statek.y=650; } break;
 //case sf::Keyboard::Space: strzal.push_back( Strzal (statek.x, statek.y-20 ,0)); break;
 case sf::Keyboard::Space: {
                            if (timerstrzal.getElapsedTime().asMilliseconds()>500)
                               {
                                strzal.push_back( Strzal( statek.x, statek.y-10, 1.0 ) );
                            timerstrzal.restart();}
                            break;}
}
   /* if (event.type == sf::Event::MouseButton(Pressed)(
 if (event.mouseButton.button == sf::Mouse::Left)
  {
  if (event.mouseButton.x>400)
   {if (event.mouseButton.y>300) {m=m+5; if (m==60) {m=0;h++;}}
   else {m=m-5; if (m==-5) {m=55;h--;}}}
  else
   {if (event.mouseButton.y>300) {h=h+1; if (h==12) h=0;}
   else  {h=h-1; if (h==-1) h=11;} }

  }*/


        }
 if (zegar.getElapsedTime().asMilliseconds() > 20 )
 {
 window.clear(sf::Color::Black);

 sprawdzstrzaly();
 sprawdzzycie();
 rysujsprite();
 przesun_przeciwnikow();
 rysujprzeciwnikow();
 rysujstrzaly();
 wypiszpunkty();
 zegar.restart();
 window.display();
 }
     }
 else
  koniec();

    }

 return 0;
}
