#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


#define log_name "log.txt"
#define exit_str "exit"

FILE *file;
pthread_mutex_t mutex;

void thread_download(void **args)
{
    char *link = (char *)args[0], *file_name = (char *)args[1];
    //sleep(2);
    execlp("curl", "", "-s", "-o", file_name, link, NULL);
}

void *thread_init(void **args)
{
    char *link = (char *)args[0], *file_name = (char *)args[1];
    printf("Started\n");
    pthread_t new_thread_downloader;
    int pid = fork();
    if (pid == 0)
    {
        thread_download(args);
    }
    else
    {
        printf("Started download\n");
        int status;
        wait(&status);
        printf("Finished download\n");
        pthread_mutex_lock(&mutex);
        printf("Mutex locked\n");
        sleep(5);
        if (status == 0)
            fprintf(file,"Successfully downloaded %s from %s\n",file_name,link);
        else
            fprintf(file,"Failed to download %s from %s\n",file_name,link);
        pthread_mutex_unlock(&mutex);
        printf("Mutex unlocked\n");
        printf("Ended\n");
    }
}

int main()
{
    file = fopen(log_name, "a+");
    pthread_mutex_init(&mutex, NULL);
    while (1)
    {
        char *link = (char *)malloc(500), *file_name = (char *)malloc(50);
        scanf("%s%s", link, file_name);
        if (!strcmp(exit_str, link) || !strcmp(exit_str, file_name))
            break;
        pthread_t new_thread;
        char **costya[2];
        costya[0] = link;
        costya[1] = file_name;
        pthread_create(&new_thread, NULL, thread_init, costya);
        //pthread_detach(new_thread);
    }
    pthread_mutex_destroy(&mutex);
    fclose(file);
}