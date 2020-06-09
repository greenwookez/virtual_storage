
#include "vm.hpp"
#include <cstdint>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <iomanip>

using namespace std;

extern Sim *g_pSim;
extern OS *g_pOS;
extern CPU *g_pCPU;
extern Computer *g_pComputer;
extern AE *g_pAE;

/* help functions */
    int randomizer(int max) { // Функция, возвращающая любое число от 0 до max.
        srand(unsigned(clock()));
        return rand()%max;
    };
/* end of help functions */

/* class TranslationTable */
    TranslationTable :: TranslationTable()  {
        for (int i = 0; i < DEFAULT_TRANSLATION_TABLE_SIZE; i++)
            records[i].virtual_address = i;
    };

    void TranslationTable :: AddRecord(TranslationTableRecord input) {
        records[free_record] = input;
        free_record++;
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
/* end of class TranslationTable */

/* class CPU */
    void CPU :: Convert(VirtualAddress address, bool write_flag) {
        /*
            Метод, решающий задачу преобразования адреса.
        */

       TranslationTable tmp = g_pOS->GetCurrentTranslationTable();

        for (int i = 0; i < DEFAULT_TRANSLATION_TABLE_SIZE; i++) {
            if (tmp.GetRecord(i).virtual_address == address && tmp.GetRecord(i).is_valid == true) {
                Log("Successful conversion.");
                Process * caller = g_pOS->GetCurrentTranslationTable().GetProcess();
                Schedule(GetTime(), caller, &Process::Wait);
            };
        };
        
        Schedule(GetTime(), g_pOS, &OS::HandleInterruption, address);
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
    void OS :: IntitializeProcess(Process *_process) {
        /*
            В этом методе моделируются действия по загрузке кода процесса в память.
        */
        Log("Initializing process " + _process->GetName());
        // Создаём новую ТП для этого процесса
        translation_tables[free_table_index] = new TranslationTable;
        translation_tables[free_table_index]->SetProcess(_process);
        current_table_index = free_table_index; // активная ТП - теперь ТП этого процесса

        
        // Заполняем нужное количетво записей в ТП
        for (int i = 0; i < _process->GetMemoryUsage(); i++)
            Schedule(GetTime()+TIME_FOR_ALLOCATION, g_pOS, &OS::Allocate, i);

        Schedule(GetTime()+TIME_FOR_PROCESS_INITIALIZATION, _process, &Process::Work);
        free_table_index++;
        return;
    };

    void OS :: HandleInterruption(VirtualAddress vaddress) {
        Log("Got an Interruption.");

        // При обработке прерывания выполняем задачу размещения
        Schedule(GetTime()+TIME_FOR_ALLOCATION, g_pOS, &OS::Allocate, vaddress);
        
        // И передадим управление обратно на процесс
        Process * caller = translation_tables[current_table_index]->GetProcess();
        Schedule(GetTime(), caller, &Process::Wait);
    };

    void OS :: Allocate(VirtualAddress vaddress) {
        Log("Allocating virtual address " + to_string(vaddress));
        Process * caller = translation_tables[current_table_index]->GetProcess();
        for (int i = 0; i < g_pComputer->GetRealMemorySize(); i++) {
            if (g_pComputer->GetMemoryAddressSpace().GetPageByAddress(i).is_allocated == false) {
                // Если нашёлся свободный реальный адрес
                g_pComputer->GetMemoryAddressSpace().GetPageByAddress(i).is_allocated = true;
                // может быть ещё какие-то данные нужно будеть здесь установить
                TranslationTableRecord tmp;
                tmp.virtual_address = vaddress;
                tmp.real_address = i;
                tmp.is_valid = true;
                translation_tables[current_table_index]->GetRecord(vaddress) = tmp;
                Log("I have found out free real address " + to_string(i));
                return;
            };
        };

        // Если свободного реального адреса нет в РАП, то решаем задачу замещения
        Log("No free real addresses. Starting the redestribution");
        int index = -1;
        RealAddress tmp;
        while (index == -1) { // В цикле найдём нужную ТП
            // Реализация выбора кандидата на перераспределение. В данном случае - рандомно.
            tmp = (RealAddress)randomizer(g_pComputer->GetRealMemorySize());
            index = FindTableByValidAddress(tmp);
        };
        Log("Redistributing real address tmp " + to_string(tmp));
        translation_tables[index]->EditRecord(tmp, -1, -1, false); // -1 значит, что это значение изменяться не будет в структуре записи
        TranslationTableRecord tmp2;
        tmp2.virtual_address = vaddress;
        tmp2.real_address = tmp;
        tmp2.is_valid = true;
        Log("Changing record with virtual address " + to_string(vaddress) + " in current translation table");
        translation_tables[current_table_index]->GetRecord(vaddress) = tmp2;

        // Загрузим в АС содержимое реального адреса
        Schedule(GetTime()+TIME_FOR_LOADING_DATA_IN_AE, g_pAE, &AE::LoadData, tmp);
        return;
    };

    void OS :: SetCurrentTable(Process *_process) {
        int i;
        for (i = 0; i < MAX_PROCESS_NUM; i++) {
            if (translation_tables[i]->GetProcess() == _process)
                break;
        };

        current_table_index = i;
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
    void AE :: LoadData(VirtualAddress address) {
        Log("Loading virtual address " + to_string(address) + "data into AE");
        int i;
        Process * caller = g_pOS->GetCurrentTranslationTable().GetProcess();
        for (i = 0; i < DEFAULT_ARCHIVE_DISK_SPACE_SIZE; i++) {
            if (g_pComputer->GetDiskAddressSpace().GetPageByAddress(i).is_allocated == false) {
                g_pComputer->GetDiskAddressSpace().GetPageByAddress(i).is_allocated = true;
                break;
            };
        };
        SwapIndexRecord tmp;
        tmp.p_process = caller;
        tmp.virtual_address = address;
        tmp.disk_address = i;
    
        SwapIndex[free_swap_index] = tmp;
        free_swap_index++;
        return;
    };

    void AE :: PopData(VirtualAddress address) {
        Process * caller = g_pOS->GetCurrentTranslationTable().GetProcess();
        for (int i = 0; i < DEFAULT_ARCHIVE_ENVIROMENT_SIZE; i++) {
            if (SwapIndex[i].p_process == caller && SwapIndex[i].virtual_address == address) {
                g_pComputer->GetDiskAddressSpace().GetPageByAddress(SwapIndex[i].disk_address).is_allocated = false;
                SwapIndex[i].p_process = nullptr;
                SwapIndex[i].virtual_address = -1;
                break;
            };
        };
        return;
    };

    void AE :: Start() {
        Schedule(GetTime(), this, &AE::Wait);
    }

    void AE :: Wait() {
        Log("I am waiting");
    };
/* end of class AE*/

/* class Process */
    void Process :: MemoryRequest(VirtualAddress virtual_address, bool write_flag) {
        Schedule(GetTime()+TIME_FOR_SETTING_CURRENT_TABLE, g_pOS, &OS::SetCurrentTable, this);
        Schedule(GetTime()+TIME_FOR_MEMORYREQUEST, g_pCPU, &CPU::Convert, virtual_address, write_flag);
    };

    void Process :: Work() {
        /*
            В рамках базовой модели процесса действует следующее предположение:
            80 процентов обращений к памяти происходят по чтению, и только 20 -
            по записи.
        */

        // int chance = randomizer(100);
        // bool write_flag = chance > 80;
        // Log("Chance for WriteFlag:" + to_string(chance));
        // MemoryRequest(write_flag);
    };
    
    void Process :: Start() {
        Log("Starting...");
        Schedule(GetTime(), g_pOS, &OS::IntitializeProcess, this);
    };

    void Process :: Wait() {
        Log("I am waiting.");
        //TODO: Добавить реализацию.
    };
/* end of class Process */

/* class MyProcess */
    void MyProcess :: Work() {
    //     /*
    //         В этом методе моделируется задача обращения матрицы большой размерности.
    //         Алгоритм взят из источника: http://www.algolib.narod.ru/Math/ObrMatrix.html
    //     */
        
    //     const int const_a = DEFAULT_MATRIX_SIZE_FOR_MYPROCESS;
    //     float input[const_a][const_a];
    //     float output[const_a][const_a];
    //     int i,j,k;
    //     for (k = 0; k <= const_a-1; k++) {
    //         for (i = 0; i <= const_a-1; i++) {
    //             for (j = 0; j <= const_a-1; j++) {
    //                 if (i == k && j == k) {
    //                     output[i][j] = 1 / input[i][j];
    //                 } else if (i == k && j != k) {
    //                     output[i][j] = -input[i][j]/input[k][k];
    //                 } else if (i != k && j == k) {
    //                     output[i][j] = input[i][k]/input[k][k];
    //                 } else if (i != k && j != k) {
    //                     output[i][j] = input[i][j] - input[k][j] * input[i][k]/input[k][k];
    //                 }
    //                 Process :: MemoryRequest(true);
    //             }
    //         }
    //     }
    };
/* end of class MyProcess */

/* class Computer */

    void Computer :: PrintCurrentConfig() {
        cout << "CURRENT CONFIG:" << endl;
        cout << setw(10) << setfill(' ') << left <<  "RM Size"
        << setw(5) << right << " = " << to_string(rm_size) << endl;

        cout << setw(10) << setfill(' ') << left <<  "AE Size"
        << setw(5) << right << " = " << to_string(ae_size) << endl;

        cout << setw(10) << setfill(' ') << left <<  "Page Size"
        << setw(5) << right << " = " << "2^" <<to_string(page_size) << endl;
    };
/* end of class Computer */