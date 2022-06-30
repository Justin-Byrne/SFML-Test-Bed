// @File: 		math.hpp
// @Project: 	Verlet
// @Author:		Justin Byrne
// @Date:		Created 6/27/22 at 8:47 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef math_hpp
#define math_hpp

#include <time.h>

// x ( t + Δt ) = x ( t ) + v ( t ) Δt + 12a ( t ) Δt2 + 16b ( t ) Δt3+  ( Δt4 )

// x = position
// v = velocity
// a = acceleration
// b = jerk/jolt
// t = time

//void verlet ( float position, float acceleration, time_t delta_time )
//{
//    float  temp_position     = nullptr;
//    float  previous_position = position;
//    
//    time_t time              = nullptr;
//    
//    while ( position > 0 )
//    {
//        time             += delta_time;
//        temp_position     = position;
//        position          = position * 2 - previous_position + accerlation * delta_time * delta_time;
//        previous_position = temp_position;
//    }
//    
//    return time;
//}
//
//struct STORMER_VERLET_RESULT
//{
//    time_t time    = nullptr;
//    float velocity = nullptr;
//}
//
//void stormer_verlet ( float position, float acceleration, time_t delta_time )
//{
//    STORMER_VERLET_RESULT result;
//    
//    float  temp_position     = nullptr;
//    float  velocity          = nullptr;
//    float  previous_position = position;
//    
//    time_t time              = nullptr;
//    
//    while ( position > 0 )
//    {
//        time             += delta_time;
//        temp_position     = position;
//        position          = position * 2 - previous_position + accerlation * delta_time * delta_time;
//        previous_position = temp_position;
//        
//        velocity         += acceleration * delta_time;
//    }
//    
//    return time, velocity;
//}

#endif /* math_hpp */
