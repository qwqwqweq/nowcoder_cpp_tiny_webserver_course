//生产者消费者
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <format>
#include <semaphore.h>

//创建一个互斥量
pthread_mutex_t mutex;
//创建两个信号量
sem_t psem, csem;

struct Node {
    int num;
    Node *next;
} *head;

void *producer(void *arg) {

    while (true) {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        std::cout << std::format("add node, num: {}, tid: {}\n", newNode->num, pthread_self());
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
    }

    return NULL;
}

void *customer(void *arg) {

    while (true) {
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        Node *tmp = head;
        head = head->next;
        std::cout << std::format("del node, num: {}, tid: {}\n", tmp->num, pthread_self());
        free(tmp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
    }

    return NULL;
}

int main() {
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    //分别5个
    pthread_t ptids[5], ctids[5];
    for (int i = 0; i < 5; i++)
        pthread_create(&ptids[i], NULL, producer, NULL);

    for (int i = 0; i < 5; i++) 
        pthread_create(&ctids[i], NULL, customer, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

    while (true) {
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);

    return 0;
}