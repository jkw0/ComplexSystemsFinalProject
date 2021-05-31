#ifndef AGENTS_HH
#define AGENTS_HH

class agents
{
public:
    vector<double> tableOfAgents;
    parameters *Parameters;
    randBin * RandBin;

    agents(parameters* _Parameters, randBin * _RandBin);
    void interactions();
    double countProportionOfBToAll();
    void saveToFile();
    void display();
};



//==========================================================================    agents()
agents::agents(parameters* _Parameters, randBin * _RandBin)
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

    int range = 1000;
    randBin Rb(range);
    auto randNum = Rb.genRandBinNumber();
    int plus_minus = 0;

    for(int i = 0; i < Parameters->n; i++)
    {
        if(randNum < Parameters->pA * range)
        {
            randNum = Rb.genRandBinNumber();
            if(randNum < Parameters->IA * range)
                tableOfAgents.push_back(+50.5);
            else
                tableOfAgents.push_back(+0.5);
        }
        else
        {
            randNum = Rb.genRandBinNumber();
            if(randNum < Parameters->IB * range)
                tableOfAgents.push_back(-50.5);
            else
                tableOfAgents.push_back(-0.5);
        }
    }

    //cout<< "Vector size = " << tableOfAgents.size() << endl;
}

//==========================================================================    interactions()
void agents::interactions()
{
    vector<int> agentsIdxs;
    for(int i = 0; i<Parameters->s; i++)
    {
        int rn = RandBin->genRandBinNumber();
        agentsIdxs.push_back(rn);
        // cout<<rn<<"\t";
    }
    int nrOfInteractions = 0;
    while(1)
    {
        //cout << agentsIdxs[0]<< ", " << agentsIdxs[1]<< ", " << agentsIdxs[2] << "\t" << tableOfAgents[agentsIdxs[0]] << ", " << tableOfAgents[agentsIdxs[1]] << ", " << tableOfAgents[agentsIdxs[2]] << endl;
        if(nrOfInteractions >= Parameters->M)
            break;
        else if(all_of(agentsIdxs.begin(), agentsIdxs.end(), [this](double v){return (tableOfAgents[v] < 0);}))
            break;
        else if(all_of(agentsIdxs.begin(), agentsIdxs.end(), [this](double v){ return (tableOfAgents[v] > 0);}))
            break;
        if(Parameters->selfInfluenceMatters == true)
        {
            int suma = 0;
            for(auto idExt : agentsIdxs)
                    suma += (tableOfAgents[idExt] > 0) ? 1 : -1;
            for(auto idExt : agentsIdxs)
                    tableOfAgents[idExt] += suma;
                
        }
        else if (Parameters->selfInfluenceMatters == false)
        {
            unordered_map<int, double> hm;
            
            for(auto idExt : agentsIdxs)
            {
                int suma = 0;
                for(auto idIn : agentsIdxs)
                {
                    if(idExt != idIn)
                        suma += (tableOfAgents[idIn] > 0) ? 1 : -1;
                }
                hm[idExt] = suma;
            }
            for(auto idExt : agentsIdxs)
                tableOfAgents[idExt] += hm[idExt];

        }
        nrOfInteractions++;
    }
}

//==========================================================================    countProportionOfBToAll()
double agents::countProportionOfBToAll()
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
    // std::cout << "proportion: " << proportion << "   countB: " << countB << "   n: " << Parameters->n << std::endl;
    return proportion;
}

//==========================================================================    saveToFile()
void agents::saveToFile()
{
    auto file = createAndOpenFileHistogram(Parameters);
    for (auto v : tableOfAgents)
        file << v << "\n";
    file.close();
}

//==========================================================================    display()
void agents::display()
{
    cout << "=================="<<endl;
    for (auto v : tableOfAgents)
        cout << v << endl;
}


#endif //AGENTS_HH