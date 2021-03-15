//////
//
// nave_v2.cpp:
//
//////
 
#include <iostream>
#include "SFML/Graphics.hpp"
#include "nave_v2.h"
 
int main()
{
 
    app.window.Create(sf::VideoMode(800,600,32),"Window");
    app.window.SetFramerateLimit(60);
 
 
 
    // Forward declaration of functions
 
    void ProcessInput();
    void EndProgram();
    void DrawBullet(int numOfBullets);
    void MoveBullet(int numOfBullets);
 
 
 
    // Main game loop
 
    while (app.window.IsOpened())
    {
        app.window.Clear();         //Clear the screen
 
        app.window.Draw(player.sprite);     //Draw the player
 
        DrawBullet(app.totalBullets);       //Draw the existing bullets
 
        app.window.Display();           //Display the sprites
 
        ProcessInput();             //Process the user's input
 
        MoveBullet(app.totalBullets);       //Move the existing bullets
 
        if (player.canShoot == false) app.CheckBulletClock(); //The player cannot shoot at any given time. There should
                                    // a "recharging" time between shots. This function checks
                                    // if that time has elapsed.
 
        EndProgram();       //Check if the user wishes to exit (it's separated from ProcessInput 
                    //because it uses sf::event, instead of sf::RenderWindow::GetInput() ).
    }
 
    return 0;
}
 
 
/////  Functions
 
 
 
/* ProcessInput receives real-time input from the user through the sf::RenderWindow::GetInput() function.
 
The player can move left or right with the arrow keys, and shoot with the space bar. When shooting, the
program must determine if the bullet to be shot doesn't already exist.
 
*/
 
void ProcessInput()
{
    if (app.window.GetInput().IsKeyDown(sf::Key::Left)) player.Move(-6,0);
    if (app.window.GetInput().IsKeyDown(sf::Key::Right)) player.Move(6,0);
    if (app.window.GetInput().IsKeyDown(sf::Key::Space))
    {
        if (!bullet[bData.nextBullet].exists) bullet[bData.nextBullet].Shoot();
    }
}
 
 
 
//EndProgram awaits for the user to produce an event. If that event is clicking on the X button
//or pressing alt-f4 (sf::Event::Closed) the window closes.
 
void EndProgram()
{
    while(app.window.GetEvent(app.event))
    {
        if (app.event.Type == sf::Event::Closed)
        app.window.Close();
    }
}
 
 
 
//DrawBullet simply draws all existing bullet in the screen. It receives the total number of
//bullets and cycles through them to check if they exist at the time.
 
void DrawBullet(int numOfBullets)
{
    for (int i = 0; i < numOfBullets; i++)
    {
        if (bullet[i].exists) app.window.Draw(bullet[i].sprite);
    }
}
 
 
 
//Same concept as before, but this functions moves the bullets up the screen.
 
void MoveBullet(int numOfBullets)
{
    for (int i = 0; i < numOfBullets; i++)
    {
        if (bullet[i].exists) bullet[i].Move(0,-10);
    }
}
 
 
 
 
 
 
 
 
 
////////
//
// nave_v2.h
//
////////
 
#ifndef CLASSES
#define CLASSES
 
 
// The Image class contains all the basic data an image should have.
 
class Image
{
 
    public:
 
    sf::Image image1;   //An image file to load from
    sf::Sprite sprite;  //A sprite to draw on screen
 
    int x;  //Coordinates that can be used to check position and collisions
    int y;
    int x2;
    int y2;
 
    int speed;  //An int that determines how fast sprites should move
 
    void Move(float x, float y) // A simple, generic Move() function.
                                // With this function you can type
                                // Object.Move(x,y), instead of
                                // Object.sprite.Move(x,y).
    {
        sprite.Move(x,y);
    }
};
 
 
 
// PlayerShip class derives from Image and adds the canShoot variable.
 
class PlayerShip: public Image
{
 
    public:
 
    bool canShoot;  //this variable helps control the rate at which the player shoots.
                    //It becomes true when a certain time after the last shot has elapsed.
 
    PlayerShip()
    {
        image1.LoadFromFile("nave.png");
        sprite.SetImage(image1);
        sprite.SetPosition(350,475);
        x = 350;
        y = 475;
        speed = 6;
        canShoot = true;    //At the beginning of the game the player is ready to shoot.
    }
 
    void Move(float a, float b)
    {
        sprite.Move(a,b);
        x += a;
        y += b;
    }
 
}player;
 
 
 
// This class contains data that concerns all bullets in the game.
 
class BulletData
{
    public:
 
        int nextBullet; // Holds the number for the next available bullet.
        
        BulletData()
        {
            nextBullet = 0;
        }
 
        void Next(int totalBullets) //This function selects the next available bullet.
        {
            if (nextBullet < totalBullets) nextBullet += 1;
            if (nextBullet == totalBullets) nextBullet = 0; // If the total has been
                                    // reached, go back to the
                                    // first bullet.
        }
}bData;
 
 
 
// The main application class. It handles all "global" variables and functions.
 
class App
{
    public:
 
        sf::RenderWindow window;
        sf::Event event;
        sf::Clock bulletClock;
        
        int totalBullets;
 
        App()
        {
            totalBullets = 7;
        }
 
        
 
        // The following function checks if 0.1 seconds have passed since the last
        // shot was fired.
 
        void CheckBulletClock()
        {
            if (bulletClock.GetElapsedTime() > 0.1)
            {
                bData.Next(totalBullets);
                player.canShoot = true;
                bulletClock.Reset();
            }
        }
 
}app;
 
 
 
class Bullet: public Image
{
 
    public:
 
        bool exists;
 
        Bullet()
        {
            image1.LoadFromFile("bala1.png");
            sprite.SetImage(image1);
            exists = false;
        }
        
        void Shoot()
        {
            exists = true;
            sprite.SetPosition((player.x + 20),(player.y + 5));
            y = (player.y + 5);
 
            player.canShoot = false;
 
            app.bulletClock.Reset();
        }
 
        void Move(float a, float b)
        {
            sprite.Move(a,b);
            y += b;
 
            if (y < (player.y - 480)) //This part of the function destroys the bullet when
                        // it reaches a certain point.
            {
                exists = false;
            }
        }
 
}bullet[7];
 
#endif