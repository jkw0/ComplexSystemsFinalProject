#ifndef PARAMS_H
#define PARAMS_H

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
    double pA_Begin;
    double IA_Begin;
    double pB_Begin;

    parameters();
    void parseInput();
    void display();
};

//==========================================================================    parameters()
parameters::parameters()
{
    parseInput();
}

//==========================================================================    parseInput()
void parameters::parseInput()
{
    ifstream file;
    file.open("params_file.txt", ifstream::in);
    string  M_,   n_,   g_,   s_,   IA_,   pA_,   IB_,   selfInfluenceMatters_,   pA_End_,   pA_Step_,   IA_End_,   IA_Step_;
    file >> M_ >> n_ >> g_ >> s_ >> IA_ >> pA_ >> IB_ >> selfInfluenceMatters_ >> pA_End_ >> pA_Step_ >> IA_End_ >> IA_Step_;
    M = stoi(M_);
    n = stoi(n_);
    g = stoi(g_);
    s = stoi(s_);
    IA = stod(IA_);
    pA = stod(pA_);
    IB = stod(IB_);
    pB = 1 - pA;
    selfInfluenceMatters = (stoi(selfInfluenceMatters_) == 0) ? false : true;

    pA_Begin = pA;
    pA_End = stod(pA_End_);
    pA_Step = stod(pA_Step_);
    IA_Begin = IA;
    IA_End = stod(IA_End_);
    IA_Step = stod(IA_Step_);
    pB_Begin = pB;
}

//==========================================================================    display()
void parameters::display()
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
            << "pA_Start = " << pA_Begin << endl
            << "pA_End = " << pA_End << endl
            << "pA_Step = " << pA_Step << endl
            << "IA_Start = " << IA_Begin << endl
            << "IA_End = " << IA_End << endl
            << "IA_Step = " << IA_Step << endl << endl;
}

#endif //PARAMS_H