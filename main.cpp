// space inviders

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include<sstream>
#include <vector>
#include <cmath>   // abs for double

#define ROWS 3
#define SCREENX 800
#define SCREENY 600

double scale;
const int enemy_x=50;
double max_timer_enemy_move=800;
bool move_right=1;
double delta_x;

sf::Clock timer_shoot;
sf::Clock timer_change_texture;
sf::Clock timer_enemy_move;

bool change_texture=true;
bool move_change;
const int enemy_total=36;
int enemy_amount=enemy_total, life_count=3, score=0;

struct Ship
{
double x;
double y;

};

struct Missle
{
double x;
double y;

sf:: RectangleShape missle;
public: Missle(double f_x =0.0, double f_y=0.0)
:x(f_x)
,y(f_y)
{}
};


struct Enemy
{
    double x;
    double y;
    int life;

};

Enemy enemylist[enemy_total];

std::vector < Missle > shoot;
std::vector < Missle > bomb;
Ship ship;

sf::RenderWindow window;
sf::Event event;
sf::Text text;

sf::Texture texture_enemy1;
sf::Texture texture_enemy2;
sf::Texture texture_ship;
sf::Sprite enemy_sprite;
sf::Sprite ship_sprite;

sf::SoundBuffer buffer_shoot;
sf::Sound shoot_sound;

sf::SoundBuffer buffer_ship_explode;
sf::Sound ship_explode_sound;

sf::SoundBuffer buffer_enemy_kill;
sf::Sound enemy_kill_sound;

sf::SoundBuffer music_list[4];
sf::Sound music_out_list[4];
int music=0;


using namespace std;
string s;
//------------------------------------------------------------
namespace std {
    std::string to_string(size_t n) {
        std::ostringstream s;
        s << n;
        return s.str();
    }
}
//------------------------------------------------------------
void life_lost(void) {

life_count--;
ship.x=400;ship.y=580;
shoot.clear();
ship_explode_sound.play();
timer_shoot.restart();
while (timer_shoot.getElapsedTime().asMilliseconds()<1000)
      {}

}
//------------------------------------------------------------
void create_enemies(void){

scale=(((2.0*SCREENX/3.0)/(enemy_total/ROWS))*2.0/3.0)/enemy_x;
delta_x= enemy_x*scale/2.0;
for (int i=0;i<enemy_total;i++) {
    enemylist[i].x = 2*delta_x+(i/ROWS)*3*delta_x;
    enemylist[i].y = 2*delta_x+(i%ROWS)*3*delta_x;
    enemylist[i].life=1;
}

}
//------------------------------------------------------------
void check_life(void) {

for (int i=0;i<enemy_total;i++)
    if (enemylist[i].y>(600-2*delta_x)) {
       life_count=0;
       life_lost();
    }

for (int i=0;i < bomb.size(); i++)
    if (ship_sprite.getGlobalBounds().intersects(bomb[i].missle.getGlobalBounds())) {
       life_lost();
       bomb.erase(bomb.begin()+i);
       i--;
    }
}

//------------------------------------------------------------
void draw_ship(void)
{
ship_sprite.setTexture(texture_ship);
ship_sprite.setOrigin(110,220);
ship_sprite.setScale(0.20,0.2);
ship_sprite.setPosition(ship.x, ship.y);
window.draw(ship_sprite);
}
//------------------------------------------------------------
void draw_enemies(void){

if (timer_change_texture.getElapsedTime().asMilliseconds()>500) {
   change_texture=!change_texture;
   timer_change_texture.restart();
}

for (int i=0;i<enemy_total;i++) {
    if (enemylist[i].life!=0) {
       if (change_texture)
          enemy_sprite.setTexture(texture_enemy1);
       else
           enemy_sprite.setTexture(texture_enemy2);
       enemy_sprite.setOrigin(25,25);   //++ przesunac do generacji?
       enemy_sprite.setScale(0.5,0.5);  //++
       enemy_sprite.setPosition(sf::Vector2f(enemylist[i].x, enemylist[i].y));
       window.draw(enemy_sprite);
       if ( ship_sprite.getGlobalBounds().intersects(enemy_sprite.getGlobalBounds()) ) {
          life_lost();
       }
       for (int ii=0;ii<shoot.size();ii++) {
           if (enemy_sprite.getGlobalBounds().contains(shoot[ii].x,shoot[ii].y)) {
              enemylist[i].life=0;
              enemy_kill_sound.play();
              score=score+10;
              shoot.erase(shoot.begin()+ii);
              ii--;
           }
       }
    }
}

enemy_amount=0;
for (int ii=0; ii<enemy_total; ii++)
    if (enemylist[ii].life!=0)
       enemy_amount++;

max_timer_enemy_move= 700-(enemy_total-enemy_amount)*20+150;
}
//------------------------------------------------------------
void draw_shots(void) {

for (int i=0;i < shoot.size(); i++)
    for (int ii=0;ii < bomb.size(); ii++) {
        if (shoot[i].missle.getGlobalBounds().intersects(bomb[ii].missle.getGlobalBounds())) {
            shoot.erase(shoot.begin()+i);
            bomb.erase(bomb.begin()+ii);
            i--;
            ii--;
        }
    }
for (int i=0;i < shoot.size(); i++) {
    shoot[i].missle.setSize(sf::Vector2f(2, 10));
    shoot[i].missle.setOrigin(1, 0);
    shoot[i].missle.setPosition(shoot[i].x , shoot[i].y );
    shoot[i].missle.setFillColor(sf::Color::Red);
    window.draw(shoot[i].missle);
    shoot[i].y=shoot[i].y-5;
    if (shoot[i].y<10) {
       shoot.erase(shoot.begin()+i);
       i--;
    }
}
for (int i=0;i < bomb.size(); i++){
    bomb[i].missle.setSize(sf::Vector2f(15,15));
    bomb[i].missle.setOrigin(2.5,2.5);
    bomb[i].missle.setPosition(bomb[i].x,bomb[i].y);
    window.draw(bomb[i].missle);
    bomb[i].y+=5;
}
}
//------------------------------------------------------------
void draw_text(void) {
text.setString("Punkty: "+to_string(score));
text.setCharacterSize(15);
text.setStyle(sf::Text::Bold);
text.setColor(sf::Color::White);
text.setPosition (100,10);
window.draw(text);
text.setString("Zycie: "+to_string(life_count));
text.setPosition (600,10);
window.draw(text);
}
//------------------------------
void game_over(void) {

bool again=0;
while(!again) {
     window.clear(sf::Color::Black);
     text.setString("Koniec gry");
     text.setCharacterSize(80);
     text.setStyle(sf::Text::Bold);
     text.setColor(sf::Color::White);
     text.setPosition (20,200);
     window.draw(text);
     text.setString("Twoje score: "+to_string(score));
     text.setPosition (20,300);
     window.draw(text);
     text.setCharacterSize(30);
     text.setString("Wyjscie: X    Jeszcze raz: 1");
     text.setPosition (20,500);
     window.draw(text);

     while (window.pollEvent(event)) {
           if (event.type == sf::Event::Closed) {
              window.close();
              exit(0);
           }

           if (event.type == sf::Event::KeyPressed) {
              if (event.key.code==sf::Keyboard::X) {
                 window.close();
                 exit(0);
              }
              if (event.key.code==sf::Keyboard::Num1) {
                 again=1;
                 score=0;
                 life_count=3;
                 ship.x=400; ship.y=580;
                 timer_enemy_move.restart();
                 timer_shoot.restart();
                 shoot.clear();
                 bomb.clear();
                 create_enemies();
                 max_timer_enemy_move=800;
                 move_right=1;
              }
           }
     }
     window.display();
}
}

 //----------------------------------------------------------
void move_enemies(void) {

bool flag=false;
int bomb_no;

if (timer_enemy_move.getElapsedTime().asMilliseconds()>max_timer_enemy_move) {
   timer_enemy_move.restart();
   music_out_list[music].play();
   music++;
   if (music>3)
      music=0;
   if (!move_change) {
      flag=0;
      for (int i=0;i<enemy_total;i++) {
          if (move_right) {
             enemylist[i].x=enemylist[i].x+ delta_x;
             if ((enemylist[i].x > (SCREENX-2*delta_x))
                                 &&(enemylist[i].life==1))
                flag=1;
          }
          else {
             enemylist[i].x=enemylist[i].x - delta_x;
             if ((enemylist[i].x < 2*delta_x)
                                 &&(enemylist[i].life==1))
                flag=1;
          }

      }
   if (flag)
      move_change=1;
   }
   else {
        for (int i=0;i<enemy_total;i++)
            enemylist[i].y=enemylist[i].y+abs(delta_x)*3;
        if (move_right)
           move_right=0;
        else
            move_right=1;

        move_change=0;
   }
   bomb_no=rand() % enemy_total;
   if (enemylist[bomb_no].life!=0)
      bomb.push_back(Missle(enemylist[bomb_no].x,enemylist[bomb_no].y));
}

}
//-----------------------------------------------------------
int load_sound_textures(void)
{
if (!buffer_shoot.loadFromFile("shoot.wav"))
        return -1;
shoot_sound.setBuffer(buffer_shoot);

if (!buffer_enemy_kill.loadFromFile("invaderkilled.wav"))
        return -1;
enemy_kill_sound.setBuffer(buffer_enemy_kill);

if (!buffer_ship_explode.loadFromFile("explosion.wav"))
        return -1;
ship_explode_sound.setBuffer(buffer_ship_explode);

if (!music_list[0].loadFromFile("fastinvader1.wav"))
        return -1;
music_out_list[0].setBuffer(music_list[0]);

if (!music_list[1].loadFromFile("fastinvader2.wav"))
        return -1;
music_out_list[1].setBuffer(music_list[1]);

if (!music_list[2].loadFromFile("fastinvader3.wav"))
        return -1;
music_out_list[2].setBuffer(music_list[2]);

if (!music_list[3].loadFromFile("fastinvader4.wav"))
        return -1;
music_out_list[3].setBuffer(music_list[3]);

if (!texture_enemy1.loadFromFile("invader.png"))
       return -1;
if (!texture_enemy2.loadFromFile("invader2.png"))
       return -1;
if (!texture_ship.loadFromFile("ship.png"))
       return -1;


       return 1;
}


//--------------------------------------------------------------
int main()
{

    srand (time(NULL));        /* initialize random seed: */
    window.create(sf::VideoMode(800, 600), "Space Invaders"); // initiate window
    window.setVerticalSyncEnabled(true);                      //

ship.x=400;
ship.y=580;

sf::Font font;
if (!font.loadFromFile("Arial.ttf"))
   return -1;
text.setFont(font);

if (load_sound_textures()==-1)
         return -1;

sf::Clock clock;
timer_enemy_move.restart();
timer_shoot.restart();
create_enemies();
game_over();

  while (window.isOpen()) {



        // check all the window's events that were triggered since the last iteration of the loop
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
     ship.x=ship.x-0.02;
     if (ship.x<20)
        ship.x=20;
     }
  else
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
         ship.x=ship.x+0.02;
         if (ship.x>780)
            ship.x=780;
      }
      else
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
             {
             if (shoot.size()>=1)
                 {if (timer_shoot.getElapsedTime().asMilliseconds()>500) {
                      shoot.push_back( Missle( ship.x, ship.y-50 ) );
                      shoot_sound.play();
                      timer_shoot.restart();
                      }
                 }
             else {
                  shoot.push_back( Missle( ship.x, ship.y-50 ) );
                  shoot_sound.play();
                  timer_shoot.restart();
                  }
             }

   if (clock.getElapsedTime().asMilliseconds() > 20 ) {
      window.clear(sf::Color::Black);
      if (enemy_amount==0) {
         create_enemies();
         max_timer_enemy_move-=100;
         ship.x=400; ship.y=580;
         shoot.clear();
         bomb.clear();
         move_right=1;
      }

      check_life();
      draw_ship();
      move_enemies();
      draw_enemies();
      draw_shots();
      draw_text();
      clock.restart();
      window.display();
      if (score==1000) life_count++;
      if (score==5000)life_count++;
      if (!life_count) game_over();
    }


  }

 return 0;
}
