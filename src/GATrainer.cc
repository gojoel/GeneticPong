
#include "GATrainer.h"

GATrainer::GATrainer(){

	int count = 0;
	double data; 

	v.push_back(0.326105);
	v.push_back(-0.377515);
	v.push_back(0.851315);

	/*

	ifstream saved ("generations.txt");

	if (saved.is_open()){

			while (saved >> data){

				if (count == 3)
					break;
				else
					v.push_back(data);
			}

		saved.close();

	}
	else
		cout << "Unable to open file" << endl;

	*/

}

float GATrainer::CalculateMove(Paddle *p1, Ball *ball){

	
	float move = Velocity(p1, ball, v);

	return move;
}

float GATrainer::Velocity(Paddle *p1, Ball *ball, vector<double> &chromo)
{
	// find best weights to calculate speed to move and position
	double velocity = chromo[2] * ball->dy + chromo[0] * ball->y + chromo[1] * p1->y;

	return velocity;
}





