#include "vm.hpp"

extern OS *g_pOS;
extern CPU *g_pCPU;
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