// @File: 		main.cpp
// @Project: 	Entity Rotation
// @Author:		Justin Byrne
// @Date:		Created 6/26/22 at 10:02 PM
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
        std::string          title  = "Rotation";
        
        static constexpr int width  = 800;
        static constexpr int height = 800;
        
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
sf::CircleShape create_circle ( POINT position, float radius, sf::Color stroke_color, int stroke_size, sf::Color fill_color );

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
/// @remarks: Coupling: Low
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

//struct SCENE_NODE
//{
//    void draw ( sf::RenderTarget & target, const sf::Transform & parent_transform ) const
//    {
//        sf::Transform combined_transform = parent_transform * this->transform;  // combine the parent transform with the node's transform
//
//        onDraw ( target, combined_transform );                                  // let the node draw itself
//
//        for ( std::size_t i = 0; i < this->children.size ( ); i++ )             // draw its children
//            this->children[i]->draw(target, combined_transform);
//    }
//
//private:
//
//    virtual void onDraw ( sf::RenderTarget & target, const sf::Transform & transform ) const = 0;
//
//    sf::Transform transform;
//    std::vector<SCENE_NODE *> children;
//};

#pragma mark - UTILITIES

sf::CircleShape create_circle ( POINT position, float radius, sf::Color stroke_color, int stroke_size, sf::Color fill_color )
{
    sf::CircleShape result;
    
    result.setRadius           ( radius );
    result.setOutlineColor     ( stroke_color );
    result.setOutlineThickness ( stroke_size );
    result.setFillColor        ( fill_color );
    result.setPosition         ( position.x, position.y );
    
    return result;
}

struct ENTITY
{
    int        id;
    static int ID;
    
    POINT position;
    float size;
    
    ENTITY ( POINT position, float size )
    {
        this->position = position;
        this->size     = size;
        
        create_body ( );
    }
    
    // Constructors (Generic) ... //
    
    ENTITY  ( ) { }
    
    ~ENTITY ( ) { }
    
    // > .. Operations ................................. //
    
    void create_body ( )
    {
        this->children.push_back ( create_circle ( POINT { this->position.x,       this->position.y },       this->size,         sf::Color::Black, 0, globals.colors.blue_bright ) );  // Body
        this->children.push_back ( create_circle ( POINT { this->position.x + 120, this->position.y + 33  }, this->size * 0.30,  sf::Color::Black, 0, sf::Color::White ) );            // Eye (Left)
        this->children.push_back ( create_circle ( POINT { this->position.x + 120, this->position.y + 100 }, this->size * 0.30,  sf::Color::Black, 0, sf::Color::White ) );            // Eye (Right)
        this->children.push_back ( create_circle ( POINT { this->position.x + 150, this->position.y + 48  }, this->size * 0.135, sf::Color::Black, 0, sf::Color::Black ) );            // Pupil (Left)
        this->children.push_back ( create_circle ( POINT { this->position.x + 150, this->position.y + 116 }, this->size * 0.135, sf::Color::Black, 0, sf::Color::Black ) );            // Pupil (Right)
    
        this->children[0].setOrigin ( this->children[0].getRadius ( ), this->children[0].getRadius ( ) );
//        this->children[1].setOrigin ( this->size * 0.30, this->size * 0.30 );
    }
    
    void draw ( sf::RenderTarget & target, const sf::Transform & transform ) const
    {
        for ( std::size_t i = 0; i < this->children.size ( ); i++ )             // draw its children
            target.draw ( this->children[i], transform );
    }
    
    void draw ( sf::RenderTarget & target ) const
    {
        for ( std::size_t i = 0; i < this->children.size ( ); i++ )             // draw its children
            target.draw ( this->children[i] );
    }
    
    std::vector<sf::CircleShape> children;
};

int ENTITY::ID;

#pragma mark - MAIN

int main ( int argc, const char * argv[] )
{
    sf::RenderWindow window ( sf::VideoMode ( globals.window.width, globals.window.height ), globals.window.title );   // Create the main window
    
    window.setFramerateLimit ( globals.window.frame_rate );
    
    POINT position = { 0.0, 0.0 };
    float radius   = 100.0;
    
    ENTITY entity = { position, radius };
    
    int k = 0;
    
    while ( window.isOpen ( ) )                                                 // Simulation Loop
    {
        window.clear ( globals.colors.gray_charcoal );                          // Clear screen
     
        // ... DRAW ......................................................... //
        
        sf::Transform transform;
        
        // ... TEST ......................................................... //
        
        POINT rendered_position = { entity.children[0].getPosition().x, entity.children[0].getPosition().y };
        POINT rendered_origin   = { entity.children[0].getOrigin().x,   entity.children[0].getOrigin().y   };
        
//        entity.children[0].circle.setPosition ( rendered_position.x + 1, rendered_position.y + 1 );
//        entity.position.x = rendered_position.x + 1;
//        entity.position.y = rendered_position.y + 1;
        
        printf ( "\nPosition:\tx: %f, y: %f", rendered_position.x, rendered_position.y );
        printf ( "\nOrigin:\t\tx: %f, y: %f", rendered_origin.x,   rendered_origin.y );
        printf ( "\nRadius:\t\t%f\n",         radius );
        
        sf::CircleShape circle_position = create_circle ( rendered_position, 5.0, sf::Color::Red,  1, sf::Color::White );
        sf::CircleShape circle_origin   = create_circle ( rendered_origin,   5.0, sf::Color::Blue, 1, sf::Color::Green );
        
        // ... TEST ......................................................... //
        
        entity.draw ( window, transform.rotate ( k ) );                         // Draw
//        entity.draw( window, transform.scale ( 0.75f, 0.75f ) );                // Draw
//        entity.draw ( window, transform.translate ( k, k ) );                   // Draw
        
//        entity.draw ( window );                                                 // Draw
        
        window.draw ( circle_position );
        
        window.draw ( circle_origin );
        
        window.display ( );                                                     // Update the window
        
        k++;
        
        // ... EVENT ........................................................ //
        
        sf::Event event;                                                        // Process events
        
        while ( window.pollEvent ( event ) )
            if ( event.type == sf::Event::Closed ) window.close ( );            // Close window: exit
    }
    
    return 0;
}
