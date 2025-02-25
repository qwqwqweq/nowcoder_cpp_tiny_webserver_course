//生产者消费者
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <format>

//创建一个互斥量
pthread_mutex_t mutex;
//创建条件变量
pthread_cond_t cond; 

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
        //只要生成了一个，就通知消费者
        pthread_cond_signal(&cond);
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
            //没有数据，需要等待
            //当这个函数调用阻塞的时候，会对互斥锁进行加锁，当不阻塞时，继续向下执行，会重新加锁
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        }

    }

    return NULL;
}

int main() {
    pthread_cond_init(&cond, NULL);
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
    pthread_cond_destroy(&cond);

    pthread_exit(NULL);

    return 0;
}