/*
This contains all the relevant functions for doing montecarlo integration.



*/


#include "DataDefinitions.h"


struct Sample{
    Point p;
    float probability;
};


Sample GenerateUniformSample()
{

}



/*

*/
Light IntegrateLightSamples(int samples,  Light (*function)(Point))
{
    Light accumulated(0,0,0);
    for(int i=0; i<samples; i++)
    {
        Sample s = GenerateUniformSample();
        accumulated += function(s.p) * s.probability;
    }
    accumulated /= samples;
}

