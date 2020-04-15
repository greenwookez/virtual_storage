#include <cstdint>
typedef uint64_t SimulatorTime; /* время в наносекундах */
typedef uint32_t PageNumber; /* номер страницы */

class Interface_OS_Process { /* Двунаправленный интерфейс ОС - Процесс */
    /* обращения от ОС к процессу */
    void Initialize(); /* операционная система инициализирует работу процесса */


    /* обращения от процесса к ОС */
    void Work(uint32_t n); /* процесс сообщает, что выполнил указанное n
число команд. ОС, моделируя работу процесса, обновит значение затраченного им
времени — так называемого виртуального времени. Здесь полагается, что все
машинные команды выполняются одинаковое время */

    void Wait(SimulatorTime time); /*процесс сообщает, что на время t он
переходит в состояние ожидания. Здесь time — время моделируемой системы. ОС вернет
управление процессу только по истечении времени t */

    void MemoryRequest(PageNumber address, bool WriteFlag); /* обращение к
памяти — строго говоря, выполняется не ОС, а памятью. Обработка этого обращения
может привести к прерыванию по отсутствию страницы. */

    void Stop(); /* завершение процесса */

    SimulatorTime GetVirtualTime(); /* возвращает виртуальное время, т. е.
общее время владения этого процесса процессором */

};

class Interface_OS_CPU{ /* Двунаправленный интерфейс ОС - ЦП */
    /*обращение от ОС к ЦП */
    void Call(PageNumber adress); /* ОС вызывает процессор для инициализации задачи
преобразоваия */

    /* обращения от ЦП к ОС */
    void Interrupt(); /* процессор отправляет прерывание операционной системе */

    PageNumber Convert(); /* процессор выполняет задачу преобразования,
в случае успешного выполнения задачи - возвращает страницу из РП, а в случае неудачи
вызывает метод Intrerrupt() */

};


class Interface_OS_AE{ /* Двунаправленный интерфейс ОС - АС */
    /*обращение от ОС к АС */
    void MoveToArchive(PageNumber adress); /* ОС перемещает страницу в архив */

    /* обращения от АС к ОС */
    void RemoveFromArchive(PageNumber adress); /* АС перемещает страницу из архива */
    
};

class Table { /* Таблица переадресации */
    /* ЗДЕСЬ БУДЕТ ИЛИ СТРУКТУРА, В КОТОРОЙ ОТОБРАЖЕНА САМА ТАБЛИЦА, ИЛИ НАБОР ДАННЫХ */

    public:
    PageNumber Accordance(PageNumber virtual_address); /* Нахождение соответствия в ТП */

    void CorrectTable(PageNumber virtual_adress, PageNumber real_adress); /* Корректирование
таблицы переадресации */
};

class Process : public Interface_OS_Process{

};

class OperatingSystem : public Interface_OS_CPU, Interface_OS_Process, Interface_OS_AE{

};

class CentralProcessingUnit : public Interface_OS_CPU {

};

class ArchivalEnviroment : public Interface_OS_AE{

};