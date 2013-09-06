#ifndef __TASKMAN_WIN_TASK_H__
#define __TASKMAN_WIN_TASK_H__

#include "taskman/win/taskman.h"

namespace TaskMan {

    class Task : public ITask
    {
    public:
        virtual void
        Run(void * data);

    public:
        inline TaskProc
        GetProc() { return mProc; }

        inline void *
        GetData() { return mData; }

    public:
        Task(TaskProc proc, TaskManagerIn taskManager);
        ~Task();

    public:
        TaskManagerRef  mTaskManager;
        TaskProc        mProc;
        void *          mData;
    };

} // namespace TaskMan

#endif // __TASKMAN_WIN_TASK_H__
