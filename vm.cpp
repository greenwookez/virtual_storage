#include "vm.hpp"

extern OS *g_pOS;
extern CPU *g_pCPU;

Computer :: Computer() {
    rm_size = DEFAULT_REAL_MEMORY_SIZE;
    ae_size = DEFAULT_ARCHIVE_ENVIROMENT_SIZE;
    page_size = DEFAULT_PAGE_SIZE;
};

void Computer :: SetRealMemorySize(uint64_t value) {
    rm_size = value;
};

void Computer :: SetArchiveEnviromentSize(uint64_t value) {
    ae_size = value;
};

void Computer :: SetPageSize(uint64_t value) {
    page_size = value;
};

uint64_t Computer :: GetRealMemorySize() {
    return rm_size;
};

uint64_t Computer :: GetArchiveEnviromentSize() {
    return ae_size;
};

uint64_t Computer :: GetPageSize() {
    return page_size;
};


PageNumber * CPU :: Convert(PageNumber address) {  // Возвращает NULL в случае прерывания...
    string tmp;
    tmp = "I have to convert virtual address(" + to_string(address) + ")";
    Log(tmp);
    return NULL;
};

void CPU :: Wait() {
    Log("I am waiting!");
};

void CPU :: Start() {
    Schedule(GetTime(), this, &CPU::Wait);
}

void OS :: CallCPU(int WriteFlag) {
    PageNumber address = rand() % 150; // ОС выделяет виртуальный адрес процессу, так как она выполняет функции виртуальной памяти. Пока он рандомный....
    
    Log("Calling CPU...");
    Schedule(GetTime()+TIME_FOR_CALLCPU, g_pCPU, &CPU::Convert, address);
};

void OS :: Wait() {
    Log("I am waiting!");
};

void OS :: Start() {
    Schedule(GetTime(), this, &OS::Wait);
};

Process :: Process() {
    memory_usage = DEFAULT_MEMORY_USAGE;
};


void Process :: SetMemoryUsage(uint64_t value) {
    memory_usage = value;
};

uint64_t Process :: GetMemoryUsage() {
    return memory_usage;
};


void Process :: Work() {
    Log("I am working.");
};

void Process :: Wait() {
    Log("I am waiting.");
};
void Process :: MemoryRequest(int WriteFlag) {
    string tmp;
    tmp = "I am requesting memory with WriteFlag = " + to_string(WriteFlag) + ".";
    Log(tmp);
    Schedule(GetTime()+TIME_FOR_MEMORYREQUEST, g_pOS, &OS::CallCPU, WriteFlag);
};

void Process :: Start() {
    Schedule(GetTime(), this, &Process::Work);
};