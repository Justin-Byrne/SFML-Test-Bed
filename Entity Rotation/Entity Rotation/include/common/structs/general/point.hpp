// @File: 		point.hpp
// @Project: 	Entity Rotation
// @Author:		Justin Byrne
// @Date:		Created 6/26/22 at 10:09 PM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef point_hpp
#define point_hpp

#include <cmath>

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

#endif /* point_hpp */
