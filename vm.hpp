#include "sim.hpp"
#include "config.hpp"

using namespace std;

class Computer { // Класс, в котором описана конфигурация комьютера
    private:
    uint64_t rm_size;
    uint64_t ae_size;
    uint64_t page_size;
    
    public:
    void SetRealMemorySize(uint64_t value);
    void SetArchiveEnviromentSize(uint64_t value);
    void SetPageSize(uint64_t value);

    uint64_t GetRealMemorySize();
    uint64_t GetArchiveEnviromentSize();
    uint64_t GetPageSize();
    Computer();
};

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


class Process : public Agent { // Базовая модель процесса, остальные модели будут являться наследниками этого класса
    private:
    uint64_t memory_usage;

    public:
    void SetMemoryUsage(uint64_t value);
    uint64_t GetMemoryUsage();

    void Work();
    void Wait();
    void MemoryRequest(int WriteFlag);
    void Start();
    Process();
};
