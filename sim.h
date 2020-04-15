//
// Virtual Storage Simulator
//

typedef uint64_t SimulatorTime;

class Sim;

class Agent
{
public:
    Agent() {}
    virtual ~Agent() {}

    virtual void Start() = 0;
    SimulatorTime GetTime();
    void SetName(std::string str);
    std::string GetName();
    void Log(std::string text);

private:
    std::string m_name;
};

class Event
{
public:
    Event() { m_pNext = NULL; }
    virtual ~Event() {}

    SimulatorTime m_time;
    uint32_t m_value;
    Event *m_pNext;

    virtual void Notify() {}
};

class EventQueue
{
public:
    EventQueue();
    ~EventQueue();

    uint64_t Schedule(Event *pEvent);
    bool CancelEvent(uint64_t handle);
    bool IsEmpty() { return m_pFirstEvent==NULL; }
    Event *GetHead() { return m_pFirstEvent; }
    void RemoveHead()
    {
        if(m_pFirstEvent)
        {
            m_pFirstEvent = m_pFirstEvent->m_pNext;
        }
    }

private:
    Event *m_pFirstEvent;
};

class Sim : public EventQueue
{
public:
    Sim();
    ~Sim();

    void SetLimit(SimulatorTime limit);
    SimulatorTime GetLimit();

    SimulatorTime GetTime();
    bool Run();

private:
    SimulatorTime m_Time;
    SimulatorTime m_Limit;
};

extern Sim *g_pSim;

template <typename METHOD, typename OBJECT>
uint64_t Schedule(SimulatorTime time, OBJECT obj, METHOD met, uint32_t value)
{
    assert(time >= g_pSim->GetTime());
    return g_pSim->Schedule(MakeEvent(time, obj, met, value));
}

template <typename METHOD, typename OBJECT>
Event *MakeEvent(SimulatorTime time, OBJECT obj, METHOD met, uint32_t value)
{
    class EventImpl : public Event
    {
    public:
        EventImpl(SimulatorTime time, OBJECT obj, METHOD met, uint32_t value) { m_time=time; m_obj=obj; m_met=met; m_value=value; }
        virtual ~EventImpl() {}

        OBJECT m_obj;
        METHOD m_met;

        void Notify()
        {
            (m_obj->*m_met)(m_value);
        }
    } *pEvent = new EventImpl(time, obj, met, value);

    return pEvent;
};

bool CancelEvent(uint64_t handle);

void PrintTime(std::ostream *str);
