/*
 * ActorGraph.cpp
 * Author: Jaime Velazquez, Nha Do
 * Date:   05/28/18
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "Movie.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
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




/*
 * Search method to traverse graph and find path between actors
 */ 
void ActorGraph::search(string one, string two, ofstream& outfile, string version, bool connections){
  queue<ActorNode*> queue; // Used for BFS
  // Used for pathfinder
  priority_queue<ActorNode*, vector<ActorNode*>, CompareNodes> pq;
  // Used for actorconnections
  priority_queue<ActorNode*, vector<ActorNode*>, CompareNodesConnection> pqc; 
  stack<ActorNode*> stack; // Used for returning path
  bool check = false;
  ActorNode *actor;
  
  // Gets starting actor node
  ActorNode *originalActor = actorGraph.at(one);
  originalActor->bandwidth = 2147483647;
  
  // Unweighted version
  if (version == "u"){
    queue.push(originalActor);
  }
  else if (version == "w" && connections == false){
    pq.push(originalActor);
  }
  else if (version == "w" && connections == true){
    cout << "Inside pqc" << endl;
    pqc.push(originalActor);
  }
 
  //cout << "Size of queue: " << queue.size() << endl;
  //cout << "Size of pq: " << pq.size() << endl;
  while (!queue.empty() || !pq.empty() || !pqc.empty()){
    // If unweighted
    if (version == "u"){
      cout << "actor = queue.front in unweighted" << endl;
      actor = queue.front();
      queue.pop();
      if (actor->actorName == two){
        stack.push(actor);
        break;
      }
    }

    // If weighted
    else if (version == "w" && connections == false){
      cout << "actor = pq.top(), weighted" << endl;
      actor = pq.top();
	//cout << "actor: " << actor->actorName << endl;
      pq.pop();
    }

    // If weighted and actorconnections
    else if (version == "w" && connections == true){
	cout << "actor = pqc.top(), weighted, true" << endl;
      actor = pqc.top();
      pqc.pop();
    }

    // Goes through each movie in the actors list, and through each actor(edge)
    for (int index = 0; index < actor->movieList.size(); index++){
      string movieName = actor->movieList.at(index);
	//cout << "movieName of actor: " << movieName << endl;
      Movie *movie = movieGraph.at(movieName);
    
      // For each movie, go through each actor and set their
      // prev Node to the original actor and add them to queue
      for (int index2 = 0; index2 < movie->actorList.size(); index2++){
        string actorsName = movie->actorList.at(index2);
	//cout << "actors name: " << actorsName << " in movie: " << movieName << endl;
        ActorNode *tmpActor = actorGraph.at(actorsName);

	// If weighted 
	if (movie->weight + actor->totalDist < tmpActor->totalDist && actorsName != actor->actorName && version == "w" && connections == false){
	  //cout << "if movie weight < actor prevWeight" << endl;
	  tmpActor->prev = actor;
	  tmpActor->prevMovie = movieName;
	  tmpActor->prevMovieWeight = movie->weight;
          tmpActor->totalDist = movie->weight + actor->totalDist;
	  pq.push(tmpActor);
        } 
	//cout << "actor->bandwidth = " << actor->bandwidth << endl;
	//cout << "movie->weight = " << movie->weight << endl;
	//cout << "tmpActor->bandwidth = " << tmpActor->bandwidth << endl;
	//cout << "min(actor, weight) = " << std::min(actor->bandwidth,movie->weight) << endl;
	// If weighted but looking for widest path
	if (std::min(actor->bandwidth,movie->weight) > tmpActor->bandwidth && version == "w" && connections == true){
	cout << "yoo" << endl;
	  tmpActor->prev = actor;
	  tmpActor->prevMovie = movieName;
	  tmpActor->prevMovieWeight = movie->weight;
	  tmpActor->bandwidth = std::min(actor->bandwidth,movie->weight);
	  pqc.push(tmpActor);
        }
	// If unweighted
        else if (tmpActor->prev == nullptr && version == "u"){
	  //cout << "if tmpActor prev == nullptr in unweighted" << endl;
          tmpActor->prev = actor;
          tmpActor->prevMovie = movieName;
          queue.push(tmpActor);
        }
      }
    }
  }


  if (version == "w" && connections == false){
    ActorNode* end = actorGraph.at(two);
    stack.push(end);
  }
  else if (version == "w" && connections == true){
    cout << "going out" << endl;
    ActorNode* newEnd = actorGraph.at(two);
     cout << "movie: " << newEnd->prevMovie << endl; 
    Movie *lastMovie = movieGraph.at(newEnd->prevMovie);
    outfile << originalActor->actorName << "\t" <<
	 newEnd->actorName << "\t" << lastMovie->year << endl;
    return;
  }
  cout << "out of while loop, going to stack" << endl;
  // Creating path 
  ActorNode *iter = stack.top();
  while (iter->actorName != originalActor->actorName){
    cout << "Pushing nodes into the stack" << endl;
    iter = iter->prev;  // Sets iter to prev node;
    stack.push(iter);
  }
  cout << "Starting to print out path" << endl;
  // Printing out path.
  iter = stack.top();
  outfile << "(";
  outfile << iter->actorName;
  outfile << ")"; 
  stack.pop();

  while (!stack.empty()){
    cout << "last while loop" << endl;
    iter = stack.top();
    string movieName = iter->prevMovie;
    Movie *movie = movieGraph.at(movieName);
    outfile << "--[" << movie->movieName << "#@" << movie->year << "]-->(" << iter->actorName << ")";
    stack.pop();
  }
  outfile << "\n";
  // Reseting pointers
  cout << "before reset" << endl;
  reset();
  cout << "after reset" << endl;

}  



/* Reset function to reset ActorNode pointers */
void ActorGraph::reset(){
  for (auto iter = actorGraph.begin(); iter != actorGraph.end(); iter++){
    ActorNode *tmp = (*iter).second;
    tmp->prev = nullptr;
    tmp->prevMovie = "";
    tmp->prevMovieWeight = 2147483647;
    tmp->totalDist = 2147483647;
    tmp->bandwidth = 0;
  }
}
