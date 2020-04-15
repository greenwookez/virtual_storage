#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <windows.h>
#include "sim.h"

using namespace std;

typedef uint32_t PageNumber; /* номер страницы */

// ссылка на единственный объект симулятора
Sim *g_pSim;

// мой класс агента наследует базовый класс
class Process : public Agent {
    public:


    void Work(uint32_t value) {
        Log("Я работаю");
    };

    void MemoryRequest(PageNumber address, bool WriteFlag) {
        Log("Я обращаюсь к памяти.");
    };



    void Start() {    
        Schedule(1500, this, &Process::Work, 0);
    };
};


class MyAgent2 : public Agent {
    public:
    void DoSomething(uint32_t n) {
        Log("Doing something...");

        if (n>0) {
            Schedule(GetTime() + 500,this, &MyAgent2::DoSomething,n-1);
        }
    };

    void Start() {
        Schedule(0, this, &MyAgent2::DoSomething, 5);
    };
};



int main()
{
    /* Для отображения русских символов в консоли на Windows */
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);


    g_pSim = new Sim;

    Process *pProcess = new Process;
    MyAgent2 *pAgent2 = new MyAgent2;

    pProcess -> SetName("Проц");
    pAgent2 -> SetName("Agent2");
    pProcess -> Start();
    pAgent2 -> Start();
    

    g_pSim->SetLimit(30000);

    g_pSim->Run();

    PrintTime(&std::cout);
    std::cout << " Do you want to proceed? [Y/n]" << std::endl;

    char c[3];
    std::cin.get(c,sizeof(c),'\n');
    if(c[0] != 'n')
    {
        g_pSim->SetLimit(60000);
        g_pSim->Run();
    }

    delete pProcess;
    delete pAgent2;

    return 0;
}
