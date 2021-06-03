#include <iostream>
#include <vector>
#include <chrono>  // for high_resolution_clock
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <array>
#include <utility>
#include <cmath>
#include <random>
#include <unordered_map>
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
    double pA_End;
    double pA_Step;
    double IA_End;
    double IA_Step;
    // int numOfKwadraciki;
    double pA_Begin;
    double IA_Begin;
    double pB_Begin;
    double conv;

    parameters()
    {
        parseInput();
    }

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
             << "pA_End = " << pA_End << endl
             << "pA_Step = " << pA_Step << endl
             << "IA_End = " << IA_End << endl
             << "IA_Step = " << IA_Step << endl
             << "conv = " << conv << endl << endl;
    }

    void parseInput()
    {
        ifstream file;
        file.open("params_file.txt", ifstream::in);
        string M_, n_, g_, s_, IA_, pA_, IB_, selfInfluenceMatters_, pA_End_, pA_Step_, IA_End_, IA_Step_, conv_;
        file >> M_ >> n_ >> g_ >> s_ >> IA_ >> pA_ >> IB_ >> selfInfluenceMatters_ >> pA_End_ >> pA_Step_ >>
            IA_End_ >> IA_Step_ >> conv_;
        M = stoi(M_);
        n = stoi(n_);
        g = stoi(g_);
        s = stoi(s_);
        IA = stod(IA_);
        pA = stod(pA_);
        IB = stod(IB_);
        selfInfluenceMatters = (stoi(selfInfluenceMatters_) == 0) ? false : true;
        pA_End = stod(pA_End_);
        pA_Step = stod(pA_Step_);
        IA_End = stod(IA_End_);
        IA_Step = stod(IA_Step_);
        conv = stod(conv_);
        pB = 1 - pA;
        pA_Begin = pA;
        IA_Begin = IA;
        pB_Begin = pB;
    }
};

class randBin
{
    const int max_nr;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<int> dis;
    uniform_real_distribution<double> dis_real;
    int num;
public:
    randBin(const int _max_nr)
    : max_nr(_max_nr)
    {
        gen = mt19937(rd());
        dis = uniform_int_distribution<int>(0, max_nr-1); //Parameters->n-1
        dis_real = uniform_real_distribution<double>(0.0, 1.0);
    }
    int genRandBinNumber()
    {
        return dis(gen);
    }
    double getRandUniform()
    {
        return dis_real(gen);
    }
    randBin(const randBin&) = delete;
};

string toStringWithPrecision(const double value, int prec)
{
    ostringstream out;
    out.precision(prec);
    out << fixed << value;
    return out.str();
}

ofstream createAndOpenFileHistogram(parameters* params)
{
    ofstream file;
    string name = "HIS__M" + to_string(params->M) + "_n" + to_string(params->n) + "_g" + to_string(params->g) + "_s" +
        to_string(params->s) + "_IA" + toStringWithPrecision(params->IA, 1) + "_pA" + toStringWithPrecision(params->pA, 1) +
        "_IB" + toStringWithPrecision(params->IB, 1) + "_pB" + toStringWithPrecision(params->pB, 1) +
        "_selfInfl" + (params->selfInfluenceMatters ? "true" : "false") + ".txt";

    file.open(name, ios::out);
    file.close();
    file.open(name, ios::out | ios::app);

    return file;
}

ofstream createAndOpenFileKwadraty(parameters* params)
{
    ofstream file;
    string name = "KWA__M" + to_string(params->M) + "_n" + to_string(params->n) + "_g" + to_string(params->g) + "_s" +
        to_string(params->s) + "_IA" + toStringWithPrecision(params->IA_Begin, 1) + "_pA" +
        toStringWithPrecision(params->pA_Begin, 1) + "_IB" + toStringWithPrecision(params->IB, 1) + "_pB" +
        toStringWithPrecision(params->pB_Begin, 1) + "_selfInfl" + (params->selfInfluenceMatters ? "true" : "false") +
        "_pA_End" + toStringWithPrecision(params->pA_End, 2) +
        "_pA_Step" + toStringWithPrecision(params->pA_Step, 2) +
        "_IA_End" + toStringWithPrecision(params->IA_End, 2) +
        "_IA_Step" + toStringWithPrecision(params->IA_Step, 2) + ".txt";

    file.open(name, ios::out);
    file.close();
    file.open(name, ios::out | ios::app);

    return file;
}

class agents
{
public:
    vector<double> tableOfAgents;
    vector<pair<double,double>> tableOfAgents2;
    parameters *Parameters;
    randBin * RandBin;

    agents(parameters* _Parameters, randBin * _RandBin)
    : RandBin(_RandBin)
    {
        Parameters = _Parameters;
        Parameters->pB = 1 - Parameters->pA;
        int nOfInflexiblesA = std::round(Parameters->n * Parameters->IA * Parameters->pA);
        int nOfRacionalA = std::round(Parameters->n * Parameters->pA - nOfInflexiblesA);
        int nOfInflexiblesB = std::round(Parameters->n * Parameters->IB * Parameters->pB);
        int nOfRacionalB = std::round(Parameters->n * Parameters->pB - nOfInflexiblesB);

        for (int i = 0; i < nOfInflexiblesA; i++)
            tableOfAgents.push_back(50.5);
        for (int i = 0; i < nOfRacionalA; i++)
            tableOfAgents.push_back(0.5);
        for (int i = 0; i < nOfInflexiblesB; i++)
            tableOfAgents.push_back(-50.5);
        for (int i = 0; i < nOfRacionalB; i++)
            tableOfAgents.push_back(-0.5);

        // for (int i = 0; i < nOfInflexiblesA; i++)
        //     tableOfAgents2.push_back(make_pair(50.5,0));
        // for (int i = 0; i < nOfRacionalA; i++)
        //     tableOfAgents2.push_back(make_pair(0.5,0));
        // for (int i = 0; i < nOfInflexiblesB; i++)
        //     tableOfAgents2.push_back(make_pair(-50.5,0));
        // for (int i = 0; i < nOfRacionalB; i++)
        //     tableOfAgents2.push_back(make_pair(-0.5,0));

        // if (Parameters->conv > 0)
        //     for (auto agent: tableOfAgents2) {
        //         if (RandBin->getRandUniform() < Parameters->conv)
        //             agent.second = 2;
        //         else agent.second = 1;
        //     }
    }

    void interactions()
    {
        vector<int> agentsIdxs;
        for(int i = 0; i<Parameters->s; i++)
        {
            agentsIdxs.push_back(RandBin->genRandBinNumber());
        }
        int nrOfInteractions = 0;
        while(1)
        {
            if(Parameters->selfInfluenceMatters == true)
            {
                int sum = 0;
                for(auto idx : agentsIdxs)
                    sum += (tableOfAgents[idx] > 0) ? 1 : -1;
                for(auto idx : agentsIdxs)
                    tableOfAgents[idx] += sum;
            }
            else if (Parameters->selfInfluenceMatters == false)
            {
                unordered_map<int, double> hm;
                for(auto idExt : agentsIdxs) {
                    int suma = 0;
                    for(auto idIn : agentsIdxs) {
                        if(idExt != idIn)
                            suma += (tableOfAgents[idIn] > 0) ? 1 : -1;
                    }
                    hm[idExt] = suma;
                }
                for(auto idExt : agentsIdxs)
                    tableOfAgents[idExt] += hm[idExt];
            }
            nrOfInteractions++;
            if(nrOfInteractions >= Parameters->M)
                break;
            else if(all_of(agentsIdxs.begin(), agentsIdxs.end(), [this](double v){return (tableOfAgents[v] < 0);}))
                break;
            else if(all_of(agentsIdxs.begin(), agentsIdxs.end(), [this](double v){ return (tableOfAgents[v] > 0);}))
                break;
        }
    }

    void display()
    {
        cout << "=================="<<endl;
        for (auto v : tableOfAgents)
            cout << v << endl;
    }

    void saveToFile()
    {
        auto file = createAndOpenFileHistogram(Parameters);
        for (auto v : tableOfAgents)
            file << v << "\n";
        file.close();
    }
    double countProportionOfBToAll()
    {
        double proportion;
        int countB = 0;
        int k = 0;
        for(auto agent : tableOfAgents)
        {
            countB += (agent < 0) ? 1 : 0;
        }
        proportion = (double) countB / Parameters->n;
        return proportion;
    }
};

class kwadrats
{
public:
    randBin *RandBin;
    parameters* Parameters;
    int pB_Dim = std::round((Parameters->pA_End - Parameters->pA_Begin)/Parameters->pA_Step + 1);
    int IA_Dim = std::round((Parameters->IA_End - Parameters->IA_Begin)/Parameters->IA_Step + 1);
    vector<vector<double>> table;
    void liczKwadraciki()
    {
        double progress = 0;
        int iterationNum = 0;
        const int numOfIterations = pB_Dim * IA_Dim;
        table.resize(IA_Dim, vector<double>(pB_Dim));
        Parameters->IA = Parameters->IA_Begin;
        int numOfRealizations = 20;
        double sumOfProportion = 0;

        for(int i = 0; i < IA_Dim; i++)
        {
            Parameters->pA = Parameters->pA_Begin;
            for(int j = 0; j < pB_Dim; j++)
            {
                sumOfProportion = 0;
                for (int realization = 0; realization < numOfRealizations; realization++)
                {
                    agents Agents(Parameters, RandBin);
                    for(int k = 0; k < Parameters->g; k++)
                    {
                        Agents.interactions();
                    }
                    table[i][j] = Agents.countProportionOfBToAll();
                    sumOfProportion += table[i][j];
                }
                Parameters->pA += Parameters->pA_Step;
                progress = (double) ++iterationNum/numOfIterations;

                if (iterationNum % 10 == 0)
                    cout << "Progress: " << toStringWithPrecision(100*progress,2) << "%" << endl;

                table[i][j] = (double)sumOfProportion / numOfRealizations;
            }
            Parameters->IA += Parameters->IA_Step;
        }
    }
    kwadrats(parameters* _Parameters, randBin * _RandBin)
    :RandBin(_RandBin), Parameters(_Parameters)
    {}
    void display()
    {
        int pB_Dim = std::round((Parameters->pA_End - Parameters->pA_Begin)/Parameters->pA_Step + 1);
        int IA_Dim = std::round((Parameters->IA_End - Parameters->IA_Begin)/Parameters->IA_Step + 1);
        auto file = createAndOpenFileKwadraty(Parameters);
        for(int i = (IA_Dim-1); i >= 0; i--)
        {
            for(int j = 0; j < pB_Dim; j++)
            {
                cout << toStringWithPrecision(table[i][j], 3) << "\t";
                file << toStringWithPrecision(table[i][j], 3) << "\t";
            }
            cout<<endl;
            file << "\n";
        }
        file.close();
    }
};

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