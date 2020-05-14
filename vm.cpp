#include "vm.hpp"

#include <cstdint>
#include <ctime>
#include <unistd.h>

extern OS *g_pOS;
extern CPU *g_pCPU;
extern Computer *g_pConfig;

int randomizer(int max) { // Функция, возвращающая любое число от 0 до max.
    srand(unsigned(clock()));
    return rand()%max;
};

/* class Computer */
    Computer :: Computer() {
        // Задание параметров памяти по умолчанию
        rm_size = DEFAULT_REAL_MEMORY_SIZE;
        ae_size = DEFAULT_ARCHIVE_ENVIROMENT_SIZE;
        page_size = DEFAULT_PAGE_SIZE;
    };
/* end of class Computer */

/* class CPU */
    void CPU :: Convert(PageNumber address) {
        Log("I have to convert virtual address(" + to_string(address) + ")");

        // Действует следующее предположение: шанс отсутствия страницы - 50%.
        // Это временная мера.
        int chance = randomizer(100);
        Log("Шанс при конвертации: " + to_string(chance));

        if (chance < 50) {
            Schedule(GetTime()+TIME_FOR_CONVERSION, g_pOS, &OS::HandleInterruption);
        } else {
            Log("Память есть, нужно что-то сделать дальше...");
        };
    };

    void CPU :: Wait() {
        Log("I am waiting!");
    };

    void CPU :: Start() {
        Schedule(GetTime(), this, &CPU::Wait);
    }
/* end of class CPU */

/* class OS */
    void OS :: CallCPU(int WriteFlag) {
        PageNumber address = rand() % g_pConfig->GetRealMemorySize(); // ОС выделяет виртуальный адрес процессу, так как она выполняет функции виртуальной памяти. Пока он рандомный....
        
        Log("Calling CPU with WriteFlag = " + to_string(WriteFlag));
        Schedule(GetTime()+TIME_FOR_CALLCPU, g_pCPU, &CPU::Convert, address);
    };

    void OS :: HandleInterruption() {
        Log("Получено прерывание.");
    };

    void OS :: Wait() {
        Log("I am waiting!");
    };

    void OS :: Start() {
        Schedule(GetTime(), this, &OS::Wait);
    };    
/* end of class OS */

/* class Process */
    void Process :: MemoryRequest(int WriteFlag) {
        // Действует следующее предположение: 80% обращений происходят по чтению, 20% - по записи.
        // Это временная мера.
        bool write_flag = randomizer(100) > 80;
        Schedule(GetTime()+TIME_FOR_MEMORYREQUEST, g_pOS, &OS::CallCPU, write_flag);
    };

    Process :: Process() {
        memory_usage = DEFAULT_MEMORY_USAGE;
    };

    void Process :: Work() {
        Log("I am working.");
    };

    void Process :: Wait() {
        Log("I am waiting.");
    };

    void Process :: Start() {
        Schedule(GetTime(), this, &Process::Work);
    };
/* end of class Process */