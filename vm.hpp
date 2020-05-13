#include "sim.hpp"
#include "config.hpp"

using namespace std;

struct TableRow { // Запись в таблице переадресации
    uint64_t real_address; // Адрес из РАП
    uint64_t virtual_address; // Адрес из ВАП
    uint64_t attribute; // Атрибут записи
};

class Computer { // Класс, в котором описана конфигурация комьютера и таблица переадресации
    private:
    uint64_t rm_size;
    uint64_t ae_size;
    uint64_t page_size;

    uint64_t *prm;
    
    public:
    void SetRealMemorySize(uint64_t value) { rm_size = value; };
    void SetArchiveEnviromentSize(uint64_t value) { ae_size = value; };
    void SetPageSize(uint64_t value) { page_size = value; };

    uint64_t GetRealMemorySize() { return rm_size; };
    uint64_t GetArchiveEnviromentSize() { return ae_size; };
    uint64_t GetPageSize() { return page_size; };
    Computer();
};

class CPU : public Agent {
    private: 

    TableRow translation_table[DEFAULT_TRANSLATION_TABLE_SIZE];

    public:

    void Convert(PageNumber address);
    void Wait();
    void Start();
};

class OS : public Agent {
    public:
    void CallCPU(int WriteFlag);
    void HandleInterruption();
    void Wait();
    void Start();
};

extern OS *g_pOS;
extern CPU *g_pCPU;


class Process : public Agent { // Базовая модель процесса, остальные модели будут являться наследниками этого класса
    private:
    uint64_t memory_usage;

    public:
    void SetMemoryUsage(uint64_t value) { memory_usage = value; };
    uint64_t GetMemoryUsage() { return memory_usage; };

    void Work();
    void Wait();
    void MemoryRequest(int WriteFlag);
    void Start();
    Process();
};

class AE : public Agent {
    private:
    

    public:
    void Wait();
    void Start();
};

