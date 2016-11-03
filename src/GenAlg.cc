#include "src/GenAlg.h"



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
																									currentPop(0)
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
											vector<double> 			&child)
{


	//choosing a cross-over point
	int crossp = RandInt(0, chromoLength - 1);

	//first half from parent
	for (int i = 0; i < crossp; ++i){
		child.push_back(mother[i]);
	}

	//second half from other parent
	for (i = crossp; i < mother.size(); i++){
		child.push_back(father[i]);
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


//---------------------------------Velocity--------------------------------
//	find the x velocity and y velocity for paddle
//-----------------------------------------------------------------------

//get y velocity
float GenAlg::getyVelocity(Paddle p1, Paddle p2, Ball ball, vector<double> &chromo)
{
	return chromo[5] * b.velocity.y + chromo[2] * b.y + chromo[3] * p1.y;
}



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

	float GenAlg::CalculateMove(Paddle *p1, Paddle *p2, Ball *ball){

		if (ball->dx == 0 && ball->dy == 0){
			ball->reset();
			return null;
		}

		if (!hasCrossed)
			lastDistance = getCrossDistance(*p1, *ball);

		return vecPop[cur].getyVelocity(*p1, *p2, *ball);

	}


 int GenAlg::GetCrossDistance(Paddle *p1, Ball *ball){

	 int distance;
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
