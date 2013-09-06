#include "common/trace.h"

#include "taskman/win/task.h"

namespace TaskMan {

    Task::Task(TaskProc proc, TaskManagerIn taskManager)
        : mTaskManager(taskManager)
        , mProc(proc)
        , mData(NULL)
    {
        TRACE_ASSERT(mProc != NULL);
    }

    Task::~Task()
    {

    }

    void Task::Run(void * data)
    {
        mData = data;
        mTaskManager->Push(this);
    }

} // namespace TaskMan
