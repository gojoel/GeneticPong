
/*-----------------------------------------GenAlg.cpp-----------------------------------
*
* Class to create and train a genetic program to play pong  
* 
* Initializes a population of Players
*	- Each Player contains vector of weights that encodes its behavior
* 	- Each Player also contains a recorded fitness 
* 	- Each Player within a population is played, and fitness is assigned accordingly
* 	- Thereafter, a new population is generated as a genetic modification of the last
*
* Fitness is determined as following:
*	1. Genetic player scores
*	2. Distance missed from ball
*	3. How many plays made in current game 

* Re-population:
*	- Find the most/least fit player, and avg fitness of population 
*	- Sort population from least to most fit
*	- Add 'n' number of the most fit players from last pop to new
* 		- Adding 'x' copies of each
*  	- Use Roulette wheel sampling to choose parents
* 		- Create children using crossover and mutation
* 		- Add to new population
* 		- Repeat genetic algorithm 
*
*	by Joel Goncalves, Bruna, Mohammad, Rachel
--------------------------------------------------------------------------------------
*/

#include "src/GenAlg.h"
#include "src/pong.h"
#include <float.h>

//-----------------------------------Constructor-------------------------
//	create a population with random weights
//-----------------------------------------------------------------------

GenAlg::GenAlg(int popsize, int numWeights) : popSize(popsize), chromoLength(numWeights), totalFitness(0),
																						bestFitness(0),
																						avgFitness(0),
																						worstFitness(0),
																						fittestPlayer(0),
																						countGen(0),
																						lastDistance(0),
																						ballCrossed(false),
																						curPlayer(0),
																						plays(0)

{

	ifstream saved ("generations.txt");

	if (saved.peek() != std::ifstream::traits_type::eof()){
		loadPopulation();
	} 

	else {
		// Loop and create players
		for (int i = 0; i < popSize; ++i){
			Population.push_back(Player());

			//random weights
			for (int j = 0; j < chromoLength; ++j){
				Population[i].vecWeights.push_back(RandomClamped());
			}
		}
	}

	countGen++;
}

//-----------------------CalculateMove()-----------------------	
//
//	calculates how fast the paddle should move 
// 	called from pong.cc for each move of the GA paddle 
//---------------------------------------------------------------------

float GenAlg::CalculateMove(Paddle *p1, Ball *ball){

	plays++; 

	if (!ballCrossed)
		lastDistance = CrossDistance(p1, ball);

	float move = Velocity(p1, ball, Population[curPlayer].vecWeights);

	return move;

}

//-----------------------CrossDistance()-----------------------	
//
//  checks if paddle has missed ball 
//	calculates the distance paddle missed 
// 	used to assign fitness scores
//---------------------------------------------------------------------

// returns the distance paddle missed -- used to assign fitness 
 int GenAlg::CrossDistance(Paddle *p1, Ball *ball){

	 int distance = 0;
	 //ball has passed paddle from either side of screen
	 if ( (p1->x >= (int)(Pong::SCREEN_WIDTH/2) && ball->x > p1->x + 1) ||
	 			(p1->x < (int)(Pong::SCREEN_WIDTH/2) && ball->x < p1->x - 1 ))
				{
					ballCrossed = true;

					if (ball->y > p1->y)
						distance = (int) (ball->y - (p1->y  + p1->HEIGHT));
					else
						distance = (int) (p1->y - (ball->y + ball->LENGTH));
				}

		return distance;
 }

//---------------------------------Velocity()--------------------------------
//	return the velocity for paddle
//-----------------------------------------------------------------------

float GenAlg::Velocity(Paddle *p1, Ball *ball, vector<double> &chromo)
{
	// find best weights to calculate speed to move and position
	double velocity = chromo[2] * ball->dy + chromo[0] * ball->y + chromo[1] * p1->y;

	return velocity;
}

//----------------------------------AssignFitness()------------------
//
//	evaluate player fitness in population 
//  called from pong.cc each time a player has score 
//	used to assign fitness
//-----------------------------------------------------------------------

 void GenAlg::AssignFitness(int score){

 	//current fitness to be assigned
 	double curFitness = 0;

 	//GA scored
 	if (score == 1) {
 		curFitness = 160.0;
 	}

 	//how far missed
 	curFitness -= lastDistance;
 	ballCrossed = false;

	//how long game has been in play
 	curFitness /= plays;

 	if (Population[curPlayer].Fitness != 0) {
            Population[curPlayer].Fitness += curFitness;
            Population[curPlayer].Fitness /= 2;
        }
    else
        Population[curPlayer].Fitness += curFitness;

    plays = 0;
    ++curPlayer;

    if (curPlayer >= Population.size()){
		NormalizeFit(Population);
    	Population = Populate(Population);
    	curPlayer = 0; 
    }
 }
/*
  void GenAlg::AssignFitness(int score){

 	//current fitness to be assigned
 	double curFitness = 0;

 	//GA scored
 	if (score == 1) {
 		curFitness = 999.0;
 	}

 	//how long game has been in play
 	curFitness += plays;
 	//how far missed
 	curFitness += 480 - 3 * lastDistance;
 	ballCrossed = false;

 	if (Population[curPlayer].Fitness != 0) {
            Population[curPlayer].Fitness += curFitness;
            Population[curPlayer].Fitness /= 2;
        }
    else
        Population[curPlayer].Fitness += curFitness;

    plays = 0;
    ++curPlayer;

    if (curPlayer >= Population.size()){
    	Population = Populate(Population);
    	curPlayer = 0;
    }
 } */
 
 
//----------------------------------NormalizeFit()------------------
//
//	no negative fitnesses
//-----------------------------------------------------------------------

void GenAlg::NormalizeFit(vector<Player> &pop){
	Population = pop;
	double lowest = DBL_MAX;
	for (Player p : Population){
		if (p.Fitness < lowest){
			lowest = p.Fitness;
		}
	}
	for (Player p : Population){
		p.Fitness -= (lowest-1);
	}
}

 //-----------------------------------Populate()-----------------------------
//
//	takes a population and runs the GA algorithm once 
//	Returns a new population
//-----------------------------------------------------------------------

vector<Player> GenAlg::Populate(vector<Player> &oldPop){

	Population = oldPop;

	Reset();

	//sort by fitness 
	sort(Population.begin(), Population.end());

	//calculate best, average and total fitness
	FindBest();

	//temporary vector to store new chromosones
	vector<Player> newPop;

	//add elitism
	Elitism(4, 1, newPop);


	// Loop over the population size and create new players 
	while (newPop.size() < popSize){

		Player mother = GARouletteWheel();
		Player father = GARouletteWheel();

		vector<double> child1, child2;

		// Crossover population
		Crossover(mother.vecWeights, father.vecWeights, child1, child2);

		// Mutate population
		Mutate(child1);
		Mutate(child2);

		newPop.push_back(Player(child1, 0));
		newPop.push_back(Player(child2, 0));

	}

		//finished so assign new pop back into population
		Population = newPop;

		countGen++;

		Report();

		return Population;

}


//-----------------------FindBest()-----------------------	
//
//	calculates the fittest and weakest player and the average/total 
//	fitness scores
//---------------------------------------------------------------------
void GenAlg::FindBest(){


	// Find the total fitness of the population.
//	totalFitness = 0;

	double highest = DBL_MAX * -1;
	double lowest = DBL_MAX;

	for (int i = 0; i < popSize; ++i){

		if (Population[i].Fitness > highest){
			highest = Population[i].Fitness;
			fittestPlayer = i;
			bestFitness = highest;
		}

		if (Population[i].Fitness < lowest){
			lowest = Population[i].Fitness;
			worstFitness = lowest;
		}

		totalFitness += Population[i].Fitness;

	}
		avgFitness = totalFitness / popSize;

	}

//-------------------------Elitism()----------------------------------
//	copy a small propotion(xCopies) of the fittest candidates(NBest), unchanged, into the next generation
//--------------------------------------------------------------------
void GenAlg::Elitism(int nBest, const int xCopies, vector<Player> &Pop)
{

	//to the supplied vector
	while(nBest--)
	{
		for (int i=0; i < xCopies; ++i)
		{
			Pop.push_back(Population[(popSize - 1) - nBest]);
	  }
	}
}


 //----------------------------------GARouletteWheel()------------------
//
//	Fitness-proportionate selection
//  The fittest candidates more likely to be chosen than weaker players
// 	Used to select parents for next generation (crossover)
//-----------------------------------------------------------------------

Player GenAlg::GARouletteWheel(){
	double range = bestFitness - worstFitness; 
	double randomFitness = (double)(Random() * totalFitness);

	Player chosenOne;

	double fitness_sum = 0;

	//  Calculate the cumulative fitness
	for (int i = 0; i < popSize; ++i)
	{
		fitness_sum += Population[i].Fitness;
		
		//if the fitness so far > random number return the chromo at 
		//this point
		if (fitness_sum >= randomFitness)
		{
			chosenOne = Population[i];
      		break;
		}	
	}

	return chosenOne;
}

//-------------------------------------Crossover()-----------------------
//	sub-tree crossover pg. 15
//  given parents and storage for the child
//	performs crossover according to the GAs crossover rate
//-----------------------------------------------------------------------

void GenAlg::Crossover(const vector<double> &mother,
											const vector<double> &father,
											vector<double> 			&child1,
											vector<double>			&child2)
{

	if ( Random() < CROSSOVER_RATE){
		//choosing a cross-over point
		int crossp = RandInt(0, chromoLength - 1);

		//first half from parent
		for (int i = 0; i < crossp; ++i){
			child1.push_back(mother[i]);
			child2.push_back(father[i]);
		}

		//second half from other parent
		for (int i = crossp; i < mother.size(); i++){
			child1.push_back(father[i]);
			child2.push_back(mother[i]);
		}
	}
	
	// just return parents
	else {

		child1 = father;
		child2 = mother;
	}

}


//---------------------------------Mutate()--------------------------------
// point-mutation pg.16
//	mutates a chromosome by modifying its weights
// 	dependent on the MUTATION_RATE
//-----------------------------------------------------------------------

void GenAlg::Mutate(vector<double> &chromo)
// mutation 0.1
{

	for (int i = 0; i<chromo.size(); ++i)
	{

		if (Random() < MUTATION_RATE)
		{
			//add or subtract a small value to the weight
			// 0.3 recommended 
			chromo[i] += (double)(RandomClamped() * 0.3);
		}
	}
}


//-------------------------Reset()------------------------------
//	resets all the relevant variables ready for a new generation
//--------------------------------------------------------------

void GenAlg::Reset(){
	totalFitness = 0;
	bestFitness = 0;
	avgFitness = 0;
	fittestPlayer = 0;

	//edit
	lastDistance = 0;
}


//-------------------------savePopulation()------------------------------
//	save the latest population to a txt file, generations.txt
//	used to track the most trained population
//--------------------------------------------------------------

void GenAlg::savePopulation(){

	vector<Player> saveCopy = Population;

	ofstream save ("generations.txt");

	if (save.is_open()){

		for (int i = 0; i < saveCopy.size(); i++){
			for (int j = 0; j < saveCopy[i].vecWeights.size(); j++){
				save << saveCopy[i].vecWeights[j] << "\t";
			}
			//save << saveCopy[i].Fitness; 
			save << endl;
		}

		save.flush();
		cout << "Generation written to file!" << endl;
	}
	else
		cout << "Unable to open file" << endl;

}



//-------------------------loadPopulation()------------------------------
//	load the last population from txt file, generations.txt
//--------------------------------------------------------------

void GenAlg::loadPopulation(){

	vector<double> dataVec;
	int counter = 0; 
	double data;

	ifstream saved ("generations.txt");

	if (saved.is_open()){

		cout << "Generation loaded from file!" << endl;

			while (saved >> data)
				dataVec.push_back(data);

		saved.close();

	}
	else
		cout << "Unable to open file" << endl;


	for (int i = 0; i < popSize; ++i){
		Population.push_back(Player());

			//load data from file
		for (int j = 0; j < chromoLength; ++j){
			Population[i].vecWeights.push_back(dataVec[counter]);
			counter++;
		}
	}

	//cout << "DEBUG: loadPopulation counter = " + to_string(counter) << endl;
}	



//-------------------------Report()------------------------------
//	output game data
//--------------------------------------------------------------


void GenAlg::Report(){
	cout << "Generation " + to_string(countGen) + ": Average Fitness = " + 
				to_string(avgFitness) + ", Best Fitness = " + to_string(bestFitness) << endl;


}

