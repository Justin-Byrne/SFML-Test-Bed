// @File: 		entity.hpp
// @Project: 	Entity Rotation
// @Author:		Justin Byrne
// @Date:		Created 6/26/22 at 10:09 PM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef entity_hpp
#define entity_hpp

#include "states.hpp"
#include "point.hpp"
#include "angle.hpp"
#include "attributes.hpp"

struct ENTITY
{
    int id;
    static int ID;
    
    int state = SILENT;
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

#endif /* entity_hpp */
