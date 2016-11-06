
//-----------------------------------------GenAlg.cpp--------------------------------------
//
//	Class to create and train genetic a genetic to play pong 
//  
//
//	by Joel Goncalves, Bruna, Mohammad, Rachel
//
//-----------------------------------------------------------------------------------------------

#include "src/GenAlg.h"



//-----------------------------------Constructor-------------------------
//	sets up the population with random weights
//-----------------------------------------------------------------------
GenAlg:GenAlg(int popsize, double mutationRate, double crossRate,
							int numWeights) : popSize(popsize), MUTATION_RATE(mutationRate),
																									CROSSOVER_RATE(crossRate),
																									chromoLength(numWeights),
																									totalFitness(0),
																									bestFitness(0),
																									avgFitness(0),
																									worstFitness(99999999),
																									fittestGenome(0),
																									countGen(0),
																									lastDistance(0),
																									hasCrossed(false),
																									currentPop(0),
																									curGenome(0),
																									plays(0)

//init random population, fitness 0
{
	for (int i = 0; i < popSize; ++i){
		vecPop.push_back(Genome());

		//random weights
		for (int j = 0; j < chromoLength; ++i){
			vecPop[i].vecWeights.push_back(RandomClamped());
		}
	}
}



//-------------------------------------Crossover()-----------------------
//	sub-tree crossover pg. 15
//  given parents and storage for the child
//	performs crossover according to the GAs crossover rate
//-----------------------------------------------------------------------

void GenAlg:Crossover(const vector<double> &mother,
											const vector<double> &father,
											vector<double> 			&child1
											vector<double>			&child2)
{


	//choosing a cross-over point
	int crossp = RandInt(0, chromoLength - 1);

	//first half from parent
	for (int i = 0; i < crossp; ++i){
		child1.push_back(mother[i]);
		child2.push_back(father[i]);
	}

	//second half from other parent
	for (i = crossp; i < mother.size(); i++){
		child1.push_back(father[i]);
		child2.push_back(mother[i]);
	}

}


//---------------------------------Mutate--------------------------------
// point-mutation pg.16
//	mutates a chromosome by modifying its weights
// 	dependent on the MUTATION_RATE
//-----------------------------------------------------------------------

void GenAlg::Mutate(vector<double> &chromo)
{

	for (int i=0; i<chromo.size(); ++i)
	{

		if (RandFloat() < MUTATION_RATE)
		{
			//add or subtract a small value to the weight
			chromo[i] += (RandomClamped() * 0.3);
		}
	}
}


//---------------------------------GetVelocity--------------------------------
//	find the x velocity and y velocity for paddle
//-----------------------------------------------------------------------

//get y velocity
float GenAlg::GetVelocity(Paddle *p1, Paddle *p2, Ball *ball, vector<double> &chromo)
{
	return chromo[5] * ball->dy + chromo[2] * b->y + chromo[3] * p1->y;
}



//-----------------------FindBestWroseAvg-----------------------	
//
//	calculates the fittest and weakest genome and the average/total 
//	fitness scores
//---------------------------------------------------------------------
void GenAlg::FindBestWorseAvg(){
	totalFitness = 0;

	double highest = 0;
	double lowest = 99999999;

	for (int i = 0; i < popSize; ++i){

		if (vecPop[i]).Fitness > highest){
			highest = vecPop[i]).Fitness;
			fittestGenome = i;
			bestFitness = highest;
		}

		if (vecPop[i]).Fitness < lowest){
			lowest = vecPop[i]).Fitness;
			worstFitness = lowest;
		}

		totalFitness += vecPop[i].Fitness;

	}
		avgFitness = totalFitness / popSize;

	}

//-----------------------CalculateBestWorstAvTot-----------------------	
//
//	calculates how fast the paddle should move 
//---------------------------------------------------------------------

float GenAlg::CalculateMove(Paddle *p1, Paddle *p2, Ball *ball){

	//ball not moving
	if (ball->dx == 0 && ball->dy == 0){
		ball->reset();
		return null;
	}

	plays++; 

	if (!hasCrossed)
		lastDistance = getCrossDistance(p1, ball);

	float move = getyVelocity(p1, p2, ball, vecPop[curGenome].vecWeights);

	return move;

}

//-----------------------CalculateBestWorstAvTot-----------------------	
//
//	calculates the distance paddle missed 
// 	used to assign fitness scores
//---------------------------------------------------------------------

// returns the distance paddle missed -- used to assign fitness 
 int GenAlg::GetCrossDistance(Paddle *p1, Ball *ball){

	 int distance = 0;
	 //ball has passed paddle from either side of screen
	 if ( (p1->x >= (int)(Pong::SCREEN_WIDTH/2) && ball->x > p1->x + 1) ||
	 			(p1->x < (int)(Pong::SCREEN_WIDTH/2) && ball->x < p1->x - 1 ))
				{
					hasCrossed = true;

					if (ball->y > p1->y)
						distance = (int) (ball->y - (paddle->y  + paddle->HEIGHT);
					else
						distance = (int) (paddle->y - (ball->y + ball->LENGTH));
				}

		return distance;
 }


 //----------------------------------AssignFitness()------------------
//
//	evaluate ball fitness and store into genome in population 
//-----------------------------------------------------------------------

 void GenAlg::AssignFitness(int score){

 	//current fitness to be assigned
 	double curFitness = 0;

 	//GA scored
 	if (score == 1) {
 		curFitness = 999.0;
 	}

 	//how long game has been in play
 	curFitness += plays;
 	curFitness += 480 - 3 * lastDistance;
 	hasCrossed = false;

 	if (vecPop[curGenome].Fitness != 0) {
            vecPop[curGenome].Fitness += curFitness;
            vecPop[curGenome].Fitness /= 2;
        }
    else
        vecPop[curGenome].Fitness += curFitness;

    plays = 0;
    ++curGenome;

    if (curGenome > vecPop.size()){
    	vecPop = Epoch(vecPop);
    	curGenome = 0; 
    }
 }


 //----------------------------------GARouletteWheel()------------------
//
//	returns a chromo based on roulette wheel sampling
//
//-----------------------------------------------------------------------

Genome GenAlg::GARouletteWheel(){

	double randomFitness = (double)(RandFloat() * totalFitness);

	Genome chosenOne;

	double foundFitness = 0;

	for (int i = 0; i < popSize; ++i)
	{
		foundFitness += vecPop[i].Fitness;
		
		//if the fitness so far > random number return the chromo at 
		//this point
		if (foundFitness >= randomFitness)
		{
			chosenOne = vecPop[i];
      		break;
		}	
	}

	return chosenOne;
}


//-----------------------------------Epoch()-----------------------------
//
//	takes a population and runs the GA algorithm once 
//	Returns a new population
//-----------------------------------------------------------------------

vector<Genome> GenAlg::Epoch(vector<Genome> &oldPop){

	vecPop = oldPop;

	//sort by fitness 
	sort(vecPop.begin(), vecPop.end());

	//calculate best, worst, average and total fitness
	FindBestWorseAvg();

	//temporary vector to store new chromosones
	vector <Genome> vecNewPop;

	//add elitism
	GrabNBest(4, 1, vecNewPop);

	while (vecNewPop.size() < popSize){

		Genome mother = GARouletteWheel();
		Genome father = GARouletteWheel();

		//create some offspring via crossover
		vector<double> child1, child2;

		Crossover(mother.vecWeights, father.vecWeights, child1, child2);

		Mutate(child1);
		Mutate(chil2);

		vecNewPop.push_back(Genome(child1, 0));
		vecNewPop.push_back(Genome(child2, 0));

		//finished so assign new pop back into m_vecPop
		vecPop = vecNewPop;

		return vecPop;

	}

}

//-------------------------MostElite----------------------------------
//
//	elitism by inserting xCopies
//  copies of the NBest most fittest genomes into a population vector
//--------------------------------------------------------------------
void CGenAlg::MostElite(int NBest, const int xCopies, vector<SGenome> &Pop)
{
  //add the required amount of copies of the n most fittest

	//to the supplied vector
	while(NBest--)
	{
		for (int i=0; i < xCopies; ++i)
		{
			Pop.push_back(vecPop[(popSize - 1) - NBest]);
	  }
	}
}

//-------------------------Reset()------------------------------
//	resets all the relevant variables ready for a new generation
//--------------------------------------------------------------

void GenAlg::Reset(Ball *ball){
	totalFitness = 0;
	bestFitness = 0;
	worstFitness = 9999999;
	avgFitness = 0;
}

