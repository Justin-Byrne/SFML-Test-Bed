// @File: 		SFML_helpers.hpp
// @Project: 	Entity Rotation
// @Author:		Justin Byrne
// @Date:		Created 6/26/22 at 10:10 PM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef SFML_helpers_hpp
#define SFML_helpers_hpp

#include "../../common/structs/general/point.hpp"

sf::CircleShape SFML_draw_circle ( float x, float y, int radius, sf::Color stroke, int stroke_size, sf::Color fill )
{
    sf::CircleShape result;
    
    result.setRadius           ( radius );
    result.setOutlineColor     ( stroke );
    result.setFillColor        ( fill );
    result.setOutlineThickness ( stroke_size );
    result.setPosition         ( x, y );
    
    return result;
}

sf::CircleShape SFML_draw_circle ( POINT point, int radius, sf::Color stroke, int stroke_size, sf::Color fill )
{
    sf::CircleShape result;
    
    result.setRadius           ( radius );
    result.setOutlineColor     ( stroke );
    result.setFillColor        ( fill );
    result.setOutlineThickness ( stroke_size );
    result.setPosition         ( point.x, point.y );
    
    return result;
}

#endif /* SFML_helpers_hpp */
