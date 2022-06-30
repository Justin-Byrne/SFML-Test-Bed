// @File: 		main.cpp
// @Project: 	Movement
// @Author:		Justin Byrne
// @Date:		Created 6/28/22 at 10:16 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SFML/Graphics.hpp>

#include <cmath>
#include <string>

struct CONFIG
{
    std::string window_title  = "Movement";
    int         window_width  = 900;
    int         window_height = 900;
    
    int         frame_rate    = 60;
    
} config;

struct POINT
{
    float x         = 0;
    float y         = 0;
    float z         = 0;
    
    float magnitude = 0;
    
    POINT ( float x, float y )
    {
        this->x = x;
        this->y = y;
    }
    
    POINT ( float x, float y, float z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    // Constructors (Generic) ... //
    
    POINT  ( ) { }
    
    ~POINT ( ) { }
    
    // > .. Operations ................................. //

    void add ( POINT velocity )
    {
        this->x += velocity.x;
        this->y += velocity.y;
    }
    
    void subract ( POINT velocity )
    {
        this->x -= velocity.x;
        this->y -= velocity.y;
    }
    
    void multiply ( float value )
    {
        this->x *= value;
        this->y *= value;
    }
    
    void divide ( float value )
    {
        this->x /= value;
        this->y /= value;
    }
    
    void normalize_magnitude ( POINT velocity )
    {
        float magnitude = this->get_magnitude ( );
        
        if ( magnitude != 0 ) this->divide ( magnitude );
    }
    
    // > .. Getters .................................... //
    
    float get_magnitude ( )
    {
        return std::sqrt ( this->x * this->x + this->y * this->y );
    }
    
    // > .. Setters .................................... //
    
    void set_magnitude ( )
    {
        this->magnitude = this->get_magnitude ( );
    }
    
    void limit ( POINT velocity );
    void heading ( POINT velocity );
    void rotate ( POINT velocity );
    void lerp ( POINT velocity );
    void distance ( POINT velocity );
    void angle_between ( POINT velocity );
    void dot_product ( POINT velocity );
    void cross_product ( POINT velocity );
};

struct PATH : POINT
{
    POINT location, velocity;
    
    PATH ( POINT location )
    {
        this->location = location;
    }
    
    // Constructors (Generic) ... //
    
    PATH  ( ) { }
    
    ~PATH ( ) { }
    
    // > .. Operations ................................. //
    
    void update ( POINT velocity )
    {
        this->location.add ( velocity );
    }
    
    void check_boundary ( )         // << TODO: FIX THIS GLOBAL COUPLING | MEDIATOR PATTERN !?
    {
        if ( location.x > config.window_width  ) location.x = 0;
        if ( location.y > config.window_height ) location.y = 0;
        
        if ( location.x < 0 ) location.x = config.window_width;
        if ( location.y < 0 ) location.y = config.window_height;
    }
    
    void display ( )
    {
        // move object around path
    }
};

sf::CircleShape create_circle ( float x, float y, int radius, sf::Color stroke, int stroke_size, sf::Color fill )
{
    sf::CircleShape result;
    
    result.setRadius           ( radius );
    result.setOutlineColor     ( stroke );
    result.setFillColor        ( fill );
    result.setOutlineThickness ( stroke_size );
    result.setPosition         ( x, y );
    
    return result;
}

sf::CircleShape create_circle ( POINT point, int radius, sf::Color stroke, int stroke_size, sf::Color fill )
{
    sf::CircleShape result;
    
    result.setRadius           ( radius );
    result.setOutlineColor     ( stroke );
    result.setFillColor        ( fill );
    result.setOutlineThickness ( stroke_size );
    result.setPosition         ( point.x, point.y );
    
    return result;
}

int main ( int argc, const char * argv[] )
{
    sf::RenderWindow window ( sf::VideoMode ( config.window_width, config.window_height ), config.window_title );   // Create the main window
    
    window.setFramerateLimit ( config.frame_rate );
    
    
    float velocity = 3;
    
    sf::CircleShape circle = create_circle( POINT { 30, 30 }, 10, sf::Color::Blue, 1, sf::Color::Transparent );
    
    while ( window.isOpen ( ) )                                                 // Simulation Loop
    {
        window.clear ( sf::Color::Black );                                      // Clear screen
        
        // location = location + velocity
        circle.setPosition ( circle.getPosition().x + velocity, circle.getPosition().y + velocity );
        
        // ... DRAW ......................................................... //
        
        window.draw ( circle );                                                 // Draw
        
        window.display ( );                                                     // Update the window
        
        sf::Event event;                                                        // Process events
        
        while ( window.pollEvent ( event ) )
            if ( event.type == sf::Event::Closed ) window.close ( );            // Close window: exit
    }
    
    return 0;
}
