## virtual_storage

### CHANGE LOG [27.04.2020]:
1. Добавлен класс Computer, который хранит в себе конфигурацию компьютера.
2. Изменён класс Process, в него добавлен новый элемент данных - memory_usage. Добавлен также конструктор без параметров по умолчанию.
3. Добавлен базовый класс AE, наследник Agent. Является моделью работы архивной среды по-умолчанию. Последующие модели работы АС должны быть реализованы в классах-наследниках класса AE. 