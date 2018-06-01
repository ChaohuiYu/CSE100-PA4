#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <string>
#include <vector>
#include "ActorNode.h"

using namespace std;

class Movie{
  public:
    string movieName; // Name of movie
    int year; // Year movies was made
    int weight; // Weight of the movie (edge weight)
    vector<string> actorList; // Vector of actors in the movie.

    /* Constructor */
    Movie(string name, int yearMade);
   
    /* Destructor */
    ~Movie();

    

};

#endif // MOVIE_H
