/*
 * ActorGraph.h
 * Author: Jaime Velazquez, Nha Do
 * Date:   05/28/18
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "ActorNode.h"
#include "Movie.h"
#include <string>

// Maybe include some data structures here

using namespace std;

/* Used for weighted graph */
class CompareNodes{
  public:
    bool operator()(ActorNode* n1, ActorNode* n2){
      return n1->prevMovieWeight > n2->prevMovieWeight;
    }
};


/* Used for actorconnections */
class CompareNodesConnection{
  public:
    bool operator()(ActorNode* n1, ActorNode* n2){
      return n1->bandwidth < n2->bandwidth;
    }
};


class ActorGraph {
    public:

        // Maybe add class data structure(s) here
        unordered_map<string, ActorNode*> actorGraph;
	unordered_map<string, Movie*> movieGraph;
        
        ActorGraph(void);

	/* Fills adjecency list */
	void search(string one, string two, ofstream& outfile, string version, bool connections);

	/* Resets node pointers */
        void reset();  


        // Maybe add some more methods here

        /** You can modify this method definition as you wish
         *
         * Load the graph from a tab-delimited file of actor->movie relationships.
         *
         * in_filename - input filename
         * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
         *
         * return true if file was loaded sucessfully, false otherwise
         */
        bool loadFromFile(const char* in_filename, bool use_weighted_edges);

};


#endif // ACTORGRAPH_H
