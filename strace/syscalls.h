#ifndef SYSCALLS_H
#define SYSCALLS_H

typedef struct syscall_entry {
    long number;               /* Syscall number */
    const char *name;          /* Syscall name */
    const char *return_type;   /* Return type */
} syscall_entry;

syscall_entry syscall_table[] = {
    {0, "read", "ssize_t"},
    {1, "write", "ssize_t"},
    {2, "open", "int"},
    {3, "close", "int"},
    {4, "stat", "int"},
    {5, "fstat", "int"},
    {6, "lstat", "int"},
    {9, "mmap", "void*"},
    {11, "mprotect", "int"},
    {12, "munmap", "int"},
    {15, "brk", "void*"},
    {16, "rt_sigaction", "int"},
    {18, "ioctl", "int"},
    {20, "pread64", "ssize_t"},
    {21, "pwrite64", "ssize_t"},
    {39, "execve", "int"},
    {57, "exit_group", "void"},
    {60, "exit", "void"}
};
#define SYSCALL_COUNT (sizeof(syscall_table) / sizeof(syscall_entry))

#endif /* SYSCALLS_H */
