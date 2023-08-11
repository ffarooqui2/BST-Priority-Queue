#include <iostream>
#include "priorityqueue.h"

using namespace std;
int main(){

    priorityqueue<string> pq1;
    pq1.enqueue("Dolores", 5);
    pq1.enqueue("Benard", 4);
    pq1.enqueue("Arnold", 8);
    pq1.enqueue("Ford", 2);
    pq1.enqueue("Jasmine", 6);
    pq1.enqueue("Carlos", 11);
    pq1.enqueue("William", 8);
    pq1.enqueue("Teddy", 8);

    // cout << pq1.toString() << endl;

    // pq1.clear();

    // cout << pq1.toString() << endl;

    // while (pq1.Size() > 0){
    //     cout << pq1.toString() << endl;
    //     pq1.dequeue();
    //     cout << "--------------------" << endl;
    // }

    // pq1.enqueue("Dolores", 5);
    // pq1.enqueue("Benard", 4);
    // pq1.enqueue("Arnold", 8);
    // pq1.enqueue("Ford", 2);
    // pq1.enqueue("Jasmine", 6);
    // pq1.enqueue("Carlos", 11);

    // pq1.print();


    string value;
    int priority;

    pq1.begin();
    while (pq1.next(value, priority)) {
        cout << priority << " value: " << value << endl;
    }
    cout << priority << " value: " << value << endl;
    
    
}