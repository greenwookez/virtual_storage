#include "sim.hpp"
#include "config.hpp"

using namespace std;

struct TranslationRecord { // Запись в таблице переадресации
    uint64_t virtual_address; // Адрес из ВАП
    uint64_t real_address; // Адрес из РАП (при наличии)
    uint64_t attribute; // Атрибут записи (при наличии)
};
class Process; // Заголовок класса Process (для класса OS)

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

    TranslationRecord *translation_table[MAX_PROCESS_NUM]; // массив ТП
    int current_translation_table_index; // текущий свободный слева индекс в массиве ТП.
    // Например, если существуют две ТП, то этот индекс будет равен 2, так как 0,1 индексы уже заняты.
    
    public:
    void CallCPU(bool WriteFlag); // Вызов процессора для решения задачи преобразования виртуального адреса
    void HandleInterruption(); // Обработка прерывания по отсутствию страницы для дальнейшего делегирования классу AE
    void IntitializeProcess(Process * _process); // Моделируется загрузка процесса в память
    
    void Wait();
    void Start();

    OS();
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
    uint64_t memory_usage; // количество страниц в памяти, необходимое для размещения этого процесса в памяти

    public:

    void SetTranslationTableIndex(int value) { translation_table_index = value; }
    void SetMemoryUsage(uint64_t value) { memory_usage = value; };
    uint64_t GetMemoryUsage() { return memory_usage; };
    
    Process(); // конструктор по-умолчанию
    void MemoryRequest(bool WriteFlag); // запрос памяти
    void Start(); // Стартует процесс загрузки себя в память (равносильно запуску исполняемого файла)
    
    virtual void Work();
    void Wait();
    
    
};

class MyProcess : public Process {
    public:
    void Work();
};

