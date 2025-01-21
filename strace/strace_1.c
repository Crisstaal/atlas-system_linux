#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include "syscalls.h"

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s command [args...]\n", program_name);
}

const char *get_syscall_name(long syscall_number) {
    for (size_t i = 0; i < SYSCALL_COUNT; i++) {
        if (syscall_table[i].number == syscall_number) {
            return syscall_table[i].name;
        }
    }
    return "unknown";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        /*Child process: request tracing and execute the command*/
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            perror("ptrace");
            return EXIT_FAILURE;
        }
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return EXIT_FAILURE;
    } else {
        /*Parent process: trace the child*/
        int status;
        struct user_regs_struct regs;

        while (1) {
            if (waitpid(child_pid, &status, 0) == -1) {
                perror("waitpid");
                return EXIT_FAILURE;
            }
            if (WIFEXITED(status)) {
                break;
            }

            if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1) {
                perror("ptrace");
                return EXIT_FAILURE;
            }

#if defined(__x86_64__)
            long syscall_number = regs.orig_rax;
#elif defined(__i386__)
            long syscall_number = regs.orig_eax;
#else
#error "Unsupported architecture"
#endif

            printf("%s\n", get_syscall_name(syscall_number));

            /*Continue the child process*/
            if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1) {
                perror("ptrace");
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}
