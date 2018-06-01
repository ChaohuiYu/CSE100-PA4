#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Movie;
class ActorNode{
  public: 
    string actorName;  // Name of the actor 
    bool check; // Checks to see if node was traversed.
    ActorNode *prev;
    string prevMovie; 
    vector<string> movieList; // Vector of movies actor is in.
    
    /* Constructor */
    ActorNode(std::string name);

    /* Destructor */
    ~ActorNode();
  
    
    

};
#endif // ACTORNODE_H
