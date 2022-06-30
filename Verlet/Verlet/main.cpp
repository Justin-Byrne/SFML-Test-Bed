// @File: 		main.cpp
// @Project: 	Verlet
// @Author:		Justin Byrne
// @Date:		Created 6/27/22 at 7:43 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SFML/Graphics.hpp>

#include <time.h>

#define FRAME_RATE              60

#pragma mark - STRUCTS

typedef struct TIME_N_VELOCITY
{
    uint32_t time     = NULL;
    float    velocity = NULL;
}
time_n_velocity;

#pragma mark - OPERATIONS

uint32_t verlet ( float position, float acceleration, uint32_t delta_time )
{
    float temp_position     = NULL;
    float previous_position = position;
    
    uint32_t time           = NULL;
    
    if ( position > 0 )
    {
        time             += delta_time;
        temp_position     = position;
        position          = position * 2 - previous_position + acceleration * delta_time * delta_time;
        previous_position = temp_position;
    }
    
    return time;
}

time_n_velocity stormer_verlet ( float position, float acceleration, uint32_t delta_time )
{
    time_n_velocity result;
    
    float temp_position     = NULL;
    float velocity          = NULL;
    float previous_position = position;
    
    uint32_t time           = NULL;
    
    if ( position > 0 )
    {
        time             += delta_time;
        temp_position     = position;
        position          = position * 2 - previous_position + acceleration * delta_time * delta_time;
        previous_position = temp_position;
        velocity         += acceleration * delta_time;
    }
    
    result.time     = time;
    result.velocity = velocity;
    
    return result;
}

time_n_velocity velocity_verlet ( float position, float acceleration, uint32_t delta_time )
{
    time_n_velocity result;
    
    float velocity  = NULL;
    
    uint32_t time   = NULL;
    
    if ( position > 0 )
    {
        time     += delta_time;
        position += velocity * delta_time + 0.5 * acceleration * delta_time * delta_time;
        velocity += acceleration * delta_time;
    }
    
    result.time     = time;
    result.velocity = velocity;
    
    return result;
}

int main ( int argc, const char * argv[] )
{
//    sf::RenderWindow window ( sf::VideoMode ( 900, 900 ), "SFML window" );      // Create the main window
    
//    window.setFramerateLimit ( FRAME_RATE );
    
//    while ( window.isOpen ( ) )                                                 // Simulation Loop
//    {
//        window.clear ( sf::Color::Black );                                      // Clear screen
//
//        window.draw  ( circle );                                                // Draw
//
//        window.draw ( text );                                                   // Draw the string
//
//        window.display ( );                                                     // Update the window
//
//        sf::Event event;                                                        // Process events
//
//        while ( window.pollEvent ( event ) )
//            if ( event.type == sf::Event::Closed ) window.close ( );            // Close window: exit
//    }

    uint32_t time = verlet ( 5.0, -10.0, 0.01 );
    
    std::printf ( "Time for verlet integration is: %d\n", time );

    time_n_velocity stormer_result = { stormer_verlet ( 5.0, -10.0, 0.01 ) };
    
    std::printf ( "Time for Stormer Verlet integration is: %d\n",      stormer_result.time );
    std::printf ( "Velocity for Stormer Verlet integration is %f\n: ", stormer_result.velocity );
    
    time_n_velocity velocity_result = { velocity_verlet ( 5.0, -10.0, 0.01 ) };

    std::printf ( "Time for velocity Verlet integration is: %d\n",       velocity_result.time );
    std::printf ( "Velocity for velocity Verlet integration is: %f\n: ", velocity_result.velocity );
    
    return EXIT_SUCCESS;
}
