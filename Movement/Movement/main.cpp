// @File: 		main.cpp
// @Project: 	Movement
// @Author:		Justin Byrne
// @Date:		Created 6/28/22 at 10:16 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SFML/Graphics.hpp>

#include <cmath>
#include <string>
#include <stdlib.h>
#include <random>

#pragma mark - MATH

#define PI 3.141592653589
#define SIN ( x ) sin ( x * PI / 180 )
#define COS ( x ) cos ( x * PI / 180 )

#pragma mark - GLOBALS

struct GLOBALS
{
    struct WINDOW
    {
        std::string          title  = "Movement";
        
        static constexpr int width  = 900;
        static constexpr int height = 900;
        
        static constexpr int frame_rate = 60;
        
    } window;
    
    struct COLORS
    {
        sf::Color white          = { 255, 255, 255 };
        sf::Color yellow_sun     = { 237, 235,   0 };
        sf::Color green_apple    = { 146, 195,   0 };
        sf::Color blue_ice       = {  28,  50, 251 };
        sf::Color blue_bright    = {   0, 248, 250 };
        sf::Color blue_baby      = {  84, 212, 255 };
        sf::Color red            = { 255,   0,  10 };
        sf::Color gray_brilliant = {  84,  84,  84 };
        sf::Color gray_faded     = { 100, 100, 100 };
        sf::Color gray_charcoal  = {  54,  56,  62 };
        sf::Color black          = {   0,   0,   0 };
    } colors;
    
} globals;

#pragma mark - FORWARD DECLARATIONS
struct POINT;
struct PATH;
struct ENTITY;

int generate_random ( int upper );
int generate_random ( int lower, int upper );
int get_distributed_value ( int mean, int standard_deviation );
sf::CircleShape create_circle ( POINT point, int radius, sf::Color stroke, int stroke_size, sf::Color fill );

#pragma mark - UTILITIES

int generate_random ( int upper )
{
    return ( std::rand ( ) % ( upper + 1 ) );
}

int generate_random ( int lower, int upper )
{
    return ( std::rand ( ) % ( upper - lower + 1 ) ) + lower;
}

int get_distributed_value ( int mean, int standard_deviation )
{
    int result = 0;
    
    std::random_device               random_device;
    std::default_random_engine       random_engine ( random_device ( ) );
    std::normal_distribution<double> distribution  ( mean, standard_deviation );
    
    do
        result = (int) std::round ( distribution ( random_engine ) );
    while ( result < 1 );
    
    return result;
}

#pragma mark - STRUCTS

struct POINT
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    
    float magnitude = 0.0;
    
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

    void add ( POINT & velocity )
    {
        this->x += velocity.x;
        this->y += velocity.y;
    }
    
    void subract ( POINT & velocity )
    {
        this->x -= velocity.x;
        this->y -= velocity.y;
    }
    
    void multiply ( float & value )
    {
        this->x *= value;
        this->y *= value;
    }
    
    void divide ( float & value )
    {
        this->x /= value;
        this->y /= value;
    }
    
    void normalize_magnitude ( POINT & velocity )
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
    
//    void limit ( POINT velocity );
//    void heading ( POINT velocity );
//    void rotate ( POINT velocity );
//    void lerp ( POINT velocity );
//    void distance ( POINT velocity );
//    void angle_between ( POINT velocity );
//    void dot_product ( POINT velocity );
//    void cross_product ( POINT velocity );
};


/// Vector Pathing
/// @remarks Coupling: Low
struct PATH : POINT
{
    POINT location, velocity, accleration;
    
    sf::RenderWindow * window = nullptr;
    
    PATH ( POINT location )
    {
        this->location = location;
    }

    // Constructors (Generic) ... //
    
    PATH  ( ) { }
    
    ~PATH ( ) { }
    
    // > .. Operations ................................. //
    
    void update ( )
    {
        this->location.add ( this->velocity );
        this->location.add ( this->accleration );
        
        this->check_boundary ( );
    }
    
    void check_boundary ( )                     // << TODO: __GLOBAL COUPLING__
    {
        if ( location.x > globals.window.width  ) location.x = 0;
        if ( location.y > globals.window.height ) location.y = 0;
        
        if ( location.x < 0 ) location.x = globals.window.width;
        if ( location.y < 0 ) location.y = globals.window.height;
    }
    
    void display ( )
    {
        // move object around path
    }
    
    // > .. Setters .................................... //
    
    void set_velocity ( POINT & velocity )
    {
        this->velocity = velocity;
    }
    
    void set_accerlation ( POINT & accerlation )
    {
        this->accleration = accerlation;
    }
};

struct ENTITY : PATH
{
    enum class GENDER
    {
        MALE,
        FEMALE
    };
    
    enum class STATE
    {
        SILENT,
        ACTIVE
    };
    
    // Constructors (Generic) ... //
    
    ENTITY  ( ) { }
    
    ~ENTITY ( ) { }
};

#pragma mark - UTILITIES

sf::CircleShape create_circle ( POINT point, int radius, sf::Color stroke, int stroke_size, sf::Color fill )
{
    sf::CircleShape result;
    
    result.setRadius           ( radius );
    result.setOrigin           ( radius / 2, radius / 2 );                      // Set: origin in center of shape
    result.setOutlineColor     ( stroke );
    result.setFillColor        ( fill );
    result.setOutlineThickness ( stroke_size );
    result.setPosition         ( point.x, point.y );
    
    return result;
}

#pragma mark - MAIN

int main ( int argc, const char * argv[] )
{
    sf::RenderWindow window ( sf::VideoMode ( globals.window.width, globals.window.height ), globals.window.title );   // Create the main window
    
    window.setFramerateLimit ( globals.window.frame_rate );
    
    float velocity = 3;
    
    POINT postion = { 30, 30 };
    
    sf::CircleShape body        = create_circle ( POINT { 30, 30 }, 400, sf::Color::Transparent, 0, globals.colors.blue_bright );      // Body
    sf::CircleShape eye_left    = create_circle ( POINT { postion.x + 213.3f, postion.y + 53.0f }, 120, sf::Color::Transparent, 0, globals.colors.blue_bright );      // Eye Left
    sf::CircleShape eye_right   = create_circle ( POINT { postion.x + 213.3f, postion.y + 213.0f }, 120, sf::Color::Transparent, 0, globals.colors.blue_bright );      // Eye Right
    sf::CircleShape pupil_left  = create_circle ( POINT { postion.x + 286.0f, postion.y + 92.3f },  54, sf::Color::Transparent, 0, globals.colors.blue_bright );      // Pupil Left
    sf::CircleShape pupil_right = create_circle ( POINT { postion.x + 286.0f, postion.y + 252.0f },  54, sf::Color::Transparent, 0, globals.colors.blue_bright );      // Pupel Right
    
    while ( window.isOpen ( ) )                                                 // Simulation Loop
    {
        window.clear ( globals.colors.gray_charcoal );                          // Clear screen
        
//        body.setPosition ( body.getPosition().x + velocity, body.getPosition().y + velocity );
        
        // ... DRAW ......................................................... //
        
        window.draw ( body );                                                   // Draw
        window.draw ( eye_left );                                               // Draw
        window.draw ( eye_right );                                              // Draw
        window.draw ( pupil_left );                                             // Draw
        window.draw ( pupil_right );                                            // Draw
        
        window.display ( );                                                     // Update the window
        
        sf::Event event;                                                        // Process events
        
        while ( window.pollEvent ( event ) )
            if ( event.type == sf::Event::Closed ) window.close ( );            // Close window: exit
    }
    
    return 0;
}
