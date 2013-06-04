//GameTimer.cpp__________________________________
#include "GameTimer.h"

gametimer::gametimer(){
	//Timing
	QueryPerformanceCounter(&start);	
}

float gametimer::gettime(){
	float numTicks;
	QueryPerformanceCounter(&end);
	QueryPerformanceFrequency(&freq);
	numTicks = end.QuadPart - start.QuadPart; // Save the number of ticks
	return (float)numTicks/freq.QuadPart; //in seconds
}