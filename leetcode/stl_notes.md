## STL data structures

### Sequence Containers
- Sequence containers store elements in a linear sequence, allowing elements to be accessed in a specific order. 
- They are designed to maintain the order of the elements and allow efficient insertion and removal of elements at both ends or within the sequence.

#### vector: 
##### initialization
- default initialization: vector`<int>` v;
- size initialization: vector`<int>`v(size, value);
- copy initialization: vector`<int>`v_copy(v);
##### access
- access element: v[i];
- access element with bound checking: v.at(i); 
- first element: v.front();
- last element: v.last();
##### modify
- add the last element at the end: v.push_back(value) or v.emplace_back(value);
- remove the last element: v.pop_back();
- insert at position i: v.insert(v.begin()+i, val);
- erase at position i: v.earse(v.begin()+i);
- erase from position i to j-1: v.erase(v.begin()+i, v.begin()+j);
- clear all the elements: v.clear();
- swap the contents of two vectors: v1.swap(v2);
##### iterate
- range based for loop: for(int value: v){};
- iterator based for loop: for(auto it = v.begin(); it!=v.end(); ++it){//*it}
##### size
- size: v.size();
- resize: v.resize(n);
##### capcity
- capacity: v.capacity();
- reserve: v.reserve(n);
##### sort
- ascending order: sort(v.begin(),v.end());
- descending order: sort(v.begin(), v.end(), greater<int>());
##### search
- search with sorted vector: bool = binary_search(v.begin(),v.end(),target);
##### bound
- lower bound: auto lb = lower_bound(v.begin(), v.end(), target);
- upper bound: auto up = upper_bound(v.begin(), v.end(), target);
##### comparison
- equality: if(v1==v2){}
- inequality: if(v1!=v2){}
##### special algorithms
- reverse: reverse(v.begin(), v.end());
- rotate: rotate(v.begin(), v.begin()+k, v.end); //by k positions
- unique: v.erase(unique(v.begin(),v.end()),v.end()); //remove duplication consecutive elements
- convert to set or map: set<int> s(v.begin(),v.end()); //convert vector to set (remove duplicates, sort)

#### list (double linked list):
##### initialization
- default initialization: list<int> l;
- size initialization: list<int>l(size, value);
- copy initialization: list<int>l_copy(l);
- initialize from an array: list<int>l(arr,arr+n);
##### insert
- add the element at the beginning of the list: l.push_front(value);
- add the element at the end of the list: l.push_back(value);
- remove the first element: l.pop_front();
- remove the last element:l.pop_back();
- insert before the position of iterator it: l.insert(it,value);
- insert n elements of value before the position of iterator it: l.insert(it,n,value);
- insert from anther range before the position of iterator it: l.insert(it,arr,arr+n);
##### erase 
- erase at iterator it: l.erase(it);
- erase in a range: l.erase(it1,it2);
- remove all the elements: l.erase();
##### access
- access the first element: l.front();
- access the last element: l.last();
##### iterate
- range based for loop: for(int value: l){};
- iterator based for loop: for(auto it = l.begin(); it!=l.end(); ++it){//*it}
- reverse iteration: for(auto it=l.rbegin(); it!=l.rend(); ++it){//*it}
##### size
- size: l.size();
- empty: l.empty();
##### sort
- ascending order: l.sort();
- descending order: l.sort(greater<int>());
- reverse the order of elements: l.reverse();
##### merge
- merge: l1.merge(l2); //l1 merges with l2, must be sorted before merging
- merge with custom sort: l.sort(greater<int>());  // Sorts in descending order
##### splice
- transfer: l1.spice(l1.end(),l2); //Transfers all elements from l2 to the end of l1
- transfer at specific range: l1.splice(l1.end(), l2, l2.begin(), l2.end()); // Moves a range from l2 to l1
##### remove
- unique: l.uinique(); //remove consecutive duplicate elements
- remove: l.remove(value); //remove all the elements equal to specified value
##### special algorithms
- find a specific value: auto it = find(l.begin(), l.end(), value); if(it!=l.end()){}

#### dequeue (double ended queue):
##### initialization
- default initialization: deque<int> dq;
- size initialization: deque<int>dq(size, value);
- copy initialization: deque<int>dq_copy(dq);
- initialize from an array: deque<int>dq(arr,arr+n);
##### insert
- add the element to the front: dq.push_front(value);
- add the element to the back: dq.push_back(value);
- remove the element at the front: dq.pop_front();
- remove the element at the back:dq.pop_back();
- insert an element at a specific position: dq.insert(it,value);
##### erase 
- erase at iterator it: dq.erase(it);
- remove all the elements: dq.erase();
##### access
- access the first element: dq.front();
- access the last element: dq.last();
##### iterate
- range based for loop: for(int value: dq){};
- iterator based for loop: for(auto it = dq.begin(); it!=dq.end(); ++it){//*it}
- reverse iteration: for(auto it=dq.rbegin(); it!=dq.rend(); ++it){//*it}
##### size
- size: dq.size();
- resize: dq.resize(n);
- empty: dq.empty();
- resize and assign: dq.resize(n, value);
##### special algorithms
- assign: dq.assign(n, value);  // Assigns n elements with the given value
- find a specific value: auto it = find(dq.begin(), dq.end(), value); if(it!=dq.end()){}
- swap: dq1.swap(dq2);

### Container Adaptors
- Container adapters are wrappers around other container types, providing a simplified or specialized interface to the underlying container. 
- They are designed to provide a restricted set of operations based on the behavior required for specific tasks.

#### stack (LIFO):
##### initialization
- default initialization: stack<int> st;
##### modify
- add the element at the top: st.push(value);
- remove the element at the top: st.pop();
##### access
- access the top element: st.top();
##### size
- size: st.size();
- empty: st.empty();

#### queue (FIFO):
- default initialization: queue<int> q;
##### modify
- add the element at the end: q.push(value);
- remove the element at the front: q.pop();
##### access
- access the element of the front of the queue: q.front();
- access the element of the back of the queue: q.back();
##### size
- size: q.size();
- empty: q.empty();

#### priority queue (heap based data structure):
##### initialization
- default initialization: priority_queue<int> pq;
##### modify
- add the element to the priority queue: pq.push(value);
- remove the top (highest priority) element of the priority queue: pq.pop();
##### access
- access the top element of the priority queue: pq.top();
##### size
- size: pq.size();
- empty: pq.empty();
##### properties
- max heap by default
- min heap: priority_queue<int, vector<int>, greater<int>> minHeap;

### Associative Containers
- Associative containers store data in such a way that elements can be quickly searched, inserted, and deleted based on a key. 
- These containers automatically organize elements to provide fast access to individual elements using a key (usually via a sorted structure).

#### set 
- Note: a sorted container that stores unique elements, and it is typically implemented using a balanced binary tree (like a Red-Black Tree).
- The elements in a set are always sorted according to the specified comparison function (by default, in ascending order).
##### initialization
- default initialization: set<int> s;
##### modify
- add the element to the set: s.insert(value); // if the element already exists, it is not added.
- remove the element of specific value in the set: s.erase(value);
- remove all the elements in the set: s.clear();
##### find
- it returns an iterator to the element if it exists, otherwise returns an iterator to the end of the set.
- auto it = s.find(value). //check the return value here
##### count
- it returns the number of occurrences of a given element.
- bool exists = s.count(value);  // returns 1 if 10 exists, 0 if not
##### size
- size: s.size();
- empty: s.empty();
##### iteration
- iterator based for loop: for(it=s.begin(); it!= s.end(); ++it){//*it}
##### bound
- lower bound: it returns an iterator to the first element that is not less than the given value. auto it = s.lower_bound(value); 
- upper bound: it returns an iterator to the first element that is greater than the given value. auto it = s.upper_bound(value); 

#### multiset:
- Note: it is similar to a set, but unlike set, it allows duplicate elements. It is also implemented using a balanced binary tree (like a Red-Black Tree), but the key difference is that it stores multiple occurrences of an element.
##### initialization
- default initialization: multiset<int> ms;
##### modify
- add the element to the set: ms.insert(value); // it can appear multiple times.
- remove the element of specific value in the set: ms.erase(value); //it removes all instances of a specific element.
- remove all the elements in the set: ms.clear();
##### find
- it returns an iterator to the first occurrence of the specified value, or an iterator to the end if not found.
- auto it = ms.find(value). //check the return value here
##### count
- it returns the number of occurrences of a given element.
- bool exists = ms.count(value);
##### size
- size: ms.size();
- empty: ms.empty();
##### iteration
- iterator based for loop: for(it=ms.begin(); it!= ms.end(); ++it){//*it}
##### bound
- lower bound: it returns an iterator to the first element that is not less than the given value. auto it = ms.lower_bound(value); 
- upper bound: it returns an iterator to the first element that is greater than the given value. auto it = ms.upper_bound(value); 

#### map:
- Note: an associative container that stores key-value pairs, where each key is unique.  
- The elements in a map are ordered based on the key using a balanced binary tree (like a Red-Black Tree).
##### initialization
- default initialization: map<int, string> m;
##### modify
- add a key-value pair to a map: m.insert(key, value); // if the key already exists, the value is not added.
- remove the pair associated with a specific key: m.erase(key);
- returns the value associated with the key: m.at(key); // it throws an out_of_range exception if the key does not exist.
- remove all the elements in the map: m.clear();
##### find
- it returns an iterator to the element with the specified key, or to end() if the key is not found.
- auto it = m.find(key). //check the return value here
##### count
- it returns 1 if the key exists, 0 if it doesn't.
- bool exists = m.count(key);  // returns 1 if 10 exists, 0 if not
##### size
- size: m.size();
- empty: m.empty();
##### iteration
- iterator based for loop: for(it=m.begin(); it!= m.end(); ++it){cout << it->first << ": " << it->second << endl;}
##### bound
- lower bound: it returns an iterator to the first element that is not less than the specified key. auto it = m.lower_bound(key); 
- upper bound: it returns an iterator to the first element that is strictly greater than the specified key. auto it = m.upper_bound(key); 
##### operator[]
- it returns a reference to the value corresponding to the specified key. If the key doesn't exist, it inserts the key with a default value.
- m[key] = value;

#### multimap:
- Note: an associative container that stores key-value pairs, where multiple elements with the same key to exist.  
- The elements in a map are ordered based on the key using a balanced binary tree (like a Red-Black Tree).
##### initialization
- default initialization: map<int, string> m;
##### modify
- add a key-value pair to a multimap: m.insert(key, value); // multiple elements with the same key can be inserted.
- remove all the pairs associated with a specific key: s.erase(key);
##### find
- it returns an iterator to the first element with the specified key, or to end() if the key is not found.
- auto it = m.find(key). //check the return value here
##### count
- it returns the number of elements associated with the given key.
- bool exists = m.count(key);  // returns 1 if 10 exists, 0 if not
##### size
- size: m.size();
- empty: m.empty();
##### iteration
- iterator based for loop: for(it=m.begin(); it!= m.end(); ++it){cout << it->first << ": " << it->second << endl;}
##### bound
- lower bound: it returns an iterator to the first element that is not less than the specified key. auto it = m.lower_bound(key); 
- upper bound: it returns an iterator to the first element that is strictly greater than the specified key. auto it = m.upper_bound(key); 
##### equal_range(key)
- it returns a pair of iterators representing the range of elements in the container that have the given key.
- auto range = m.equal_range(key);for (auto it = range.first; it != range.second; ++it) { cout << it->second << endl;}

### Unordered Associative Containers
- Unordered associative containers also store key-value pairs but do not guarantee any specific order for the elements. 
- These containers typically use hash tables for fast lookups, inserts, and deletes.

#### unordered_set:
- Note: it is an associative container that stores unique elements with no specific order. 
- It uses hashing to provide average constant time complexity for many operations, making it very efficient for problems that involve frequent lookups, insertions, or deletions.
##### initialization
- default initialization: unordered_set<int> s;
##### modify
- add the element to the set: s.insert(value); // if the element already exists, the set remains unchanged.
- remove the element of specific value in the set: s.erase(value);
- remove all the elements in the set: s.clear();
##### find
- it returns an iterator to the element if it exists, otherwise returns an iterator to the end of the set.
- auto it = s.find(value). //check the return value here
##### count
- it returns the number of occurrences of a given element.
- bool exists = s.count(value);  // returns 1 if 10 exists, 0 if not
##### size
- size: s.size();
- empty: s.empty();
##### iteration
- iterator based for loop: for(it=s.begin(); it!= s.end(); ++it){//*it}
##### bucket_count()
- it returns the number of buckets used by the unordered_set for storing its elements.
- s.bucket_count();
##### load_factor()
- it returns the average number of elements per bucket (helps to understand if rehashing is needed).
- s.load_factor();

#### unordered_multiset:
- Note: itis similar to the unordered_set, but it allows multiple instances of the same element (i.e., it can store duplicates).
- It uses a hash table for storage, providing average constant time complexity for many operations like insertions, deletions, and lookups. 
##### initialization
- default initialization: unordered_set<int> s;
##### modify
- add the element to the set: s.insert(value); // multiple occurrences of the same value are allowed.
- remove the element of specific value in the set: s.erase(value); //if there are multiple occurrences, only one is removed
- remove all the elements in the set: s.clear();
##### find
- it only returns the first occurrence of the element, otherwise returns an iterator to the end of the set.
- auto it = s.find(value). //check the return value here
##### count
- it returns the number of occurrences of a given element.
- bool exists = s.count(value);  // returns 1 if 10 exists, 0 if not
##### size
- size: s.size();
- empty: s.empty();
##### iteration
- iterator based for loop: for(it=s.begin(); it!= s.end(); ++it){//*it}
##### bucket_count()
- it returns the number of buckets used by the unordered_set for storing its elements.
- s.bucket_count();
##### load_factor()
- it returns the average number of elements per bucket (helps to understand if rehashing is needed).
- s.load_factor();
##### equal_range()
- it returns a pair of iterators representing the range of elements equal to value. 
- auto range = s.equal_range(value); for (auto it = range.first; it != range.second; ++it) { cout << *it << " ";}

#### unordered_map:
- Note: it is an associative container that stores key-value pairs, where each key is unique. 
- It provides average constant time complexity for lookups, insertions, and deletions due to its underlying hash table implementation. 
##### initialization
- default initialization: unordered_map<int, string> m;
##### modify
- add a key-value pair to a map: m.insert(key, value); // if the key already exists, the value is not added.
- returns the value associated with the key: m.at(key); // it throws an out_of_range exception if the key does not exist.
- remove the pair associated with a specific key: m.erase(key);
- remove all the elements in the map: m.clear();
##### find
- it returns an iterator to the element with the specified key, or to end() if the key is not found.
- auto it = m.find(key). //check the return value here
##### count
- it returns 1 if the key exists, 0 if it doesn't.
- bool exists = m.count(key);  // returns 1 if 10 exists, 0 if not
##### size
- size: m.size();
- empty: m.empty();
##### iteration
- iterator based for loop: for(it=m.begin(); it!= m.end(); ++it){cout << it->first << ": " << it->second << endl;}
##### operator[]
- it returns a reference to the value corresponding to the specified key. If the key doesn't exist, it inserts the key with a default value.
- m[key] = value;

#### unordered_multimap:
- Note: it is a container that stores key-value pairs, like the unordered_map. However, it allows duplicate keys. 
- It provides average constant time complexity for lookups, insertions, and deletions due to its underlying hash table implementation. 
##### initialization
- default initialization: unordered_multimap<int, string> m;
##### modify
- add a key-value pair to a map: m.insert(key, value); //  it allows multiple entries with the same key.
- remove all the pairs associated with a specific key: m.erase(key);
- remove all the elements in the map: m.clear();
##### find
- it returns the iterator to the first one, or to end() if the key is not found.
- auto it = m.find(key). //check the return value here
##### count
- it returns 1 if the key exists, 0 if it doesn't.
- bool exists = m.count(key);  // returns 1 if 10 exists, 0 if not
##### size
- size: m.size();
- empty: m.empty();
##### iteration
- iterator based for loop: for(it=m.begin(); it!= m.end(); ++it){cout << it->first << ": " << it->second << endl;}
##### equal_range(key)
- it returns a pair of iterators representing the range of elements in the container that have the given key.
- auto range = m.equal_range(key);for (auto it = range.first; it != range.second; ++it) { cout << it->second << endl;}
