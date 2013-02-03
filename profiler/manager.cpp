#include "profiler/manager.h"
#include "profiler/config.h"

#include "common/counter.h"

#include <iostream>
#include <vector>
#include <algorithm>

namespace Profiler {

    static Manager sGlobalManager;

    Manager::Manager()
        : m_firstBucket(0)
        , m_lastBucket(0)
        , m_outFileName(DEFAULT_DATA_FILE_NAME)
    {
        m_firstBucket = m_lastBucket = new EventBucket;

        m_eventCriticalSection = Common::CreateCriticalSection();
    }

    Manager::~Manager()
    {
        std::cout << "Processing profiled data..." << std::endl;

        Dump();

        Common::DeleteCriticalSection(m_eventCriticalSection);
    }

    void Manager::CreateEvent(const Token * token, RegionEnding ending)
    {
        time_t t = Common::GetCounter();

        Common::EnterCriticalSection(m_eventCriticalSection);
        {
            // Reached the end of the bucket
            if (m_lastBucket->lastEvent == &m_lastBucket->end)
            {
                m_lastBucket->nextBucket = new EventBucket;

                if (m_lastBucket->nextBucket != 0)
                {
                    m_lastBucket = m_lastBucket->nextBucket;
                }
                // Cannot allocate a new bucket
                else
                {
                    Dump();
                }
            }

            Event * e  = m_lastBucket->lastEvent;
            e->token   = token;
            e->time    = t;
            e->ending  = ending;

            m_lastBucket->lastEvent ++;
        }
        Common::LeaveCriticalSection(m_eventCriticalSection);
    }

    void Manager::Dump()
    {

    }

    Manager * GetGlobalManager()
    {
        return &sGlobalManager;
    }

} // namespace Profiler
