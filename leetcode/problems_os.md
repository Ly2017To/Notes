# Solutions of Operating System Related Problems in C/C++
---
## 1114. Print in Order
### Problem
The "Print in Order" problem is a synchronization challenge involving multiple threads. In this problem, you are required to implement a system where three different threads print numbers in a specific order. The objective is to ensure that the threads print the numbers in the sequence of `first`, `second`, and `third`. You need to implement a class called `Foo` with the following methods:
1. **`Foo()`**: Constructor that initializes the data.
2. **`void first()`**:
    - This method will be called by the first thread.
    - It should print "first" and then signal the next thread to proceed.
3. **`void second()`**:
    - This method will be called by the second thread.
    - It should wait until the first thread has finished printing before printing "second".
4. **`void third()`**:
    - This method will be called by the third thread.
    - It should wait until the second thread has finished printing before printing "third".
### Solution
```c
/*
    Solution with Semaphore in C
*/
typedef struct {
    // User defined data may be declared here.
    sem_t firstSem;
    sem_t secondSem;
    sem_t thirdSem;
} Foo;

// Function Declaration, do not remove
void printFirst();
void printSecond();
void printThird();

Foo* fooCreate() {
    Foo* obj = (Foo*) malloc(sizeof(Foo));
    // Initialize user defined data here.
    sem_init(&(obj->firstSem),0,1);
    sem_init(&(obj->secondSem),0,0);
    sem_init(&(obj->thirdSem),0,0);
    return obj;
}

void first(Foo* obj) {
    sem_wait(&(obj->firstSem));
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    sem_post(&(obj->secondSem));
}

void second(Foo* obj) {
    sem_wait(&(obj->secondSem));
    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();
    sem_post(&(obj->thirdSem));
}

void third(Foo* obj) {
    sem_wait(&(obj->thirdSem));
    // printThird() outputs "third". Do not change or remove this line.
    printThird();
    sem_post(&(obj->firstSem));
}

void fooFree(Foo* obj) {
    // User defined data may be cleaned up here.
    sem_destroy(&(obj->firstSem));
    sem_destroy(&(obj->secondSem));
    sem_destroy(&(obj->thirdSem));
    free(obj);
}
```
## 1115. Print FooBar Alternately
### Problem
The "Print FooBar Alternately" problem involves synchronizing two threads to print "Foo" and "Bar" alternately. One thread will print "Foo" and the other will print "Bar", with the requirement that they alternate their outputs for a specified number of times. You need to implement a class called `FooBar` with the following methods:
1. **`FooBar(int n)`**: Constructor that initializes the object so that it will print "Foo" and "Bar" up to `n` times.
2. **`void foo()`**:
    - Called by the thread that is supposed to print "Foo".
    - It should print "Foo" and notify the other thread to print "Bar".
3. **`void bar()`**:
    - Called by the thread that is supposed to print "Bar".
    - It should wait until it is its turn to print and then print "Bar".
### Solution
```c
/*
    Solution with Semaphore in C
*/
typedef struct {
    int n;
    sem_t semaphoreFoo;
    sem_t semaphoreBar;
} FooBar;

// Function declarations. Do not change or remove this line
void printFoo();
void printBar();

FooBar* fooBarCreate(int n) {
    FooBar* obj = (FooBar*) malloc(sizeof(FooBar));
    obj->n = n;
    sem_init(&(obj->semaphoreFoo),0,1);
    sem_init(&(obj->semaphoreBar),0,0);
    return obj;
}

void foo(FooBar* obj) {
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&(obj->semaphoreFoo));
        // printFoo() outputs "foo". Do not change or remove this line.
        printFoo();
        sem_post(&(obj->semaphoreBar));   
    }
}

void bar(FooBar* obj) {
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&(obj->semaphoreBar));
        // printBar() outputs "bar". Do not change or remove this line. 
        printBar();
        sem_post(&(obj->semaphoreFoo));
    }
}

void fooBarFree(FooBar* obj) {
    if(obj!=NULL){
        sem_destroy(&(obj->semaphoreFoo));
        sem_destroy(&(obj->semaphoreBar));
        free(obj);
    }
}
```
## 1117. Building H2O
### Problem
The problem involves synchronizing the printing of hydrogen (H) and oxygen (O) atoms to form water (H₂O) molecules. Each water molecule consists of two hydrogen atoms and one oxygen atom. The challenge is to ensure that the threads responsible for printing these atoms do so in the correct order: specifically, two "H" should be printed for every one "O". You need to implement a class called `H2O` with the following methods:
1. **`H2O()`**: Constructor that initializes the object.
2. **`void hydrogen()`**:
    - This method will be called by the thread that prints "H".
    - It should print "H" when it's allowed to do so.
3. **`void oxygen()`**:
    - This method will be called by the thread that prints "O".
    - It should print "O" when it's allowed to do so.
4. **Constraints**:
    - The methods will be called in separate threads.
    - The number of calls to `hydrogen` and `oxygen` will always correspond to a valid formation of water molecules; thus, there will be exactly twice as many hydrogen calls as oxygen calls.
### Solution
```cpp
/*
    Solution with mutex and conditional variable in C++
*/
class H2O {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
public:
    H2O() {
        this->count = 2;
    }
    void hydrogen(function<void()> releaseHydrogen) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock,[this]{return count>0;});
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        count--;
        cv.notify_all(); //pay attention here, there are two kinds of threads.
    }
    void oxygen(function<void()> releaseOxygen) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock,[this]{return count==0;});
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        count=2;
        cv.notify_all(); //pay attention here, there are two kinds of threads.
    }
};
```
```c
/*
    Solution with semaphore and count in C
*/
typedef struct {
    // User defined data may be declared here.
    sem_t hydrogenSem;
    sem_t oxygenSem;
    int count;
} H2O;

void releaseHydrogen();

void releaseOxygen();

H2O* h2oCreate() {
    H2O* obj = (H2O*) malloc(sizeof(H2O));
    // Initialize user defined data here.
    sem_init(&(obj->hydrogenSem),0,2);
    sem_init(&(obj->oxygenSem),0,0);
    obj->count = 2;
    return obj;
}

void hydrogen(H2O* obj) {
    // releaseHydrogen() outputs "H". Do not change or remove this line.
    sem_wait(&(obj->hydrogenSem));
    releaseHydrogen();
    obj->count--;
    if(obj->count==0){
        sem_post(&(obj->oxygenSem));
    }
}

void oxygen(H2O* obj) {
    // releaseOxygen() outputs "O". Do not change or remove this line.
    sem_wait(&(obj->oxygenSem));
    releaseOxygen();
    obj->count=2;
    sem_post(&(obj->hydrogenSem));
    sem_post(&(obj->hydrogenSem));
}

void h2oFree(H2O* obj) {
    // User defined data may be cleaned up here.
    if(obj!=NULL){
        sem_destroy(&(obj->hydrogenSem));
        sem_destroy(&(obj->oxygenSem));
        free(obj);
    }
}
```
## 1195. Fizz Buzz Multithreaded
### Problem
The "Fizz Buzz Multithreaded" problem involves multiple threads that need to print numbers from 1 to `n` while adhering to the following rules:
- Print "Fizz" for numbers that are multiples of 3.
- Print "Buzz" for numbers that are multiples of 5.
- Print "FizzBuzz" for numbers that are multiples of both 3 and 5.
- Print the number itself if it is not a multiple of either.
The challenge is to implement this logic using multithreading, ensuring that the outputs are printed in the correct order from 1 to `n`. You need to implement a class called `FizzBuzz` with the following methods:
1. **`FizzBuzz(int n)`**: Constructor that initializes the object with the maximum number `n`.
2. **`void fizz()`**:
    - This method will be called by the thread that prints "Fizz".
3. **`void buzz()`**:
    - This method will be called by the thread that prints "Buzz".
4. **`void fizzbuzz()`**:
    - This method will be called by the thread that prints "FizzBuzz".
5. **`void number()`**:
    - This method will be called by the thread that prints the numbers.
6. **Constraints**
- The input `n` will be a positive integer.
- You can assume that the methods will be called exactly `n` times each, and they will be invoked on separate threads.

### Solution
```c
/*
    Solution with mutex and conditional variable in C
*/
typedef struct {
    int n;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
} FizzBuzz;

FizzBuzz* fizzBuzzCreate(int n) {
    FizzBuzz* obj = (FizzBuzz*) malloc(sizeof(FizzBuzz));
    obj->n = n;
    pthread_mutex_init(&(obj->mutex),NULL);
    pthread_cond_init(&(obj->cond),NULL);
    obj->count = 1;
    return obj;
}
// Don't change the following declarations
void printNumber(int a);
void printFizz();
void printBuzz();
void printFizzBuzz();

void fizz(FizzBuzz* obj) {
    while(1){
        pthread_mutex_lock(&(obj->mutex));
        if(obj->count>obj->n){
            pthread_mutex_unlock(&(obj->mutex));
            return;
        } 
        if((obj->count%3==0)&&(obj->count%5!=0)){
            printFizz();
            obj->count++;
            pthread_cond_broadcast(&(obj->cond));
        }else{
            pthread_cond_wait(&(obj->cond),&(obj->mutex));
        }
        pthread_mutex_unlock(&(obj->mutex));
    }    
}

void buzz(FizzBuzz* obj) {
    while(1){
        pthread_mutex_lock(&(obj->mutex));
        if(obj->count>obj->n){
            pthread_mutex_unlock(&(obj->mutex));
            return;
        } 
        if((obj->count%5==0)&&(obj->count%3!=0)){
            printBuzz();
            obj->count++;
            pthread_cond_broadcast(&(obj->cond));
        }else{
            pthread_cond_wait(&(obj->cond),&(obj->mutex));
        }
        pthread_mutex_unlock(&(obj->mutex));
    }    
}

void fizzbuzz(FizzBuzz* obj) {
    while(1){
        pthread_mutex_lock(&(obj->mutex));
        if(obj->count>obj->n){
            pthread_mutex_unlock(&(obj->mutex));
            return;
        } 
        if((obj->count%3==0)&&(obj->count%5==0)){
            printFizzBuzz();
            obj->count++;
            pthread_cond_broadcast(&(obj->cond));
        }else{
            pthread_cond_wait(&(obj->cond),&(obj->mutex));
        }
        pthread_mutex_unlock(&(obj->mutex));
    }  
}

// You may call global function `void printNumber(int x)`
// to output "x", where x is an integer.
void number(FizzBuzz* obj) {
    while(1){
        pthread_mutex_lock(&(obj->mutex));
        if(obj->count>obj->n){
            pthread_mutex_unlock(&(obj->mutex));
            return;
        } 
        if((obj->count%3!=0)&&(obj->count%5!=0)){
            printNumber(obj->count);
            obj->count++;
            pthread_cond_broadcast(&(obj->cond));
        }else{
            pthread_cond_wait(&(obj->cond),&(obj->mutex));
        }
        pthread_mutex_unlock(&(obj->mutex));
    }  
}

void fizzBuzzFree(FizzBuzz* obj) {
    pthread_mutex_destroy(&(obj->mutex));
    pthread_cond_destroy(&(obj->cond));
    free(obj);
}
```
```cpp
/*
    Solution with mutex and conditional variable in C++
*/
class FizzBuzz {
private:
    int n;
    std::mutex mtx;
    std::condition_variable cv;
    int count;

public:
    FizzBuzz(int n) {
        this->n = n;
        this->count = 1;
    }

    void fizz(function<void()> printFizz) {
        std::unique_lock<std::mutex> lock(mtx);
        while(count<=n){
            cv.wait(lock,[this]{return (count%3==0 && count%5!=0)||(count>n);});
            if(count>n) return;
            printFizz();
            //cout << count << "A" << endl;
            count++;
            cv.notify_all();
        }
    }

    void buzz(function<void()> printBuzz) {
        std::unique_lock<std::mutex> lock(mtx);
        while(count<=n){
            cv.wait(lock,[this]{return (count%3!=0 && count%5==0)||(count>n);});
            if(count>n) return;
            printBuzz();
            //cout << count << "B" << endl;
            count++;
            cv.notify_all();
        }
    }

	void fizzbuzz(function<void()> printFizzBuzz) {
        std::unique_lock<std::mutex> lock(mtx);
        while(count<=n){
            cv.wait(lock,[this]{return (count%3==0 && count%5==0)||(count>n);});
            if(count>n) return;
            printFizzBuzz();
            //cout << count << "C" << endl;
            count++;
            cv.notify_all();
        }
    }

    void number(function<void(int)> printNumber) {
        std::unique_lock<std::mutex> lock(mtx);
        while(count<=n){
            cv.wait(lock,[this]{return (count%3!=0 && count%5!=0)||(count>n);});
            if(count>n) return;
            printNumber(count);
            //cout << count << "D" << endl;
            count++;
            cv.notify_all();
        }
    }
};
```
---
### 1226. The Dining Philosophers
### Problem
The Dining Philosophers problem is a classic synchronization problem that illustrates the challenges of resource sharing among concurrent processes. It involves a scenario with five philosophers sitting at a dining table, where they alternate between thinking and eating. Each philosopher needs two forks to eat, and there is one fork placed between each pair of philosophers. The challenge is to design a system where the philosophers can eat without running into deadlock or starvation situations. You need to implement a solution using threads that satisfies the following conditions:
1. **Philosopher Behavior**:
   - Each philosopher must alternate between thinking and eating.
   - A philosopher can only eat if they have acquired both forks (the left fork and the right fork).
2. **Resource Management**:
   - There are a limited number of forks (one between each pair of philosophers).
   - Philosophers should not starve; they need to be able to eat within a reasonable time frame.
3. **Synchronization**:
   - Proper synchronization mechanisms must be applied to prevent deadlocks.
   - Ensure that no two philosophers try to pick up the same fork simultaneously.

### Solution
```cpp
/*
    Solution with Semaphore in C++
*/
class DiningPhilosophers {
private:
    vector<sem_t> semaphores; // a vector of semaphores of forks
public:
    DiningPhilosophers() {
        semaphores.resize(5); 
        for(int i=0; i<5; ++i){
            sem_init(&semaphores[i],0,1);
        }
    }
    ~DiningPhilosophers(){
        for(int i=0; i<5; ++i){
            sem_destroy(&semaphores[i]);
        }
    }
    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {

        int leftFork = philosopher, rightFork = (philosopher+1)%5;
        if(philosopher==4) swap(leftFork,rightFork); //to break circular waiting

        sem_wait(&semaphores[leftFork]); //wait for the left fork
        sem_wait(&semaphores[rightFork]); //wait for the right fork
        
        pickLeftFork();
        pickRightFork();
        
        eat();
        
        putLeftFork();
        putRightFork();
        
        sem_post(&semaphores[leftFork]); //release the left fork
        sem_post(&semaphores[rightFork]); //release the right fork   
    }
};
```
```cpp
/*
    Solution in C++ with semaphore(arbitrator)
*/
class DiningPhilosophers {
private:
    vector<sem_t> semaphores; // a vector of semaphores of forks
    sem_t arbitrator; // the semaphores of arbitrator
public:
    DiningPhilosophers() {
        semaphores.resize(5); 
        for(int i=0; i<5; ++i){
            sem_init(&semaphores[i],0,1);
        }
        sem_init(&arbitrator,0,1);
    }
    ~DiningPhilosophers(){
        for(int i=0; i<5; ++i){
            sem_destroy(&semaphores[i]);
        }
        sem_destroy(&arbitrator);
    }
    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {

        sem_wait(&arbitrator);

        int leftFork = philosopher, rightFork = (philosopher+1)%5;
        
        sem_wait(&semaphores[leftFork]); //wait for the left fork
        sem_wait(&semaphores[rightFork]); //wait for the right fork

        pickLeftFork();
        pickRightFork();

        eat();

        putLeftFork();
        putRightFork();

        sem_post(&semaphores[leftFork]); //release the left fork
        sem_post(&semaphores[rightFork]); //release the right fork

        sem_post(&arbitrator);   
    }

};
```
---
## 621. Task Scheduler
### Problem
You are given a list of tasks represented by uppercase letters, where each letter denotes a specific task. The tasks are to be executed in a sequence, but there is a cooling period, n, that must be maintained between two same tasks. This means that after executing a task, the same task cannot be executed again until n other tasks have been executed. The goal is to determine the least amount of time required to complete all tasks while adhering to the cooling period.
### Solution
```c
/*
    Solution in C with greedy algorithm
*/
#define SIZE 26
int leastInterval(char* tasks, int tasksSize, int n) {
    //an array of counter of tasks
    int count[SIZE] = {0};
    //iterate through the array to count the tasks
    int i=0, maxCount=0;
    for(i=0; i<tasksSize; ++i){
        count[tasks[i]-'A']++;
    }
    //get the maximum count of tasks
    for(i=0; i<SIZE; ++i){
        if(count[i]>maxCount){
            maxCount=count[i];
        }
    }
    //if the tasks can be scheduled in a greedy way
    int greedyCount = (maxCount-1)*(n+1);
    for(i=0;i<SIZE; ++i){
        if(count[i]==maxCount){
            greedyCount++;
        }
    }
    return greedyCount>tasksSize? greedyCount: tasksSize;
    //time complexity: O(taskSize+SIZE)
    //space complexity: O(SIZE)
}
```
---
## 1834. Single-Threaded CPU
### Problem
Input: You have n tasks represented as a 2D integer array tasks, where each task tasks[i] is defined by:
- enqueueTime[i]: The time at which the task becomes available for processing.
- processingTime[i]: The time it takes to complete the task.
Behavior of the CPU
- Idle State: If the CPU is idle and no tasks are available, it remains idle.
- Task Selection: If the CPU is idle but there are available tasks:
    - It selects the task with the shortest processing time.
    - If multiple tasks have the same processing time, the task with the smallest index (original position) is selected.
    - Once a task starts processing, it continues without interruption until completion, after which it can start a new task immediately.
Output: The output should be an array representing the order in which the tasks are processed based on their indices in the original tasks array.
### Solution
```cpp
/**
 * Solution in C++
 */
class Solution {
public:
    vector<int> getOrder(vector<vector<int>>& tasks) {
        vector<int> ans;
        //the number of tasks
        int numTasks = tasks.size();
        //index of tasks
        vector<int> idx(numTasks);
        iota(idx.begin(),idx.end(),0);
        //sort the index based on equeue time
        sort(idx.begin(),idx.end(),[&](int i, int j){
            return tasks[i][0]<tasks[j][0];
        });
        priority_queue <pair<unsigned int,int>, vector<pair<unsigned int,int>>, greater<pair<unsigned int,int>>> pq;
        //time count of CPU
        unsigned int timeCnt = 0;
        //index for idx array
        int i=0;
        //schedule each task based on rules described
        while(ans.size()<numTasks){
            if(pq.empty()){
                timeCnt = max(timeCnt,static_cast<unsigned int>(tasks[idx[i]][0]));
            }
            while(i<numTasks&&tasks[idx[i]][0]<=timeCnt){
                pq.push({tasks[idx[i]][1],idx[i]});
                ++i;
            }
            //a single threaded CPU execute one task each time
            if(!pq.empty()){
                auto node = pq.top();
                timeCnt+= node.first;
                ans.push_back(node.second);
                pq.pop();
            }
        }
        return ans;
        //time complexity: O(nlogn)
        //space complexity: O(n)
    }
};
```
```c
/**
 * Solution in C
 * Note: The returned array must be malloced, assume caller calls free().
 */
//task struct with index
typedef struct{
    int idx;
    int enqueueTime;
}taskEnqueue_t;
//compare function for qsort
int compare(const void *a, const void *b) {
    // Cast the pointers to taskEnqueue_t pointers
    const taskEnqueue_t *taskA = (const taskEnqueue_t *)a;
    const taskEnqueue_t *taskB = (const taskEnqueue_t *)b;
    // Compare the enqueueTime fields
    return taskA->enqueueTime - taskB->enqueueTime; // Ascending order
}
//elements of min heap
typedef struct{
    int processingTime;
    int idx;
}taskProcess_t;
//min heap for available tasks
typedef struct{
    int capacity;
    int size;
    taskProcess_t * elements;
}minHeap_t;
//create struct with id and enqueue time
taskEnqueue_t * createTaskEnqueue(int ** tasks, int tasksSize){
    taskEnqueue_t * taskEnqueue = (taskEnqueue_t *)malloc(tasksSize*sizeof(taskEnqueue_t));
    if(taskEnqueue==NULL){
        return NULL;
    }
    for(int i=0; i<tasksSize; ++i){
        taskEnqueue[i].idx=i;
        taskEnqueue[i].enqueueTime=tasks[i][0];
    }
    return taskEnqueue;
}
//create min heap
minHeap_t * creatMinHeap(int capacity){
    minHeap_t * minHeap = (minHeap_t *)malloc(sizeof(minHeap_t));
    if(minHeap==NULL){
        return NULL;
    }
    minHeap->capacity=capacity;
    minHeap->size=0;
    minHeap->elements = (taskProcess_t *)malloc(capacity*sizeof(taskProcess_t));
    if(minHeap->elements==NULL){
        return NULL;
    }
    return minHeap;
}
//parent index
int parentIndex(int index){
    return (index-1)/2;
}
//left child index
int leftChildIndex(int index){
    return 2*index+1;
}
//right child index
int rightChildIndex(int index){
    return 2*index+2;
}
//swap function
void swap(taskProcess_t * a, taskProcess_t * b){
    taskProcess_t  tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}
//push new elements into min heap
void push(minHeap_t * minHeap, taskProcess_t task){
    if(minHeap->size==minHeap->capacity){
        return;
    }
    minHeap->size++;
    int i = minHeap->size-1;
    minHeap->elements[i]=task;
    int p = parentIndex(i);
    while(i>=0&&((minHeap->elements[p].processingTime>minHeap->elements[i].processingTime)
        ||(minHeap->elements[p].processingTime==minHeap->elements[i].processingTime
        &&minHeap->elements[p].idx>minHeap->elements[i].idx))){
        swap(&(minHeap->elements[p]),&(minHeap->elements[i]));
        i=p;
        p=parentIndex(i);
    }
    return;
}
//pop new elements into min heap
taskProcess_t pop(minHeap_t * minHeap){
    taskProcess_t task = minHeap->elements[0];
    swap(&(minHeap->elements[0]),&(minHeap->elements[minHeap->size-1]));
    minHeap->size--;
    
    int i=0;
    int l=leftChildIndex(i), r=rightChildIndex(i), smallest=i;
    while(l<minHeap->size){
        smallest=l;
        if(r<minHeap->size&&((minHeap->elements[smallest].processingTime>minHeap->elements[r].processingTime)
        ||(minHeap->elements[smallest].processingTime==minHeap->elements[r].processingTime&&
        minHeap->elements[smallest].idx>minHeap->elements[r].idx))){
            smallest=r;
        }
        if(((minHeap->elements[smallest].processingTime>minHeap->elements[i].processingTime)
        ||(minHeap->elements[smallest].processingTime==minHeap->elements[i].processingTime&&
        minHeap->elements[smallest].idx>minHeap->elements[i].idx))){
            break;
        }
        swap(&(minHeap->elements[i]),&(minHeap->elements[smallest]));
        i=smallest;
        l=leftChildIndex(i);
        r=rightChildIndex(i);
    }
    return task;
}
//free min heap
void freeMinHeap(minHeap_t * minHeap){
    if(minHeap!=NULL){
        free(minHeap->elements);
        free(minHeap);
    }
    return;
}
//solution function
int* getOrder(int** tasks, int tasksSize, int* tasksColSize, int* returnSize) {
    //the size of answer array
    *returnSize=tasksSize;
    //initialize answer array
    int * ans = (int *)malloc(tasksSize*sizeof(int));
    if(ans==NULL){
        return NULL;
    }
    //initialize index and enqueue time array
    taskEnqueue_t * taskEnqueue = createTaskEnqueue(tasks, tasksSize);
    //sort
    qsort(taskEnqueue, tasksSize, sizeof(taskEnqueue_t), compare);
    //a min heap to store available tasks based on dequeue time
    minHeap_t * available = creatMinHeap(tasksSize);
    //schedule the tasks based on the rule
    int count=0, i=0;
    unsigned int timeCount=0;
    while(count<tasksSize){
        //if available tasks is empty, update timeCount
        if(available->size==0){
            timeCount=fmax(timeCount,tasks[taskEnqueue[i].idx][0]);
        }
        //put available tasks in the min heap based on timeCount
        while(i<tasksSize&&tasks[taskEnqueue[i].idx][0]<=timeCount){
            taskProcess_t task;
            task.processingTime=tasks[taskEnqueue[i].idx][1];
            task.idx=taskEnqueue[i].idx;
            push(available,task);
            i++;
        }
        //process the available tasks based on processing time and then update timeCount
        if(available->size>0){
            taskProcess_t task = pop(available);
            timeCount+=task.processingTime;
            ans[count++]=task.idx;
        }
    }
    //free allocated heap
    free(taskEnqueue);
    freeMinHeap(available);
    return ans;
}
```
---
## 1882. Process Tasks Using Servers
### Problem
You are given two integers, n (the number of servers) and m (the number of tasks), and an array tasks representing the time each task takes to complete. The goal of the problem is to determine when each task will be completed by processing them using the available servers. Operations:
- Server Assignment: Each server can handle one task at a time. If multiple servers are available, tasks should be assigned to the server that becomes available the earliest. If there is a tie, assign the task to the server with the smallest index.
- Task Processing: When a server completes a task, it becomes available again. The task assignment continues until all tasks are processed.
- Return Completion Times: For each task in the input array, return the time at which it is completed.
- Constraints:
    - The number of servers (n) and the number of tasks (m) can vary, but both will be within the limits that ensure efficient processing.
    - The length of the tasks array will match the number of tasks to be processed.
### Solution
- a priority queue to store the available servers 
- a priority queue to store the busy servers
```cpp
class Solution {
public:
    vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
        //get the length of vectors
        int n = servers.size(), m= tasks.size();
        //a priority queue to store server weight and index
        priority_queue<pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>>> availableServers;
        for(int i=0; i<n; ++i){
            availableServers.emplace(servers[i],i);
        }
        //a queue for servers not free
        priority_queue<pair<unsigned int,int>, vector<pair<unsigned int,int>>, greater<pair<unsigned int,int>>> busyServers;
        //answer vector 
        vector<int> ans;
        unsigned int secondsCnt = 0;
        for(int i=0; i<m; ++i){
            //if servers not free is not empty, free the servers based on time
            secondsCnt=max(secondsCnt,static_cast<unsigned int>(i));
            if((availableServers.empty())&&(!busyServers.empty())){  
                secondsCnt=busyServers.top().first;
            }
            while(!busyServers.empty()&&busyServers.top().first<=secondsCnt){
                availableServers.emplace(servers[busyServers.top().second],busyServers.top().second);
                busyServers.pop();
            }
           
            auto serverTmp = availableServers.top();
            ans.emplace_back(serverTmp.second);
            availableServers.pop();
            busyServers.emplace(secondsCnt+static_cast<unsigned int>(tasks[i]),serverTmp.second);    
        }
        return ans;
        //time complexity: O((n+m)logn)
        //space complexity: O(n+m)
    }
};
```
```c
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
//a struct of pair of unsigned int
typedef struct{
    unsigned int first;
    unsigned int second;
}pair_t;
//a struct of server priority queue
typedef struct{
    pair_t *elements;
    int capacity;
    int size;
}priorityQueue_t;
//create server priority queue
priorityQueue_t * createPriorityQueue(int capacity){
    priorityQueue_t * obj = (priorityQueue_t *)malloc(sizeof(priorityQueue_t));
    if(obj==NULL){
        return NULL;
    }
    obj->elements = (pair_t *)malloc(capacity*sizeof(pair_t));
    if(obj->elements==NULL){
        return NULL;
    }
    obj->capacity=capacity;
    obj->size=0;
    return obj;
}
//parent index
int parentIndex(int index){
    return (index-1)/2;
}
//left child index
int leftChildIndex(int index){
    return 2*index+1;
}
//right child index
int rightChildIndex(int index){
    return 2*index+2;
}
//swap
void swap(pair_t *a, pair_t *b){
    pair_t tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}
//push operation 
void push(priorityQueue_t * pq, unsigned int first, unsigned int second){
    if(pq->size==pq->capacity){
        return;
    }
    //push at the last position
    int i=pq->size++;
    pq->elements[i].first=first;
    pq->elements[i].second=second;
    int p=parentIndex(i);
    //move up
    while(i>=0&&((pq->elements[p].first>pq->elements[i].first)||
    (pq->elements[p].first==pq->elements[i].first&&pq->elements[p].second>pq->elements[i].second))){
        swap(&(pq->elements[p]),&(pq->elements[i]));
        i=p;
        p=parentIndex(i);
    }
    return;
}
//pop operation
pair_t pop(priorityQueue_t * pq){
    //extract the top of this prority queue
    pair_t pair = pq->elements[0];
    swap(&(pq->elements[0]),&(pq->elements[--pq->size]));
    //move down
    int i=0, l=leftChildIndex(0), r=rightChildIndex(0), smallest=0;
    while(l<pq->size){
        smallest=l;
        if(r<pq->size&&((pq->elements[smallest].first>pq->elements[r].first)||
        (pq->elements[smallest].first==pq->elements[r].first&&pq->elements[smallest].second>pq->elements[r].second))){
            smallest=r;
        }
        if((pq->elements[smallest].first>pq->elements[i].first)||
        (pq->elements[smallest].first==pq->elements[i].first&&pq->elements[smallest].second>pq->elements[i].second)){
            break;
        }
        swap(&(pq->elements[i]),&(pq->elements[smallest]));
        i=smallest;
        l=leftChildIndex(i);
        r=rightChildIndex(i);
    }
    return pair;
}
//free priority queue
void freePriorityQueue(priorityQueue_t * pq){
    if(pq!=NULL){
        free(pq->elements);
        free(pq);
    }
    return;
}
//solution function
int* assignTasks(int* servers, int serversSize, int* tasks, int tasksSize, int* returnSize) {
    //assign return Size
    *returnSize=tasksSize;
    //construct ans array 
    int * ans = (int *)malloc(tasksSize*sizeof(int));
    if(ans==NULL){
        return NULL;
    }
    //construct available servers priority queue
    //first: weight, second: index
    priorityQueue_t * availableServers = createPriorityQueue(serversSize);
    for(int i=0; i<serversSize; ++i){
        push(availableServers,servers[i],i);
    }
    //initialize busy servers priority queue 
    //first: time to finish task, second: index
    priorityQueue_t * busyServers = createPriorityQueue(serversSize);
    //process tasks with servers
    unsigned int seconds=0;
    for(int i=0;i<tasksSize;++i){
        //update seconds
        seconds=fmax(seconds,i);
        //available server is empty but busy server is not empty
        if(availableServers->size==0&&busyServers->size>0){
            //update seconds
            seconds=busyServers->elements[0].first;
        }
        //based on the seconds, pop from busy servers and push them in available servers
        while(busyServers->size>0&&busyServers->elements[0].first<=seconds){
            pair_t serverTmp = pop(busyServers);
            push(availableServers,servers[serverTmp.second],serverTmp.second);
        }
        //pop an available server, put it into ans array
        if(availableServers->size>0){
            pair_t serverTmp = pop(availableServers);
            ans[i]=serverTmp.second;
            //update busy server
            push(busyServers,seconds+tasks[i],serverTmp.second);
        }
    }
    free(availableServers);
    free(busyServers);
    return ans;
}
```
---
## 146. LRU Cache
### Problem
Design and implement a data structure for an LRU (Least Recently Used) Cache. The cache should support the following operations:
- LRUCache(int capacity): Initialize the LRU cache with a positive integer capacity.
- int get(int key): Retrieve the value associated with the key if it exists in the cache. If the key does not exist, return -1. Accessing a key updates its usage to be the most recently used.
- void put(int key, int value): Insert or update the value associated with the key. If the key already exists, update its value and mark it as recently used. If the cache is full, evict the least recently used item before inserting the new key-value pair.
- Constraints
    - The capacity will be a positive integer, indicating the maximum number of items the cache can hold.
    - Both get and put operations must run in O(1) time complexity.
### Solution
```cpp
class LRUCache {
private:
    int capacity;
    unordered_map<int, list<pair<int, int>>::iterator> hashmap;
    list<pair<int,int>> accessOrder;

    void moveRUfront(list<pair<int, int>>::iterator it){
        accessOrder.splice(accessOrder.begin(),accessOrder,it);
    }

public:
    LRUCache(int capacity) {
        this->capacity=capacity;
    }
    
    int get(int key) {
        if(hashmap.find(key)==hashmap.end()){
            return -1;
        }
        moveRUfront(hashmap[key]);
        return hashmap[key]->second;
    }
    
    void put(int key, int value) {
        auto it = hashmap.find(key);
        if(it!=hashmap.end()){
            hashmap[key]->second=value;
            moveRUfront(hashmap[key]);
            return;
        }
        
        //pay attention here, should be equal, not larger
        if(hashmap.size()==capacity){
            int deleteKey = accessOrder.back().first;
            hashmap.erase(deleteKey);
            accessOrder.pop_back();
        }

        accessOrder.push_front({key,value});
        hashmap[key] = accessOrder.begin();
    }
};
```
```cpp
class LRUCache {
private:
    struct Node{
        int key;
        int value;
        Node * pre;
        Node * next;
        Node(int _key, int _value): key(_key), value(_value), pre(nullptr), next(nullptr) {}
    };

    typedef Node node_t;

    int capacity;
    node_t * head;
    node_t * tail;
    unordered_map<int, node_t *> hashmap;

    void removeNode(node_t * node){
        if(node->pre!=nullptr&&node->next!=nullptr){
            node->pre->next=node->next;
            node->next->pre=node->pre;
        }
    }

    void addFront(node_t * node){
        node->next=head->next;
        node->pre=head;
        head->next->pre=node;
        head->next=node;
    }

    void moveFront(node_t * node){
        removeNode(node);
        addFront(node);
    }

    int removeLast(){
        node_t * pTmp = tail->pre;
        removeNode(pTmp);
        int deleteKey = pTmp->key;
        delete pTmp;
        return deleteKey;
    }    

public:
    LRUCache(int capacity) {
        this->capacity=capacity;
        this->head=new node_t(0,0);
        this->tail=new node_t(0,0);
        this->head->next=this->tail;
        this->tail->pre=this->head;
    }

    ~LRUCache() {
        while(this->head){
            node_t * pTmp = this->head;
            this->head=this->head->next;
            delete pTmp;
        }
    }
    
    int get(int key) {
        auto it = hashmap.find(key);
        if(it!=hashmap.end()){
            moveFront(it->second);
            return it->second->value;
        }
        return -1;
    }
    
    void put(int key, int value) {
        auto it = hashmap.find(key);
        if(it!=hashmap.end()){
            it->second->value=value;
            moveFront(it->second);
            return;
        }
        if(hashmap.size()==capacity){
            int deleteKey = removeLast();
            hashmap.erase(deleteKey);
        }
        node_t * node = new node_t(key,value);
        moveFront(node);
        hashmap[key]=node;
    }
};
```
---
## 460. LFU Cache
### Problem
Design and implement a data structure for a Least Frequently Used (LFU) Cache. It should support the following operations:
- LFUCache(int capacity): Initialize the LFU cache with a positive capacity. If the capacity is zero, the cache should be empty.
- int get(int key): Retrieve the value of the key if the key exists in the cache. If the key does not exist, return -1.
- void put(int key, int value): Update or insert the value if the key is not already present. When the cache reaches its capacity, it should invalidate the least frequently used key before inserting a new key.
- Constraints:
    - The operations get and put must run in O(1) time complexity.
    - The cache will store up to a specified maximum number of items (the capacity).
    - If there is a tie in the frequency count, the least recently used item among those with the same frequency should be removed.
### Solution
```cpp
class LFUCache {
private:
    struct Node{
        int key;
        int value;
        int counter;
        Node(int _key, int _value, int _counter): key(_key), value(_value), counter(_counter){}
    };
    typedef Node node_t;
    int capacity;
    int minCounter;
    unordered_map<int, list<node_t>::iterator> keyMap;
    unordered_map<int, list<node_t>> counterMap;

public:
    LFUCache(int capacity) {
        this->capacity=capacity;
        this->minCounter=0;
    } 
    
    int get(int key) {
        auto it = keyMap.find(key);
        if(it!=keyMap.end()){
            //extract iterator
            auto nodeIt = it->second;
            int deleteCounter = nodeIt->counter;
            int returnValue = nodeIt->value;
            //update counter
            nodeIt->counter++;
            //update counterMap hashmap by moving itertor from source list to destination list
            counterMap[nodeIt->counter].splice(counterMap[nodeIt->counter].begin(),counterMap[deleteCounter],nodeIt);
            //check if the source list is empty
            if(counterMap[deleteCounter].empty()){
                //erase the key and value
                counterMap.erase(deleteCounter);
                //update minCounter
                if(minCounter==deleteCounter){
                    minCounter++;
                }
            }
            //update keyMap hashmap
            keyMap[nodeIt->key]=counterMap[nodeIt->counter].begin();
            return returnValue;
        }
        return -1;
    }
    
    void put(int key, int value) {
        auto it = keyMap.find(key);
        if(it!=keyMap.end()){
            //extract iterator
            auto nodeIt = it->second;
            nodeIt->value = value;
            int deleteCounter = nodeIt->counter;
            //update counter
            nodeIt->counter++;
            //update counterMap hashmap by moving itertor from source list to destination list
            counterMap[nodeIt->counter].splice(counterMap[nodeIt->counter].begin(),counterMap[deleteCounter],nodeIt);
            //check if the source list is empty
            if(counterMap[deleteCounter].empty()){
                //erase the key and value
                counterMap.erase(deleteCounter);
                //update minCounter
                if(minCounter==deleteCounter){
                    minCounter++;
                }
            }
            //update keyMap hashmap
            keyMap[nodeIt->key]=counterMap[nodeIt->counter].begin();
            return;
        }
        if(keyMap.size()==capacity){
            //extract element based on minCounter from counterMap hashmap
            auto it = counterMap[minCounter].back();
            //extract key
            int deleteKey = it.key;
            //erase from keyMap
            keyMap.erase(deleteKey);
            //delete element from counterMap hashmap
            counterMap[minCounter].pop_back();
        }
        //update minCounter
        minCounter=1;
        //insert to the begin of minCounter list
        counterMap[minCounter].push_front(node_t(key,value,1));
        //update keyMap
        keyMap[key]=counterMap[minCounter].begin();
        return;
    }   
};
```
---
## 2502. Design Memory Allocator
### Problem
You are tasked with designing a memory allocator that manages a 0-indexed memory array of size n, where all memory units are initially free. The allocator should support the following functionalities:
- Allocate:
    - Allocate a block of size "size" consecutive free memory units and assign it a unique identifier mID.
    - Return the starting index of the allocated block or -1 if no such block exists.
- Free Memory:
    - Free all memory units associated with the given identifier mID.
    - Return the total number of memory units freed.
- Constraints:
    - Multiple blocks can be allocated to the same mID.
    - All memory units corresponding to an mID must be freed, even if they were allocated in different blocks.
### Solution
```c
typedef struct {
    int * memArr;
    int memSize;
} Allocator;
//1 <= n, size, mID <= 1000, use 0 to indicate free memory elements
Allocator* allocatorCreate(int n) {
    Allocator* obj = (Allocator*)malloc(sizeof(Allocator));
    if(obj==NULL){
        return NULL;
    }
    obj->memSize=n;
    obj->memArr=(int *)calloc(n,sizeof(int));
    if(obj->memArr==NULL){
        return NULL;
    }
    return obj;
}
int allocatorAllocate(Allocator* obj, int size, int mID) {
    int i=0, j=0, consecutiveFreeCount=0;
    if(obj!=NULL){
        for(int i=0; i<obj->memSize; i++){
            if(obj->memArr[i]==0){
                consecutiveFreeCount++;
                if(consecutiveFreeCount==size){
                    for(int j=i-size+1; j<=i; j++){
                        obj->memArr[j]=mID;
                    }
                    return i-size+1;
                }
            }else{
                consecutiveFreeCount=0;
            }  
        }
    }
    return -1;
}
int allocatorFreeMemory(Allocator* obj, int mID) {
    int i=0, count=0;
    if(obj!=NULL){
        for(i=0; i<obj->memSize; ++i){
            if(obj->memArr[i]==mID){
                obj->memArr[i]=0;
                count++;
            }
        }
    }
    return count;
}
void allocatorFree(Allocator* obj) {
    if(obj!=NULL){
        free(obj->memArr);
        free(obj);
    }
    return;
}
---
```
## 352. Data Stream as Disjoint Intervals
### Problem
You are tasked with designing a data structure that handles a stream of integers and maintains a collection of disjoint intervals. The key operations you need to support are adding new integers to the stream and retrieving the current list of disjoint intervals. Operations:
- SummaryIntervals(): Initializes the data structure.
- void addNum(int val): Adds an integer val to the stream. This operation may cause new intervals to be created or existing intervals to be merged if val falls within an existing interval or adjacent to it.
- int[][] getIntervals(): Returns a list of the current disjoint intervals in sorted order.
- Constraints:
    - The integers added to the stream can be any valid integer, and the number of calls to addNum and getIntervals will be within reasonable limits.
    - The intervals should be represented as arrays of size 2, where each interval is defined by its start and end values.
### Solution
```cpp
class SummaryRanges {
private:
    set<pair<int, int>> intervals;
public:
    SummaryRanges() {}
    
    void addNum(int value) {
        auto it = intervals.lower_bound({value,value});
        bool merged = false;
        //check the previous interval
        if(it!=intervals.begin()){
            --it;
            if(it->second>=value){
                //no need to add
                return;
            }
            //check if it can be merged or not
            if(it->second+1>=value){
                int valueLeft = it->first;
                intervals.erase(it);
                intervals.insert({valueLeft, value});
                merged = true;
            }
        }
        //to prevent "heap-use-after-free"
        it = intervals.lower_bound({value,value});
        //check the current interval
        if(it!=intervals.end()&&value+1>=it->first){
            //merge with the current interval
            int valueLeft = min(value,it->first);
            int valueRight = it->second;
            auto itTmp = it;
            if(it!=intervals.begin()){
                it--;
                //check whether need to merge with the previous interval or not
                if(it->second+1>=valueLeft){
                    valueLeft=min(valueLeft,it->first);
                    intervals.erase(it);
                }
            }
            intervals.erase(itTmp);
            intervals.insert({valueLeft,valueRight});
            return;
        }
        if(!merged){
            intervals.insert({value,value});
        }
    }
    
    vector<vector<int>> getIntervals() {
        vector<vector<int>> ans;
        for(const auto& interval: intervals){
            ans.push_back({interval.first, interval.second});
        }
        return ans;        
    }
    //time complexity: O(nlogn)
    //space complexity: O(n)
};
```
---
## 622. Design Circular Queue
### Problem
You need to implement a circular queue with the following functionalities:
- Initialization: Create a circular queue with a specified maximum size.
- Enqueue: Insert an element at the rear of the queue. If the queue is full, return false.
- Dequeue: Remove an element from the front of the queue. If the queue is empty, return false.
- Front: Get the front item of the queue without removing it. If the queue is empty, return -1.
- Rear: Get the last item of the queue without removing it. If the queue is empty, return -1.
- isEmpty: Check if the queue is empty.
- isFull: Check if the queue is full.
### Solution
```c
typedef struct {
    int * buff;
    int size;
    int front;
    int rear;
} MyCircularQueue;
//circular queue struct
MyCircularQueue* myCircularQueueCreate(int k) {
    MyCircularQueue* obj = malloc(sizeof(MyCircularQueue));
    if(obj==NULL){
        return NULL;
    }
    //pay attention the size here: the last element of buff is left empty
    obj->size=k+1;
    obj->buff=(int *)malloc((obj->size)*sizeof(int));
    if(obj->buff==NULL){
        return NULL;
    }
    obj->front=0;
    obj->rear=0;
    return obj;
}
//EnQueue in the direction of rear, when full, return false
bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    if(obj!=NULL){
        if((obj->rear+1)%(obj->size)==(obj->front)){
            return false;
        }
        obj->buff[(obj->rear)]=value;
        obj->rear=(obj->rear+1)%(obj->size);
        return true;
    }
    return false;
}
//DeQueue in the direction of front, when empty, return false
bool myCircularQueueDeQueue(MyCircularQueue* obj) {
    if(obj!=NULL){
        if(obj->front==obj->rear){
            return false;
        }
        obj->front=(obj->front+1)%(obj->size);
        return true;
    }
    return false;
}
int myCircularQueueFront(MyCircularQueue* obj) {
    if(obj!=NULL){
        if(obj->front==obj->rear){
            return -1;
        }else{
            return obj->buff[obj->front];
        }
    }
    return -1;
}
int myCircularQueueRear(MyCircularQueue* obj) {
    if(obj!=NULL){
        if(obj->front==obj->rear){
            return -1;
        }else{
            //pay attention the index here
            return obj->buff[(obj->rear+obj->size-1)%(obj->size)];
        }
    }
    return -1;
}
bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    if(obj!=NULL){
        if(obj->front==obj->rear){
            return true;
        }
        return false;
    }
    return false;
}
bool myCircularQueueIsFull(MyCircularQueue* obj) {
    if(obj!=NULL){
        if((obj->rear+1)%(obj->size)==(obj->front)){
            return true;
        }else{
            return false;
        }
    }
    return false;
}
void myCircularQueueFree(MyCircularQueue* obj) {
    if(obj!=NULL){
        if(obj->buff==NULL){
            free(obj->buff);
        }
        free(obj);
    }
    return;
}
```
---
## 641. Design Circular Deque
### Problem
The task is to design a circular deque (double-ended queue), which supports the following operations:
- Initialization: Initialize the deque with a maximum size.
- insertFront: Insert an item at the front of the deque. Return true if the operation is successful; otherwise, return false.
- insertLast: Insert an item at the rear of the deque. Return true if the operation is successful; otherwise, return false.
- deleteFront: Delete an item from the front of the deque. Return true if the operation is successful; otherwise, return false.
- deleteLast: Delete an item from the rear of the deque. Return true if the operation is successful; otherwise, return false.
- getFront: Get the front item from the deque. If the deque is empty, return -1.
- getRear: Get the last item from the deque. If the deque is empty, return -1.
- isEmpty: Check whether the deque is empty.
- isFull: Check whether the deque is full.
### Solution
```c
typedef struct {
    int * buff;
    int size;
    int front;
    int rear;
    int count;
} MyCircularDeque;
//circular deque struct
MyCircularDeque* myCircularDequeCreate(int k) {
    MyCircularDeque* obj = malloc(sizeof(MyCircularDeque));
    if(obj==NULL){
        return NULL;
    }
    //pay attention the size here: the last element of buff is left empty
    obj->size=k+1;
    obj->buff=(int *)malloc((obj->size)*sizeof(int));
    if(obj->buff==NULL){
        return NULL;
    }
    obj->front=0;
    obj->rear=0;
    obj->count=0;
    return obj;
}
//circular deque insert front: front move backward then insert
bool myCircularDequeInsertFront(MyCircularDeque* obj, int value) {
    if(obj!=NULL){
        if(obj->count==obj->size-1){
            return false;
        }
        obj->front=(obj->front+obj->size-1)%(obj->size);
        obj->buff[obj->front]=value;
        obj->count++;
        return true;
    }
    return false;
}
//circular deque insert last: insert at rear and then move rear forward
bool myCircularDequeInsertLast(MyCircularDeque* obj, int value) {
    if(obj!=NULL){
        if(obj->count==obj->size-1){
            return false;
        }
        obj->buff[obj->rear]=value;
        obj->rear=(obj->rear+1)%(obj->size);
        obj->count++;
        return true;
    }
    return false;
}
bool myCircularDequeDeleteFront(MyCircularDeque* obj) {
    if(obj!=NULL){
        if(obj->count==0){
            return false;
        }
        obj->front=(obj->front+1)%(obj->size);
        obj->count--;
        return true;
    }
    return false;
}
bool myCircularDequeDeleteLast(MyCircularDeque* obj) {
    if(obj!=NULL){
        if(obj->count==0){
            return false;
        }
        obj->rear=(obj->rear+obj->size-1)%(obj->size);
        obj->count--;
        return true;
    } 
    return false;
}
int myCircularDequeGetFront(MyCircularDeque* obj) {
    if(obj!=NULL){
        if(obj->count==0){
            return -1;
        }
        return obj->buff[obj->front];
    } 
    return -1;
}
int myCircularDequeGetRear(MyCircularDeque* obj) {
    if(obj!=NULL){
        if(obj->count==0){
            return -1;
        }
        return obj->buff[(obj->rear+obj->size-1)%(obj->size)];
    } 
    return -1;
}
bool myCircularDequeIsEmpty(MyCircularDeque* obj) {
    if(obj!=NULL){
        return obj->count==0;
    } 
    return false;   
}
bool myCircularDequeIsFull(MyCircularDeque* obj) {
    if(obj!=NULL){
        return obj->count==obj->size-1;
    } 
    return false;
}
void myCircularDequeFree(MyCircularDeque* obj) {
    if(obj!=NULL){
        free(obj->buff);
        free(obj);
    }
    return;
}
```
---
## 1860. Incremental Memory Leak
### Problem
You have two memory sticks with initial available memory given by two integers, memory1 and memory2. A faulty program runs and attempts to allocate an increasing amount of memory every second. Specifically, at the i-th second, it tries to allocate i bits of memory to the stick that has more available memory (or to the first stick if both have the same amount). The program will crash if neither memory stick has enough available memory to fulfill the allocation request at any given second. You need to return an array containing three elements:
- crashTime: The time (in seconds) when the program crashes.
- memory1crash: The amount of available memory left in the first memory stick at the time of the crash.
- memory2crash: The amount of available memory left in the second memory stick at the time of the crash.
### Solution
```c
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* memLeak(int memory1, int memory2, int* returnSize) {
    //malloc answer array
    *returnSize=3;
    int *ans=(int *)malloc(3*sizeof(int));
    if(ans==NULL){
        return NULL;
    }
    //simulate memory crash process
    int seconds=1;
    while(1){
        if(memory1>=memory2){
            if(memory1>=seconds){
                memory1-=seconds;
            }else{
                break;
            }
        }else{
            if(memory2>=seconds){
                memory2-=seconds;
            }else{
                break;
            }
        }
        seconds++;
    }
    //fill the answer array
    ans[0]=seconds;
    ans[1]=memory1;
    ans[2]=memory2;

    return ans;
}
```
---
## 1381. Design a Stack With Increment Operation
### Problem
Design a stack that supports the following operations:
- CustomStack(int maxSize): Initializes the stack with a maximum size maxSize.
- void push(int x): Pushes an integer x onto the stack. If the stack has reached its maxSize, this operation will not do anything.
- int pop(): Removes the element on the top of the stack and returns it. If the stack is empty, it should return -1.
- void increment(int k, int val): Increments the bottom k elements of the stack by val. If there are fewer than k elements in the stack, it increments all of them.
### Solution
```c
typedef struct {
    int * buff;
    int capacity;
    int count;
} CustomStack;

CustomStack* customStackCreate(int maxSize) {
    CustomStack* obj = (CustomStack*)malloc(sizeof(CustomStack));
    if(obj==NULL){
        return NULL;
    }
    obj->capacity = maxSize;
    obj->count = 0;
    obj->buff = (int *)malloc(maxSize*sizeof(int));
    if(obj->buff==NULL){
        return NULL;
    }
    return obj;
}
void customStackPush(CustomStack* obj, int x) {
    if(obj!=NULL&&obj->count<obj->capacity){
        obj->count++;
        obj->buff[obj->count-1]=x;   
    }
}
int customStackPop(CustomStack* obj) {
    if(obj!=NULL&&obj->count>0){
        return obj->buff[--obj->count];
    }
    return -1;
}
void customStackIncrement(CustomStack* obj, int k, int val) {
    int i=0;
    if(obj!=NULL){
        for(i=0;i<obj->count&&i<k;i++){
            obj->buff[i]+=val;
        }
    }
}
void customStackFree(CustomStack* obj) {
    if(obj!=NULL){
        free(obj->buff);
        free(obj);
    }
}
```
---
## 388. Longest Absolute File Path
### Problem
You are given a string input representing a list of files and directories in a file system. The format of this input is as follows:
- Each line represents a file or directory.
- The number of leading tabs (\t) indicates the depth of the item in the file hierarchy. For example, an item with no leading tabs is at the root level, one leading tab indicates it is a child of the previous item, and so on.
- A file name ends with an extension (e.g., .txt, .jpg), while a directory name does not.
- Your task is to find the length of the longest absolute path to a file in the file system. The absolute path is defined as the sequence of directory names leading to the file, including the filename itself. The length of the path is the sum of the lengths of all the directory and file names in the path, including the slashes (/) that separate them.

Input:
- A single string input containing lines representing the file system structure. The lines consist of:
- A combination of directory names and file names.
- Each line is separated by a newline character (\n).
- Each line starts with zero or more tab characters (\t) followed by the name of the file or directory.

Output:
- Return an integer representing the length of the longest absolute path to a file. If there are no files in the input, return 0.
### Solution
```c
int lengthLongestPath(char* input) {
    //calculate the length of input
    int length=strlen(input);
    char * tmp = input;
    //initialize stack
    int * stack = (int *)calloc(length,sizeof(int));
    if(stack==NULL){
        return 0;
    }
    //variables initialization
    int depth=0, count=0, maxCount=0;
    //process the string
    char * token = strtok(input, "\n");
    while(token!=NULL){
        depth = 0;
        while(token[depth]=='\t'){
            depth++;
        }
        //get the str, jump over the '\t'
        char *str= token+depth;
        if(depth>0){
            //include '/' or '\0'
            count=stack[depth-1]+strlen(str)+1;
        }else{
            count=strlen(str);
        }
        //store the count in the stack
        stack[depth]=count;
        if(strchr(str,'.')){
            maxCount= count>maxCount? count:maxCount;
        }
        // Get next token
        token = strtok(NULL, "\n"); 
    }
    free(stack);
    return maxCount; 
    //time complexity: O(n)
    //space complexity: O(n)
}
```
---
## 609. Find Duplicate File in System
### Problem
The input is an array of strings where each string represents a directory listing. Each directory contains files, and each file is represented as filename(content). The goal is to find all groups of duplicate files (files with the same content) and return their paths.
### Solution
```cpp
class Solution {
public:
    vector<vector<string>> findDuplicate(vector<string>& paths) {
        //a hashmap with key content and value files
        unordered_map<string, vector<string>> contentFiles;
        //loop through the paths
        for(auto &path: paths){
            //use stringstream to process path
            stringstream ss(path);
            string dir;
            //extract directory name
            getline(ss,dir,' ');
            //get file name and content
            string fileData;
            while(getline(ss, fileData, ' ')){
                size_t left = fileData.find('(');
                if(left!=std::string::npos){
                    //extract file name
                    string fileName = fileData.substr(0,left);
                    //extract file content
                    string content = fileData.substr(left+1,fileData.size()-left-2);
                    //insert it into the hashtable
                    contentFiles[content].emplace_back(dir+'/'+fileName);
                }
            }
        }
        //get the answer
        vector<vector<string>> ans;
        for(auto &tmp: contentFiles){
            if(tmp.second.size()>1){
                ans.emplace_back(tmp.second);
            }
        }
        return ans;
        //the number of paths: n; the average number of files per path: m; the average length per file: l;
        //time complexity:O(n⋅m⋅l)
        //space complexity:O(n⋅m)
    }
};
```
---
## 1233. Remove Sub-Folders from the Filesystem
### Problem
You are given a list of strings folder representing the paths of folders in a file system. A folder is considered a sub-folder of another folder if the path of the first folder starts with the path of the second folder followed by a '/'. The goal is to return the paths of all sub-folders that are not direct sub-folders of other folders.
### Solution
```cpp
class Solution {
public:
    vector<string> removeSubfolders(vector<string>& folder) {
        //sort the folder string
        sort(folder.begin(),folder.end());
        vector<string> ans;
        ans.emplace_back(folder[0]);
        //loop through the sorted array
        string lastFolder=folder[0];
        for(int i=1; i<folder.size(); ++i){
            if(folder[i].compare(0,lastFolder.size()+1, lastFolder+"/")!=0){
                lastFolder=folder[i];
                ans.emplace_back(lastFolder);
            }
        }
        return ans;
        //time complexity: O(nlogn)
        //space complexity: O(n)
    }
};
```
---
## 1487. Making File Names Unique
### Problem
You are given an array of strings names, where each string represents a file name. Your task is to rename the files such that all file names are unique. If the file name already exists, you should append a number (starting from 1) to the name in parentheses, e.g., if "file" already exists, the new name will be "file(1)," and if that also exists, it will be "file(2)," and so on.
### Solution
```cpp
class Solution {
public:
    vector<string> getFolderNames(vector<string>& names) {
        //a dictionary to store the file name with its next count
        unordered_map<string, int> dic;
        vector<string> ans;
        //loop throught names
        for(const auto& name: names){
            auto it=dic.find(name);
            if(it==dic.end()){
                dic[name]=1;
                ans.emplace_back(name);
            }else{
                int k=dic[name];
                std::string tmp = name+"("+std::to_string(k++)+")";
                while(dic.find(tmp)!=dic.end()){
                    tmp = name+"("+std::to_string(k++)+")";
                }
                ans.emplace_back(tmp);
                dic[name]=k;
                dic[tmp]=1;
            }
        }
        return ans;
        //time complexity: O(n)
        //space complexity: O(n)
    }
};
```
---