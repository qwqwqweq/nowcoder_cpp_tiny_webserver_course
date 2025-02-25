//生产者消费者
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <format>

//创建一个互斥量
pthread_mutex_t mutex;

struct Node {
    int num;
    Node *next;
} *head;

void *producer(void *arg) {

    while (true) {
        pthread_mutex_lock(&mutex);
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        std::cout << std::format("add node, num: {}, tid: {}\n", newNode->num, pthread_self());
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }

    return NULL;
}

void *customer(void *arg) {

    while (true) {
        pthread_mutex_lock(&mutex);
        //有数据
        if (head) {
            Node *tmp = head;
            head = head->next;
            std::cout << std::format("del node, num: {}, tid: {}\n", tmp->num, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        } else {
            //没有数据
            pthread_mutex_unlock(&mutex);
        }

    }

    return NULL;
}

int main() {
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