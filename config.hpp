typedef uint64_t PageNumber;
typedef uint64_t SimulatorTime;

/* КОНФИГУРАЦИЯ КОМПЬЮТЕРА */
const uint64_t DEFAULT_REAL_MEMORY_SIZE = 20000; // Размер реальной памяти в страницах
const uint64_t DEFAULT_ARCHIVE_ENVIROMENT_SIZE = 50; // Размер архивной среды в страницах
const uint64_t DEFAULT_PAGE_SIZE = 2048; // Размер страницы в байтах

/* КОНФИГУРАЦИЯ ПРОЦЕССОВ */
const uint64_t DEFAULT_MEMORY_USAGE = 4; // Память, занимаемая процессом в количестве страниц по-умолчанию

/* ТАЙМИНГИ ВСЕХ ПРОЦЕССОВ */
const SimulatorTime TIME_FOR_CALLCPU = 100;
const SimulatorTime TIME_FOR_MEMORYREQUEST = 200;