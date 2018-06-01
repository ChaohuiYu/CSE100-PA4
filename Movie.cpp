#include "Movie.h"
#include <string>
#include <list>
#include <iostream>


/* Constructor */
Movie::Movie(std::string name, int yearMade){
  movieName = name;
  year = yearMade;
  weight = 1 + (2015 - year);
}

/* Destructor */
Movie::~Movie(){

}
