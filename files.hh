#ifndef FILES_H
#define FILES_H

//==========================================================================    toStringWithPrecision()
string toStringWithPrecision(const double value, int prec)
{
    ostringstream out;
    out.precision(prec);
    out << fixed << value;
    return out.str();
}

//==========================================================================    createAndOpenFileHistogram()
ofstream createAndOpenFileHistogram(parameters* params)
{
    ofstream file;
    string name = "output_H/HIS__M" + to_string(params->M) + "_n" + to_string(params->n) + "_g" + to_string(params->g) + "_s" +
        to_string(params->s) + "_IA" + toStringWithPrecision(params->IA, 1) + "_pA" + toStringWithPrecision(params->pA, 1) +
        "_IB" + toStringWithPrecision(params->IB, 1) + "_pB" + toStringWithPrecision(params->pB, 1) +
        "_selfInfl" + (params->selfInfluenceMatters ? "true" : "false") + ".txt";

    file.open(name, ios::out);
    file.close();
    file.open(name, ios::out | ios::app);

    return file;
}

//==========================================================================    createAndOpenFileKwadraty()
ofstream createAndOpenFileKwadraty(parameters* params)
{
    ofstream file;
    string name = "output_Heat/KWA__M" + to_string(params->M) + "_n" + to_string(params->n) + "_g" + to_string(params->g) + "_s" +
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

#endif //FILES_H