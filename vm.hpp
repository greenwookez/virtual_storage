#include "sim.hpp"
#include "config.hpp"

using namespace std;
typedef uint64_t PageNumber;
typedef PageNumber VirtualAddress;
typedef PageNumber RealAddress;
typedef PageNumber DiskAddress;

struct TranslationTableRecord { // Запись в таблице переадресации
    public:
    TranslationTableRecord() {
        real_address = -1;
        is_valid = false;
    }
    VirtualAddress virtual_address; // Адрес из ВАП
    RealAddress real_address; // Адрес из РАП (при наличии)
    bool is_valid; // бит действительности
};


class AgentVM : public Agent {
    public:
    void Log(string text); // Переопределение логирования для вывода загруженности системы в конце каждого сообщения
};

class Process : public AgentVM {
    /*
        Класс - базовая модель процесса.
        Остальные модели будут являться наследниками этого класс
     */
    uint64_t memory_usage; // количество страниц в памяти, необходимое для размещения этого процесса в памяти
    uint64_t call_number;
    public:
    Process() {
        memory_usage = DEFAULT_MEMORY_USAGE;
        call_number = CPU_TIME*OPERATIONS_PER_SECOND;
    };

    void SetMemoryUsage(uint64_t value) { memory_usage = value; };
    uint64_t GetMemoryUsage() { return memory_usage; };
    void SetCallNumber(uint64_t value) { call_number = value; };

    void MemoryRequest(VirtualAddress virtual_address, bool write_flag); // запрос памяти
    void Start(); // Стартует процесс загрузки себя в память (равносильно запуску исполняемого файла)

    virtual void Work();
    void Wait();
};

class TranslationTable {
    Process *p_process;
    TranslationTableRecord records[DEFAULT_TRANSLATION_TABLE_SIZE]; // массив записей
    int index; // индекс ТП
    int free_record; // индекс текущей первой свободной записи в массиве records

    public:
    TranslationTable();

    void SetProcess(Process * _process) { p_process = _process; };
    Process *GetProcess() { return p_process; };
    void SetIndex(int value) { index = value; };
    int GetIndex() { return index; };
    TranslationTableRecord *GetRecords() { return records; };

    void AddRecord(TranslationTableRecord input);
    void EditRecord(RealAddress _real_address, VirtualAddress input_vaddress, RealAddress input_raddress, bool valid_flag ); // изменяет запись, зная реальный адрес
    TranslationTableRecord &GetRecord(VirtualAddress index) { return records[index]; };
    TranslationTableRecord *GetRecordByRealAddress(RealAddress r_address);
};

class MemoryRecord {
    public:
    bool is_allocated; // флаг распределения, по умолчанию все страницы НЕ распределены

    MemoryRecord() {
        is_allocated = 0;
    };
    // здесь можно добавить какие-либо ещё данные
};

class MemoryAddressSpace { // РАП
    MemoryRecord memory[DEFAULT_REAL_MEMORY_SIZE];

    public:
    MemoryRecord & GetPageByAddress(RealAddress address) { return memory[address]; };
};

class DiskAddressSpace { // ААП
    MemoryRecord memory[DEFAULT_ARCHIVE_DISK_SPACE_SIZE];

    public:
    MemoryRecord & GetPageByAddress(DiskAddress address) { return memory[address]; };
};

class Computer {
    /*
        Класс, в котором описана конфигурация комьютера.
        Здесь также будут высчитываться загруженность системы
        и накладные расходы на страничный обмен.
    */
    uint64_t rm_size; // размер реальной памяти
    uint64_t ae_size; // размер архивной среды
    uint64_t page_size; // размер страницы в байтах в виде степени двойки

    uint64_t time_count; // счетчик времени, затраченного на страничный обмен

    MemoryAddressSpace rm; // РАП
    DiskAddressSpace archive;

    public:
    Computer() {
        time_count = 0;
        rm_size = DEFAULT_REAL_MEMORY_SIZE;
        ae_size = DEFAULT_ARCHIVE_ENVIROMENT_SIZE;
        page_size = DEFAULT_PAGE_SIZE;
    };

    void SetRealMemorySize(uint64_t value) { rm_size = value; };
    void SetArchiveEnviromentSize(uint64_t value) { ae_size = value; };
    void SetPageSize(uint64_t value) { page_size = value; };
    MemoryAddressSpace & GetMemoryAddressSpace() { return rm; };
    DiskAddressSpace & GetDiskAddressSpace() { return archive; };
    uint64_t GetRealMemorySize() { return rm_size; };
    uint64_t GetArchiveEnviromentSize() { return ae_size; };
    uint64_t GetPageSize() { return page_size; };
    void PrintCurrentConfig();

    void AddTimeCount(uint64_t value) { time_count += value; };
    uint64_t GetTimeCount() { return time_count; };

    uint64_t CountRM(); // считает количество распределенных ячеек памяти
    uint64_t CountAE(); // 
};

class CPU : public AgentVM {
    /*
        Класс, который моделирует работу процессора.
    */
    public:
    CPU() { SetName("CPU"); };
    void Convert(VirtualAddress address, bool write_flag, Process *_process); // Метод, решающий задачу преобразования виртуального адреса

    void Wait();
    void Start();
};

class OS : public AgentVM {
    /*
        Класс, который моделирует работу ОС, а также решает задачи
        управления ВП, в том числе обеспечивает ТП.
    */

    TranslationTable *translation_tables[MAX_PROCESS_NUM];
    int free_table_index;
    int current_table_index;

    public:
    OS() {
        SetName("OS");
        free_table_index = 0;
        current_table_index = 0;
    };

    void CallCPU(bool WriteFlag, int tt_index); // Вызов процессора для решения задачи преобразования виртуального адреса
    void HandleInterruption(VirtualAddress vaddress, Process * _process); // Обработка прерывания по отсутствию страницы для дальнейшего делегирования классу AE
    void IntitializeProcess(Process * _process); // Моделируется загрузка процесса в память
    void Allocate(VirtualAddress vaddress, Process * _process); // размещение
    void Substitute(VirtualAddress vaddress); // замещение
    void SetCurrentTable(Process *_process);
    int FindTableByValidAddress(RealAddress _address);
    int FindTableByPointer(Process * _process);
    void Wait();
    void Start();

    TranslationTable GetCurrentTranslationTable() { return *translation_tables[current_table_index]; };
    ~OS(); // Деструктор для освобождения динамически выделенной памяти
};

extern OS *g_pOS;
extern CPU *g_pCPU;

class AE : public AgentVM {
    /*
        Класс, который моделирует работу архивной среды.
    */

    class SwapIndexRecord {
        public:
        Process *p_process;
        VirtualAddress virtual_address;
        DiskAddress disk_address;

        
        SwapIndexRecord() {
            p_process = nullptr;
            virtual_address = -1;
        };
    };

    SwapIndexRecord SwapIndex[DEFAULT_ARCHIVE_ENVIROMENT_SIZE];
    int free_swap_index;

    public:
    AE() {
        SetName("AE");
        free_swap_index = 0;
    };
    void LoadData(VirtualAddress vaddress, Process *caller, RealAddress raddress);
    void PopData(VirtualAddress address);
    int FindRecord(Process *caller, VirtualAddress address); // 0 if ok, -1 if not ok

    void Start();
    void Wait();
};

extern AE *g_pAE;

class MyProcess : public Process {
    public:
    void Work();
};
