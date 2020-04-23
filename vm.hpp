#include "sim.hpp"
#include "config.hpp"
typedef uint64_t PageNumber;
using namespace std;

class CPU : public Agent {
    public:
    PageNumber *Convert(PageNumber address);// Возвращает NULL в случае прерывания...
    void Wait();

    void Start();
};

class OS : public Agent {
    public:
    void CallCPU(int WriteFlag);
    void Wait();

    void Start();
};

extern OS *g_pOS;
extern CPU *g_pCPU;


class Process : public Agent {
    public:
    void Work();
    void Wait();
    void MemoryRequest(int WriteFlag);
    
    void Start();
};
