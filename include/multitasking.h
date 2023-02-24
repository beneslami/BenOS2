#ifndef MULTITASKING_H
#define MULTITASKING_H

#include <gdt.h>
#include <common/types.h>

namespace BenOS{

    struct CPUState{
        BenOS::common::uint32_t eax;
        BenOS::common::uint32_t ebx;
        BenOS::common::uint32_t ecx;
        BenOS::common::uint32_t edx;

        BenOS::common::uint32_t esi;
        BenOS::common::uint32_t edi;
        BenOS::common::uint32_t ebp;

        /*BenOS::common::uint32_t gs;
        BenOS::common::uint32_t fs;
        BenOS::common::uint32_t es;
        BenOS::common::uint32_t ds;*/

        BenOS::common::uint32_t error;

        BenOS::common::uint32_t eip;
        BenOS::common::uint32_t cs;
        BenOS::common::uint32_t eflags;
        BenOS::common::uint32_t esp;
        BenOS::common::uint32_t ss;
    }__attribute__((packed));

    class Task{
        friend class TaskManager;

    private:
        BenOS::common::uint8_t stack[4096]; // 4KB
        CPUState *cpustate;
    public:
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
    };

    class TaskManager{
    private:
        Task *tasks[256];
        int numTasks;
        int currentTask;
    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task *task);
        CPUState *Schedule(CPUState *cpustate);
    };
}

#endif