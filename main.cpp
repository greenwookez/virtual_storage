#include <iostream>
#include <stdint.h>
#include <assert.h>
#include "vm.hpp"


using namespace std;

Sim *g_pSim;
OS *g_pOS;
CPU *g_pCPU;
Computer *g_pConfig;

int main()
{
    g_pSim = new Sim;

    g_pConfig = new Computer;
    
    g_pOS = new OS;
    g_pOS->SetName("OS");
    
    g_pCPU = new CPU;
    g_pCPU->SetName("CPU");
    
    MyProcess *pProcess01 = new MyProcess;
    pProcess01->SetName("Process1");

    MyProcess *pProcess02 = new MyProcess;
    pProcess02->SetName("Process2");

    pProcess02->Start();


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
    delete pProcess02;
    delete g_pSim;

    return 0;
}
