#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "ActorNode.h"
#include "Movie.h"

using namespace std;

// Class used to compare movie years in pq.
class MovieCompare{
  public:
    bool operator()(Movie* n1, Movie* n2){
      return n1->year > n2->year;
    }
};


class UnionFind{
  public:

    // Maps to store nodes before creating union sets
    unordered_map<string, ActorNode*> actorGraph;
    unordered_map<string, Movie*> movieGraph;
    priority_queue<Movie*, vector<Movie*>, MovieCompare> movieQueue; 

 /**** METHODS ****/
    /* find root and make parent of node */
    ActorNode* find(ActorNode* node);

    /* Unites two sets by rank */
    void Union(ActorNode* first, ActorNode* second);

    /* Creates sets and searches for nodes */
    void usearch(string one, string two, ofstream& file);
   
    /* Initializes actorNodes for union-find */
    void initialize();

    /* Resets the pq */
    void reset();


    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
     bool loadFromFile(const char* in_filename);

};


bool UnionFind::loadFromFile(const char* in_filename){
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }
        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

	string year = std::to_string(movie_year);
	string titleYear = movie_title+year;

        // we have an actor/movie relationship, now what?

        // Create a node from actor name, if doesn't already exist.
        auto checker = actorGraph.find(actor_name);
        if (checker == actorGraph.end()){
          ActorNode *actor = new ActorNode(actor_name);
 	  actorGraph[actor_name] = actor;
	 // cout << "Adding actor node to graph" << endl;
        }

	// Adding to actor's movie list
        ActorNode* tmp = actorGraph.at(actor_name);
	tmp->movieList.push_back(titleYear);

	// Create a movie node, if doesn't already exist.
	auto checker2 = movieGraph.find(titleYear);
	if (checker2 == movieGraph.end()){
	  Movie *movie = new Movie(movie_title, movie_year);
	  movieGraph[titleYear] = movie;
	 // cout << "adding Movie node to graph" << endl;
	}
        Movie* tmp2 = movieGraph.at(titleYear);
	tmp2->actorList.push_back(actor_name);	
    }


    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }

    infile.close();

    return true;
}



/* Initialize method */
void UnionFind::initialize(){
  for (auto iter = actorGraph.begin(); iter != actorGraph.end(); iter++){
    ActorNode *tmp = (*iter).second;
    tmp->prev = tmp;
  }
  // Pushing movies into priority queue
  for (auto iter = movieGraph.begin(); iter != movieGraph.end(); iter++){
    movieQueue.push((*iter).second);
  }
}

/* find method */
ActorNode* UnionFind::find(ActorNode *node){
  // If node is the root, return itself
  if (node->prev == node){
    return node;
  }
  // else return its parent
  return find(node->prev);
}



/* Union method */
void UnionFind::Union(ActorNode* first, ActorNode* second){
  ActorNode* firstParent = find(first);
  ActorNode* secondParent = find(second);
  firstParent->prev = secondParent;
}


/* reset function */
void UnionFind::reset(){
  while (!movieQueue.empty()){
    movieQueue.pop();
  }
}


/* the search function */
void UnionFind::usearch(string one, string two, ofstream& file){
  // Initializes the actorNodes.
  initialize();
  // Creates pointers for start and end actors
  ActorNode *originalActor = actorGraph.at(one);
  ActorNode *finalActor = actorGraph.at(two);

  // While loop to go through movies pq
  while (!movieQueue.empty()){
    Movie *movie = movieQueue.top();
    movieQueue.pop();
    
    // Gets the first actor in the movie list
    ActorNode *actor = actorGraph.at(movie->actorList[0]);
    // For loop to union all actors in movie to first actor
    for (int index = 1; index < movie->actorList.size(); index++){
      ActorNode *tmp = actorGraph.at(movie->actorList[index]);
      Union(actor, tmp);
    }
    // Check to see if two nodes point to same sentinal node, are in same set
    ActorNode *first = find(originalActor);
    ActorNode *last = find(finalActor);
    if (first->actorName == last->actorName){
      // Print out and call reset and return;
      file << originalActor->actorName << "\t" <<
	 finalActor->actorName << "\t" << movie->year << endl;
      reset();
      return;
    }
  }
}
