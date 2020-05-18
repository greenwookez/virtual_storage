# Virtual Storage Simulator
## CHANGE LOG:
[18.05.2020]:
1. Доавлен метод InitilalizeProcess(), моделирующий действия ОС при загрузке нового процесса в память.
2. Добавлен механизм создания ТП при загрузки в память нового процесса. Память под ТП выделяется динамически.
3. Добавлен деструктор класса OS, для освобождения динамически выделенной памяти под ТП.
4. В классе Process появилось новое поле данных translatin_table_index - индекс, связывающий процесс с конкретной ТП.
5. В методе Work() класса MyProcess (наследник Process) реализована задача образения матрицы. DEFAULT_MATRIX_SIZE_FOR_MYPROCESS - параметр, определяющий размер измерения квардатной матрицы.

[15.05.2020]:
1. Немного изменён формат логирования: имя объекта форматируется по правой стороне,а отступ между именем и сообщением увеличен до 3-х пробелов.
2. Метод MemoryRequest() у класса Process теперь виртуальный.
3. Добавлена специальная модель процесса, реализованная в классе MyProcess. В этой модели типом обращения (по чтению или по записи) управляет пользователь симулятора.

[13.05.2020]:
1. Добавлена структура записи для таблиц переадресации TranslationRecord.
2. Добавлена псевдореализация метода Convert() класса CPU.
3. Добавлен метод HandleInterruption у класса OS для обработки прерывания, вызванного процессором.
4. Таблица переадресации расположена в классе CPU до дальнейшего согласования.

[27.04.2020]:
1. Добавлен класс Computer, который хранит в себе конфигурацию компьютера.
2. Изменён класс Process, в него добавлен новый элемент данных - memory_usage. Добавлен   также конструктор без параметров по умолчанию.
3. Добавлен базовый класс AE, наследник Agent. Является моделью работы архивной среды по-умолчанию. Последующие модели работы АС должны быть реализованы в классах-наследниках класса AE. 



## ADDITIONAL:
1. Должна быть определена конфигурация компьютера (размер реальной памяти, размер архивной среды, размер страницы). - done
2. Создание процесса. - done 50/50
    2.1. Должна быть выбрана одна из имеющихся (предлагаемых симулятором!) моделей процесса.
    2.2. Должны быть заданы параметры модели.
3. Если надо, должны быть созданы другие процессы. - done
4. Должна быть выбрана одна из нескольких предлагаемых симулятором стратегий замещения страниц. - TODO
5. Запускаем симулятор.
6. Во время (или по окончании) симулятор показывает осредненную за некоторый интервал времени картину:
- % загруженности реальной памяти,
- % загруженности архивной среды,
- % накладных расходов ОС на страничный обмен
- еще много что (потом уточним)

Другой use case.

1. Подготавливаем все исходные данные (п.п. 1-4)
2. Запускаем симулятор в пошаговом режиме.
3. Прослеживаем все действия ЦП, ОС, АС.
4. Когда надоест - останавливаем.
