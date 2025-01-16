#ifndef SYSCALLS_H
#define SYSCALLS_H

#define SYSCALL_COUNT 4 // Adjust this to match the actual count

typedef struct syscall_entry {
    long number;
    const char *name;
    const char *return_type;
    int arg_count;
    const char *arg_types[6];
} syscall_entry_t;

static const syscall_entry_t syscall_table[] = {
    {0, "read", "ssize_t", 3, {"int", "void *", "size_t"}},
    {1, "write", "ssize_t", 3, {"int", "const void *", "size_t"}},
    {2, "open", "int", 3, {"const char *", "int", "mode_t"}},
    {3, "close", "int", 1, {"int"}}
};

#endif /* SYSCALLS_H */
