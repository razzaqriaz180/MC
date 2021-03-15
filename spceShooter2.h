 
//
// An SFML-based Space Shooter
//
 
 
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <iostream> // Included in case I wanted to print data into the console (for debugging purposes)
 
// Avoiding magic numbers
 
#define RIGHT_BORDER 725
#define LEFT_BORDER 25
#define TOP_BORDER 0
#define BOTTOM_BORDER 600
#define SPAWN_POS1 350
 
using namespace std;    // For ease of use of iostream
 
//
// Global Variables
//
 
sf::RenderWindow window(sf::VideoMode(800,600,32), "Space Shooter");
sf::Event event;
sf::Clock Clock;
sf::Clock bulletClock[3];
bool bulletExists[3] = {false,false,false};
 
//
// Forward declaration of functions (all functions are at the last portion of the code)
//
 
void EndProgram();
void Move();
void Shoot();
void MoveBullet(int i);
void MoveNme();
 
//
// Data Structures (I use structs because I want everything to be public)
//
 
 
// Game Data (only images for now)
 
struct Data {
 
    sf::Image bullet1;
    sf::Image bullet2;
    sf::Image Nme1;
    sf::Image Nme2;
    sf::Image player1;
    sf::Image player2;
 
    Data()
    {
        bullet1.LoadFromFile("bala1.png");
        bullet2.LoadFromFile("bala2.png");
        Nme1.LoadFromFile("malo1.png");
        Nme2.LoadFromFile("malo2.png");
        player1.LoadFromFile("nave.png");
        player2.LoadFromFile("nave2.png");
    }
 
}data;
 
 
 
// Generic struct for all image objects
 
struct Image
{
 
    sf::Sprite sprite;
 
    // The following are the ints that hold the coordinates for each image. In this case, I think there are no better variable names than
    // 'x', 'y', 'x2' and 'y2'. Just like in geometry :)
 
    int x;
    int y;
    int x2;
    int y2;
 
    int speed;
};
 
 
//
// In the following structs, each object is derived from the image struct. They are assigned a sprite, a position and a speed.
//
 
 
// The player's ship
 
struct PlayerShip: Image
{
 
    PlayerShip()
    {
 
        sprite.SetImage(data.player1);
        sprite.SetPosition(350,475);    // I don't know if using plain numbers here is really that bad. It's just a position.
        x = 350;
        y = 475;
        speed = 6;
    }
 
}player;
 
 
// The enemy's ship
 
struct Nme: Image
{
 
    Nme()
    {
        sprite.SetImage(data.Nme1);
        sprite.SetPosition(SPAWN_POS1,0);
        x = 350;
        y = 0;
        speed = 2;
    }
 
}enemy;
 
 
// The "bullets" fired by the player
 
struct BulletStruct: Image
{
 
    BulletStruct()
    {
        speed = 10;
        sprite.SetImage(data.bullet1);
        sprite.SetPosition( (player.x + 15),(player.y) );   // Positioned at the center of the player's ship.
        y = player.y;
    }
 
}*bullet[3]; /* Maybe there was no need for bullets to be pointers, but I wanted to practice using them. The reason why there are only 3 bullets is                 personal preference. Remember the NES Mega Man games? You could only fire 3 shots at a time. */
 
 
//
// Main Function
//
 
int main()
{
    window.SetFramerateLimit(60);
    Clock.Reset();
 
    while (window.IsOpened())
    {
        EndProgram();
        window.Clear();
        window.Draw(player.sprite);
        Shoot();
        Move();
        MoveNme();
 
        // Because the number of bullets on the screen is variable, the functions related to them must be controlled.
 
        for (int i = 0; i < 3; i++)
        {
            if (bulletExists[i]) MoveBullet(i);
            if (bulletExists[i]) window.Draw(bullet[i]->sprite);
            if (!bulletExists[i]) bulletClock[i].Reset();
        }
 
        window.Display();
    }
    return EXIT_SUCCESS;
}
 
 
// Exit Function
 
void EndProgram()
{
    while(window.GetEvent(event))
    {
        if (event.Type == sf::Event::Closed)
        window.Close();
    }
}
 
 
// Moving the player
 
void Move()
{
 
    // The player moves the ship by pressing the left or right arrow keys. It stops when it reaches the predefined borders of the screen.
 
    if ( window.GetInput().IsKeyDown(sf::Key::Left) && (player.x > LEFT_BORDER) ) {player.sprite.Move(-player.speed,0); player.x -= player.speed;}
    if ( window.GetInput().IsKeyDown(sf::Key::Right) && (player.x < RIGHT_BORDER) ) {player.sprite.Move(player.speed,0); player.x += player.speed;}
 
    // The following code controls the animation of the sprite. Every tenth of a second, the sprite will change. I haven't figured out a way to
    // simplify this process. (I know of the SetSubRect function of SFML, but I didn't feel like making a "sprite map" kind of image)
 
    if ((Clock.GetElapsedTime() > 0.0) && (Clock.GetElapsedTime() < 0.1)) player.sprite.SetImage(data.player1);
    if ((Clock.GetElapsedTime() > 0.1) && (Clock.GetElapsedTime() < 0.2)) player.sprite.SetImage(data.player2);
    if (Clock.GetElapsedTime() > 0.2) Clock.Reset();
}
 
 
// Shooting
 
/*
    The player shoots by pressing the Space key.
 
    The shooting function needs to check for three main states. First, which bullet are we going to fire? That's the reason behind the for loop. Second,        does this bullet already exists? That's the reason behind the (!bulletExists[i]). And third, has the last bullet moved up enough for the next shot to       be fired (has moved beyond the 370 'y' value.
*/
 
void Shoot()
{
    for (int i = 0; i < 3; i++)
    {
        if ( (i != 0) && (!bulletExists[i]) && (bulletExists[i-1]) )
        {
            if ( (window.GetInput().IsKeyDown(sf::Key::Space)) && (bullet[i-1]->y < 370) )
            {
                bullet[i] = new BulletStruct;
                bulletExists[i] = true;
            }
        }
        else if ( (i == 0) && (window.GetInput().IsKeyDown(sf::Key::Space)) && (bulletExists[i] == false) )
        {
            bullet[i] = new BulletStruct;
            bulletExists[i] = true;
        }
    }
}
 
 
// Moving the bullet sprite
 
void MoveBullet(int i)
{
    bullet[i]->sprite.Move(0,-bullet[i]->speed);
    bullet[i]->y -= bullet[i]->speed;
 
    if ((bulletClock[i].GetElapsedTime() > 0.0) && (bulletClock[i].GetElapsedTime() < 0.1)) bullet[i]->sprite.SetImage(data.bullet1);
    if ((bulletClock[i].GetElapsedTime() > 0.1) && (bulletClock[i].GetElapsedTime() < 0.2)) bullet[i]->sprite.SetImage(data.bullet2);
    if (bulletClock[i].GetElapsedTime() > 0.2) bulletClock[i].Reset();
    
    window.Draw(bullet[i]->sprite);
 
    if (bullet[i]->y < 0) {delete bullet[i]; bulletExists[i] = false;}
}
 
 
// Moving the enemy sprite
 
void MoveNme()
{
    enemy.sprite.Move(0,enemy.speed);
    enemy.y += enemy.speed;
 
    if (enemy.y > BOTTOM_BORDER) {enemy.sprite.SetPosition(SPAWN_POS1,TOP_BORDER); enemy.y = 0;}
 
    if ((Clock.GetElapsedTime() > 0.0) && (Clock.GetElapsedTime() < 0.1)) enemy.sprite.SetImage(data.Nme1);
    if ((Clock.GetElapsedTime() > 0.1) && (Clock.GetElapsedTime() < 0.2)) enemy.sprite.SetImage(data.Nme2);
    window.Draw(enemy.sprite);
}