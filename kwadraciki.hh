#ifndef KWADRACIKI_HH
#define KWADRACIKI_HH

class kwadrats
{
public:
    randBin *RandBin;
    parameters* Parameters;
    int pA_Dim = std::round((Parameters->pA_End - Parameters->pA_Begin)/Parameters->pA_Step + 1);
    int IA_Dim = std::round((Parameters->IA_End - Parameters->IA_Begin)/Parameters->IA_Step + 1);
    vector<vector<double>> table;

    void liczKwadraciki();
    kwadrats(parameters* _Parameters, randBin * _RandBin);
    void display();
};

//==========================================================================    kwadrats()
kwadrats::kwadrats(parameters* _Parameters, randBin * _RandBin)
:RandBin(_RandBin), Parameters(_Parameters)
{}

//==========================================================================    liczKwadraciki()
void kwadrats::liczKwadraciki()
{
    double progress = 0;
    int iterationNum = 0;
    const int numOfIterations = pA_Dim * IA_Dim;
    table.resize(IA_Dim, vector<double>(pA_Dim));
    Parameters->IA = Parameters->IA_Begin;
    double sumOfProportion = 0;

    for(int i = 0; i < IA_Dim; i++)
    {
        Parameters->pA = Parameters->pA_Begin;
        for(int j = 0; j < pA_Dim; j++)
        {
            sumOfProportion = 0;
            for (int realization = 0; realization < 20; realization++)
            {
                //cout << "realization nr " << realization+1 << endl;
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

            table[i][j] = sumOfProportion / 20;
            //cout << "AVG = " << average << endl;
        }
        Parameters->IA += Parameters->IA_Step;
    }
}

//==========================================================================    display()
void kwadrats::display()
{
    int pA_Dim = std::round((Parameters->pA_End - Parameters->pA_Begin)/Parameters->pA_Step + 1);
    int IA_Dim = std::round((Parameters->IA_End - Parameters->IA_Begin)/Parameters->IA_Step + 1);
    auto file = createAndOpenFileKwadraty(Parameters);
    for(int i = (IA_Dim-1); i >= 0; i--)
    {
        for(int j = 0; j < pA_Dim; j++)
        {
            cout << toStringWithPrecision(table[i][j], 3) << "\t";
            file << toStringWithPrecision(table[i][j], 3) << "\t";
        }
        cout<<endl;
        file << "\n";
    }
    file.close();
}


#endif //KWADRACIKI_HH