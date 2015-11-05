#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>
#include <string>
#include <fstream>

struct singleton
{

  static singleton& get() { return init(); }

private:

  static singleton& init() {
    static singleton obj;
    return obj;
  }

  singleton(const singleton&);              // disable the copy constructor
  singleton& operator=(const singleton&);   // disable assignment
  ~singleton() { }                           // disable destructor 


  // private constructor------------------------------------------------
  singleton()
  {
    std::ifstream in("./include/parameters.dat");

    char dummy[500];

    if(!in) { 
      std::cerr<<__FILE__<<__LINE__<<" cannot open input file "<<std::endl;
      throw;
    }

    in>> dummy >> dummy >> dummy;     			// #1-3
    in >> dummy >>   _MAX_TREE_NUM;    			// #4
    in >> dummy >>   _IMG_SUBSET_SIZE;     		// #5
    in >> dummy >>   _PX_SUBSET_SIZE;			// #6
    in >> dummy >>   _MAX_TREE_DEPTH;			// #7
    in >> dummy >>   _SAMPLE_UV_NUM; 			// #8
    in >> dummy >>   _SAMPLE_THRESHOLD_NUM;    		// #9
    in >> dummy >>   _UV_RANGE;             		// #10
    in >> dummy >>   _BANDWIDTH;            		// #11
    in >> dummy >>   _DELTA;            		// #12
    in >> dummy >>   _MIN_LEAF_SIZE;            	// #13

    in.close();

  }

  // ------------------------------------------
  unsigned int  _MAX_TREE_NUM;
  unsigned int  _IMG_SUBSET_SIZE;
  unsigned int 	_PX_SUBSET_SIZE;
  unsigned int 	_MAX_TREE_DEPTH;
  unsigned int 	_SAMPLE_UV_NUM;
  unsigned int 	_SAMPLE_THRESHOLD_NUM;
  unsigned int 	_UV_RANGE;
  double	_BANDWIDTH;
  double	_DELTA;
  double	_MIN_LEAF_SIZE;

public:
  unsigned int  MAX_TREE_NUM()         	{ return _MAX_TREE_NUM;         }
  unsigned int  IMG_SUBSET_SIZE()       { return _IMG_SUBSET_SIZE; 	}	
  unsigned int 	PX_SUBSET_SIZE()        { return _PX_SUBSET_SIZE;    	}
  unsigned int 	MAX_TREE_DEPTH() 	{ return _MAX_TREE_DEPTH; 	}
  unsigned int  SAMPLE_UV_NUM()		{ return _SAMPLE_UV_NUM; 	}
  unsigned int  SAMPLE_THRESHOLD_NUM()	{ return _SAMPLE_THRESHOLD_NUM; }
  unsigned int 	UV_RANGE()		{ return _UV_RANGE;		}
  double 	BANDWIDTH()		{ return _BANDWIDTH;		}
  double 	DELTA()			{ return _DELTA;		}
  double 	MIN_LEAF_SIZE()		{ return _MIN_LEAF_SIZE;	}

};

#endif
