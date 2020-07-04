/* КОНФИГУРАЦИЯ КОМПЬЮТЕРА */
const uint64_t DEFAULT_REAL_MEMORY_SIZE = 512; // Размер реальной памяти в страницах
const uint64_t DEFAULT_ARCHIVE_ENVIROMENT_SIZE = 50; // Размер архивной среды в страницах (размер массива структуры SwapIndexRecord)
const uint64_t DEFAULT_ARCHIVE_DISK_SPACE_SIZE = 50; // Размер файла подкачки в страницах
const uint64_t DEFAULT_PAGE_SIZE = 11; // Размер страницы в байтах (представлен в виде показателя степени 2)
const uint64_t DEFAULT_TRANSLATION_TABLE_SIZE = 100; // Размер таблицы переадресации
const uint64_t MAX_PROCESS_NUM = 60; // Максимальное количество процессов

/* КОНФИГУРАЦИЯ ПРОЦЕССОВ */
const uint64_t DEFAULT_MEMORY_USAGE = 50; // Память, занимаемая процессом в количестве страниц 


/* ВРЕМЕННЫЕ ЕДИНИЦЫ */
const SimulatorTime nanoSec = 1;
const SimulatorTime microSec = 1000000;
const SimulatorTime Sec = 1000000000;

/* ТАЙМИНГИ ВСЕХ ПРОЦЕССОВ В НАНОСЕКУНДАХ */
const SimulatorTime TIME_FOR_MEMORYREQUEST = 0;
const SimulatorTime TIME_FOR_CONVERSION = 0;
const SimulatorTime TIME_FOR_PROCESS_INITIALIZATION = 0;
const SimulatorTime TIME_FOR_SETTING_CURRENT_TABLE = 0;
const SimulatorTime TIME_FOR_LOADING_DATA_IN_AE = 10*microSec; // LoadData
const SimulatorTime TIME_FOR_POP = 10*microSec; // PopData
const SimulatorTime TIME_FOR_ALLOCATION = 10*microSec; // Allocate


/* ДРУГОЕ */
const int DEFAULT_MATRIX_SIZE_FOR_MYPROCESS = 3; // Размер квадратной матрицы в методе Work класса MyProcess
