// @File: 		attributes.hpp
// @Project: 	Entity Rotation
// @Author:		Justin Byrne
// @Date:		Created 6/26/22 at 10:08 PM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef attributes_hpp
#define attributes_hpp

#include "RNG.hpp"

//#include <random>       // TODO: MOVE TO RNG.HPP

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
    
    // Functions ............................................................ //
    
    // > .. Getters ................. //
    
//    int get_distributed_value ( int mean, int standard_deviation )              // TODO: MOVE TO RNG.HPP
//    {
//        int result = 0;
//
//        std::random_device               random_device;
//        std::default_random_engine       random_engine ( random_device ( ) );
//        std::normal_distribution<double> distribution  ( mean, standard_deviation );
//
//        do
//            result = (int) std::round ( distribution ( random_engine ) );
//        while ( result < 1 );
//
//        return result;
//    }
};

#endif /* attributes_hpp */
