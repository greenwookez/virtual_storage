
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
        /*
            Действует следующее предположение: шанс отсутствия страницы - 50%.
            Это временная мера.
        */
        Log("I have to convert virtual address(" + to_string(address) + ")");

        
        int chance = randomizer(100);
        Log("Conversion chance: " + to_string(chance));

        if (chance > 50) {
            Schedule(GetTime()+TIME_FOR_CONVERSION, g_pOS, &OS::HandleInterruption);
        } else {
            
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
    void OS :: CallCPU(bool WriteFlag) {
        /*
            ОС выделяет виртуальный адрес процессу, так как она выполняет функции
            виртуальной памяти. Пока он рандомный....
        */
        PageNumber address = rand() % g_pConfig->GetRealMemorySize(); 
        
        Log("Calling CPU with WriteFlag = " + to_string(WriteFlag));
        Schedule(GetTime()+TIME_FOR_CALLCPU, g_pCPU, &CPU::Convert, address);
    };

    void OS :: HandleInterruption() {
        Log("Got an Interruption.");
    };

    void OS :: Wait() {
        Log("I am waiting!");
    };

    void OS :: Start() {
        Schedule(GetTime(), this, &OS::Wait);
    };    
/* end of class OS */

/* class Process */
    void Process :: MemoryRequest() {
        /*
            В рамках базовой модели процесса действует следующее предположение:
            80 процентов обращений к памяти происходят по чтению, и только 20 -
            по записи.
        */
        int chance = randomizer(100);
        bool write_flag = chance > 80;
        Log("Chance for WriteFlag:" + to_string(chance));
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

/* class MyProcess */
    void MyProcess :: MemoryRequest(bool WriteFlag) {
        /*
            В рамках этой специальной модели процесса, тип обращения
            (по чтению или по записи) определяется пользователем симулятора
            через формальный параметр WriteFlag.
        */
        Schedule(GetTime()+TIME_FOR_MEMORYREQUEST, g_pOS, &OS::CallCPU, WriteFlag);
    };

    void MyProcess :: Work() {
        /*
            В этом методе моделируется задача обращения матрицы большой размерности.
            Алгоритм взят из источника: http://www.algolib.narod.ru/Math/ObrMatrix.html
        */
        
        const int const_a = 10;//DEFAULT_MATRIX_SIZE_FOR_MYPROCESS;
        float input[const_a][const_a];
        float output[const_a][const_a];
        int i,j,k;
        for (k = 0; k <= const_a-1; k++) {
            for (i = 0; i <= const_a-1; i++) {
                for (j = 0; j <= const_a-1; j++) {
                    if (i == k && j == k) {
                        output[i][j] = 1 / input[i][j];
                    } else if (i == k && j != k) {
                        output[i][j] = -input[i][j]/input[k][k];
                    } else if (i != k && j == k) {
                        output[i][j] = input[i][k]/input[k][k];
                    } else if (i != k && j != k) {
                        output[i][j] = input[i][j] - input[k][j] * input[i][k]/input[k][k];
                    }
                    Process :: MemoryRequest();
                }
            }
        }
    };
/* end of class MyProcess */

