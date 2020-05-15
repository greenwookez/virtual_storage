#include "sim.hpp"
#include "config.hpp"

using namespace std;

struct TranslationRecord { // Запись в таблице переадресации
    uint64_t virtual_address; // Адрес из ВАП
    uint64_t real_address; // Адрес из РАП (при наличии)
    uint64_t archive_address; //Адрес из ААП (при наличии)
    uint64_t attribute; // Атрибут записи (при наличии)
};

class Computer {
    /*
        Класс, в котором описана конфигурация комьютера.
        Здесь также будут высчитываться загруженность системы
        и накладные расходы на страничный обмен.
    */

    uint64_t rm_size; // размер реальной памяти
    uint64_t ae_size; // размер архивной среды
    uint64_t page_size; // размер страницы
    
    public:
    void SetRealMemorySize(uint64_t value) { rm_size = value; };
    void SetArchiveEnviromentSize(uint64_t value) { ae_size = value; };
    void SetPageSize(uint64_t value) { page_size = value; };

    uint64_t GetRealMemorySize() { return rm_size; };
    uint64_t GetArchiveEnviromentSize() { return ae_size; };
    uint64_t GetPageSize() { return page_size; };

    Computer(); // Конструктор по-умолчанию
};

class CPU : public Agent {
    /*
        Класс, который моделирует работу процессора.
        В нём также хранится таблица переадресации.
    */
    
    TranslationRecord translation_table[DEFAULT_TRANSLATION_TABLE_SIZE];

    public:

    void Convert(PageNumber address); // Метод, решающий задачу преобразования виртуального адреса
    
    void Wait();
    void Start();
};

class OS : public Agent {
    /*
        Класс, который моделирует работу ОС.
        В том числе, он полностью занимается контроллированием
        работы виртуальной памяти.
     */

    public:
    void CallCPU(bool WriteFlag); // Вызов процессора для решения задачи преобразования виртуального адреса
    void HandleInterruption(); // Обработка прерывания по отсутствию страницы для дальнейшего делегирования классу AE
    void Wait();
    void Start();
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
    
    uint64_t memory_usage; // количество страниц в памяти, необходимое для размещения этого процесса

    public:
    virtual void MemoryRequest(); // запрос памяти

    void SetMemoryUsage(uint64_t value) { memory_usage = value; };
    uint64_t GetMemoryUsage() { return memory_usage; };

    void Work();
    void Wait();
    void Start();
    Process(); // конструктор по-умолчанию
};

class MyProcess : public Process {
    public:
    void MemoryRequest(bool WriteFlag);
};

