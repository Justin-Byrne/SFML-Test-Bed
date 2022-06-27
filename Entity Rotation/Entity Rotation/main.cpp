// @File: 		main.cpp
// @Project: 	Entity Rotation
// @Author:		Justin Byrne
// @Date:		Created 6/26/22 at 10:02 PM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SFML/Graphics.hpp>

#include "include/headers.hpp"

int main ( int argc, const char * argv[] )
{
    std::srand ( ( unsigned ) time ( 0 ) );                                     // Seed randomizer
    
    sf::RenderWindow window ( sf::VideoMode ( 900, 900 ), "SFML window" );      // Create the main window
    
    window.setFramerateLimit ( FRAME_RATE );
    
    
    ENTITY entity[ENTITY_MAX];
    
    int padding = 50;

    for ( int i = 0; i < ENTITY_MAX; i++ )
    {
        entity[i]       = { POINT { (float) generate_random ( padding, WINDOW_WIDTH - padding ), (float) generate_random ( padding, WINDOW_HEIGHT - padding ) } };
        entity[i].angle = { generate_random ( 0, 360 ), generate_random ( 0, 360 ) };
    }
    
    POINT rotate_origin, rotate_destination;
    
    sf::Color * step_colors = generate_step_colors ( 255, true );
    
    sf::CircleShape entity_body;        // TODO: SEE ABOUT ENCAPSULATING WITHIN obj ENTITY
    
    
    while ( window.isOpen ( ) )                                                 // Simulation Loop
    {
        window.clear ( colors::gray_charcoal );                                 // Clear screen
 
        for ( int i = 0; i < ENTITY_MAX; i++ )
        {
            switch ( entity[i].state )
            {
                case SILENT:
                case ROTATE:
                    
                    #if DEBUG_ROTATE
                    rotate_origin      = entity[i].rotate ( entity[i].angle.a, entity[i].attributes.size );
                    SDL_RenderDrawLine ( renderer, entity[i].origin, rotate_origin, colors::yellow_sun );

                    rotate_destination = entity[i].rotate ( entity[i].angle.b, entity[i].attributes.size );
                    SDL_RenderDrawLine ( renderer, entity[i].origin, rotate_destination, colors::black );
                    #endif
                    
                    ( entity[i].angle.clockwise ) ? entity[i].angle.advance ( ) : entity[i].angle.regress ( );
                    
                    if ( entity[i].angle.distance == 0 )
                    {
                        entity[i].state = MOVING;
                        entity[i].walk  = generate_random ( 1, 30 );
                    }
                    
                    break;
                    
                case MOVING:
                    
                    entity[i].next_step ( 2 );

                    if ( entity[i].walk == 0 )
                    {
                        entity[i].state = SILENT;
                        entity[i].angle = { entity[i].angle.b, generate_random ( 0, 360 ) };
                    }

                    break;
            }
            
            entity_body = SFML_draw_circle ( entity[i].origin, entity[i].attributes.size, colors::blue_baby, 1, sf::Color::Transparent );
            
            #if DEBUG_STEPS
            sf::VertexArray entity_steps ( sf::Points, STEP_MAX );
            
            for ( int j = 0; j < STEP_MAX; j++ )
            {
                entity_steps[j].position = sf::Vector2f ( entity[i].steps[j].x, entity[i].steps[j].y );
                entity_steps[j].color    = step_colors[j];
            }
            
            window.draw ( entity_steps );
            #endif
            
            window.draw ( entity_body );
        }
        
//        window.draw  ( circle );                                                // Draw
 
//        window.draw ( text );                                                   // Draw the string
        
        window.display ( );                                                     // Update the window
        
        sf::Event event;                                                        // Process events
        
        while ( window.pollEvent ( event ) )
            if ( event.type == sf::Event::Closed ) window.close ( );            // Close window: exit
    }
 
    return EXIT_SUCCESS;
}
