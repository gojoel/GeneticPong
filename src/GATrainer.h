#ifndef GETRA_H
#define GETRA_H



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

class GATrainer {

private:

	vector<double> v;

	float Velocity(Paddle *p1, Ball *ball, vector<double> &chromo);

public:
	
	float CalculateMove(Paddle *p1, Ball *ball);
	GATrainer();

};


#endif