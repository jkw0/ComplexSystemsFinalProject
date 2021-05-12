#include <iostream>
#include <vector>
#include <chrono>  // for high_resolution_clock
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>
#include <random>
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
    double initsupEnd;
    double initsupStep;
    double inflexEnd;
    double inflexStep;
    // int numOfKwadraciki;
    double initial_pA;
    double initial_IA;
    double initial_pB;

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
             << "initsupEnd = " << initsupEnd << endl
             << "initsupStep = " << initsupStep << endl
             << "inflexEnd = " << inflexEnd << endl
             << "inflexStep = " << inflexStep << endl << endl;
    }

    void parseInput()
    {
        ifstream file;
        file.open("params_file.txt", ifstream::in);
        string M_, n_, g_, s_, IA_, pA_, IB_, selfInfluenceMatters_, initsupEnd_, initsupStep_, inflexEnd_, inflexStep_;
        file >> M_ >> n_ >> g_ >> s_ >> IA_ >> pA_ >> IB_ >> selfInfluenceMatters_ >> initsupEnd_ >> initsupStep_ >>
            inflexEnd_ >> inflexStep_;
        M = stoi(M_);
        n = stoi(n_);
        g = stoi(g_);
        s = stoi(s_);
        IA = stod(IA_);
        pA = stod(pA_);
        IB = stod(IB_);
        selfInfluenceMatters = (stoi(selfInfluenceMatters_) == 0) ? false : true;
        initsupEnd = stod(initsupEnd_);
        initsupStep = stod(initsupStep_);
        inflexEnd = stod(inflexEnd_);
        inflexStep = stod(inflexStep_);
        pB = 1 - pA;
        initial_pA = pA;
        initial_IA = IA;
        initial_pB = pB;
    }
};

class randBin
{
    const int max_nr;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<int> dis;
    int num;
public:
    randBin(const int _max_nr)
    : max_nr(_max_nr)
    {
        gen = mt19937(rd());
        dis = uniform_int_distribution<int>(0, max_nr-1); //Parameters->n-1
        // std::srand(std::time(nullptr));
    }
    int genRandBinNumber()
    {
        // return rand()%max_nr;
        return dis(gen);
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

ofstream createAndOpenFileKwadraty(parameters* params)
{
    ofstream file;
    string name = "KWA__M" + to_string(params->M) + "_n" + to_string(params->n) + "_g" + to_string(params->g) + "_s" +
        to_string(params->s) + "_IA" + toStringWithPrecision(params->initial_IA, 1) + "_pA" +
        toStringWithPrecision(params->initial_pA, 1) + "_IB" + toStringWithPrecision(params->IB, 1) + "_pB" +
        toStringWithPrecision(params->initial_pB, 1) + "_selfInfl" + (params->selfInfluenceMatters ? "true" : "false") +
        "_initsupEnd" + toStringWithPrecision(params->initsupEnd, 2) +
        "_initsupStep" + toStringWithPrecision(params->initsupStep, 2) +
        "_inflexEnd" + toStringWithPrecision(params->inflexEnd, 2) +
        "_inflexStep" + toStringWithPrecision(params->inflexStep, 2) + ".txt";

    file.open(name, ios::out | ios::app);

    return file;
}

void cleanKwadratyFile(parameters params)
{
    ofstream file;
    string name = "KWA__M" + to_string(params.M) + "_n" + to_string(params.n) + "_g" + to_string(params.g) + "_s" +
        to_string(params.s) + "_IA" + toStringWithPrecision(params.initial_IA, 1) + "_pA" +
        toStringWithPrecision(params.initial_pA, 1) + "_IB" + toStringWithPrecision(params.IB, 1) + "_pB" +
        toStringWithPrecision(params.initial_pB, 1) +  "_selfInfl" + (params.selfInfluenceMatters ? "true" : "false") +
        "_initsupEnd" + toStringWithPrecision(params.initsupEnd, 2) +
        "_initsupStep" + toStringWithPrecision(params.initsupStep, 2) +
        "_inflexEnd" + toStringWithPrecision(params.inflexEnd, 2) +
        "_inflexStep" + toStringWithPrecision(params.inflexStep, 2) + ".txt";

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
        Parameters->pB = 1 - Parameters->pA;
        int nOfInflexiblesA = std::round(Parameters->n * Parameters->IA * Parameters->pA);
        int nOfRacionalA = std::round(Parameters->n * Parameters->pA - nOfInflexiblesA);
        int nOfInflexiblesB = std::round(Parameters->n * Parameters->IB * Parameters->pB);
        int nOfRacionalB = std::round(Parameters->n * Parameters->pB - nOfInflexiblesB);
        // cout << "IA: " << Parameters->IA << endl;
        // cout << "he?: " << (int)(Parameters->n * Parameters->pB - nOfInflexiblesB + 0.5) << endl;
        // cout << "pA: " << Parameters->pA << "  pB: " << Parameters->pB << endl;  
        // cout << nOfInflexiblesA << endl << nOfRacionalA << endl  << nOfInflexiblesB  << endl << nOfRacionalB << endl ;

        for (int i = 0; i < nOfInflexiblesA; i++)
            tableOfAgents.push_back(50.5);
        for (int i = 0; i < nOfRacionalA; i++)
            tableOfAgents.push_back(0.5);
        for (int i = 0; i < nOfInflexiblesB; i++)
            tableOfAgents.push_back(-50.5);
        for (int i = 0; i < nOfRacionalB; i++)
            tableOfAgents.push_back(-0.5);

        //cout<< "Vector size = " << tableOfAgents.size() << endl;
    }

    void interactions()
    {
        vector<int> agentsIdxs;
        for(int i = 0; i<Parameters->s; i++)
        {
            int val1, val2, val3;
            do {
                val1 = RandBin->genRandBinNumber();
                val2 = RandBin->genRandBinNumber();
                val3 = RandBin->genRandBinNumber();
            } while (val1 == val2 && val2 == val3 && val1 == val3);


            agentsIdxs.push_back(val1);
            agentsIdxs.push_back(val2);
            agentsIdxs.push_back(val3);
            //cout<<agentsIdxs.back()<<endl;
        }
        int nrOfInteractions = 0;
        while(1)
        {
            // int num = 0;
            // cout << "BEFORE INTERACTIONS:" << endl;
            // for (auto agents : tableOfAgents)
            //     cout << num++ << ")" << agents << " ";
            // cout << endl;
            if(Parameters->selfInfluenceMatters == true)
            {
                for(auto idExt : agentsIdxs)
                    for(auto idInt : agentsIdxs)
                    {
                        tableOfAgents[idExt] += (tableOfAgents[idInt] > 0) ? 1 : -1;
                    }
            }
            else if (Parameters->selfInfluenceMatters == false)
            {
                for(auto idExt : agentsIdxs)
                    for(auto idInt : agentsIdxs)
                    {
                        if (idExt == idInt) continue;
                        tableOfAgents[idExt] += (tableOfAgents[idInt] > 0) ? 1 : -1;
                    }
            }

            // num = 0;
            // cout << "AFTER INTERACTIONS:" << endl;
            // for (auto agents : tableOfAgents)
            //     cout << num++ << ")" << agents << " ";
            // cout << endl;

            nrOfInteractions++;

            if(nrOfInteractions >= Parameters->M)
            {
                // std::cout << "break1" << std::endl;
                break;
            }

            vector<double> tableOfAgentsSmall;
            tableOfAgentsSmall.reserve(Parameters->s);
            for (auto idx : agentsIdxs)
                tableOfAgentsSmall.push_back(tableOfAgents[idx]);

            if(all_of(tableOfAgentsSmall.begin(), tableOfAgentsSmall.end(), [](double v){return (v < 0);}))
            {
                // std::cout << "break2" << std::endl;
                break;
            }
            else if(all_of(tableOfAgentsSmall.begin(), tableOfAgentsSmall.end(), [](double v){ return (v > 0);}))
            {
                // std::cout << "break3" << std::endl;
                break;
            }
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
        for (auto v : tableOfAgents)
            writeToFileHistogram(Parameters, v);
    }
    double countProportionOfBToAll()
    {
        double proportion;
        int countB = 0;
        int k = 0;
        for(auto agent : tableOfAgents)
        {
            // cout << k++ << ")" << agent << " ";
            countB += (agent < 0) ? 1 : 0;
        }
        // cout << endl;
        proportion = (double) countB / Parameters->n;
        // if (proportion > 1.0)
        // cout << "PARAMS: pA= " << Parameters->pA << " IA=" << Parameters->IA << endl;
        std::cout << "proportion: " << proportion << "   countB: " << countB << "   n: " << Parameters->n << std::endl;
        return proportion;
    }
};

class kwadrats
{
public:
    randBin *RandBin;
    parameters* Parameters;
    int initialSupportersDim = std::round((Parameters->initsupEnd - Parameters->initial_pA)/Parameters->initsupStep + 1);
    int inflexiblesDim = std::round((Parameters->inflexEnd - Parameters->initial_IA)/Parameters->inflexStep + 1);
    vector<vector<double>> table;
    void liczKwadraciki()
    {
        double progress = 0;
        int iterationNum = 0;
        const int numOfIterations = initialSupportersDim * inflexiblesDim;
        table.resize(inflexiblesDim, vector<double>(initialSupportersDim));
        Parameters->IA = Parameters->initial_IA;
        double sumOfProportion = 0;

        for(int i = 0; i < inflexiblesDim; i++)
        {
            Parameters->pA = Parameters->initial_pA;
            for(int j = 0; j < initialSupportersDim; j++)
            {
                for (int realization = 0; realization < 20; realization++)
                {
                    cout << "realization nr " << realization+1 << endl;
                    agents Agents(Parameters, RandBin);
                    for(int k = 0; k < Parameters->g; k++)
                    {
                        Agents.interactions();
                    }
                    table[i][j] = Agents.countProportionOfBToAll();
                    sumOfProportion += table[i][j];
                }
                Parameters->pA += Parameters->initsupStep;
                progress = (double) ++iterationNum/numOfIterations;

                if (iterationNum % 10 == 0)
                    cout << "Progress: " << toStringWithPrecision(100*progress,2) << "%" << endl;

                double average = sumOfProportion / 20;
                cout << "AVG = " << average << endl;
            }
            Parameters->IA += Parameters->inflexStep;
        }
    }
    kwadrats(parameters* _Parameters, randBin * _RandBin)
    :RandBin(_RandBin), Parameters(_Parameters)
    {}
    void display()
    {
        int initialSupportersDim = std::round((Parameters->initsupEnd - Parameters->initial_pA)/Parameters->initsupStep + 1);
        int inflexiblesDim = std::round((Parameters->inflexEnd - Parameters->initial_IA)/Parameters->inflexStep + 1);
        auto file = createAndOpenFileKwadraty(Parameters);
        for(int i = (inflexiblesDim-1); i >= 0; i--)
        {
            for(int j = 0; j < initialSupportersDim; j++)
            {
                // cout<< table[i][j] << "\t";
                file << toStringWithPrecision(table[i][j], 3) << "\t";
            }
            // cout<<endl;
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
            cleanHistogramFile(Parameters);
            agents Agents(&Parameters, &RandBin);
            for(int i = 0; i < Parameters.g; i++)
                Agents.interactions();

            Agents.saveToFile();
        }
        else if (simulationType == "kwadraty")
        {
            cleanKwadratyFile(Parameters);
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