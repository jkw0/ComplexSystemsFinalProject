#ifndef RANDBIN_H
#define RANDBIN_H

class randBin
{
    const int max_nr;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<int> dis;
    int num;
public:
    randBin(const int _max_nr);
    int genRandBinNumber();
    randBin(const randBin&) = delete;
};

//==========================================================================    randBin()
randBin::randBin(const int _max_nr)
: max_nr(_max_nr)
{
    gen = mt19937(rd());
    dis = uniform_int_distribution<int>(0, max_nr-1); //Parameters->n-1
    // std::srand(std::time(nullptr));
}

//==========================================================================    genRandBinNumber()
int randBin::genRandBinNumber()
{
    // return rand()%max_nr;
    return dis(gen);
}


#endif //RANDBIN_H