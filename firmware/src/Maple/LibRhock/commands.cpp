#include <rhock/memory.h>
#include <rhock/obj.h>
#include <rhock/store.h>
#include <rhock/chain.h>
#include <rhock/program.h>
#include <rhock/vm.h>
#include <rhock/native.h>
#include "rhock.h"
#include <terminal.h>

TERMINAL_COMMAND(ps, "Process list")
{
    struct rhock_obj **objs = rhock_get_programs();

    int i=0;
    while (*objs != NULL) {
        i++;
        struct rhock_obj *obj = *objs;
        objs++;

        terminal_io()->print(i);
        terminal_io()->print("] ");
        // XXX: Todo handle better these 16 chars!
        terminal_io()->print(obj->name);
        terminal_io()->print(" (");
        terminal_io()->print(obj->id);
        terminal_io()->print(") ");
        rhock_memory_addr prog = rhock_vm_get_program(obj->id);
        if (prog != RHOCK_NULL) {
            struct rhock_program *program = rhock_get_program(prog);
            if (program->state == RHOCK_PROGRAM_RUNNING) {
                terminal_io()->print("running");
            } else if (program->state == RHOCK_PROGRAM_FREEZED) {
                terminal_io()->print("paused");
            } else if (program->state == RHOCK_PROGRAM_CRASHED) {
                terminal_io()->print("crashed with code ");
                terminal_io()->print(program->error);
            }
        } else {
            terminal_io()->print("not loaded");
        }
        terminal_io()->println();
    }
}

TERMINAL_COMMAND(psj, "Process list (json)")
{
    rhock_memory_addr addr = rhock_vm_get_objs();
    int i=0;
    terminal_io()->print("psj=[");
    while (addr != RHOCK_LAST) {
        terminal_io()->print("{");
        i++;
        struct rhock_obj *obj = rhock_get_obj(addr);
        terminal_io()->print("\"number\":\"");
        terminal_io()->print(i);
        terminal_io()->print("\", \"name\": \"");
        terminal_io()->print(obj->name);
        terminal_io()->print("\", \"status\": \"");

        rhock_memory_addr prog = rhock_vm_get_program(obj->id);
        if (prog != RHOCK_NULL) {
            struct rhock_program *program = rhock_get_program(prog);
            if (program->state == RHOCK_PROGRAM_RUNNING) {
                terminal_io()->print("2");
            } else {
                terminal_io()->print("1");
            }
        } else {
            terminal_io()->print("0");
        }
        terminal_io()->print("\"}");
        addr = rhock_chain_next(addr);
        if (addr != RHOCK_LAST) {
            terminal_io()->print(",");
        }
    }
    terminal_io()->println("]");
}

TERMINAL_COMMAND(run, "Runs a program")
{
    if (argc != 1) {
        terminal_io()->println("Usage: run [id]");
    } else {
        int pid = atoi(argv[0]);
        rhock_memory_addr addr = rhock_vm_get_objs();
        int i=0;
        while (addr != RHOCK_LAST) {
            i++;
            if (i == pid) {
                struct rhock_obj *obj = rhock_get_obj(addr);
                rhock_memory_addr addr = rhock_vm_get_program(obj->id);
                terminal_io()->print("Running ");
                terminal_io()->print(obj->name);
                terminal_io()->println();
                if (addr == RHOCK_NULL) {
                    rhock_program_load(obj);
                } else {
                    rhock_program_run(obj->id);
                }
                return;
            }
            addr = rhock_chain_next(addr);
        }
        terminal_io()->print("Unknown pid: ");
        terminal_io()->print(pid);
        terminal_io()->println();
    }
}

TERMINAL_COMMAND(freeze, "Freezes a program")
{
    if (argc != 1) {
        terminal_io()->println("Usage: freeze [id]");
    } else {
        int pid = atoi(argv[0]);
        rhock_memory_addr addr = rhock_vm_get_objs();
        int i=0;
        while (addr != RHOCK_LAST) {
            i++;
            if (i == pid) {
                struct rhock_obj *obj = rhock_get_obj(addr);
                rhock_memory_addr addr = rhock_vm_get_program(obj->id);
                if (addr != RHOCK_NULL) {
                    terminal_io()->print("Freezing ");
                    terminal_io()->print(obj->name);
                    terminal_io()->println();
                    rhock_program_freeze(obj->id);
                }
                return;
            }
            addr = rhock_chain_next(addr);
        }
        terminal_io()->print("Unknown pid: ");
        terminal_io()->print(pid);
        terminal_io()->println();
    }
}

TERMINAL_COMMAND(kill, "Kills a program")
{
    if (argc != 1) {
        terminal_io()->println("Usage: kill [id]");
    } else {
        int pid = atoi(argv[0]);
        rhock_memory_addr addr = rhock_vm_get_objs();
        int i = 0;
        rhock_program_kill(pid);
        while (addr != RHOCK_LAST) {
            i++;
            if (i == pid) {
                struct rhock_obj *obj = rhock_get_obj(addr);
                rhock_memory_addr progAddr = rhock_vm_get_program(obj->id);
                if (progAddr != RHOCK_NULL) {
                    rhock_program_unload(progAddr);
                    terminal_io()->print("Unloading ");
                    terminal_io()->print(obj->name);
                    terminal_io()->println();
                } else {
                    terminal_io()->println("Process not running.");
                }
                return;
            } 
            addr = rhock_chain_next(addr);
        }
        terminal_io()->print("Unknown pid: ");
        terminal_io()->print(pid);
        terminal_io()->println();
    }
}

TERMINAL_COMMAND(killall, "Kills all programs")
{
    rhock_program_killall();
}

TERMINAL_COMMAND(erase, "Erase a program")
{
    if (argc != 1) {
        terminal_io()->println("Usage: erase [id]");
    } else {
        int pid = atoi(argv[0]);
        rhock_memory_addr addr = rhock_vm_get_objs();
        int i = 0;
        rhock_program_kill(pid);
        while (addr != RHOCK_LAST) {
            i++;
            if (i == pid) {
                struct rhock_obj *obj = rhock_get_obj(addr);
                terminal_io()->print("Erasing ");
                terminal_io()->print(obj->name);
                terminal_io()->println();
                rhock_vm_remove_obj(addr);
                return;
            }
            addr = rhock_chain_next(addr);
        }
        terminal_io()->print("Unknown pid: ");
        terminal_io()->print(pid);
        terminal_io()->println();
    }
}

TERMINAL_COMMAND(erase_all, "Erase all the programs")
{
    rhock_vm_clear_objs();
    rhock_store_reset();
}

TERMINAL_COMMAND(meminfo, "Informations about memory")
{
    int total, used, overhead;
    rhock_meminfo(&total, &used, &overhead);

    terminal_io()->print("Total: ");
    terminal_io()->print(total);
    terminal_io()->print(", used: ");
    terminal_io()->print(used);
    terminal_io()->print(", free: ");
    terminal_io()->print(total-used);
    terminal_io()->print(", overhead: ");
    terminal_io()->print(overhead);
    terminal_io()->println();
}

TERMINAL_COMMAND(mem, "Test")
{
    rhock_alloc(16, 1);
}
