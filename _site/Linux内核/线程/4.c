#include <stdio.h>
 #include <stdlib.h>
 #include <pthread.h>
 
 void *thrd_func1(void *arg);
 void *thrd_func2(void *arg);
 
 pthread_t tid1,tid2;
 
 int main(){
     // 创建线程tid1，线程函数thrd_func1
     if (pthread_create(&tid1,NULL,thrd_func1,NULL)!=0) {
         printf("Create thread 1 error!\n");
         exit(1);
     }
     // 创建线程tid2，线程函数thrd_func2
     if (pthread_create(&tid2,NULL,thrd_func2,NULL)!=0) {
         printf("Create thread 2 error!\n");
         exit(1);
     }
     // 等待线程tid1退出
     if (pthread_join(tid1,NULL)!=0){
         printf("Join thread 1 error!\n");
         exit(1);
     }else
         printf("Thread 1 Joined!\n");
     // 等待线程tid2退出
     if (pthread_join(tid2,NULL)!=0){
         printf("Join thread 2 error!\n");
         exit(1);
     }else
         printf("Thread 2 Joined!\n");
 
     return 0;
 }
 
 void *thrd_func1(void *arg){
 //    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL); // 设置其他线程可以cancel掉此线程
 
     while(1) {
         printf("Thread 1 is running!\n");
         sleep(1);
     }
     pthread_exit((void *)0);
 }
 
 void *thrd_func2(void *arg){
     printf("Thread 2 is running!\n");
     sleep(5);
     if (pthread_cancel(tid1)==0)  // 线程tid2向线程tid1发送cancel
         printf("Send Cancel cmd to Thread 1.\n");
         
     pthread_exit((void *)0);
 }