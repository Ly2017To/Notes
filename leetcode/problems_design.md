# Solutions of Design Pattern Related Problems in C++

## 355. Design Twitter
### Problem
You are tasked with designing a simplified version of Twitter. Your goal is to implement a system that supports the following functionalities:
- Post Tweet: Users can post tweets.
- Follow User: Users can follow other users.
- Unfollow User: Users can unfollow other users.
- Get News Feed: Users can view a list of their most recent tweets from themselves and the users they follow
### Solution
```cpp
class Twitter {
private:
    //define the struct of tweet
    struct tweet{
        int tweetId;
        int tweetTs;
        tweet(int _tweetId, int _tweetTs): tweetId(_tweetId), tweetTs(_tweetTs){}
    };
    unordered_map<int, list<tweet>> userTweets;
    unordered_map<int, unordered_set<int>> userFollowers;
    int timestamp;
    const int maxTweetsNum = 10;
public:
    Twitter() {
        timestamp=0;
    }
    
    void postTweet(int userId, int tweetId) {
        if(userTweets[userId].size()==maxTweetsNum){
            userTweets[userId].pop_back();
        }
        userTweets[userId].push_front(tweet(tweetId,timestamp));
        //increament the timestamp
        timestamp++;
    }
    
    vector<int> getNewsFeed(int userId) {
        priority_queue<pair<int,int>> pq;
        if(!userTweets[userId].empty()){
            for(const auto& tweet: userTweets[userId]){
                pq.push({tweet.tweetTs, tweet.tweetId});
            }
        }
        if(!userFollowers[userId].empty()){
            for(const auto& followerId: userFollowers[userId]){
                if(!userTweets[followerId].empty()){
                    for(const auto& tweet: userTweets[followerId]){
                        pq.push({tweet.tweetTs, tweet.tweetId});
                    }
                }
            }
        }
        vector<int> recentTweetsId;
        for(int i=0; i<maxTweetsNum&&!pq.empty(); ++i){
            recentTweetsId.emplace_back(pq.top().second);
            pq.pop();
        }
        return recentTweetsId;
    }
    
    void follow(int followerId, int followeeId) {
        if(followerId!=followeeId){
            userFollowers[followerId].insert(followeeId);
        }
    }
    
    void unfollow(int followerId, int followeeId) {
        if(userFollowers[followerId].find(followeeId)!=userFollowers[followerId].end()){
            userFollowers[followerId].erase(followeeId);
        }
    }
};
```
## 707. Design Linked List
### Problem
Design your implementation of the linked list. You can choose to use a singly or doubly linked list. The implementation should support the following operations:
- MyLinkedList() Initializes the linked list.
- int get(int index) Get the value of the index-th node in the linked list. If the index is invalid, return -1.
- void addAtHead(int val) Adsd a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list.
- void addAtTail(int val) Append a node of value val as the last element of the linked list.
- void addAtIndex(int index, int val) Add a node of value val before the index-th node in the linked list. If index is equal to the length of the linked list, the node will be appended to the end of the list. If index is greater than the length, the node will not be inserted.
- void deleteAtIndex(int index) Delete the index-th node in the linked list, if the index is valid.
### Solution
```cpp
/*
** solution1: a linked list without dummy node at the head
*/
class MyLinkedList {
private:
    struct node{ 
        int val;
        node * next;
        node(int _val): val(_val), next(nullptr){}
    };
    typedef node node_t;
    node_t * head;
    int length=0;
public:
    MyLinkedList() {
        head=new node(0);
        length=0;
    }
    int get(int index) {
        if(length==0){
            return -1;
        }
        node_t * nodeTmp = head;
        int indexTmp = 0;
        while(nodeTmp!=nullptr){
            if(indexTmp==index){
                return nodeTmp->val;
            }
            nodeTmp=nodeTmp->next;
            indexTmp++;
        }
        return -1;
    }
    void addAtHead(int val) {
        if(length>0){
            node_t * newNode = new node_t(val);
            newNode->next = head;
            head = newNode;
        }else{
            head->val=val;
        }
        length++;
        return;
    }
    void addAtTail(int val) {
        if(length>0){
            node_t * newNode = new node_t(val);
            node_t * nodeTmp = head;
            while(nodeTmp->next!=nullptr){
                nodeTmp=nodeTmp->next;
            }
            nodeTmp->next = newNode;
        }else{
            head->val=val;
        }
        length++;
        return;
    }
    void addAtIndex(int index, int val) {
        if(index==0){
            addAtHead(val);
        }else if(index==length){
            addAtTail(val);
            return;
        }else if(index>length){
            return;
        }else{
            node_t * nodeTmp = head;
            for(int i=0; i<index-1; ++i){
                nodeTmp=nodeTmp->next;
            }
            node_t * newNode = new node_t(val);
            newNode->next = nodeTmp->next;
            nodeTmp->next = newNode;
            length++;
        }
        return;
    }
    void deleteAtIndex(int index) {
        if(index==0){
            node_t * nodeDelete = head;
            head = head->next;
            delete nodeDelete;
        }else if(index>=length){
            return;
        }else {
            node_t * nodeTmp = head;
            for(int i=0; i<index-1; ++i){
                nodeTmp=nodeTmp->next;
            }
            if(nodeTmp!=nullptr&&nodeTmp->next!=nullptr){
                node_t * nodeDelete = nodeTmp->next;
                nodeTmp->next = nodeDelete->next;
                delete nodeDelete;
                length--;
            }
        }
        return;
    }
    //Destructor
    ~MyLinkedList() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }
};
```
```cpp
/*
** solution2: a linked list with dummy node at the head
*/
class MyLinkedList {
private:
    struct node{ 
        int val;
        node * next;
        node(int _val): val(_val), next(nullptr){}
    };
    typedef node node_t;
    node_t * dummyHead;
    int length;
public:
    MyLinkedList() {
        dummyHead=new node(-1);
        length=0;
    }
    int get(int index) {
        if(index>=length||length==0){
            return -1;
        }
        node_t * nodeTmp = dummyHead;
        for(int i=0; i<=index; ++i){
            nodeTmp=nodeTmp->next;
        }
        return nodeTmp->val;
    }
    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    void addAtTail(int val) {
        addAtIndex(length, val);
    }
    void addAtIndex(int index, int val) {
        if(index>length) return;
        node_t * newNode = new node(val);
        node_t * nodeTmp = dummyHead;
        for(int i=0; i<index; ++i){
            nodeTmp = nodeTmp->next;
        }
        newNode->next = nodeTmp->next;
        nodeTmp->next = newNode;
        length++;
    }
    void deleteAtIndex(int index) {
        if(index>=length) return;
        node_t * nodeTmp = dummyHead;
        for(int i=0; i<index; ++i){
            nodeTmp = nodeTmp->next;
        }
        node_t * nodeDelete = nodeTmp->next;
        nodeTmp->next = nodeDelete->next;
        delete nodeDelete;
        length--;
    }
};
```
```cpp
/*
** solution3: a double linked list
*/
class MyLinkedList {
private:
    struct node{ 
        int val;
        node * next;
        node * prev;
        node(int _val): val(_val), next(nullptr), prev(nullptr){}
    };
    typedef node node_t;
    node_t * dummyHead;
    node_t * dummyTail;
    int length;
public:
    MyLinkedList() {
        dummyHead = new node(-1);
        dummyTail = new node(-1);
        dummyHead->next = dummyTail;
        dummyTail->prev = dummyHead;
        length=0;
    }
    int get(int index) {
        if(index>=length||length==0){
            return -1;
        }
        node_t * nodeTmp = nullptr;
        if(index < length-index){
            nodeTmp=dummyHead;
            for(int i=0; i<=index; ++i){
                nodeTmp=nodeTmp->next;
            }
        }else{
            nodeTmp=dummyTail;
            for(int i=0; i<length-index; ++i){
                nodeTmp=nodeTmp->prev;
            }
        } 
        return nodeTmp->val;
    }
    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    void addAtTail(int val) {
        addAtIndex(length, val);
    }
    void addAtIndex(int index, int val) {
        if(index>length) return;
        node_t * newNode = new node_t(val);
        node_t * nodePrev = nullptr, *nodeNext = nullptr;
        if(index<length-index){
            nodePrev=dummyHead;
            for(int i=0; i<index; ++i){
                nodePrev = nodePrev->next;
            }
            nodeNext = nodePrev->next;
        }else{
            nodeNext=dummyTail;
            for(int i=0; i<length-index; ++i){
                nodeNext = nodeNext->prev;
            }
            nodePrev = nodeNext->prev;
        }
        newNode->next = nodeNext;
        newNode->prev = nodePrev;
        nodePrev->next = newNode;
        nodeNext->prev = newNode;
        length++;
    }
    void deleteAtIndex(int index) {
        if(index>=length||length==0) return;
        node_t * nodePrev = nullptr, *nodeNext = nullptr, *nodeDelete=nullptr;
        if(index<length-index){
            nodePrev=dummyHead;
            for(int i=0; i<index; ++i){
                nodePrev = nodePrev->next;
            }
            nodeDelete = nodePrev->next;
            nodeNext = nodeDelete->next;
        }else{
            nodeNext=dummyTail;
            for(int i=0; i<length-index-1; ++i){
                nodeNext = nodeNext->prev;
            }
            nodeDelete = nodeNext->prev;
            nodePrev = nodeDelete->prev;
        }
        nodePrev->next = nodeNext;
        nodeNext->prev = nodePrev;
        length--;
        delete nodeDelete;
    }
};
```
## 211. Design an Add and Search Word Data Structure
### Problem
Design a data structure that supports adding new words and searching for them. All words only consist of lowercase letters a-z. Your data structure should be able to perform the following operations:
- addWord(word): Adds a word into the data structure.
- search(word): Returns if the word is in the data structure. A word could contain the dot character '.' to represent any one letter.
### Solution
```cpp
class WordDictionary {
private:
    struct trie{
        unordered_map<char, trie *> children;
        bool isEnd;
        trie(): isEnd(false){};
    };
    trie * root;
public:
    WordDictionary() {
        root = new trie();
    }
    void addWord(string word) {
        trie * node = root;
        for(char c: word){
            if(node->children.find(c)==node->children.end()){
                node->children[c] = new trie();
            } 
            node = node->children[c];
        }
        node->isEnd = true;
    }
    bool search(string word) {
        stack<pair<trie *, int>> st;
        st.push({root,0});
        //using a stack to simulate dfs
        while(!st.empty()){
            auto [node, index] = st.top();
            st.pop();
            //stop condition
            if(index == word.size()){
                if(node->isEnd) return true;
                continue;
            }
            char c = word[index];
            if(c!='.'){
                if(node->children.find(c)!=node->children.end()){
                    st.push({node->children[c], index+1});
                }
            }else{
                for(auto &child: node->children){
                    st.push({child.second,index+1});
                }
            }
        }
        return false;
    }
};
```
## 173. Design a Binary Search Tree Iterator
### Problem
Implement an iterator over a binary search tree (BST). Your iterator should be able to traverse the tree in in-order manner, which means it visits nodes in ascending order.
- BSTIterator(TreeNode* root) Initializes an object of the BSTIterator class. The given root represents the root of the BST.
- bool hasNext() Returns true if there exists a number in the traversal to the right of the current position.
- int next() Moves the iterator to the next number and returns that number.
### Solution
```cpp
class BSTIterator {
private:
    list<int> numbers;
public:
    BSTIterator(TreeNode* root) {
        inOrderTraversal(root);
    }
    int next() {
        int number = numbers.front();
        numbers.pop_front();
        return number;
    }
    bool hasNext() {
        if(numbers.empty()){
            return false;
        }
        return true;
    }
    void inOrderTraversal(TreeNode* node){
        if(node==nullptr){
            return;
        }
        inOrderTraversal(node->left);
        numbers.push_back(node->val);
        inOrderTraversal(node->right);
    }
};
```
```cpp
class BSTIterator {
private:
    vector<int> numbers;
    int index;
public:
    BSTIterator(TreeNode* root) {
        index=0;
        inOrderTraversal(root);
    }
    int next() {
        return numbers[index++];
    }
    bool hasNext() {
        return index<numbers.size();
    }
    void inOrderTraversal(TreeNode* node){
        if(node==nullptr){
            return;
        }
        inOrderTraversal(node->left);
        numbers.emplace_back(node->val);
        inOrderTraversal(node->right);
    }
};
```
## 1472. Design Browser History
### Problem
The problem involves designing a simple browser history system that allows users to navigate back and forth through their browsing history. The key operations that need to be supported are:
- visit(url): Add a new URL to the history. When visiting a new URL, it should clear the forward history (if any).
- back(steps): Go back a certain number of steps in the history. If there aren't enough steps, go back to the oldest page.
- forward(steps): Go forward a certain number of steps in the history. If there aren't enough steps, go forward to the most recent page.
- You must maintain the current URL at all times.
### Solution
```cpp
class BrowserHistory {
private:
    vector<string> browserUrl;
    int idx;
public:
    BrowserHistory(string homepage) {
        browserUrl.emplace_back(homepage);
        idx=0;
    }
    void visit(string url) {
        browserUrl.resize(idx+1);
        browserUrl.emplace_back(url);
        idx++;
    }
    string back(int steps) {
        idx = max(0,idx-steps);
        return browserUrl[idx];
    }
    string forward(int steps) {
        idx = min(idx+steps,int(browserUrl.size()-1));
        return browserUrl[idx];
    }
};
```
## 284. Peeking Iterator 
### Problem
You are given an interface Iterator with the following methods
- bool hasNext(): Returns true if the iteration has more elements.
- int next(): Returns the next element in the iteration.

Your task is to implement the PeekingIterator class that extends the functionality of the Iterator. The PeekingIterator should have the following methods:
- PeekingIterator(Iterator& iterator): Initializes the peeking iterator with the given iterator.
- int peek(): Returns the next element in the iteration without advancing the iterator.
- int next(): Returns the next element in the iteration and advances the iterator.
- bool hasNext(): Returns true if there are more elements to iterate over.
### Solution
```cpp
class PeekingIterator : public Iterator {
private:
    bool flag;
    int nextNum;
public:
	PeekingIterator(const vector<int>& nums) : Iterator(nums) {
	    // Initialize any member here.
	    // **DO NOT** save a copy of nums and manipulate it directly.
	    // You should only use the Iterator interface methods.
        flag = Iterator::hasNext();
        if(flag){
            nextNum = Iterator::next();
        }
	}
    // Returns the next element in the iteration without advancing the iterator.
	int peek() {
        return nextNum;
	}
	// hasNext() and next() should behave the same as in the Iterator interface.
	// Override them if needed.
	int next() {
        int num = nextNum;
        flag = Iterator::hasNext();
	    if(flag){
            nextNum = Iterator::next();
        }
        return num;
	}
	bool hasNext() const {
	    return flag;
	}
};
```
## 232. Implementing Queue Using Stacks
### Problem
Implement a queue(FIFO) using only two stacks. The implemented queue should support the following operations:
- void push(int x): Add an element x to the back of the queue.
- int pop(): Remove the element from the front of the queue and return it.
- int peek(): Get the front element of the queue without removing it.
- bool empty(): Check whether the queue is empty.
### Solution
```cpp
class MyQueue {
private:
    stack<int> s;
    stack<int> q;

    void transfer(){
        while(!s.empty()){
            q.push(s.top());
            s.pop();
        }   
    }
public:
    MyQueue() {}
    void push(int x) {
        s.push(x);
    }
    int pop() {
        if(q.empty()){
            transfer();
        }
        int front = q.top();
        q.pop();
        return front;
    }
    int peek() {
        if(q.empty()){
            transfer();
        }
        return q.top();
    }
    bool empty() {
        return q.empty()&&s.empty();
    }
};
```
## 227. Basic Calculator II
### Problem
Input: A string representing a mathematical expression, which may include:
- Non-negative integers (e.g., 3, 12, etc.)
- Operators: +, -, *, /
- Whitespace characters that can be ignored.

Output: An integer value representing the result of the evaluated expression.
### Solution
```cpp
class Solution {
public:
    int calculate(string s) {
        stack<int> nums;
        int currentNum = 0, tmp=0;
        char preOpt= '+';
        for(int i=0; i<s.size(); ++i){
            if(isdigit(s[i])){
                currentNum=currentNum*10+(s[i]-'0');
            }
            if(!isdigit(s[i])&&!isspace(s[i])||i==s.size()-1){
                switch(preOpt){
                    case '+':
                        nums.push(currentNum);
                        break;
                    case '-':
                        nums.push(-currentNum);
                        break;
                    case '*':
                        if(!nums.empty()){
                            tmp = nums.top();
                            nums.pop();
                            nums.push(tmp*currentNum);
                        }
                        break;
                    case '/':
                        if(!nums.empty()){
                            tmp = nums.top();
                            nums.pop();
                            nums.push(tmp/currentNum);
                        }
                        break;
                }
                preOpt=s[i];
                currentNum=0;
            } 
        }
        int ans = 0;
        while(!nums.empty()){
            ans+=nums.top();
            nums.pop();
        }
        return ans;
    }
};
```
## 1603. Design Parking System
### Problem
Design a parking system that accommodates three types of vehicles in a parking lot: big, medium, and small cars. The parking lot has a fixed number of slots for each vehicle type, specified at the time of initialization.
- Class Specification: ParkingSystem
- Constructor: ParkingSystem(int big, int medium, int small) 
    - Initializes the parking system with a given number of slots for big, medium, and small cars.
- Method: bool addCar(int carType)
    - Takes an integer carType (1 for big, 2 for medium, 3 for small) and checks if there is an available parking space for that type.
    - If a space is available, parks the car and returns true; otherwise, returns false.
### Solution
```cpp
class ParkingSystem {
private:
    int big, medium, small;
public:
    ParkingSystem(int big, int medium, int small) {
        this->big = big;
        this->medium = medium;
        this->small = small;
    }
    bool addCar(int carType) {
        if(carType==1){
            return big-->0? true:false;
        }else if(carType==2){
            return medium-->0? true:false;
        }else if(carType==3){
            return small-->0? true:false;
        }
        return false;
    }
};
```
## 1396. Design Underground System
### Problem
The problem requires you to design an underground system that tracks the check-in and check-out times of passengers at various stations. The requirements are as follows:
- Check-In: When a passenger checks in at a station, we need to record the station name and the time of check-in.
- Check-Out: When a passenger checks out from a station, we record the time of check-out and calculate the travel time between the check-in and check-out stations.
- Average Travel Time: We need to be able to retrieve the average travel time between any two stations.
### Solution
```cpp
class UndergroundSystem {
private:
    unordered_map<int, pair<string,int>> checkInData; //id->{stationName,t}
    unordered_map<string, pair<double,int>> travelData; //checkInSation_checkOutStation->{travelTime,count}
public:
    UndergroundSystem() {}
    
    void checkIn(int id, string stationName, int t) {
        checkInData[id]={stationName,t};
    }
    
    void checkOut(int id, string stationName, int t) {
        auto tmp = checkInData[id];
        string checkInstation = tmp.first;
        int checkInTime = tmp.second;
        double travelTime = t-checkInTime;

        string key = checkInstation+'_'+stationName;
        if(travelData.find(key)==travelData.end()){
            travelData[key]={travelTime,1};
        }else{
            travelData[key]={travelData[key].first+travelTime,travelData[key].second+1};
        }
        //to prevent dupliations at the end
        checkInData.erase(id);
    }
    
    double getAverageTime(string startStation, string endStation) {
        string key = startStation+'_'+endStation;
        if(travelData.find(key)!=travelData.end()){
            double travelTime = travelData[key].first;
            int count = travelData[key].second;
            return travelTime/count;
        }
        return 0.0;
    }
};
```
## 410. Split Array Largest Sum
### Problem
Given an integer array nums and an integer k, split nums into k non-empty subarrays such that the largest sum of any subarray is minimized.
- Return the minimized largest sum of the split.
- A subarray is a contiguous part of the array.
### Solution
```cpp
/*
** solution1: binary search + greedy
*/
class Solution {
public:
    bool checkSplit(vector<int>& nums, int k, int mid){
        int count=1, currentSum=0;
        for(int num: nums){
            currentSum += num;
            if(currentSum>mid){
                count++;
                currentSum = num;
                if(count>k){
                    return false;
                }
            }
        }
        return true;
    }

    int splitArray(vector<int>& nums, int k) {
        //define the search space
        //binary search lower bound
        int left = *max_element(nums.begin(),nums.end());
        //binary search upper bound
        int right = accumulate(nums.begin(),nums.end(),0);
        //binary search
        while(left<right){
            int mid = left+(right-left)/2;
            if(checkSplit(nums,k,mid)){
                right = mid;
            }else{
                left = mid+1;
            }
        }
        return left;
        //Time Complexity: O(n*log*(sum(nums)-max(nums))); Space Complexity: O(1);
    }
};
```
## 133. Clone Graph
### Problem
The "Clone Graph" problem is a common graph-related problem where the goal is to create a deep copy of an undirected graph. Each node in the graph contains a value and a list of its neighbors. The challenge is to ensure that the cloned graph has the same structure as the original but does not share any references with it.
### Solution
```cpp
class Solution {
private:
    unordered_map<Node *, Node *> cloneMap; //original node->cloned node

    Node * dfs(Node * node){
        //if already cloned, return
        if(cloneMap.find(node)!=cloneMap.end()){
            return cloneMap[node];
        }
        //clone node
        Node * nodeClone = new Node(node->val);
        cloneMap[node]=nodeClone;
        //clone its neighbors
        for(Node * neighbor:node->neighbors){
            nodeClone->neighbors.emplace_back(dfs(neighbor)); 
        }
        return nodeClone;
    }

    Node * bfs(Node * node){        
        //the root the of graph
        Node * root = new Node(node->val);
        cloneMap[node]=root;
        //push it into the queue
        queue<Node *> q;
        q.push(node);
        //bfs loop
        while(!q.empty()){
            //extract node from the queue
            Node * tmp = q.front();
            q.pop();
            //visit its neighbors
            for(Node * neighbor: tmp->neighbors){
                if(cloneMap.find(neighbor)==cloneMap.end()){
                    cloneMap[neighbor] = new Node(neighbor->val);
                    q.push(neighbor);
                }
                cloneMap[tmp]->neighbors.emplace_back(cloneMap[neighbor]);
            }
        }
        return root;
    }

public:
    Node* cloneGraph(Node* node) {
        if(node==nullptr){
            return nullptr;
        }
        //return dfs(node);
        return bfs(node);
        //Time Complexity: O(v+e); Space Complexity: O(v);
    }
};
```
