
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

/* class CPU */
    void CPU :: Convert(PageNumber address) {
        /*
            Действует следующее предположение: шанс отсутствия страницы - 50%.
            Это временная мера.
            TODO: Полностью переработать
        */

        //Log("I have to convert virtual address(" + to_string(address) + ")");

        
        int chance = randomizer(100);
        Log("Conversion chance: " + to_string(chance));

        if (chance > 50) {
            Schedule(GetTime()+TIME_FOR_CONVERSION, g_pOS, &OS::HandleInterruption);
        } else {
            //TODO: Добавить реализацию.
        };
    };

    void CPU :: Wait() {
        Log("I am waiting!");
        //TODO: Добавить реализацию.
    };

    void CPU :: Start() {
        Schedule(GetTime(), this, &CPU::Wait);
    };
/* end of class CPU */

/* class OS */
    void OS :: CallCPU(bool WriteFlag) {
        /*
            ОС выделяет виртуальный адрес процессу, так как она выполняет функции
            виртуальной памяти. Пока он рандомный...
            TODO: Полностью переработать
        */
        PageNumber address = rand() % g_pConfig->GetRealMemorySize(); 
        
        //Log("Calling CPU with WriteFlag = " + to_string(WriteFlag));
        Schedule(GetTime()+TIME_FOR_CALLCPU, g_pCPU, &CPU::Convert, address);
    };

    void OS :: HandleInterruption() {
        Log("Got an Interruption.");
        //TODO: Добавить реализацию.
    };

    void OS :: IntitializeProcess(Process *_process) {
        /*
            В этом методе моделируются действия по загрузке кода процесса в память.
        */
        // Создаём новую ТП для этого процесса
        translation_table[current_translation_table_index] = new TranslationRecord[DEFAULT_TRANSLATION_TABLE_SIZE];
        _process->SetTranslationTableIndex(current_translation_table_index);
        current_translation_table_index++;
        
        //TODO: Расположить в памяти процесс


        Schedule(GetTime()+TIME_FOR_PROCESS_INITIALIZATION, _process, &Process::Work);
    };

    void OS :: Wait() {
        Log("I am waiting!");
        //TODO: Добавить реализацию.
    };

    void OS :: Start() {
        Schedule(GetTime(), this, &OS::Wait);
    };

    OS :: OS() {
        current_translation_table_index = 0;
    };

    OS :: ~OS() {
        /*
            Деструктор, освобождающий динамически выделенную память.
        */
        int i;
        for (i = 0; i <= MAX_PROCESS_NUM; i++) {
            delete[] translation_table[i];
        };
    };
/* end of class OS */

/* class Process */
    void Process :: MemoryRequest(bool WriteFlag) {
        Schedule(GetTime()+TIME_FOR_MEMORYREQUEST, g_pOS, &OS::CallCPU, WriteFlag);
    };

    void Process :: Work() {
        /*
            В рамках базовой модели процесса действует следующее предположение:
            80 процентов обращений к памяти происходят по чтению, и только 20 -
            по записи.
        */

        int chance = randomizer(100);
        bool write_flag = chance > 80;
        Log("Chance for WriteFlag:" + to_string(chance));
        MemoryRequest(write_flag);
    };
    
    void Process :: Start() {
        Schedule(GetTime(), g_pOS, &OS::IntitializeProcess, this);
    };

    void Process :: Wait() {
        Log("I am waiting.");
        //TODO: Добавить реализацию.
    };

    Process :: Process() {
        memory_usage = DEFAULT_MEMORY_USAGE;
    };
/* end of class Process */

/* class MyProcess */
    void MyProcess :: Work() {
        /*
            В этом методе моделируется задача обращения матрицы большой размерности.
            Алгоритм взят из источника: http://www.algolib.narod.ru/Math/ObrMatrix.html
        */
        
        const int const_a = DEFAULT_MATRIX_SIZE_FOR_MYPROCESS;
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
                    Process :: MemoryRequest(true);
                }
            }
        }
    };
/* end of class MyProcess */

/* class Computer */
    Computer :: Computer() {
        // Задание параметров памяти по умолчанию
        rm_size = DEFAULT_REAL_MEMORY_SIZE;
        ae_size = DEFAULT_ARCHIVE_ENVIROMENT_SIZE;
        page_size = DEFAULT_PAGE_SIZE;
    };
/* end of class Computer */