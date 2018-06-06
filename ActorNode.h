#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
class ActorNode{
  public: 
    string actorName;  // Name of the actor 
    bool check; // Checks to see if node was traversed.
    ActorNode *prev; // Pointer to the previously connecting node
    string prevMovie; // Name of the prev Move
    int prevMovieWeight; // Weight of the prev movie
    int totalDist; // Total weighted path 
    int bandwidth; // Bandwidth of node 
    vector<string> movieList; // Vector of movies actor is in.
    
    /* Constructor */
    ActorNode(std::string name);

    /* Destructor */
    ~ActorNode();
  
    
    

};
#endif // ACTORNODE_H
