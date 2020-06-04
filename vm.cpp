
#include "vm.hpp"
#include <cstdint>
#include <ctime>
#include <unistd.h>

extern OS *g_pOS;
extern CPU *g_pCPU;
extern Computer *g_pComputer;

/* class MemoryAddressSpace */

/* end of class MemoryAddressSpace */

/* class TranslationTable */
    void TranslationTable :: AddRecord(TranslationTableRecord input) {
        records[free_record] = input;
        free_record++;
    };

    void TranslationTable :: EditRecord(VirtualAddress _virtual_address, VirtualAddress input_vaddress, RealAddress input_raddress, bool valid_flag) {
        
        
        for (int i = 0; i < free_record; i++) {
            if (records[i].virtual_address == _virtual_address) {
                if (input_vaddress != -1) {
                    records[i].virtual_address = input_vaddress;
                };

                if (input_raddress != -1) {
                    records[i].real_address = input_raddress;
                };

                records[i].is_valid = valid_flag;
                return;
            }
        }
    };

    void TranslationTable :: EditRecord(RealAddress _real_address, VirtualAddress input_vaddress, RealAddress input_raddress, bool valid_flag) {
        for (int i = 0; i < free_record; i++) {
            if (records[i].real_address == _real_address) {
                if (input_vaddress != -1) {
                    records[i].virtual_address = input_vaddress;
                };

                if (input_raddress != -1) {
                    records[i].real_address = input_raddress;
                };

                records[i].is_valid = valid_flag;
                return;
            }
        }
    };

    TranslationTableRecord TranslationTable :: GetRecord(RealAddress _real_address) {
        for (int i = 0; i < free_record; i++) {
            if (records[i].real_address == _real_address) {
                return records[i];
            }
        }
    };

/* end of class TranslationTable */

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
    // void OS :: CallCPU(bool WriteFlag, int tt_index) {
    //     /*
    //         ОС выделяет виртуальный адрес процессу, так как она выполняет функции
    //         виртуальной памяти. Пока он рандомный...
    //         TODO: Полностью переработать
    //     */
    //     PageNumber address = rand() % g_pComputer->GetRealMemorySize(); 
        
    //     translation_tables[tt_index]->EditRecord();
        
    //     //Log("Calling CPU with WriteFlag = " + to_string(WriteFlag));
    //     Schedule(GetTime()+TIME_FOR_CALLCPU, g_pCPU, &CPU::Convert, address);
    // };

    void OS :: HandleInterruption() {
        Log("Got an Interruption.");
        //TODO: Добавить реализацию.
    };

    void OS :: IntitializeProcess(Process *_process) {
        /*
            В этом методе моделируются действия по загрузке кода процесса в память.
        */

        // Создаём новую ТП для этого процесса
        translation_tables[free_table_index] = new TranslationTable;
        translation_tables[free_table_index]->SetProcess(_process);
        current_table_index = free_table_index; // активная ТП - теперь ТП этого процесса

        

        
        // Пока предположим, что каждый процесс занимает одну страницу в памяти

        TranslationTableRecord tmp = { 0, Allocate(_process),0 }; // по виртуальному адресу 0 будет храниться
        translation_tables[free_table_index]->AddRecord(tmp);
        free_table_index++;

        Schedule(GetTime()+TIME_FOR_PROCESS_INITIALIZATION, _process, &Process::Work);
    };

    RealAddress OS :: Allocate(Process * _process) {
        /*
            Метод, решающий задачу размещения.
        */
        for (int i = 0; i < g_pComputer->GetRealMemorySize(); i++) {
            if (g_pComputer->GetMemoryAddressSpace().GetPageByAddress(i).is_allocated == false) {
                g_pComputer->GetMemoryAddressSpace().GetPageByAddress(i).is_allocated = true;
                // может быть ещё какие-то данные нужно будеть здесь установить
                return i;
            };
        };

        Substitute(_process);
    };

    RealAddress OS :: Substitute(Process *_process) {
        /*
            Для начала модель, решающая задачу замещения, будет выбирать любой
            адрес абсолютно рандомно.
        */
        int index = -1;
        RealAddress tmp;
        while (index == -1) { // В цикле найдём нужную ТП
            tmp = (RealAddress)randomizer(g_pComputer->GetRealMemorySize());
            index = FindTableByValidAddress(tmp);
        };

        translation_tables[index]->EditRecord(tmp, -1, -1, false); // -1 значит, что это значение изменяться не будет
        

        //Загрузить в АС содержимое адреса tmp
        g_pAE->LoadData(_process, translation_tables[index]->GetRecord(tmp).virtual_address);
    };

    int OS :: FindTableByValidAddress(RealAddress _address) {
        /*
            Ищет ТП, в который адрес _address является доступным.
        */

        for (int i = 0; i < MAX_PROCESS_NUM; i++) {
            for (int j = 0; j < DEFAULT_TRANSLATION_TABLE_SIZE; j++) {
                TranslationTableRecord *tmp = translation_tables[i]->GetRecords();
                if (tmp[j].real_address == _address && tmp[j].is_valid == true) {
                    return i;
                }
            }
        }

        return -1;
    };

    void OS :: Wait() {
        Log("I am waiting!");
        //TODO: Добавить реализацию.
    };

    void OS :: Start() {
        Schedule(GetTime(), this, &OS::Wait);
    };

    OS :: ~OS() {
        /*
            Деструктор, освобождающий динамически выделенную память.
        */
        int i;
        for (i = 0; i < free_table_index; i++) {
            delete translation_tables[i];
        };
    };
/* end of class OS */

/* class AE */
    void AE :: LoadData(Process *_p_process, VirtualAddress _virtual_address) {
        int i;
        for (i = 0; i < DEFAULT_ARCHIVE_DISK_SPACE_SIZE; i++) {
            if (g_pComputer->GetDiskAddressSpace().GetPageByAddress(i).is_allocated == false) {
                g_pComputer->GetDiskAddressSpace().GetPageByAddress(i).is_allocated = true;
                break;
            };
        };

        SwapIndex[free_swap_index] = {_p_process, _virtual_address, i};
        free_swap_index++;
    };

    void AE :: PopData(Process *_process, VirtualAddress _virtual_address) {
        for (int i = 0; i < DEFAULT_ARCHIVE_ENVIROMENT_SIZE; i++) {
            if (SwapIndex[i].p_process == _process && SwapIndex[i].virtual_address == _virtual_address) {
                g_pComputer->GetDiskAddressSpace().GetPageByAddress(SwapIndex[i].disk_address).is_allocated = false;
                SwapIndex[i].p_process = nullptr;
                SwapIndex[i].virtual_address = -1;
                return;
            };
        };
    };
/* end of class AE*/

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