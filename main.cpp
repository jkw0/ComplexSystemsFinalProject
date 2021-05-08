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
    int M; // max number of interaction in a discussion group
    int n; // number of agents in the whole system
    int g; // number of all discussion groups formed in a single simulation
    int s; // size of one discussion group
    double IA; // proportion of inflexibles among supporters of A
    double pA; // proportion of supporters of A
    double IB; // proportion of inflexibles among supporters of B
    double pB; // proportion of supporters of B
    bool selfInfluenceMatters = 1;
    int numOfKwadraciki;

    parameters(char ** argv)
    {
        parseInput();
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
             << "pB = "<< pB << endl
             << "selfInfluenceMatters = " << selfInfluenceMatters << endl
             << "numOfKwadraciki = " << numOfKwadraciki << endl << endl;
    }

    void parseInput()
    {
        ifstream file;
        file.open("params_file.txt", ifstream::in);
        string M_, n_, g_, s_, IA_, pA_, IB_, selfInfluenceMatters_, numOfKwadraciki_;
        file >> M_ >> n_ >> g_ >> s_ >> IA_ >> pA_ >> IB_ >> selfInfluenceMatters_ >> numOfKwadraciki_;
        M = stoi(M_);
        n = stoi(n_);
        g = stoi(g_);
        s = stoi(s_);
        IA = stod(IA_);
        pA = stod(pA_);
        IB = stod(IB_);
        selfInfluenceMatters = (stoi(selfInfluenceMatters_) == 0) ? false : true;
        numOfKwadraciki = stoi(numOfKwadraciki_);
        pB = 1 - pA;
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

string toStringWithPrecision(const double value, int prec)
{
    ostringstream out;
    out.precision(prec);
    out << fixed << value;
    return out.str();
}

void writeToFileHistogram(parameters* params, double val)
{
    ofstream file;
    string name = "HIS__M" + to_string(params->M) + "_n" + to_string(params->n) + "_g" + to_string(params->g) + "_s" +
        to_string(params->s) + "_IA" + toStringWithPrecision(params->IA, 1) + "_pA" + toStringWithPrecision(params->pA, 1) +
        "_IB" + toStringWithPrecision(params->IB, 1) + "_pB" + toStringWithPrecision(params->pB, 1) +
        "_selfInfl" + (params->selfInfluenceMatters ? "true" : "false") + ".txt";

    file.open(name, ios::out | ios::app);
    file << val << "\n";

    file.close();
}

void cleanHistogramFile(parameters params)
{
    ofstream file;
    string name = "HIS__M" + to_string(params.M) + "_n" + to_string(params.n) + "_g" + to_string(params.g) + "_s" +
        to_string(params.s) + "_IA" + toStringWithPrecision(params.IA, 1) + "_pA" + toStringWithPrecision(params.pA, 1) +
        "_IB" + toStringWithPrecision(params.IB, 1) + "_pB" + toStringWithPrecision(params.pB, 1) +
        "_selfInfl" + (params.selfInfluenceMatters ? "true" : "false") + ".txt";

    file.open(name, ios::out);
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
            writeToFileHistogram(Parameters, v);
        }
    };

    double countProportionOfBToAll()
    {
        double proportion;
        int countB = 0;

        for(auto agent : tableOfAgents)
        {
            countB += (agent < 0) ? 1 : 0;
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
                agents Agents(Parameters, RandBin);
                for(int i = 0; i < Parameters->g; i++)
                {
                    Agents.interactions();
                }
                table[i][j] = Agents.countProportionOfBToAll();
                cout<< "pA = " << Parameters->pA <<  "   IA = " << Parameters->IA << "   prop = " << table[i][j] <<endl;
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
    // Kwadrats.display();

    return 0;
}