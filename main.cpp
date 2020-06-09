#include <iostream>
#include <stdint.h>
#include <assert.h>
#include "vm.hpp"


using namespace std;

Sim *g_pSim;
OS *g_pOS;
CPU *g_pCPU;
Computer *g_pComputer;
AE *g_pAE;

int main()
{
    g_pSim = new Sim;

    
    
    g_pOS = new OS;
    g_pOS->SetName("OS");
    
    g_pCPU = new CPU;
    g_pCPU->SetName("CPU");
    
    g_pAE = new AE;
    g_pAE->SetName("AE");

    g_pComputer = new Computer;
    g_pComputer->PrintCurrentConfig();

    MyProcess *pProcess01 = new MyProcess;
    pProcess01->SetName("Process01");

    pProcess01->Start();


    g_pSim->SetLimit(30000);
    while(!g_pSim->Run())
    {
        PrintTime(&std::cout);
        std::cout << "Do you want to proceed? [Y/n]" << std::endl;

        char c[3];
        std::cin.getline(c,sizeof(c));

        if(c[0] == 'n')
        {
            break;
        };

        g_pSim->SetLimit(g_pSim->GetTime()+30000);
    }

    delete pProcess01;

    delete g_pSim;
    delete g_pComputer;
    delete g_pOS;
    delete g_pCPU;
    delete g_pAE;

    return 0;
}
