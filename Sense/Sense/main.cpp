// @File: 		main.cpp
// @Project: 	Sense
// @Author:		Justin Byrne
// @Date:		Created 7/10/22 at 4:55 PM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <random>

#pragma mark - GLOBAL CONSTANTS

// DEBUG
#define DEBUG_ROTATE                   1
#define DEBUG_BODY                     1
#define DEBUG_SENSE                    1
#define DEBUG_SIGHTLINE                1
#define DEBUG_STEPS                    0

// PROGRAM
#define WINDOW_TITLE "Entity Simulation"
#define WINDOW_WIDTH                 400
#define WINDOW_HEIGHT                400
#define ENTITY_MAX                     2
#define STEP_MAX                      25
#define LINE_SEGMENTS                  5
#define FRAME_RATE                    15

#pragma mark - MATHS

#define PI 3.141592653589
#define SIN ( x ) sin ( x * PI / 180 )
#define COS ( x ) cos ( x * PI / 180 )

#pragma mark - RANDOM

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

#pragma mark - UTILITIES (SPECIFIC)

sf::Color * generate_step_colors ( sf::Uint8 top_color, bool invert = false )
{
    const sf::Uint8 COLOR_MIN = 84;
//    const int COLOR_MAX = 255;
    
    static sf::Color result[STEP_MAX];
    
//    top_color = std::clamp ( top_color, COLOR_MIN, COLOR_MAX );
    
    int difference = ( invert ) ? ( top_color - COLOR_MIN ) / STEP_MAX : top_color / STEP_MAX;
    
    if ( invert )
        for ( int i = STEP_MAX - 1; i > -1; i-- )
            result[i] = {
                static_cast<sf::Uint8> ( ( top_color - ( i * difference ) ) ),
                static_cast<sf::Uint8> ( ( top_color - ( i * difference ) ) ),
                static_cast<sf::Uint8> ( ( top_color - ( i * difference ) ) )
            };
    else
        for ( int i = 0; i < STEP_MAX; i++ )
            result[i] = {
                static_cast<sf::Uint8> ( ( ( (sf::Uint8) i + 1 ) * difference ) ),
                static_cast<sf::Uint8> ( ( ( (sf::Uint8) i + 1 ) * difference ) ),
                static_cast<sf::Uint8> ( ( ( (sf::Uint8) i + 1 ) * difference ) )
            };
    
    return result;
}

#pragma mark - STRUCTS . . . . . . . . . . . . . . //

enum STATE
{
    SILENT,
    ROTATE,
    MOVING,
};

#pragma mark - POINT

struct POINT
{
    float x, y, z;
    
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
    
    // > .. Getters .................................... //
        
    double get_distance_from ( POINT one, POINT two )         // TODO: MOVE THIS SHIT !!!
    {
        return std::sqrt ( ( one.x - two.x ) * ( one.x - two.x ) +
                           ( one.y - two.y ) * ( one.y - two.y ) );
    }
};

#pragma mark - ANGLE

std::map<int, int> hash =
{
    {   1,  1 }, {   2,  1 }, {   3,  1 }, {   4,  1 },
    {   5,  2 }, {   6,  2 }, {   7,  2 }, {   8,  2 }, {   9,  2 }, {  10,  2 },
    {  11,  3 }, {  12,  3 }, {  13,  3 }, {  14,  3 }, {  15,  3 }, {  16,  3 },
    {  17,  4 }, {  18,  4 }, {  19,  4 }, {  20,  4 }, {  21,  4 }, {  22,  4 }, {   23, 4 }, {  24,  4 },
    {  25,  5 }, {  26,  5 }, {  27,  5 }, {  28,  5 }, {  29,  5 }, {  30,  5 }, {   31, 5 }, {  32,  5 }, {  33,  5 }, {  34,  5 }, {  35,  5 }, {  36,  5 }, {  37,  5 }, {  38,  5 }, {  39,  5 }, {  40,  5 },
    {  41,  6 }, {  42,  6 }, {  43,  6 }, {  44,  6 }, {  45,  6 }, {  46,  6 }, {   47, 6 }, {  48,  6 }, {  49,  6 }, {  50,  6 }, {  51,  6 }, {  52,  6 }, {  53,  6 }, {  54,  6 },
    {  55,  7 }, {  56,  7 }, {  57,  7 }, {  58,  7 }, {  59,  7 }, {  60,  7 }, {   61, 7 }, {  62,  7 }, {  63,  7 }, {  64,  7 }, {  65,  7 }, {  66,  7 }, {  67,  7 }, {  68,  7 }, {  69,  7 }, {  70,  7 },
    {  71,  8 }, {  72,  8 }, {  73,  8 }, {  74,  8 }, {  75,  8 }, {  76,  8 }, {   77, 8 }, {  78,  8 }, {  79,  8 }, {  80,  8 }, {  81,  8 }, {  82,  8 }, {  83,  8 }, {  84,  8 },
    {  85,  9 }, {  86,  9 }, {  87,  9 }, {  88,  9 }, {  89,  9 }, {  90,  9 }, {   91, 9 }, {  92,  9 }, {  93,  9 }, {  94,  9 }, {  95,  9 }, {  96,  9 }, {  97,  9 }, {  98,  9 }, {  99,  9 }, { 100,  9 },
    { 101, 10 }, { 102, 10 }, { 103, 10 }, { 104, 10 }, { 105, 10 }, { 106, 10 }, { 107, 10 }, { 108, 10 }, { 109, 10 }, { 110, 10 }, { 111, 10 }, { 112, 10 }, { 113, 10 }, { 114, 10 },
    { 115, 11 }, { 116, 11 }, { 117, 11 }, { 118, 11 }, { 119, 11 }, { 120, 11 }, { 121, 11 }, { 122, 11 }, { 123, 11 }, { 124, 11 }, { 125, 11 }, { 126, 11 }, { 127, 11 }, { 128, 11 }, { 129, 11 }, { 130, 11 },
    { 131, 12 }, { 132, 12 }, { 133, 12 }, { 134, 12 }, { 135, 12 }, { 136, 12 }, { 137, 12 }, { 138, 12 }, { 139, 12 }, { 140, 12 }, { 141, 12 }, { 142, 12 }, { 143, 12 }, { 144, 12 },
    { 145, 13 }, { 146, 13 }, { 147, 13 }, { 148, 13 }, { 149, 13 }, { 150, 13 }, { 151, 13 }, { 152, 13 }, { 153, 13 }, { 154, 13 }, { 155, 13 }, { 156, 13 }, { 157, 13 }, { 158, 13 }, { 159, 13 }, { 160, 13 },
    { 161, 14 }, { 162, 14 }, { 163, 14 }, { 164, 14 }, { 165, 14 }, { 166, 14 }, { 167, 14 }, { 168, 14 }, { 169, 14 }, { 170, 14 }, { 171, 14 }, { 172, 14 }, { 173, 14 }, { 174, 14 },
    { 175, 15 }, { 176, 15 }, { 177, 15 }, { 178, 15 }, { 179, 15 }, { 180, 15 }
};

struct ANGLE
{
    int MIN = 0, MAX = 360;
    
    int a, b, distance, step = -1;
    
    bool clockwise;
    
    ANGLE ( int A, int B )
    {
        this->a = std::clamp ( A, MIN, MAX );
        this->b = std::clamp ( B, MIN, MAX );
        
        set_clockwise ( );
        set_distance  ( );
    }
    
    // Constructors (Generic) ... //
    
    ANGLE  ( ) { }
    
    ~ANGLE ( ) { }
    
    // Functions ............................................................ //
    
    // > .. Setters .................................... //
    
    void set_step ( )
    {
        this->step = ( this->step == -1 ) ? this->get_step_divisor ( this->distance ) : this->step;
    }

    void set_distance ( )
    {
        int raw_difference = ( this->a > this->b )
                                ? this->a - this->b
                                : this->b - this->a;
        
        int mod_difference = std::fmod ( raw_difference, 360 );
        
        this->distance     = ( mod_difference > 180 )
                                ? 360 - mod_difference
                                : mod_difference;
    }
    
    void set_clockwise ( )
    {
        int distance  = this->b % MAX - this->a % MAX;
        
            distance += ( distance > 180 )
                            ? ( - MAX )
                            : ( distance <= -180 )
                                ? MAX
                                : distance;
        
        this->clockwise = ( distance == MIN )
                            ? generate_random ( MIN, MAX )
                            : ( distance > MIN )
                                ? true
                                : false;
    }
    
    // > .. Getters .................................... //
    
    POINT rotate ( POINT & origin, int & degree, int step = 0 )
    {
        POINT point = { origin.x + step, origin.y };

        double radians = convert_to_radian ( degree );

        double sine    = sin ( radians );
        double cosine  = cos ( radians );

        point.x       -= origin.x;                                              // translate point back to origin
        point.y       -= origin.y;

        double x_new   = point.x * cosine - point.y * sine;                     // rotate point
        double y_new   = point.x * sine   - point.y * cosine;

        point.x        = x_new + origin.x;                                      // translate point back
        point.y        = y_new + origin.y;

        return point;
    }
    
    int get_step_divisor ( int degree_distance )
    {
        return hash [ degree_distance ];
    }
    
    int get_angle ( POINT origin, POINT destination )
    {
        int result = std::fmod ( this->convert_to_degree ( std::atan2 ( destination.y - origin.y, destination.x - origin.x ) ), 360 );
        
        if ( result < 0 ) result += 360;
        
        return result;
    }
    
    // > .. Iterators .................................. //
    
    void advance ( )    // right || counter clockwise
    {
        set_step ( );

        validate_step_v_distance ( );
        
        this->a = ( ( this->a + this->step ) > 360 )
                    ? this->step - ( 360 - this->a )
                    : this->a + this->step;
        
        decrement_distance ( );
    }
    
    void regress ( )    // left || clockwise
    {
        set_step ( );

        validate_step_v_distance ( );
        
        this->a = ( ( this->a - this->step ) < 0 )
                    ? 360 - ( this->step - this->a )
                    : this->a - this->step;
        
        decrement_distance ( );
    }
    
    void decrement_distance ( )
    {
        this->distance = this->distance - this->step;
    }
    
    // > .. Conversions ................................ //
    
    double convert_to_radian ( int degree )
    {
        return ( degree * PI / 180 );
    }

    int convert_to_degree ( float radian )
    {
        return ( radian * 180 ) / PI;
    }
    
    // > .. Validators ................................. //
    
    void validate_step_v_distance ( )
    {
        if ( this->distance < this->step ) this->step = this->distance;
    }
};

#pragma mark - ATTRIBUTES

struct ATTRIBUTES
{
    int vitality;
    
    int size;
    int sense;
    
    int health;
    int walk_speed;
    int walk_distance;
    int stamina;
    int stamina_refactor;
    
    // Constructors (Generic) ............................................... //
    
    ATTRIBUTES ( )
    {
        // SET MASTER ATTRIBUTE
        this->vitality = get_distributed_value ( 50, 20 );
        
        // DISTRIBUTE TO SLAVE ATTRIBUTES
        
        // TODO: FIX THIS SHIT FOR SIZE & SENSE !!!
        int mean               = ( this->vitality < 5 ) ? this->vitality : this->vitality * 0.1;
        int standard_deviation = ( this->vitality < 2 ) ? this->vitality : this->vitality * 0.2;
        
        this->size = get_distributed_value ( mean, standard_deviation );
        
        do
            this->sense = get_distributed_value ( mean, standard_deviation );
        while ( this->sense < this->size );
        
        this->health           = get_distributed_value ( mean, standard_deviation );
        this->walk_speed       = get_distributed_value ( mean, standard_deviation );
        this->walk_distance    = get_distributed_value ( mean, standard_deviation );
        this->stamina          = get_distributed_value ( mean, standard_deviation );
        this->stamina_refactor = get_distributed_value ( mean, standard_deviation );
    }
    
    ~ATTRIBUTES ( ) { };
};

#pragma mark - ENTITY

struct ENTITY
{
    int id;
    static int ID;
    
    int state = MOVING;
    int walk  = 0;
    
    POINT origin = { 0, 0 };
    POINT steps[STEP_MAX];
    
    ANGLE angle;
    
    ATTRIBUTES attributes;
    
    // Constructors ......................................................... //
    
    ENTITY ( POINT origin )
    {
        this->origin = origin;
        this->id     = ID++;
    }
    
    // Constructors (Generic) ....... //
    
    ENTITY  ( ) { };

    ~ENTITY ( ) { };

    // Functions ............................................................ //

    // > .. Getters ................. //
    
    POINT rotate ( int degree )
    {
        this->state = ROTATE;

        return ANGLE().rotate ( this->origin, degree );
    }

    POINT rotate ( int degree, int step_length )
    {
        this->state = ROTATE;

        return ANGLE().rotate ( this->origin, degree, step_length );
    }
    
    // > .. Iterators ............... //
    
    void next_step ( int step_size )
    {
        cache_steps ( );
        
        this->origin = ANGLE().rotate ( this->origin, this->angle.a, step_size );
        
        this->walk--;
        
        check_boundary ( );
    }
    
    // > .. Validators .............. //
    
    void check_boundary ( )
    {
        this->origin.x = ( this->origin.x <= 0 ) ? 1 : this->origin.x;                              // left
        this->origin.y = ( this->origin.y <= 0 ) ? 1 : this->origin.y;                              // top
        
        this->origin.x = ( this->origin.x >= WINDOW_WIDTH  ) ? WINDOW_WIDTH  - 1 : this->origin.x;  // right
        this->origin.y = ( this->origin.y >= WINDOW_HEIGHT ) ? WINDOW_HEIGHT - 1 : this->origin.y;  // bottom
    }
    
    bool is_inside_sense ( ENTITY walker )
    {
        int distance = POINT().get_distance_from ( this->origin, walker.origin ) - this->attributes.sense;
        
        return ( distance <= walker.attributes.size ) ? true : false;
    }
    
    // > .. Misc .................... //
    
    void cache_steps ( )
    {
        for ( int i = STEP_MAX - 1; i > 0; i-- )
            this->steps[i] = this->steps[i - 1];
        
        this->steps[0] = this->origin;
    }
};

int ENTITY::ID;

#pragma mark - NAMESPACES . . . . . . . . .  . . . //

namespace colors
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
}

namespace SFML
{
    sf::CircleShape create_circle ( float x, float y, int radius, sf::Color stroke, int stroke_size, sf::Color fill )
    {
        sf::CircleShape result;
        
        result.setRadius           ( radius );
        result.setOrigin           ( result.getRadius ( ), result.getRadius ( ) );    // WARNING: Could cause issues with graphical translations !
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
        result.setOrigin           ( result.getRadius ( ), result.getRadius ( ) );    // WARNING: Could cause issues with graphical translations !
        result.setOutlineColor     ( stroke );
        result.setFillColor        ( fill );
        result.setOutlineThickness ( stroke_size );
        result.setPosition         ( point.x, point.y );
        
        return result;
    }

    int render_dotted_line ( sf::RenderWindow & window, POINT origin, POINT destination, int segments, sf::Color color )
    {
        sf::Vertex lines[segments];
        
        int line_space = POINT().get_distance_from ( origin, destination ) / segments;
        int angle      = ANGLE().get_angle ( origin, destination );
        
        for ( int i = 0; i < segments; i++ )
        {
            origin = ANGLE().rotate ( origin, angle, line_space );
            
            lines[i].position = sf::Vector2f ( origin.x, origin.y );
            lines[i].color    = color;
        }
        
        window.draw ( lines, segments, sf::Points );
        
        return 0;
    }

    int render_line ( sf::RenderWindow & window, POINT origin, POINT destination, sf::Color color )
    {
        sf::Vertex line[2];
        
        int line_space = POINT().get_distance_from ( origin, destination );
        int angle      = ANGLE().get_angle ( origin, destination );
            
        line[0].position = sf::Vector2f ( origin.x, origin.y );                 // Origin
        line[0].color    = color;
        
        origin = ANGLE().rotate ( origin, angle, line_space );
        
        line[1].position = sf::Vector2f ( destination.x, destination.y );       // Destination
        line[1].color    = color;
        
        window.draw ( line, 2, sf::Lines );
        
        return 0;
    }
}

#pragma mark - MAIN

int main ( int argc, const char * argv[] )
{
    std::srand ( ( unsigned ) time ( 0 ) );                                     // Seed randomizer
    
    sf::RenderWindow window ( sf::VideoMode ( WINDOW_WIDTH, WINDOW_HEIGHT ), "SFML window" );      // Create the main window
    
    window.setFramerateLimit ( FRAME_RATE );
    
    
    ENTITY entity[ENTITY_MAX];
        
    entity[0] = { POINT { WINDOW_WIDTH / 2, 0 } };
    entity[1] = { POINT { WINDOW_WIDTH / 2, WINDOW_HEIGHT } };
    
    entity[0].angle = {  90,  90 };
    entity[1].angle = { 270, 270 };
    
    entity[0].attributes.size  = 20;
    entity[0].attributes.sense = 50;
    
    entity[1].attributes.size  = 40;
    entity[1].attributes.sense = 60;
    
    
    POINT rotate_origin, rotate_destination;
    
    #if DEBUG_STEPS
    sf::Color * step_colors = generate_step_colors ( 255, true );
    #endif
    
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
                    SFML::render_line ( window, entity[i].origin, rotate_origin, colors::yellow_sun );

                    rotate_destination = entity[i].rotate ( entity[i].angle.b, entity[i].attributes.size );
                    SFML::render_line ( window, entity[i].origin, rotate_origin, colors::white );
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

                    if ( entity[i].walk == 0 ) window.close ( );

                    break;
            }
            
            #if DEBUG_BODY
            sf::Color color             = ( !i ) ? colors::blue_baby : colors::gray_faded;
            sf::CircleShape entity_body = SFML::create_circle ( entity[i].origin, entity[i].attributes.size, color, 1, sf::Color::Transparent );
            window.draw ( entity_body );
            #endif
            
            #if DEBUG_SENSE
            sf::CircleShape entity_sense = SFML::create_circle ( entity[i].origin, entity[i].attributes.sense, colors::gray_faded, 1, sf::Color::Transparent );
            window.draw ( entity_sense );
            #endif
            
            #if DEBUG_SIGHTLINE
            for ( int j = i + 1; j < ENTITY_MAX; j++ )
            {
                if ( entity[i].is_inside_sense ( entity[j] ) )
                {
                    SFML::render_line ( window, entity[i].origin, entity[j].origin, colors::yellow_sun );
//                    SFML::render_dotted_line ( window, entity[i].origin, entity[j].origin, LINE_SEGMENTS, colors::yellow_sun );
                    window.setFramerateLimit ( 5 );
                }
                else
                    window.setFramerateLimit ( FRAME_RATE );
                
                if ( entity[j].is_inside_sense ( entity[i] ) )
                {
                    SFML::render_line ( window, entity[j].origin, entity[i].origin, colors::red );
//                    SFML::render_dotted_line ( window, entity[j].origin, entity[i].origin, LINE_SEGMENTS, colors::red );
                    window.setFramerateLimit ( 5 );
                }
                else
                    window.setFramerateLimit ( FRAME_RATE );
            }
            #endif
            
            #if DEBUG_STEPS
            sf::VertexArray entity_steps ( sf::Points, STEP_MAX );
            
            for ( int j = 0; j < STEP_MAX; j++ )
            {
                entity_steps[j].position = sf::Vector2f ( entity[i].steps[j].x, entity[i].steps[j].y );
                entity_steps[j].color    = step_colors[j];
            }
            
            window.draw ( entity_steps );
            #endif
        }
        
        window.display ( );                                                     // Update the window
        
        sf::Event event;                                                        // Process events
        
        while ( window.pollEvent ( event ) )
            if ( event.type == sf::Event::Closed ) window.close ( );            // Close window: exit
    }
 
    return EXIT_SUCCESS;
}
