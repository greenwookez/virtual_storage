#include "sim.hpp"
#include "config.hpp"

using namespace std;


struct TranslationRecord { // Запись в таблице переадресации
    uint64_t virtual_address; // Адрес из ВАП
    uint64_t real_address; // Адрес из РАП (при наличии)
    uint64_t attribute; // Атрибут записи (при наличии)
};

class TranslationTable {
    TranslationRecord records[DEFAULT_TRANSLATION_TABLE_SIZE]; // массив записей
    int index; // индекс ТП
    int free_record = 0; // индекс текущей первой свободной записи в массиве records

    public:
    void SetIndex(int value) { index = value; };
    int GetIndex() { return index; };

    void AddRecord(TranslationRecord input);
};
struct MemoryRecord {
    bool is_allocated = 0; // флаг распределения, по умолчанию все страницы НЕ распределены
    // здесь можно добавить какие-либо ещё данные
};

class MemoryAddressSpace { // РАП
    MemoryRecord memory[DEFAULT_REAL_MEMORY_SIZE];

    public:
    void SetPageByAddress(MemoryRecord input, PageNumber address) { memory[address] = input; };
    MemoryRecord GetPageByAddress(PageNumber address) { return memory[address]; };

    PageNumber Allocate();
};

class Process; // Заголовок класса Process (для класса OS)

class Computer {
    /*
        Класс, в котором описана конфигурация комьютера.
        Здесь также будут высчитываться загруженность системы
        и накладные расходы на страничный обмен.
    */

    uint64_t rm_size = DEFAULT_REAL_MEMORY_SIZE; // размер реальной памяти
    uint64_t ae_size = DEFAULT_ARCHIVE_ENVIROMENT_SIZE; // размер архивной среды
    uint64_t page_size = DEFAULT_PAGE_SIZE; // размер страницы в байтах в виде степени двойки
    
    MemoryAddressSpace rm; // РАП

    public:
    void SetRealMemorySize(uint64_t value) { rm_size = value; };
    void SetArchiveEnviromentSize(uint64_t value) { ae_size = value; };
    void SetPageSize(uint64_t value) { page_size = value; };
    MemoryAddressSpace & GetAddressSpace() { return rm; };

    uint64_t GetRealMemorySize() { return rm_size; };
    uint64_t GetArchiveEnviromentSize() { return ae_size; };
    uint64_t GetPageSize() { return page_size; };
};

class CPU : public Agent {
    /*
        Класс, который моделирует работу процессора.
    */
    public:

    void Convert(PageNumber address); // Метод, решающий задачу преобразования виртуального адреса
    
    void Wait();
    void Start();
};

class OS : public Agent {
    /*
        Класс, который моделирует работу ОС, а также решает задачи
        управления ВП, в том числе обеспечивает ТП.
    */

    TranslationTable *translation_tables[MAX_PROCESS_NUM];
    int current_translation_table_index = 0;

    public:
    void CallCPU(bool WriteFlag); // Вызов процессора для решения задачи преобразования виртуального адреса
    void HandleInterruption(); // Обработка прерывания по отсутствию страницы для дальнейшего делегирования классу AE
    void IntitializeProcess(Process * _process); // Моделируется загрузка процесса в память
    
    void Wait();
    void Start();

    ~OS(); // Деструктор для освобождения динамически выделенной памяти
};

extern OS *g_pOS;
extern CPU *g_pCPU;

class AE : public Agent {
    /*
        Класс, который моделирует работу архивной среды.
    */

    public:
    void Wait();
    void Start();
};

class Process : public Agent {
    /* 
        Класс - базовая модель процесса.
        Остальные модели будут являться наследниками этого класс
     */ 
    int translation_table_index; // идентификатор процесса (для связки с ТП)
    uint64_t memory_usage = DEFAULT_MEMORY_USAGE; // количество страниц в памяти, необходимое для размещения этого процесса в памяти

    public:

    void SetTranslationTableIndex(int value) { translation_table_index = value; }
    void SetMemoryUsage(uint64_t value) { memory_usage = value; };
    uint64_t GetMemoryUsage() { return memory_usage; };
    
    void MemoryRequest(bool WriteFlag); // запрос памяти
    void Start(); // Стартует процесс загрузки себя в память (равносильно запуску исполняемого файла)
    
    virtual void Work();
    void Wait();
    
    
};

class MyProcess : public Process {
    public:
    void Work();
};

