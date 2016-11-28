

#include "src/pong.h"
#include "src/GenAlg.h"


volatile sig_atomic_t interrupted=false; 

void signal_handler(int s)
{
   // ...
   interrupted = true;
}

int main(int argc, char* argv[]) {

	while (!interrupted)
	{
	     // Perform computations
		Pong pong(argc, argv);
	    pong.execute();

	    pong.saveGame();
	    //save exit
	    //GenAlg::savePopulation();
	    exit(0);
	}

}
