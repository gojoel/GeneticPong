#ifndef GENALG_H
#define GENALG_H


#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>

#include "src/paddle.h"
#include "src/ball.h"
#include "src/utilities.h"

using namespace std;


const double MUTATION_RATE = 0.1;
const double CROSSOVER_RATE = 0.7;

//-----------------------------------------------------------------------
//
//	create a structure to hold each player
//-----------------------------------------------------------------------
struct Player
{
	vector <double>	vecWeights;

	double          Fitness;


	// similar to constructors for classes
	Player():Fitness(0.0){}
	Player( vector <double> w, double f): vecWeights(w), Fitness(f){}

	//overload < for sorting
	friend bool operator<(const Player& lhs, const Player& rhs)
	{
		return (lhs.Fitness < rhs.Fitness);
	}

};

class GenAlg
{

	private:

		//holds entire population
		vector <Player> Population;

		//max population
		int popSize;

<<<<<<< HEAD
=======
		//probability that a chromosomes weight will mutate
		double MUTATION_RATE;
		double CROSSOVER_RATE;

>>>>>>> 5b946c1af1af525f0bd6074ce18c0de95b0ebbd1
		//amount of weights(size)
		int chromoLength;

		//population Fitness
		double totalFitness;

		//fittest population
		double bestFitness;

		//avg Fitness
		double avgFitness;

		//fittest player
		int fittestPlayer;

		//count each generation
		int countGen;

		int lastDistance;

		//has ball crossed
		bool ballCrossed;

		//current population
		int currentPop;

		//current genome
		int curGenome;
		
		//how many plays have been made in current game
		int plays;

<<<<<<< HEAD
		//current player 
		int curPlayer;

=======



>>>>>>> 5b946c1af1af525f0bd6074ce18c0de95b0ebbd1

		void Mutate(vector<double> &chromo);

		void Crossover(const vector<double> &mother,
									 const vector<double> &father,
									 vector<double> 			&child1,
									 vector<double> 			&child2);

<<<<<<< HEAD
		void FindBest();
=======
		void FindBestWorstAvg();
>>>>>>> 5b946c1af1af525f0bd6074ce18c0de95b0ebbd1

		float Velocity(Paddle *p1, Ball *ball, vector<double> &chromo);

		int CrossDistance(Paddle *p1, Ball *ball);

		Player GARouletteWheel();

		void Elitism(int NBest, const int xCopies, vector<Player> &Pop);

		void Reset();

		void Report();


	public:
<<<<<<< HEAD

		// constructor 
		GenAlg(int popsize, int numWeights);
=======
		// constructor
		GenAlg(int popsize, double mutationRate, double crossRate, int numWeights);
>>>>>>> 5b946c1af1af525f0bd6074ce18c0de95b0ebbd1

		//called when someone has score, -1 if opponent, 1 if player(GA)
		void AssignFitness(int score);

		vector<Player> Populate(vector<Player> &oldPop);

		float CalculateMove(Paddle *p1, Ball *ball);

		void savePopulation();

		void loadPopulation();

};

#endif
