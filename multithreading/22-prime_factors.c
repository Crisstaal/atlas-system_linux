#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "tprintf.h"

#define NB_THREADS 8

typedef void *(*task_entry_t)(void *);

/**
 * enum task_status_e - Task statuses
 *
 * @PENDING: Task is pending
 * @STARTED: Task has been started
 * @SUCCESS: Task has completed successfully
 * @FAILURE: Task has completed with issues
 */
typedef enum task_status_e
{
    PENDING = 0,
    STARTED,
    SUCCESS,
    FAILURE
} task_status_t;

/**
 * struct task_s - Executable task structure
 *
 * @entry:  Pointer to a function to serve as the task entry
 * @param:  Address to a custom content to be passed to the entry function
 * @status: Task status, default to PENDING
 * @result: Stores the return value of the entry function
 * @lock:   Task mutex
 */
typedef struct task_s
{
    task_entry_t    entry;
    void        *param;
    task_status_t   status;
    void        *result;
    pthread_mutex_t lock;
} task_t;

/**
 * create_task - Creates a new task
 * @entry: Pointer to the entry function
 * @param: Parameter to be passed to the entry function
 *
 * Return: Pointer to the created task, or NULL on failure
 */
task_t *create_task(task_entry_t entry, void *param)
{
    task_t *task = malloc(sizeof(task_t));
    if (!task)
        return NULL;

    task->entry = entry;
    task->param = param;
    task->status = PENDING;
    task->result = NULL;
    pthread_mutex_init(&task->lock, NULL);

    return task;
}

/**
 * destroy_task - Destroys the task
 * @task: Pointer to the task to be destroyed
 */
void destroy_task(task_t *task)
{
    if (task)
    {
        pthread_mutex_destroy(&task->lock);
        free(task);
    }
}

/**
 * exec_tasks - Executes tasks in the list
 * @tasks: Pointer to the list of tasks
 *
 * Return: NULL
 */
void *exec_tasks(list_t const *tasks)
{
    node_t *node;
    task_t *task;

    for (node = tasks->head; node; node = node->next)
    {
        task = (task_t *)node->content;

        pthread_mutex_lock(&task->lock);
        if (task->status == PENDING)
        {
            task->status = STARTED;
            tprintf("[%lu] Started\n", (unsigned long)pthread_self());
            pthread_mutex_unlock(&task->lock);

            task->result = task->entry(task->param);  // Execute the task's entry function

            pthread_mutex_lock(&task->lock);
            task->status = SUCCESS;
            tprintf("[%lu] Success\n", (unsigned long)pthread_self());
        }
        pthread_mutex_unlock(&task->lock);
    }

    return NULL;
}

/**
 * print_task_result - Prints the result of a task
 * @task: Pointer to the task to print the result of
 */
void print_task_result(task_t *task)
{
    list_t *factors = (list_t *)task->result;
    node_t *factor;

    printf("%s =", (char *)task->param);
    for (factor = factors->head; factor; factor = factor->next)
    {
        unsigned long n = *((unsigned long *)factor->content);
        printf("%s %lu", factor->prev ? " *" : "", n);
    }
    printf("\n");
}

/**
 * prime_factors - Entry function for the task
 * @param: The number as a string
 *
 * Return: List of prime factors for the number
 */
void *prime_factors(void *param)
{
    unsigned long num = strtoul((char *)param, NULL, 10);
    list_t *factors = list_init();
    unsigned long i = 2;

    while (i * i <= num)
    {
        while (num % i == 0)
        {
            list_add(factors, &i);
            num /= i;
        }
        i++;
    }
    if (num > 1)
    {
        list_add(factors, &num);
    }

    return factors;
}
