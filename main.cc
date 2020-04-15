#include <iostream>
#include <stdint.h>
#include <assert.h>
#include "sim.h"

using namespace std;

// ссылка на единственный объект симулятора
Sim *g_pSim;

// мой класс агента наследует базовый класс
class MyAgent : public Agent
{
public:
    // конструктор, деструктор
    MyAgent() {}
    ~MyAgent() {}

    // метод, выполняющий некоторую деятельность
    // в данном примере - выводящий строку в журнал
    void KuKu(uint32_t n);
    void Ku();
    void KuStr(std::string str, int k);

    // метод запуска агента
    // (переопределение виртуального метода базового класса)
    void Start();
};

// реализация метода, выполняющего некоторую деятельность
void MyAgent::KuKu(uint32_t n)
{
    // вывод строки
    Log("Ку-ку");

    // если не истекло заданное количество срабатываний
    if(n>0)
    {
        // то планируем еще один вызов этого метода через 1500 нс,
        // а также уменьшаем счетчик срабатываний
        Schedule(GetTime()+1500, this, &MyAgent::KuKu,n-1);
    }
}

// реализация метода, выполняющего некоторую деятельность
void MyAgent::Ku()
{
    // вывод строки
    Log("Ку!");
}

void MyAgent::KuStr(std::string str, int val)
{
    Log(str);

    if(val>0)
    {
        // то планируем еще один вызов этого метода через 1500 нс,
        // а также уменьшаем счетчик срабатываний
        Schedule(GetTime()+500, this, &MyAgent::KuStr,str,val-1);
    }
}

// метод запуска моего агента
// в этом методе необходимо создать как минимум одно событие
void MyAgent::Start()
{
    // планирование события: в момент врмени 5000 нс для объекта this
    // будет вызван метод KuKu и ему передано количество срабатываний 20
    Schedule(5000, this, &MyAgent::KuKu, 100);
}

// главная функция программы, вызывается из оболочки
int main()
{
    // создаем единственный объект симулятора
    g_pSim = new Sim;

    // создаем объект агента
    MyAgent *pAgent = new MyAgent;

    // устанавливаем имя агента (будет выводиться в журнал)
    // желательно не более 9 символов
    pAgent->SetName("Agent1");

    // запуск агента
//    pAgent->Start();
    Schedule(5000, pAgent, &MyAgent::KuKu, 0);
    Schedule(2000, pAgent, &MyAgent::KuKu, 0);
    Schedule(25000, pAgent, &MyAgent::KuKu, 0);
    Schedule(4000, pAgent, &MyAgent::Ku);
    Schedule(2000, pAgent, &MyAgent::KuKu, 0);
    Schedule(15000, pAgent, &MyAgent::KuKu, 0);
    Schedule(26000, pAgent, &MyAgent::Ku);
    Schedule(5000, pAgent, &MyAgent::KuKu, 0);
    Schedule(5000, pAgent, &MyAgent::KuStr, "длинная строка", 50);

    // предел времени симуляции 30000 нс
    g_pSim->SetLimit(30000);

    // запускаем симулятор. Он будет работать до тех пор, пока
    // есть события в очереди и не достигнут заданный предел времени
    while(!g_pSim->Run())
    {
        // выводим запрос на продолжение работы симулятора
        PrintTime(&std::cout);
        std::cout << "Do you want to proceed? [Y/n]" << std::endl;

        // вводим ответ пользователя
        char c[3];
        std::cin.getline(c,sizeof(c));

        // если пользователь не против продолжения
        if(c[0] == 'n')
        {
            break;
        }

        // устанавливаем новый предел времени и вновь запускаем симулятор
        g_pSim->SetLimit(g_pSim->GetTime()+30000);
    }

    // удаляем объект агента
    delete pAgent;

    // удаляем объект симулятора
    delete g_pSim;

    return 0;
}
