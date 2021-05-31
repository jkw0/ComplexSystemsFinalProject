#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>  // for high_resolution_clock
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>
#include <random>

using namespace std;

#include "params.hh"
#include "randbin.hh"
#include "files.hh"
#include "agents.hh"
#include "kwadraciki.hh"


int main(int argc, char ** argv)
{
    if (argc >= 2)
    {
        parameters Parameters;
        Parameters.display();
        randBin RandBin(Parameters.n);

        string simulationType = argv[1];

        if (simulationType == "histogram")
        {
            agents Agents(&Parameters, &RandBin);
            for(int i = 0; i < Parameters.g; i++)
                Agents.interactions();

            Agents.saveToFile();
        }
        else if (simulationType == "kwadraty")
        {
            kwadrats Kwadrats(&Parameters, &RandBin);
            Kwadrats.liczKwadraciki();
            Kwadrats.display();
        }
        else 
        {
            cout << "niepoprawny typ symulacji: histogram lub kwadraty" << endl;
            exit(0);
        }
    }
    else
    {
        cout << "podaj typ symulacji: histogram lub kwadraty" << endl;
        exit(0);
    }

    return 0;
}