/* 

Generic Map File 

This file is fully commented and can serve as a basis for more complicated map.
For illustration, we implement the Leslie Map.

*/

#ifndef MODELMAP_H
#define MODELMAP_H

#include "chomp/Rect.h"
#include <vector>

#ifdef USE_BOOST_INTERVAL
#include "database/numerics/boost_interval.h"
#endif

#ifdef USE_CAPD
#undef None
#include "capd/capdlib.h"
using namespace capd;
#endif

#ifndef USE_BOOST_INTERVAL
#ifndef USE_CAPD
#include "database/numerics/simple_interval.h"
#endif
#endif


struct ModelMap {
  typedef chomp::Rect Rect;
#ifdef USE_CAPD
typedef capd::intervals::Interval<double> interval;
#endif  
#ifndef USE_BOOST_INTERVAL
#ifndef USE_CAPD
  typedef simple_interval<double> interval;
#endif
#endif  
  std::vector < interval > parameter;

  // constructor
  ModelMap ( const Rect & rectangle ) {
    parameter . resize ( rectangle . dimension ( ) );
    for ( unsigned int i=0; i<rectangle.dimension(); ++i ) 
      parameter [ i ] = interval (rectangle . lower_bounds [ i ], rectangle . upper_bounds [ i ]);
    return;
  }

  Rect operator () 
    ( const Rect & rectangle ) const {    

    std::vector < interval > x; 
    std::vector < interval > y;
    
    x . resize ( rectangle . dimension ( ) );
    y . resize ( rectangle . dimension ( ) );

    Rect return_value ( rectangle . dimension ( ) );
    
    for ( unsigned int i=0; i<rectangle.dimension(); ++i ) 
      x [ i ] = interval (rectangle . lower_bounds [ i ], rectangle . upper_bounds [ i ]);

    /********************************************************************* 
      Define the map in terms of the phase space variables and parameters. 
    *********************************************************************/
    
    y [ 0 ] = ( parameter[0] * x[0] + parameter[1] * x[1] ) * exp( -0.1 * ( x[0] + x[1] ) );

    y [ 1 ] = 0.7 * x [0]; 
    
    /*********************************************************************  
    *********************************************************************/

#ifdef USE_BOOST_INTERVAL
    for ( unsigned int i=0; i<rectangle.dimension(); ++i ) {
      return_value . lower_bounds [ i ] = y [ i ] . lower ( );
      return_value . upper_bounds [ i ] = y [ i ] . upper ( );
    }
#endif

#ifdef USE_CAPD
    for ( unsigned int i=0; i<rectangle.dimension(); ++i ) {
      return_value . lower_bounds [ i ] = y [ i ] . leftBound ( );
      return_value . upper_bounds [ i ] = y [ i ] . rightBound ( );
    }
#endif

    return return_value;
  } 
  bool good ( void ) const {
    return true;
  }
  
};

#endif
