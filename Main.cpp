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
#include <chrono>
#include <atomic>


using namespace std;

int item;
int cons;

std::mutex mu;
std::condition_variable cond;
int buffer [20];
const unsigned int maxBufferSize = 20;

int numE = 0;

std:: atomic<bool> stop( false );

int spaceProd = 0;
int spaceCons = 0;

void initializer(){
    int i = 0;

    for ( i = 0; i<maxBufferSize; i++){
        buffer[i] = 99;
    }
}


void println(){

    int i;
    if(cons == 9)
        cout<< "Prod: " << item << "Cons: "<< " Se Quedo Dormido " << endl;
    else
        if(item == 2)
        cout<< "Prod: " << " Se Quedo Dormido"<< " Cons" << cons << endl;
        else
            cout<< "Prod: " << item << "Cons: " << cons <<endl;
    for(i = 0; i < maxBufferSize-1; i++){
        if(buffer[i] == 99)
            cout << "- ";
        else
            cout << "^ ";
    }
    cout << endl;
    std:: this_thread::sleep_for(std::chrono::milliseconds(200));
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
        println();
        system("cls");
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
        println();
        system("cls");
        locker.unlock();
        cond.notify_one();
    }
}


void loop(){
    srand((unsigned)time(NULL));


    char choice;
    int ass;

    int slepCons = 0;
    int slepProd = 0;


    initializer();


    while(!kbhit()){
        item  = (rand() % 10) + 1;
        srand(time(NULL));
        cons  = (rand() % 9) +1;




        if(item >= maxBufferSize - numE){
            item = maxBufferSize - numE;
        }

        if(item != 2){
            std:: thread t1(producer, item);
            t1.join();
        }

        if(cons >= numE){
            cons = numE;
        }

        if(cons !=9){

            std:: thread t2(consumer, cons);
            t2.join();
          }

//        println();

//        cout << "'ESC para salir' \n Otra Por Continuar" <<endl;

//        cin.clear();
//        fflush(stdin);

//        choice=getch();
//        ass=choice;

//        if(ass==27)
//            break;
    }

}


int main()
{

   loop();

   println();

    return 0;
}
