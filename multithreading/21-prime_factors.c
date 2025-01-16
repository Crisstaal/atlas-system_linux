#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "multithreading.h"

/**
 * prime_factors_task - Executes the prime factorization task for a given number
 * @param: The string representation of the number to be factorized
 * Return: A pointer to the result list containing the prime factors
 */
void *prime_factors_task(void *param)
{
    char *s = (char *)param;
    list_t *factors = prime_factors(s);  // Reuse your existing prime_factors function
    return factors;
}

/**
 * create_task - Create a task for execution in the thread pool
 * @entry: The function to be executed for the task
 * @param: The parameter to be passed to the task function
 * Return: A pointer to the created task structure
 */
task_t *create_task(task_entry_t entry, void *param)
{
    task_t *task = malloc(sizeof(task_t));
    if (!task)
        return NULL;

    task->entry = entry;
    task->param = param;
    task->status = PENDING;
    pthread_mutex_init(&task->lock, NULL);

    return task;
}

/**
 * destroy_task - Destroy a task and free its resources
 * @task: The task to be destroyed
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
 * exec_tasks - Execute the list of tasks in the thread pool
 * @tasks: A list of tasks to be executed
 * Return: NULL
 */
void *exec_tasks(list_t const *tasks)
{
    node_t *node;
    task_t *task;

    for (node = tasks->head; node; node = node->next)
    {
        task = (task_t *)node->content;

        /*Ensure that the task is executed only once*/
        pthread_mutex_lock(&task->lock);
        if (task->status == PENDING)
        {
            task->status = STARTED;
            tprintf("[%ld] [00] Started\n", pthread_self());
            task->result = task->entry(task->param);
            task->status = SUCCESS;
            tprintf("[%ld] [00] Success\n", pthread_self());
        }
        pthread_mutex_unlock(&task->lock);
    }

    return NULL;
}