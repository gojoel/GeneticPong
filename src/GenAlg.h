#ifndef GENALG_H
#define GENALG_H


#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "src/paddle.h"
#include "src/ball.h"
#include "src/utilities.h"

using namespace std;

//-----------------------------------------------------------------------
//
//	create a structure to hold each genome
//-----------------------------------------------------------------------
struct Genome
{
	vector <double>	vecWeights;

	double          Fitness;


	// similar to constructors for classes
	Genome():Fitness(0.0){}
	Genome( vector <double> w, double f): vecWeights(w), Fitness(f){}

	//overload < for sorting
	friend bool operator<(const Genome& lhs, const Genome& rhs)
	{
		return (lhs.Fitness < rhs.Fitness);
	}

};

class GenAlg
{

	private:
		//holds entire population
		vector <Genome> vecPop;

		//max population
		int popSize;

		//current population
		int currentPop;

		//amount of weights(size)
		int chromoLength;

		//population Fitness
		double totalFitness;

		//fittest population
		double bestFitness;

		//avg Fitness
		double avgFitness;

		//worst Fitness
		double worstFitness;

		//fittest genome
		int fittestGenome;

		//count each generation
		int countGen;

		//has ball crossed
		bool hasCrossed;

		int lastDistance;

		//how many plays have been made in current game
		int plays;

		//current genome
		int curGenome;

		//probability that a chromosomes weight will mutate
		double MUTATION_RATE;
		double CROSSOVER_RATE;

		void Mutate(vector<double> &chromo);

		void Crossover(const vector<double> &mother,
									 const vector<double> &father,
									 vector<double> 			&child1,
									 vector<double> 			&child2);

		void FindBestWorstAvg();


		float GetVelocity(Paddle *p1, Paddle *p2, Ball *ball, vector<double> &chromo);

		int GetCrossDistance(Paddle *p1, Ball *ball);

		Genome GARouletteWheel();

		void MostElite(int NBest, const int xCopies, vector<Genome> &Pop);

		void Reset();

	public:
		// constructor
		GenAlg(int popsize, double mutationRate, double crossRate, int numWeights);

		//called when someone has score, -1 if opponent, 1 if player(GA)
		void AssignFitness(int score);

		vector<Genome> Populate(vector<Genome> &oldPop);

		float CalculateMove(Paddle *p1, Paddle *p2, Ball *ball);

};

#endif
