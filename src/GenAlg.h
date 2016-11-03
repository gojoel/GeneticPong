#ifndef GENALG_H
#define GENALG_H


#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "src/paddle.h"
#include "src/ball.h"
#include "src/utilities.h"
#include "src/pong.h"



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
	Genome():dFitness(0.0){}
	Genome( vector <double> w, double f): vecWeights(w), dFitness(f){}

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

	//probability that a chromosomes weight will mutate
	double MUTATION_RATE = 0.1;
	double CROSSOVER_RATE = 0.7;

	void Mutate(vector<float> &chromo);

	void Crossover(const vector<double> &mother,
								 const vector<double> &father,
								 vector<double> 			&child);

	void FindBestWorseAvg();

	float CalculateMove(Paddle *p1, Paddle *p2, Ball *ball);

	float getyVelocity(Paddle *p1, Paddle *p2, Ball *ball, vector<double> &chromo);

	int GetCrossDistance(Paddle *p1, Ball *ball);

}


#endif
