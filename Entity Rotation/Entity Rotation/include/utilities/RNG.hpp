// @File: 		RNG.hpp
// @Project: 	Entity Rotation
// @Author:		Justin Byrne
// @Date:		Created 6/26/22 at 10:14 PM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef RNG_hpp
#define RNG_hpp

#include <stdlib.h>
#include <random>

/// Generates a random number, using the value passed as the upper range value; 0 - upper
/// @param      upper               Upper bounds value
int generate_random ( int upper )
{
    return ( std::rand ( ) % ( upper + 1 ) );
}

/// Generates a random number in the range passed
/// @param      lower               Lower bounds value
/// @param      upper               Upper bounds value
int generate_random ( int lower, int upper )
{
    return ( std::rand ( ) % ( upper - lower + 1 ) ) + lower;
}

/// Generate value from normal distribution
/// @param      mean            Distribution mean
/// @param      standard_deviation      Standard Deviation
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

#endif /* RNG_hpp */
