#include <iostream>
#include <vector>
#include <chrono>  // for high_resolution_clock
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;


class parameters
{
public:
    int M;
    int n;
    int g;
    int s;
    double IA;
    double pA;
    double IB;
    double pB;

    parameters(char ** argv)
    {
        int i = 1;
        M = stoi(argv[i++]);
        n = stoi(argv[i++]);
        g = stoi(argv[i++]);
        s = stoi(argv[i++]);
        IA = stod(argv[i++]);
        pA = stod(argv[i++]);
        IB = stod(argv[i++]);
        pB = 1 - pA;
    };

    void display()
    {
        cout << "M = "<< M << endl
             << "n = "<< n << endl
             << "g = "<< g << endl
             << "s = "<< s << endl
             << "IA = "<< IA << endl
             << "pA = "<< pA << endl
             << "IB = "<< IB << endl
             << "pB = "<< pB << endl << endl;
    }
};

class randBin
{
    const int max_nr;
public:
    randBin(const int _max_nr)
    : max_nr(_max_nr)
    {
        std::srand(std::time(nullptr));
        cout<<"Robie srand() "<<endl;
    };
    int genRandBinNumber()
    {
        return rand()%max_nr;
    };
    randBin(const randBin&) = delete;
};

string toStringWithPrecision(const double value)
{
    ostringstream out;
    out.precision(1);
    out << fixed << value;
    return out.str();
}

void writeToFile(parameters* params, double val)
{
    ofstream file;
    string name = "M" + to_string(params->M) + "_n" + to_string(params->n) + "_g" + to_string(params->g) + "_s" + to_string(params->s) + "_IA" + toStringWithPrecision(params->IA) + "_pA" +
        toStringWithPrecision(params->pA) + "_IB" + toStringWithPrecision(params->IB) + "_pB" + toStringWithPrecision(params->pB) + ".txt";
    file.open(name, ios::out | ios::app);
    file << val << "\n";

    file.close();
}

class agents
{
public:
    vector<double> tableOfAgents;
    parameters *Parameters;
    randBin * RandBin;

    agents(parameters* _Parameters, randBin * _RandBin)
    : RandBin(_RandBin)
    {
        Parameters = _Parameters;
        int nOfInflexiblesA = Parameters->n * Parameters->IA * Parameters->pA;
        int nOfRacionalA = Parameters->n * Parameters->pA - nOfInflexiblesA;
        int nOfInflexiblesB = Parameters->n * Parameters->IB * Parameters->pB;
        int nOfRacionalB = Parameters->n * Parameters->pB - nOfInflexiblesB;
       
        //cout << nOfInflexiblesA << endl << nOfRacionalA << endl  << nOfInflexiblesB  << endl << nOfRacionalB << endl ;

        for (int i = 0; i < nOfInflexiblesA; i++)
            tableOfAgents.push_back(50.5);
        for (int i = 0; i < nOfRacionalA; i++)
            tableOfAgents.push_back(0.5);
        for (int i = 0; i < nOfInflexiblesB; i++)
            tableOfAgents.push_back(-50.5);
        for (int i = 0; i < nOfRacionalB; i++)
            tableOfAgents.push_back(-0.5);

        //cout<< "Vector size = " << tableOfAgents.size() << endl;

        
    };

    void interactions()
    {
        vector<int> agentsIdxs;
        for(int i = 0; i<Parameters->s; i++)
        {
            agentsIdxs.push_back(RandBin->genRandBinNumber());
            //cout<<agentsIdxs.back()<<endl;
        }
        int nrOfInteractions = 0;
        while(1)
        {
            for(auto idExt : agentsIdxs)
            {
                for(auto idInt : agentsIdxs)
                {
                    tableOfAgents[idExt] += (tableOfAgents[idInt] > 0) ? 1 : -1;
                }
            }
            nrOfInteractions++;
            if(nrOfInteractions >= Parameters->M)
                break;
            else if(all_of(tableOfAgents.begin(), tableOfAgents.end(), [](int v){ return (v > 0);}))
                break;
            else if(all_of(tableOfAgents.begin(), tableOfAgents.end(), [](int v){ return (v < 0);}))
                break;
        }
    };
    void display()
    {
        cout << "=================="<<endl;
        for (auto v : tableOfAgents)
        {
            cout << v << endl;
            writeToFile(Parameters, v);
        }
    };

    double countProportionOfBToAll()
    {
        double proportion;
        int countB = 0;

        for(auto agent : tableOfAgents)
        {
            countB += (agent > 0) ? 1 : 0;
        }

        return (double) countB / Parameters->n;
    }

};

class kwadrats
{
public:
    randBin *RandBin;
    double table[20][20];
    parameters* Parameters;
    void liczKwadraciki()
    {
        for(int i = 0; i < 20; i++)
        {
            Parameters->pA = 0.3;
            for(int j = 0; j < 20; j++)
            {
                cout<< "pA = " << Parameters->pA <<  "\t IA = " << Parameters->IA <<endl;
                agents Agents(Parameters, RandBin);
                for(int i = 0; i < Parameters->g; i++)
                {
                    Agents.interactions();
                }
                table[i][j] = Agents.countProportionOfBToAll();
                Parameters->pA += 0.01;
            }
            Parameters->IA += 0.05;
            
        }

    }
    kwadrats(parameters* _Parameters, randBin * _RandBin)
    :RandBin(_RandBin), Parameters(_Parameters)
    {}
    void display()
    {
        for(int i = 0; i < 20; i++)
        {
            for(int j = 0; j < 20; j++)
            {
                cout<< table[i][j] << "\t";
            }
            cout<<endl;
        }
    }

};

int main(int argc, char ** argv)
{
    parameters Parameters (argv);
    Parameters.display();

    randBin RandBin(Parameters.n);
    // agents Agents(&Parameters, &RandBin);
    // for(int i = 0; i < Parameters.g; i++)
    // {
    //     Agents.interactions();
    // }
        
    // Agents.display();

    kwadrats Kwadrats(&Parameters, &RandBin);
    Kwadrats.liczKwadraciki();
    Kwadrats.display();

    return 0;
}