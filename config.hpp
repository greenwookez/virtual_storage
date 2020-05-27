/* КОНФИГУРАЦИЯ КОМПЬЮТЕРА */
const uint64_t DEFAULT_REAL_MEMORY_SIZE = 20000; // Размер реальной памяти в страницах
const uint64_t DEFAULT_ARCHIVE_ENVIROMENT_SIZE = 50; // Размер архивной среды в страницах
const uint64_t DEFAULT_PAGE_SIZE = 11; // Размер страницы в байтах (представлен в виде показателя степени 2)
const uint64_t DEFAULT_TRANSLATION_TABLE_SIZE = 100; // Размер таблицы переадресации

/* КОНФИГУРАЦИЯ ПРОЦЕССОВ */
const uint64_t DEFAULT_MEMORY_USAGE = 1; // Память, занимаемая процессом в количестве страниц 

/* ТАЙМИНГИ ВСЕХ ПРОЦЕССОВ */
const SimulatorTime TIME_FOR_CALLCPU = 100;
const SimulatorTime TIME_FOR_MEMORYREQUEST = 200;
const SimulatorTime TIME_FOR_CONVERSION = 300;
const SimulatorTime TIME_FOR_PROCESS_INITIALIZATION = 200;

/* ДРУГОЕ */
const int DEFAULT_MATRIX_SIZE_FOR_MYPROCESS = 3; // Размер квадратной матрицы в методе Work класса MyProcess
const int MAX_PROCESS_NUM = 15; // Максимальное количество процессов