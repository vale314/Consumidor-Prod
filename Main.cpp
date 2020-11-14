#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>
#include <mutex>
#include <iostream>
#include <thread>
#include <conio.h>

using namespace std;

std::mutex mu;
std::condition_variable cond;
int buffer [20];
const unsigned int maxBufferSize = 20;

int numE = 0;



int spaceProd = 0;
int spaceCons = 0;

void initializer(){
    int i = 0;

    for ( i = 0; i<maxBufferSize; i++){
        buffer[i] = 99;
    }
}


void producer(int val){
    while(val){
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, [](){return numE < maxBufferSize;});
        //buffer.push_back(val);

        buffer[spaceProd] = val;
        cout<< "Produced: " << val << endl;

        val--;
        spaceProd++;


        numE++;


        if(spaceProd == maxBufferSize-1){
            spaceProd = 0;
        }

        locker.unlock();
        cond.notify_one();
    }
}

void consumer(int value){
    while(value){
        std::unique_lock<std:: mutex> locker(mu);
        cond.wait(locker, [](){return numE > 0;});

        //int val = buffer.back();
        //buffer.pop_back();

        int val = buffer[spaceCons];
        buffer[spaceCons] = 99;
        spaceCons++;

        numE--;

        if(spaceCons == maxBufferSize-1){
            spaceCons = 0;
        }

        cout << "Consumed: " << val << endl;
        value--;
        locker.unlock();
        cond.notify_one();
    }
}

void println(){

    int i;

    for(i = 0; i < maxBufferSize-1; i++){
        if(buffer[i] == 99)
            cout << "- ";
        else
            cout << "^ ";
    }
    cout << endl;

}

int main()
{

    srand((unsigned)time(NULL));

    int item;
    int cons;


    char choice;
    int ass;

    initializer();

    std:: chrono::seconds dura(2);

    while(true){
        item  = (rand() % 10) + 1;
        srand(time(NULL));
        cons  = (rand() % 9) +1;

        cout<< "Prod: " << item << "Cons: "<< cons << endl;

        //std::this_thread::sleep_for(dura);

        if(item >= maxBufferSize - numE){
            item = maxBufferSize - numE;
        }

        std:: thread t1(producer, item);


        if(cons >= numE){
            cons = numE;
        }

        std:: thread t2(consumer, cons);

        t1.join();
        t2.join();

        println();


        cin.clear();
        fflush(stdin);

        choice=getch();
        ass=choice;

        if(ass==27)
            break;
    }

    return 0;
}
