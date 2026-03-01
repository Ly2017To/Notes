# LeetCode 75 Problems and Solutions in C/C++ 
---
## array or string: 1768. Merge Strings Alternately
### Problem
Given two strings word1 and word2. Merge the strings by adding letters in alternating order, starting with word1.
### Solution
```c
#define LENGTH 201
#define TERMINATE '\0'
char* mergeAlternately(char* word1, char* word2) {
    int i=0;
    char * word = malloc(LENGTH*sizeof(char));
    if(word==NULL){
        return NULL;
    }

    while((*word1!=TERMINATE)&&(*word2!=TERMINATE)){
        word[i]=*word1;
        i++;
        word1++;
        word[i]=*word2;
        i++;
        word2++;
    }

    while(*word1!=TERMINATE){
        word[i]=*word1;
        i++;
        word1++;
    }

    while(*word2!=TERMINATE){
        word[i]=*word2;
        i++;
        word2++;
    }

    word[i]=TERMINATE;

    return word;

    //Space Complexity: O(n);
    //Time Complexity: O(n);
    //should free(word) in the caller.
}
```
## array or string: 1071. Greatest Common Divisor of Strings
### Problem
Given two strings str1 and str2, return the largest string x such that x divides both str1 and str2.
### Solution
```c
// Recursive function to calculate GCD using the Euclidean algorithm
int gcd(int a, int b) {
    if (b == 0){
        return a;
    }
    return gcd(b, a % b); // Recursion
}

bool checkAns(int len, char *str, int lenx, char * strx){
    int i=0, times=lenx/len;
    char * strTmp = calloc((lenx+1),sizeof(char));
    bool correct=false;
    if(strTmp==NULL){
        return NULL;
    }
    strTmp[len]='\0'; //termination '\0' is a must
    for(i=0;i<times;i++){
        strcat(strTmp,str);
    }
    if(memcmp(strx,strTmp,lenx*sizeof(char))==0){
        correct=true;
    }
    free(strTmp);
    return correct;
}

char* gcdOfStrings(char* str1, char* str2) {
    int len1 = strlen(str1), len2=strlen(str2);
    int len = gcd(len1,len2);
    char * str = calloc((len+1),sizeof(char));
    if(str==NULL){
        return NULL;
    }
    memcpy(str,str1,len*sizeof(char));
    str[len]='\0'; //termination '\0' is a must
    if(checkAns(len,str,len1,str1)&&checkAns(len,str,len2,str2)){
        return str;
    }
    str[0]='\0'; //termination '\0' is a must
    
    return str;

    //Space Complexity: O(n);
    //Time Complexity: O(n);
    //should free(word) in the caller.
}
```
## array or string: 1431. Kids With the Greatest Number of Candies
### Problem
Given an array of candies of size n for n kids, and an integer extraCandies, return a boolean array results, where results[i] is true if all the extraCandies are given to the ith kid.
### Solution
```c
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
bool* kidsWithCandies(int* candies, int candiesSize, int extraCandies, int* returnSize) {
    //get the maximum of candies
    int maxCandies = 1, i=0;
    for(i=0; i<candiesSize; ++i){
        if(candies[i]>maxCandies){
            maxCandies=candies[i];
        }
    }

    bool * results = (bool *)malloc(candiesSize*sizeof(bool));
    if(results==NULL){
        return NULL;
    }

    for(i=0; i<candiesSize; ++i){
        if(candies[i]+extraCandies>=maxCandies){
            results[i]=true;
        }else{
            results[i]=false;
        }
    }

    *returnSize=candiesSize;

    return results;

    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## array or string: 605. Can Place Flowers
### Problem
Given an integer array flowerbed containing 0's and 1's, where 0 means empty and 1 means not empty, and an integer n, return true if n new flowers can be planted in the flowerbed without violating the no-adjacent-flowers rule and false otherwise.
### Solution
```c
bool canPlaceFlowers(int* flowerbed, int flowerbedSize, int n) {
   
    bool ans = false;
    int i=0;
    if(flowerbedSize==1){
        if(flowerbed[i]==0&&n<2){
            n--;
        }
    }else{
        if(flowerbed[i]==0&&flowerbed[i+1]==0){
            flowerbed[0]=1;
            n--;
            i=i+2;
        }else{
            i++;
        }

        while(i<flowerbedSize-1){
            if(flowerbed[i-1]==0&&flowerbed[i]==0&&flowerbed[i+1]==0){
                flowerbed[i]==1;
                n--;
                i=i+2;
            }else{
                i++;
            }
        }

        if(i==flowerbedSize-1){
            if(flowerbed[i-1]==0&&flowerbed[i]==0){
                flowerbed[i]==1;
                n--;
            }
        }   
    }

    if(n<=0) return true;

    return ans;

    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## array or string: 345. Reverse Vowels of a String
### Problem
Write a function that takes a string as input and reverses only the vowels in the string. The vowels are 'a', 'e', 'i', 'o', 'u' (both lowercase and uppercase).
### Solution
```c
bool isVowel(char c){
    c=tolower(c);
    return c=='a'||c=='e'||c=='i'||c=='o'||c=='u';
}

char* reverseVowels(char* s) {
    int length = strlen(s), count=0;
    
    char *vowels = malloc(length*sizeof(char));
    if(vowels==NULL){
        return NULL;
    }

    for(int i=0; i<length; ++i){
        if(isVowel(s[i])){
            vowels[count++]=s[i];
        }
    }

    count--;
    for(int i=0; i<length; ++i){
        if(isVowel(s[i])){
            s[i]=vowels[count--];
        }
    }

    free(vowels);
    return s;
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## array or string: 151. Reverse Words in a String
### Problem
Given an input string, you need to reverse the order of the words. A word is defined as a sequence of non-space characters. The string can contain leading or trailing spaces or multiple spaces between words. You should return a new string without any extra spaces.
### Solution
```cpp
class Solution {
public:
    string reverseWords(string s) {
        string s_reverse;
        string word;
        char pre_ch = ' ';
        // loop through characters
        for (char ch : s) {
            if (ch == ' ') {
                if (pre_ch == ' '){
                    continue;
                }else{
                    s_reverse.insert(0,word+' ');
                    word.clear();
                }
            }else{
                word.push_back(ch);
            }
            pre_ch=ch;
        }
        //handling the last word
        if(pre_ch!=' '){
            s_reverse.insert(0,word+ ' ');
        }
        //remove the last space
        s_reverse.pop_back();

        return s_reverse;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
## array or string: 238. Product of Array Except Self
### Problem
Given an integer array nums of length n, return an array output such that output[i] is equal to the product of all the elements of nums except nums[i].
### Solution
```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(n,1);
        //compute product from left
        for(int i=1; i<n; ++i){
            ans[i]=ans[i-1]*nums[i-1];
        }
        //compute product from right and multiply together
        int tmp = 1;
        for(int i=n-1; i>=0; --i){
            ans[i]=ans[i]*tmp;
            tmp *= nums[i];
        }
        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
## array or string: 334. Increasing Triplet Subsequence
### Problem
Given an integer array nums, return true if there exists a triple of indices (i, j, k) such that:
- 0 <= i < j < k < nums.length
- nums[i] < nums[j] < nums[k]
- If no such triplet exists, return false.
### Solution
```c
bool increasingTriplet(int* nums, int numsSize) {
    if(numsSize<3) return false;

    int first = INT_MAX, second = INT_MAX;

    for(int i=0; i<numsSize; ++i){
        if(nums[i]<=first){
            first = nums[i];
        }else if(nums[i]<=second){
            second = nums[i];
        }else{
            return true;
        }
    }

    return false;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## array or string: 443. String Compression
### Problem
You are given an array of characters, chars, which consists of a series of characters grouped together. The goal is to compress this array in-place and return the new length of the array after compression. The compression works as follows:
- Consecutive duplicate characters are replaced by a single character followed by the count of occurrences if the count is greater than 1.
- For example, if chars = ['a', 'a', 'b', 'b', 'c', 'c', 'c'], the compressed version would be ['a', '2', 'b', '2', 'c', '3'] and the length returned would be 6.
- You must modify the input array in-place without using extra space for another array.
- The order of characters in the array should not be changed.
### Solution
```c
#define MAXLENGTH 5
int compress(char* chars, int charsSize) {
    if(charsSize<2) return 1;
    //readIndex to read chars, writeIndex to write chars
    int readIndex=0, writeIndex=0;
    //loop through chars
    while(readIndex<charsSize){
        char c = chars[readIndex];
        int count=0;
        //caluculate count of char c
        while(readIndex<charsSize&&c==chars[readIndex]){
            count++;
            readIndex++;
        }
        //write it
        chars[writeIndex++]=c;
        //write count if its count is larger than 1
        if(count>1){
            char countStr[MAXLENGTH];
            int l = sprintf(countStr,"%d",count);
            for(int i=0; i<l; ++i){
                chars[writeIndex++]=countStr[i];
            }
        }
    }

    return writeIndex;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## double pointer: 283. Move Zeroes
### Problem
Given an array nums, write a function to move all 0s to the end of it while maintaining the relative order of the non-zero elements.

Note that you must do this in-place without making a copy of the array. Minimize the total number of operations.
### Solution
```c
void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void moveZeroes(int* nums, int numsSize) {
    int i=0, j=0;

    while(j<numsSize){
        if(nums[j]){
            swap(&nums[i],&nums[j]);
            i++;
        }
        j++;
    }

    return;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## double pointer: 392. Is Subsequence
### Problem
Given two strings s and t, return true if s is a subsequence of t, or false otherwise.

A string s is a subsequence of string t if you can derive s from t by deleting some (can be none) of the characters without changing the order of the remaining characters.
### Solution
```c
bool isSubsequence(char* s, char* t) {
    int sLen = strlen(s), tLen = strlen(t), i=0, j=0;
    
    while(i<sLen&&j<tLen){
        if(t[j]!=s[i]){
            j++;
        }else{
            j++;
            i++;
        }
    }

    return i==sLen;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## double pointer: 11. Container With Most Water
### Problem
You are given an integer array height where height[i] represents the height of a vertical line at index i. The two endpoints of a container will be formed by the lines at indices i and j (where i < j). The container can hold water up to the minimum of the two heights. Find two lines that together with the x-axis form a container, such that the container contains the most water.

Return the maximum amount of water a container can store.
### Solution
```c
int maxArea(int* height, int heightSize) {
    int l=0, r=heightSize-1, ans=0;

    while(l<r){
        int hTmp = height[l] < height[r] ? height[l]:height[r];
        int area = hTmp*(r-l);
        if (area > ans) ans = area;
        //move the index
        if(height[l]<height[r]){
            l++;
        }else{
            r--;
        }
    }

    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## double pointer: 1679. Max Number of K-Sum Pairs
### Problem
You are given an integer array nums and an integer k. Your task is to find the maximum number of unique pairs (i, j) such that:
- 0 <= i < j < nums.length
- nums[i] + nums[j] == k
### Solution
```c
int compare(const void * a, const void *b){
    return (*(int *)a)-(*(int *)b);
}
int maxOperations(int* nums, int numsSize, int k) {
    qsort(nums,numsSize,sizeof(int),compare);

    int l=0, r=numsSize-1, ans=0;

    while(l<r){
        int sum = nums[l]+nums[r]; 
        if(sum < k){
            l++;
        }else if(sum > k){
            r--;
        }else{
            ans++;
            l++;
            r--;
        }
    }
    
    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## sliding window: 643. Maximum Average Subarray I
### Problem
Given an integer array nums and an integer k, you need to find the maximum average of any contiguous subarray of length k.
### Solution
```c
double findMaxAverage(int* nums, int numsSize, int k) {
    int ans=0, sum=0; //pay attention to the relationship of speed and type

    for(int i=0; i<k; ++i){
        sum+=nums[i];
    }
    ans=sum;

    for(int i=k; i<numsSize; ++i){
        sum=sum+nums[i]-nums[i-k];
        if(sum>ans) ans=sum;
    }

    return (double)ans/(double)k;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## sliding window: 1456. Maximum Number of Vowels in a Substring of Given Length
Given a string s and an integer k, return the maximum number of vowels in any substring of s with length k.

Vowels are defined as one of the following characters: 'a', 'e', 'i', 'o', 'u'.
### Problem
### Solution
```c
bool isVowel(char c){
    return c=='a'||c=='e'||c=='i'||c=='o'||c=='u';
}

int maxVowels(char* s, int k) {
    int len=strlen(s), ans=0, count=0;

    for(int i=0; i<k; ++i){
        if(isVowel(s[i])){
            ++count;
        }
    }
    ans=count;

    for(int i=k; i<len; ++i){
        if(isVowel(s[i-k])){
            --count;
        }
        if(isVowel(s[i])){
            ++count;
        }
        if(count>ans){
            ans=count;
        }
    }
    
    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## sliding window: 1004. Max Consecutive Ones III
### Problem
Given a binary array nums and an integer k, you can flip at most k zeros. Your task is to return the maximum number of consecutive 1s in the array after flipping at most k zeros.
### Solution
```c
int longestOnes(int* nums, int numsSize, int k) {
    int l=0, r=0, count0=0, ans=0;
    //keep tracking of k number of zeros within the window
    while(r<numsSize){
        if(nums[r]==0){
            ++count0;
        }
        while(count0>k){
            if(nums[l]==0){
                --count0;
            }
            ++l;
        }
        //calculate the maximum window size
        int length=r-l+1;
        if(length>ans){
            ans = length;
        }
        ++r;
    } 
    
    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## sliding window: 1493. Longest Subarray of 1's After Deleting One Element
### Problem
Given a binary array nums, you need to find the length of the longest subarray containing only 1s after deleting exactly one element from the array.
### Solution
```c
int longestSubarray(int* nums, int numsSize) {
    int l=0, r=0, ans=0, count0=0;

    while(r<numsSize){
        //a more efficient way to calculate the number of zero
        count0+=(1-nums[r]);
        while(count0>1){
            count0-=(1-nums[l]);
            ++l;
        }  
        int tmp=r-l;
        if(tmp>ans) ans=tmp;
        ++r;
    }

    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## prefix sum: 1732. Find the Highest Altitude
### Problem
You are given an array gain where gain[i] is the net gain in altitude at the i-th step. You start at an altitude of 0 and want to find the highest altitude you can reach after following all the steps in the array.

Your task is to return the highest altitude you can reach after all steps.
### Solution
```c
int largestAltitude(int* gain, int gainSize) {
    int ans=0, sum=0;

    for(int i=0; i<gainSize; ++i){
        sum+=gain[i];
        if(sum>ans){
            ans=sum;
        }
    }
    
    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## prefix sum: 724. Find Pivot Index
### Problem
Given an array of integers nums, you need to find the "pivot index" of this array. The pivot index is defined as the index where the sum of all numbers to the left of the index is equal to the sum of all numbers to the right of the index.

If no such index exists, return -1. If there are multiple pivot indexes, you should return the leftmost pivot index.
### Solution
```c
int pivotIndex(int* nums, int numsSize) {
    int sum=0, lsum=0;
    //calculate sum
    for(int i=0; i<numsSize; ++i){
        sum+=nums[i];
    }
    //calculate pivot
    for(int i=0; i<numsSize; ++i){
        if(lsum==sum-lsum-nums[i]){
            return i;
        }
        lsum+=nums[i];
    }
    return -1;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## hash table or set: 2215. Find the Difference of Two Arrays
### Problem
You are given two 0-indexed integer arrays, nums1 and nums2. You need to find the unique elements in each array and return them as two separate arrays.

Specifically, you want to:
- Find all the unique elements in nums1 that are not present in nums2
- Find all the unique elements in nums2 that are not present in nums1
- Return the result as a 2D array. The first element of the result should be the array ans1, and the second element should be the array ans2.
### Solution
```cpp
class Solution {
public:
    vector<vector<int>> findDifference(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> set1(nums1.begin(),nums1.end());
        unordered_set<int> set2(nums2.begin(),nums2.end());

        vector<int> ans1;
        for(int num:set1){
            if(set2.count(num)==0){
                ans1.emplace_back(num);
            }
        }
        vector<int> ans2;
        for(int num:set2){
            if(set1.count(num)==0){
                ans2.emplace_back(num);
            }
        }
        return {ans1,ans2};
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
## hash table or set: 1207. Unique Number of Occurrences
### Problem
Given an array of integers arr, you need to determine if the number of occurrences of each value in the array is unique.
### Solution
```cpp
class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        unordered_map<int, int> countValue;
        unordered_set<int> countSet;

        for(int num:arr){
            countValue[num]++;
        }

        for(auto it = countValue.begin(); it!=countValue.end(); ++it){
            if(countSet.count(it->second)==0){
                countSet.insert(it->second);
            }else{
                return false;
            }
        }

        return true;
        //Space Complexity: O(1);
        //Time Complexity: O(n);
    }
};
```
## hash table or set: 1657. Determine if Two Strings Are Close
### Problem
You are given two strings word1 and word2. The strings are considered close if you can achieve word2 from word1 under the following conditions:
- You can rearrange the characters in word1 to get word2.
- The frequency of each character in word1 can be changed to match the frequency of characters in word2, meaning the character counts must be able to be rearranged.
### Solution
```c
int compare(const void * a, const void * b){
    return (*(int *)a) - (*(int *)b);
}

bool closeStrings(char* word1, char* word2) {
    //check the length of word1 and words 
    if(strlen(word1)!=strlen(word2)){
        return false;
    }
    //get frequencies
    int count1[26]={0};
    while(*word1){
        count1[*word1-'a']++;
        word1++;
    }
    int count2[26]={0};
    while(*word2){
        count2[*word2-'a']++;
        word2++;
    }
    //check if word1 and word2 have the same set of characters
    for(int i=0; i<26; ++i){
        if((count1[i]==0&&count2[i]>0)||(count1[i]>0&&count2[i]==0)){
            return false;
        }
    }
    //sort the frequencies
    qsort(count1, 26, sizeof(int), compare);
    qsort(count2, 26, sizeof(int), compare);
    //compare the frequencies
    for(int i=0; i<26; ++i){
        if(count1[i]!=count2[i]){
            return false;
        }
    }

    return true;
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## hash table or set: 2352. Equal Row and Column Pairs
### Problem
Given a 0-indexed 2D integer array grid, your task is to count the number of pairs (r, c) such that the r-th row and c-th column of grid are equal.

A row and column are considered equal if they contain the same elements in the same order.
### Solution
```c
//solution 1: compare directly
int equalPairs(int** grid, int gridSize, int* gridColSize) {
    int ans = 0 ;
    for(int r=0; r<gridSize; ++r){
        for(int c=0; c<gridSize; ++c){
            bool equal = true;
            for(int n=0; n<gridSize; ++n){
                if(grid[r][n]!=grid[n][c]){
                    equal = false;
                    break;
                }
            }
            if(equal){
                ++ans;
            }
        }
    }
    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n^3);
}
```
```cpp
//solution 1: with hashmap
class Solution {
public:
    int equalPairs(vector<vector<int>>& grid) {
        int ans=0;
        unordered_map<string, int> cnt;

        for(auto row: grid){
            string tmp;
            for(int num: row){
                tmp+=to_string(num)+",";
            }
            cnt[tmp]++;
        }

        for(int i=0; i<grid.size(); ++i){
            string tmp;
            for(int j=0; j<grid.size(); ++j){
                tmp+=to_string(grid[j][i])+",";
            }
            ans+=cnt[tmp];
        }

        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n^2);
    }
};
```
---
## stack: 2390. Removing Stars From a String
### Problem
You are given a string s which contains lowercase letters and stars (*). Every time you encounter a star (*), it means you should remove the star and the character immediately preceding it. If the star is at the beginning of the string or if there is no character preceding it, you simply remove the star.

Your task is to process the string and return the final result after all the removals.
### Solution
```cpp
//solution 1: with stack
class Solution {
public:
    string removeStars(string s) {
        string ans;
        stack<char> st;
        for(char c:s){
            if(c!='*'){
                st.push(c);
            }else{
                st.pop();
            }
        }
        while(!st.empty()){
            ans+=st.top();
            st.pop();
        }
        reverse(ans.begin(), ans.end());
        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
```c
//solution 2: without stack
char* removeStars(char* s) {
    int n = strlen(s);
    char *ans = (char *)malloc((n+1)*sizeof(char)); //null terminator is not included in strlen
    if(ans==NULL){
        return NULL;
    } 

    int j=0;
    for(int i=0; i<n; ++i){
        if(s[i]!='*'){
            ans[j++]=s[i];
        }else{
            --j;
        }
    }
    ans[j]='\0';

    return ans;
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## stack: 735. Asteroid Collision
### Problem
You are given an array of integers asteroids, where each integer represents the size and direction of an asteroid. Positive integers represent asteroids moving to the right, and negative integers represent asteroids moving to the left. An asteroid can collide with another asteroid when they move towards each other:
- When two asteroids collide, the smaller one is destroyed. If both asteroids are of the same size, both are destroyed.
- Asteroids that are moving in the same direction will never collide.

You need to simulate the collision process and return the state of the asteroids after all collisions have taken place.
### Solution
```cpp
class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        stack<int> st;
        for(int asteroid: asteroids){
            bool survived = true;

            while(survived && !st.empty()){
                int topAsteroid = st.top();
                if( topAsteroid >0 && asteroid < 0){
                    if(topAsteroid < -asteroid){
                        st.pop();
                    }else if(topAsteroid > -asteroid){
                        survived=false;
                    }else{
                        st.pop();
                        survived=false;
                    }
                }else{
                    break;
                }
            }

            if(survived){
                st.push(asteroid);
            }
        }
        
        vector<int> ans;
        while(!st.empty()){
            ans.emplace_back(st.top());
            st.pop();
        }
        reverse(ans.begin(), ans.end());

        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
## stack: 394. Decode String
### Problem
Given an encoded string, return its decoded string.

The encoding rule is: k[encoded_string], where the encoded_string inside the square brackets is being repeated exactly k times.

You may assume that the input string is always valid. No extra white spaces, square brackets are well-formed, etc.
### Solution
```cpp
//solution 1: double stack
class Solution {
public:
    string decodeString(string s) {
        stack<int> stCount;
        stack<string> stStr;
        int currCount=0;
        string currStr="";

        for(char c: s){
            if(isdigit(c)){
                //calculate count
                currCount = 10*currCount + c-'0';
            }else if(c=='['){
                //push the count to stack
                stCount.push(currCount);
                //push the string to stack
                stStr.push(currStr);
                //set the count to zero
                currCount=0;
                //set the string to empty
                currStr="";
            }else if(c==']') {
                //get the most recent count
                int count = stCount.top();
                //pop the most recent count
                stCount.pop();
                //get the previous string
                string preStr = stStr.top();
                //pop the most recent string
                stStr.pop();
                //concatenation
                for(int i=0; i<count; ++i){
                    preStr+=currStr;
                }
                //update
                currStr=preStr;
            }else{
                currStr += c;
            }
        }

        return currStr;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
```cpp
//solution 2: using stack to simulate recursion
class Solution {
public:
    string decodeString(string s) {
        //this problem can be transformed to multiple sub problems
        //a number with strings inside a []
        //stack pair to store variables when loop through string
        stack<pair<int, string>> stack_pair;
        //intialize variables
        int multiplier = 0;
        string s_tmp = "";

        for (char c: s){
            if (c>='0'&&c<='9'){
                //calculate multiplier
                multiplier=multiplier*10+c-'0';
            }else if (c=='['){
                //push to stack
                stack_pair.push(make_pair(multiplier,s_tmp));
                //intialize variables for subproblems
                s_tmp="";
                multiplier=0;
            }else if (c==']'){
                //check whether this stack pair is empty or not
                if(!stack_pair.empty()){
                    //extract the last pushed pair
                    pair<int, string> top_sp = stack_pair.top();
                    //cout << top_sp.first << "," << top_sp.second << endl; 
                    int num = top_sp.first;
                    string ss_tmp = "";
                    //calculate duplicate string
                    for(int i=0; i<num; i++){
                        ss_tmp+=s_tmp;
                    }
                    //connect with the string of the last pushed pair
                    stack_pair.top().second += ss_tmp;
                    //set this value to s_tmp
                    s_tmp = stack_pair.top().second;
                    //pop the last pushed pair
                    stack_pair.pop();  
                }
            }else{
                //accumulate sub string
                s_tmp+=c;
            }
        }
        
        return s_tmp;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
---
## queue: 933. Number of Recent Calls
### Problem
You need to implement the RecentCounter class with the following methods:
- RecentCounter(): Initializes the counter with an empty list of recent calls.
- int ping(int t): Adds a new call at time t and returns the number of calls that have taken place in the last 3000 milliseconds (including the new call).
### Solution
```c
//solution: linked list based approach 
//Space Complexity: O(n);
//Time Complexity: O(n);
typedef struct Node {
    int t;
    struct Node * next;
}Node_t; 

typedef struct {
    Node_t * head;
    Node_t * tail;
    int count;
} RecentCounter;

Node_t * newNode(int t){
    Node_t * node = (Node_t *)malloc(sizeof(Node_t));
    if(node==NULL){
        return NULL;
    }
    node->t = t;
    node->next = NULL;
    return node;
}

RecentCounter* recentCounterCreate() {
    RecentCounter * obj = (RecentCounter * )malloc(sizeof(RecentCounter));
    if(obj==NULL){
        return NULL;
    }
    obj->head = NULL;
    obj->tail = NULL;
    obj->count = 0;
    return obj;
}

int recentCounterPing(RecentCounter* obj, int t) {
    if(obj==NULL){
        return 0;
    }
    Node_t * node = newNode(t);
    obj->count++;
    if(obj->head == NULL && obj->tail == NULL){
        obj->head = node;
        obj->tail = node;
    }else{
        obj->tail->next = node;
        obj->tail = node;
    }
    Node_t * pCurr = obj->head;
    while(pCurr!=NULL){
        if(pCurr->t<t-3000){
            Node_t * pTmp = pCurr;
            pCurr=pCurr->next;
            free(pTmp);
            obj->count--;
        }else{
            break;
        }
    }
    obj->head=pCurr;
    return obj->count;
}

void recentCounterFree(RecentCounter* obj) {
    if(obj!=NULL){
        Node_t * pCurr = obj->head;
        while(pCurr!=NULL){
            Node_t * pTmp = pCurr;
            pCurr=pCurr->next;
            free(pTmp);
        }
    }
    free(obj);
    return;
}
```
## queue: 649. Dota2 Senate
### Problem
In the game, players from both factions take turns. Each player can ban one player from the opposing faction. The order of players is determined by their positions in a string representing the senate. The game's rules are as follows:
- If a player from the Radiant faction ('R') is chosen to act, they will ban one player from the Dire faction ('D').
- Conversely, if a player from the Dire faction ('D') is chosen to act, they will ban one player from the Radiant faction ('R').
- The next player to act will be the one who is still present and has not been banned yet.
- The game continues until all players from one faction have been banned.

Input: A string senate where each character represents a player:
- 'R' for a player in the Radiant faction.
- 'D' for a player in the Dire faction.

Output:
Return "Radiant" if the Radiant faction wins, or "Dire" if the Dire faction wins.

### Solution
```c
//solution: circular buffer based approach 
//Space Complexity: O(n);
//Time Complexity: O(n);
#define CAPACITY 64
typedef struct queue{
    int * arr;
    int front;
    int rear;
    int capacity;
}queue_t;

queue_t * iniQueue(){
    queue_t * obj = (queue_t *)malloc(sizeof(queue_t));
    if(obj==NULL){
        return NULL;
    }
    obj->capacity = CAPACITY;
    obj->front = 0;
    obj->rear = 0;
    obj->arr = (int *)malloc((obj->capacity)*sizeof(int));
    if(obj->arr==NULL){
        free(obj);
        return NULL;
    }
    return obj;
}

bool isFull(queue_t * obj){
    return obj->rear == (obj->front+1) % obj->capacity;
}

bool isEmpty(queue_t * obj){
    return obj->front == obj->rear;
}

void enqueue(queue_t * obj, int val){
    if(isFull(obj)){
        int oldCapacity = obj->capacity;
        obj->capacity = obj->capacity * 2;
        int * p = (int *)realloc(obj->arr,obj->capacity*sizeof(int));
        if(p==NULL){
            return;
        }
        obj->arr = p;
        if(obj->front<obj->rear){
            memcpy(&(obj->arr[oldCapacity]),obj->arr,obj->front*sizeof(int));
            obj->front = (obj->rear + oldCapacity) % obj->capacity;
        }   
    }
    obj->arr[obj->front] = val;
    obj->front = (obj->front+1) % obj->capacity;
    return;
}

int dequeue(queue_t * obj){
    if(isEmpty(obj)){
        return -1;
    }
    int val = obj->arr[obj->rear];
    obj->rear = (obj->rear+1) % obj->capacity;
    return val;
}

void freeQueue(queue_t * obj){
    if(obj!=NULL){
        free(obj->arr);
        free(obj);
    }
    return;
}

char* predictPartyVictory(char* senate) {
    int n = strlen(senate);
    queue_t * queueR = iniQueue();
    queue_t * queueD = iniQueue();

    for(int i=0; i<n; ++i){
        if(senate[i]=='R'){
            enqueue(queueR,i);
        }else{
            enqueue(queueD,i);
        }
    }

    while(!isEmpty(queueR)&&(!isEmpty(queueD))){
        int idxR = dequeue(queueR);
        int idxD = dequeue(queueD);
        if(idxR<idxD){
            enqueue(queueR,idxR+n);
        }else{
            enqueue(queueD,idxD+n);
        }
    }

    const char * winner = isEmpty(queueR)? "Dire" : "Radiant";
    freeQueue(queueR);
    freeQueue(queueD);

    return winner;
}
```
---
## list: 2095. "Delete the Middle Node of a Linked List"
### Problem
You are given the head of a linked list. Delete the middle node, and return the head of the modified linked list.

The middle node is defined as follows:

If there are two middle nodes, the second middle node should be deleted.
### Solution
```c
struct ListNode* deleteMiddle(struct ListNode* head) {
    if(head == NULL || head->next == NULL) {
        free(head);
        return NULL;
    }
    struct ListNode * fast = head, * slow = head, *pre = NULL;

    while(fast != NULL && fast->next!= NULL){
        fast = fast->next->next;
        pre = slow;
        slow = slow->next;
    }

    pre->next = slow->next;
    free(slow);

    return head;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## list: 206.  "Reversed Linked List"
### Problem
Given the head of a singly linked list, reverse the list, and return the reversed list.
### Solution
```c
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode *previousNode = NULL, *currentNode = head, *nextNode = NULL;

    while(currentNode!=NULL){
        nextNode = currentNode->next;
        currentNode->next = previousNode;
        previousNode = currentNode;
        currentNode = nextNode;
    }

    return previousNode;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## list: 328.  "Odd Even Linked List"
### Problem
Given a singly linked list, group all odd nodes together followed by the even nodes. Please note that the relative order inside both the even and odd groups should remain as it was in the input.

You should do it in place and require O(1) extra space.

The nodes in the input linked list are labeled from 1 to n, where n is the total number of nodes in the linked list.
### Solution
```c
struct ListNode* oddEvenList(struct ListNode* head) {
    //special case
    if(head == NULL){
        return NULL;
    }
    //initialize variables
    struct ListNode * pEven = head->next, * pOddCurr = head, * pEvenCurr = pEven;
    //the condition here works for list of odd length and even length respectively
    while(pEvenCurr != NULL && pEvenCurr->next != NULL){
        pOddCurr->next = pEvenCurr->next; //connet odd nodes list
        pOddCurr = pOddCurr->next; //move odd nodes list pointer
        pEvenCurr->next = pOddCurr->next; //connect event nodes list
        pEvenCurr = pEvenCurr->next; //move even nodes list pointer
    }
    //connect odd and even list
    pOddCurr->next = pEven;
    //return head pointer
    return head;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## list: 2130. "Maximum Twin Sum of a Linked List"
### Problem
You are given the head of a linked list. The linked list is 0-indexed and consists of even number of nodes. The twin sum is defined as the sum of the i-th node from the beginning and the (n-1-i) th node, where i goes from 0 to n/2. Your task is to return the maximum twin sum of this linked list.
### Solution
```c
struct ListNode * reverseList(struct ListNode* head){
    struct ListNode * curr = head, * pre = NULL, * next = NULL;

    while(curr != NULL){
        next = curr->next;
        curr->next = pre;
        pre = curr;
        curr = next;
    } 

    return pre;
}

int pairSum(struct ListNode* head) {

    struct ListNode * fast = head, * slow = head;

    while(fast != NULL && fast->next != NULL){
        fast = fast->next->next;
        slow = slow->next;
    }

    struct ListNode * tail = reverseList(slow);
    slow = NULL;

    int ans = 0, sum = 0;
    while(head != NULL && tail != NULL){
        sum = head->val+tail->val;
        if(sum > ans){
            ans = sum;
        }
        head = head->next;
        tail = tail->next;
    }

    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## binary tree - dfs: 104. Maximum Depth of Binary Tree
### Problem
Given a binary tree, return its maximum depth.

A binary tree's maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node. A leaf is a node with no children.
### Solution
```c
int maxDepth(struct TreeNode* root) {
    if(root == NULL){
        return 0;
    }

    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);

    return (leftDepth>rightDepth? leftDepth:rightDepth)+1;
    //Space Complexity: O(h); //height of the tree, recursion stack
    //Time Complexity: O(n);
}
```
## binary tree - dfs: 872. Leaf-Similar Trees
### Problem
Given two binary trees, root1 and root2, return true if and only if the sequence of leaf values of both trees are the same.

A leaf value is a value of a node that has no children.
### Solution
```cpp
class Solution {
public:

    void collectLeaves(TreeNode * root, vector<int> & leaves){
        if(root == nullptr){
            return;
        }
        if(root->left == nullptr && root->right == nullptr){
            leaves.emplace_back(root->val);
        }
        collectLeaves(root->left, leaves);
        collectLeaves(root->right, leaves);
        return;
    }

    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> leaves1;
        vector<int> leaves2;

        collectLeaves(root1,leaves1);
        collectLeaves(root2,leaves2);

        return leaves1==leaves2;
        //Space Complexity: O(n+m);
        //Time Complexity: O(n+m);
    }
};
```
## binary tree - dfs: 1448. Count Good Nodes in Binary Tree
### Problem
Given a binary tree, you need to count the number of "good nodes."

A node is considered "good" if, in the path from the root to that node, there are no nodes with a value greater than the node’s value.

Return the number of good nodes in the binary tree.
### Solution
```c
void countGoodNodes(struct TreeNode* root, int max, int *count){
    if(root==NULL){
        return;
    }

    if(root->val>=max){
        max=root->val;
        *count=*count+1;
    }

    countGoodNodes(root->left, max, count);
    countGoodNodes(root->right, max, count);

    return;
}

int goodNodes(struct TreeNode* root) {
    if(root==NULL){
        return 0;
    }
    int count = 0; 
    countGoodNodes(root, root->val, &count);
    return count;
    //Space Complexity: O(h); //height of the tree, recursion stack
    //Time Complexity: O(n);
}
```
## binary tree - dfs: 437. Path Sum III
### Problem
You are given a binary tree in which each node contains an integer value. You need to find the number of paths that sum to a given value.

The path does not need to start or end at the root or a leaf, but it must be continuous (i.e., follow parent-child connections). A path sum is defined as the sum of the values along the path.
### Solution
```c
int helper(struct TreeNode* root, long long targetSum){
    if(root == NULL){
        return 0;
    }

    int ret = 0;
    targetSum = targetSum-root->val;
    if(targetSum == 0){
        ret = 1;
    }
    ret += helper(root->left, targetSum);
    ret += helper(root->right, targetSum);

    return ret;
}

int pathSum(struct TreeNode* root, int targetSum) {
    if(root == NULL){
        return 0;
    }
    
    int count = helper(root, targetSum);
    count += pathSum(root->left, targetSum);
    count += pathSum(root->right, targetSum);
    return count;
    //Space Complexity: O(n); recursion stack
    //Time Complexity: O(n^2);
}
```
## binary tree - dfs: 1372. Longest ZigZag Path in a Binary Tree
### Problem
You are given the root of a binary tree. A ZigZag path is defined as follows:

- Starting from some node, you can go to one of its children and switch directions at each step.
- The length of the path is the number of edges traversed.

Your goal is to return the length of the longest ZigZag path in the tree.
### Solution
```c
void dfs(struct TreeNode * node, int direction, int len, int *ans){
    if(node == NULL){
        return;
    }

    if(len>*ans){
        *ans=len;
    }

    if(direction == 0){
        dfs(node->left, 0, 1, ans);
        dfs(node->right, 1, len+1, ans);
    }else{
        dfs(node->left, 0, len+1, ans);
        dfs(node->right, 1, 1, ans);
    }

    return;
}

int longestZigZag(struct TreeNode* root) {
    if(root == NULL){
        return 0;
    }

    int ans = 0;
    dfs(root, 0, 0, &ans);
    dfs(root, 1, 0, &ans);
    return ans;
    //Space Complexity: O(n); recursion stacck
    //Time Complexity: O(n);
}
```
## binary tree - dfs: 236. Lowest Common Ancestor of a Binary Tree
### Problem
Given a binary tree, find the lowest common ancestor (LCA) of two given nodes in the tree.

The lowest common ancestor is defined as the lowest node that has both nodes as descendants (where we allow a node to be a descendant of itself).
### Solution
```c
bool helper(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q, struct TreeNode ** ans){
    if(root == NULL){
        return false;
    }

    bool inLeftSubTree = helper(root->left, p, q, ans);
    bool inRightSubTree = helper(root->right, p, q, ans);

    if((inLeftSubTree&&inRightSubTree)||((root->val==p->val||root->val==q->val)&&(inLeftSubTree||inRightSubTree))){
        *ans = root;
    }
    
    return root->val==p->val||root->val==q->val||inLeftSubTree||inRightSubTree;
}

struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q) {
    struct TreeNode * ans = NULL;
    helper(root,p,q,&ans);
    return ans;
    //Space Complexity: O(n); recursion stack
    //Time Complexity: O(n);
}
```
---
## binary tree - bfs: 199. Binary Tree Right Side View
### Problem
Given a binary tree, imagine you are standing on the right side of it, return the values of the nodes you can see ordered from top to bottom.
### Solution
```cpp
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> ans;
        if(root == nullptr){
            return ans;
        }
        
        queue<TreeNode *> q;
        q.push(root);

        while(!q.empty()){
            int size = q.size();
            for(int i=0; i<size; ++i){
                TreeNode * tmp = q.front();
                q.pop();
                if(i==size-1){
                    ans.emplace_back(tmp->val);
                }
                if(tmp->left!=nullptr){
                    q.push(tmp->left);
                }
                if(tmp->right!=nullptr){
                    q.push(tmp->right);
                }
            }
        }

        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
```c
#define SIZE 100

void helper(struct TreeNode * node, int depth, int *ans, int *returnSize){
    if(node == NULL){
        return;
    }

    if(*returnSize == depth){
        ans[*returnSize] = node->val;
        *returnSize = *returnSize+1;
    }

    depth = depth+1;
    helper(node->right,depth,ans,returnSize);
    helper(node->left,depth,ans,returnSize);

    return;
}

int* rightSideView(struct TreeNode* root, int* returnSize) {
    *returnSize = 0;
    if(root == NULL){
        return NULL;
    }

    int *ans = (int *)malloc(SIZE*sizeof(int));
    if(ans == NULL){
        return NULL;
    }
    
    helper(root,0,ans,returnSize);

    return ans;
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## binary tree - bfs: 1161. Maximum Level Sum of a Binary Tree
### Problem
Given a binary tree, the goal is to find the maximum level sum. The level of a binary tree is defined by the depth of the tree, with the root node being at level 1, its children at level 2, and so on.

You need to return the smallest level (1-indexed) that has the maximum sum of values.
### Solution
```cpp
class Solution {
private:
    queue<TreeNode *> q; 
public:
    int maxLevelSum(TreeNode* root) {
        int level = 1, ans = 1, maxSum = INT_MIN;
        q.push(root);

        while(!q.empty()){
            int size = q.size();
            int sum = 0; 
            for(int i=0; i<size; ++i){
                TreeNode * tmp = q.front();
                q.pop();
                sum += tmp->val;
                if(tmp->left != nullptr){
                    q.push(tmp->left);
                }
                if(tmp->right != nullptr){
                    q.push(tmp->right);
                }
            }
            if(sum > maxSum){
                maxSum = sum;
                ans = level;
            }
            level++;
        }

        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n);
    }
};
```
---
## binary search tree: 700. Search in a Binary Search Tree
### Problem
You are given the root of a binary search tree (BST) and an integer val. You need to search for the val in the BST. 

If the node with the value val exists, return the subtree rooted with that node. Otherwise, return NULL.
### Solution
```c
struct TreeNode* searchBST(struct TreeNode* root, int val) {
    struct TreeNode * node = root; 

    while(node != NULL){
        if(node->val < val){
            node = node->right;
        }else if(node->val > val){
            node = node->left;
        }else{
            return node;
        }
    }

    return node;
    //Space Complexity: O(1);
    //Time Complexity: O(logn);
}
```
## binary search tree: 450. Delete Node in a BST
### Problem
Given a root node reference of a Binary Search Tree (BST) and a key, delete the node with the given key in the BST. Return the root node reference (possibly updated) of the BST.
### Solution
```c
struct TreeNode* minValueNode(struct TreeNode* node){
    struct TreeNode* curr = node;
    while(curr != NULL && curr->left != NULL){
        curr = curr->left;
    }
    return curr;
}

struct TreeNode* deleteNode(struct TreeNode* root, int key) {
    if(root == NULL){
        return NULL;
    }

    if(root->val > key){
        root->left = deleteNode(root->left, key);
    }else if(root->val < key){
        root->right = deleteNode(root->right, key);
    }else{
        if(root->left == NULL){
            //if one or all of its child is NULL
            struct TreeNode* tmp = root->right;
            free(root);
            return tmp;
        }else if(root->right == NULL){
            //if one or all of its child is NULL
            struct TreeNode* tmp = root->left;
            free(root);
            return tmp;
        }else{
            //if none of its child is NULL
            struct TreeNode * tmp = minValueNode(root->right);
            int val = tmp->val;
            tmp->val = key;
            root->val = val;
            root->right = deleteNode(root->right, key);
        }
    }

    return root;
    //Space Complexity: O(logn);
    //Time Complexity: O(logn);
}
```
---
## graph - dfs: 841. Keys and Rooms
### Problem
There are n rooms labeled from 0 to n - 1 and initially, all rooms are locked. However, you are given a list of lists rooms where each rooms[i] contains a list of keys that are in room i. Each key is an integer representing a room that can be unlocked with that key.
- You start in room 0.
- You can only enter a room if you have its key.
- Return true if you can visit all rooms, otherwise, return false.
### Solution
```cpp
class Solution {
public:

    bool check(vector<bool> &visited){
        for(bool v: visited){
            if(!v){
                return false;
            }
        }
        return true;
    }

    void dfs(vector<vector<int>>& rooms, int i, vector<bool> &visited){

        visited[i] = true;

        if(check(visited)){
            return;
        }
        
        for(int j=0; j<rooms[i].size(); ++j){
            if(!visited[rooms[i][j]]){
                dfs(rooms, rooms[i][j], visited);
            }
        }
    }

    bool canVisitAllRooms(vector<vector<int>>& rooms) {
 
        vector<bool> visited(rooms.size(), false);

        dfs(rooms, 0, visited);

        return check(visited);
        //Space Complexity: O(n); //n is the number of rooms
        //Time Complexity: O(n+m); //m is the number of all keys
    }
};
```
## graph - dfs: 547. Number of Provinces
### Problem
There are n cities. Some of them are directly connected, and some are not.

If city A is directly connected to city B, and city B is directly connected to city C, then city A is indirectly connected to city C.

You are given an n x n matrix isConnected where:
- isConnected[i][j] == 1 → city i is connected to city j
- isConnected[i][j] == 0 → city i is NOT connected to city j
- A province is a group of directly or indirectly connected cities.

Return the total number of provinces.
### Solution
```cpp
class Solution {
public:

    void dfs(vector<vector<int>>& isConnected, int i, int n, vector<bool> &visited){
        if(visited[i]){
            return;
        }

        visited[i] = true;

        for(int j=0; j<n; ++j){
            if(isConnected[i][j]==1){
                dfs(isConnected, j, n, visited);
            }
        }
    }

    int findCircleNum(vector<vector<int>>& isConnected) {
        int ans = 0, n = isConnected.size();
        vector<bool> visited(n, false);

        for(int i=0; i<n; ++i){
            if(!visited[i]){
                ans++;
            }else{
                continue;
            }
            dfs(isConnected, i, n, visited);
        }
    
        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n^2);
    }
};
```
## graph - dfs: 1466. Reorder Routes to Make All Paths Lead to the City Zero
### Problem
There are n cities numbered from 0 to n - 1 and a list of connections where connections[i] = [a_i, b_i] represents a directed route from city a_i to city b_i.

The goal is to reorder the routes such that all paths lead to city 0. A route can be reordered in either direction. You need to return the minimum number of edges that need to be reversed.
### Solution
```cpp
class Solution {
public:

    void dfs(vector<vector<pair<int, int>>>& graph, int i, vector<int> &visited, int &count){

        visited[i] = 1;

        for(auto &neighbor: graph[i]){
            int city = neighbor.first, direction = neighbor.second;
            if (!visited[city]) {
                if (direction == 1) {
                    count++;
                }
                dfs(graph, city, visited, count);
            }
        }
    }

    int minReorder(int n, vector<vector<int>>& connections) {
        //change graph to bidirectional 
        //vector<vector<int>> isConnected(n,vector<int>(n,0));
        //adjacency matrix exceeds the memory limit

        vector<vector<pair<int, int>>> graph(n);

        for(auto c: connections){
            graph[c[0]].push_back({c[1],1});
            graph[c[1]].push_back({c[0],-1});
        }

        int ans = 0;
        vector<int> visited(n, 0);
        dfs(graph, 0, visited, ans);
        
        return ans;
        //Space Complexity: O(m+n); //n is the number of cities, m is the number of connections
        //Time Complexity: O(m+n);
    }
};
```
## graph - dfs: 399. Evaluate Division
### Problem
You are given:
- A list of equations like Ai / Bi = values[i], where Ai and Bi are variables (strings).
- A list of queries like [Cj, Dj], asking for Cj / Dj.

You need to:
- Return the result for each query.
- If the variables in the query are undefined or there’s no connection between them, return -1.0.
- Assume the input is valid: no division by zero and no contradictions.

### Solution
```cpp
class Solution {
private:
    unordered_map<string, unordered_map<string,double>> graph;
public:
    bool dfs(const string& start, const string& end, double &val, unordered_map<string, bool> & visited){

        if(start == end) {
            return true;
        }

        visited[start] = true;

        for(auto &pair: graph[start]){
            const string & next = pair.first;
            const double & weight = pair.second;

            if(!visited[next]){
                val = val * weight;
                if(dfs(next,end,val,visited)){
                    return true;
                }
                val = val / weight;
            }
        }

        return false;
    }

    vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
        //build the graph
        for(int i=0; i<equations.size(); ++i){
            const string& a = equations[i][0];
            const string& b = equations[i][1];

            graph[a][b] = values[i];
            graph[b][a] = 1.0/values[i];
        }

        vector<double> ans;

        for(auto & query: queries){
            const string & start = query[0];
            const string & end = query[1];
            
            if(graph.find(start) == graph.end() || graph.find(end) == graph.end()){
                ans.push_back(-1.0);
                continue;
            }

            double val = 1.0;

            if(start == end){
                ans.push_back(val);
                continue;
            }

            unordered_map<string, bool> visited; //by default, the bool is false
            if(dfs(start, end, val, visited)){
                ans.push_back(val);
            }else{
                ans.push_back(-1.0);
            }
            
        }

        return ans;
        //Space Complexity: O(n); //n is the number of equations
        //Time Complexity: O(q*n); //q is the number of queries
    }
};
```
---
## graph - bfs: 1926. Nearest Exit from Entrance in Maze
### Problem
### Solution
```cpp
class Solution {
private:
    const char empty = '.';
    const char wall = '+';
    const char visited = '-';
    vector<vector<int>> directions = {{-1,0},{1,0},{0,1},{0,-1}};
public:
    void bfs(vector<vector<char>>& maze, int m, int n, int xStart, int yStart, int & count){
        queue<pair<int, int>> q;
        q.push({xStart,yStart});
        maze[xStart][yStart] = visited;

        while(!q.empty()){
            //bfs for a layer
            count++;
            int size = q.size();
            for(int i=0; i<size; ++i){
                auto [x,y] = q.front();
                q.pop();

                for(int j=0; j<4; ++j){
                    int xNext = x+directions[j][0], yNext = y+directions[j][1];
                    if((xNext>=0 && xNext<m && yNext>=0 && yNext<n) && (maze[xNext][yNext]=='.')){
                        if(xNext ==0 || xNext == m-1 || yNext == 0 || yNext == n-1){
                            return;
                        }
                        maze[xNext][yNext] = visited;
                        q.push({xNext, yNext});
                    }
                }
            }
        }

        count = -1; //no such path exists
        return; 
    }

    int nearestExit(vector<vector<char>>& maze, vector<int>& entrance) {
        int m = maze.size(), n = maze[0].size();
        int ans = 0;

        bfs(maze, m, n, entrance[0], entrance[1], ans);

        return ans;
        //Space Complexity: O(mxn); //addtional space of the queue
        //Time Complexity: O(mxn); //bfs process each cell of the maze
    }
};
```
## graph - bfs: 994. Rotting Oranges
### Problem
You are given an m x n grid where each cell can have one of three values:
- 0 representing an empty cell.
- 1 representing a fresh orange.
- 2 representing a rotten orange.

Every minute, any fresh orange that is adjacent (4-directionally) to a rotten orange becomes rotten. After n minutes, no more fresh oranges can become rotten.

Return the minimum number of minutes that must elapse until no fresh oranges remain. If this is impossible, return -1.
### Solution
```cpp
class Solution {
private:
    vector<vector<int>> directions = {{-1,0},{1,0},{0,1},{0,-1}};
public:
    void bfs(vector<vector<int>>& grid, int m, int n, int & minutes){
        //put rotten oranges in the queue
        queue<pair<int, int>> q;
        //count of fresh oranges
        int freshCount = 0;
        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                if(grid[i][j]==2){
                    q.push({i,j});
                }else if(grid[i][j]==1){
                    freshCount++;
                }
            }
        }
        if(freshCount == 0){
            return;
        }

        //bfs search
        while(!q.empty()){
            //get size of queue
            int size = q.size();
            bool rot = false;
            //loop this layer of bfs
            for(int i=0; i<size; ++i){
                auto [x, y]= q.front();
                q.pop();

                for(int j=0; j<4; ++j){
                    int xNext = x+directions[j][0], yNext = y+directions[j][1];
                    if((xNext>=0 && xNext<m && yNext>=0 && yNext<n) && (grid[xNext][yNext]==1)){
                        q.push({xNext, yNext});
                        grid[xNext][yNext]=2;
                        freshCount--;
                        rot = true;
                    }
                }
            }
            //only increase minutes when rotting happens
            if(rot){
                minutes++;
            }
        }

        if(freshCount>0){
            minutes = -1;
        }else{
            minutes;
        }
    }

    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), ans = 0;

        bfs(grid, m , n, ans);

        return ans;
        //Space Complexity: O(mxn); //addtional space of the queue
        //Time Complexity: O(mxn); //bfs process each cell of the grid
    }
};
```
---
## priority queue: 215. Kth Largest Element in an Array
### Problem
Given an integer array nums and an integer k, return the kth largest element in the array. Note that to solve this problem without sorting.
### Solution
```c
//solution1: max heap
void swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
//heapify subtree recursively
void heapify(int* heap, int heapSize, int i){
    int largest = i, left = 2*i+1, right = 2*i+2;
    if(left < heapSize && heap[left] > heap[largest]){
        largest = left;
    }
    if(right < heapSize && heap[right] > heap[largest]){
        largest = right;
    }
    if(largest != i){
        swap(&heap[i],&heap[largest]);
        //heapify subtree
        heapify(heap, heapSize, largest);
    }
}
void buildMaxHeap(int* heap, int heapSize){
    //call heapify on all non-leaf node
    for(int i=heapSize/2-1; i>=0; --i){
        heapify(heap, heapSize, i);
    }
}
void removeMax(int* heap, int* heapSize){
    if(*heapSize <=0 ) return;
    (*heapSize)--;
    heap[0] = heap[(*heapSize)];
    heapify(heap,*heapSize,0);
}
int findKthLargest(int* nums, int numsSize, int k) {
    int heapSize = numsSize;
    //build max heap of this array
    buildMaxHeap(nums, numsSize);
    //remove k-1 times the root
    for(int i=0; i<k-1; ++i){
        removeMax(nums, &heapSize);
    }

    return nums[0];
    //Space Complexity: O(log(n)); //recursive heapify stack
    //Time Complexity: O(n*log(n));
}
```
```c
//solution2: quick select
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *nums, int l, int r){
    int randomIndex = l + rand() % (r - l + 1); 
    int pivot = nums[randomIndex], i=l-1, j=r+1;

    while(i<j){
        do{
            i++;
        }while(nums[i]<pivot);
        do{
            j--;
        }while(nums[j]>pivot);
        if(i<j){
            swap(&nums[i],&nums[j]);
        }
    }

    return j;
}

int quickSelect(int *nums, int k, int l, int r){
    //termination
    if(l==r){
        return nums[k];
    }
    //partition
    int pivotIndex = partition(nums, l, r);
    //recursion
    if(pivotIndex>=k){
        return quickSelect(nums,k,l,pivotIndex);
    }
    return quickSelect(nums,k,pivotIndex+1,r);
}

int findKthLargest(int* nums, int numsSize, int k) {
    return quickSelect(nums, numsSize-k, 0, numsSize-1);
    //Space Complexity: O(log(n)); //recursive heapify stack
    //Time Complexity: O(n);
}
```
## priority queue: 2336. Smallest Number in Infinite Set
### Problem
You have a set S that contains all positive integers except the integers that have been removed. Initially, the set S contains all integers starting from 1, i.e., {1, 2, 3, 4, 5, ...}.

Implement the class SmallestInfiniteSet that supports:
- popSmallest(): Removes and returns the smallest element of the set.
- addBack(num): Adds a positive integer num back into the set if it is not already in the set.

The set S has the following properties:
- The smallest element in the set is always returned by popSmallest().
- If addBack(num) is called with a number that is already in the set, it will not change the set.
### Solution
```cpp
class SmallestInfiniteSet {
private:
    set<int> addedBackSet;
    int smallestInSet;
public:
    SmallestInfiniteSet() {
        smallestInSet = 1;
    }
    
    int popSmallest() {
        int val; 
        if(addedBackSet.empty()){
            val = smallestInSet;
            smallestInSet++;
        }else{
            val = *(addedBackSet.begin());
            addedBackSet.erase(val);
        }
        
        return val;
    }
    
    void addBack(int num) {
        if(num < smallestInSet){
           addedBackSet.insert(num);
        }
    }
    //Space Complexity: O(1);
    //Time Complexity: O(1);
};
```
## priority queue: 2542. Maximum Subsequence Score
### Problem
You are given two 0-indexed integer arrays nums1 and nums2, both of length n. You want to choose a subsequence of indices from 0 to n - 1.

A subsequence is a sequence that can be derived from another sequence by deleting some or no elements without changing the order of the remaining elements.
The score of a subsequence is calculated as follows:
- The subsequence must consist of elements nums1[i] and nums2[i] for any selected index i.
- The score is the sum of nums1[i] * nums2[i] for all the selected indices.
- Your task is to maximize the score by choosing the subsequence optimally.

Constraints:
- 1 <= nums1.length == nums2.length == n <= 10^5
- 1 <= nums1[i], nums2[i] <= 10^6
### Solution
```cpp
//solution: greedy algorithm
class Solution {
public:
    long long maxScore(vector<int>& nums1, vector<int>& nums2, int k) {
        int n = nums1.size();
        vector<pair<int,int>> nums;

        for(int i=0; i<n; ++i){
            nums.push_back({nums1[i],nums2[i]});
        }

        sort(nums.begin(),nums.end(), [](pair<int, int> &a, pair<int, int> &b){
            return a.second > b.second;
        });

        priority_queue<int, vector<int>, greater<>> pq;
        long long sum = 0;
        for(int i=0; i<k; ++i){
            pq.push(nums[i].first);
            sum += nums[i].first;
        }

        long long ans = sum * nums[k-1].second;

        for(int i=k; i<n; ++i){
            int pqMin = pq.top(); 
            if(nums[i].first>pqMin){
                pq.pop();
                pq.push(nums[i].first);
                sum = sum - pqMin + nums[i].first;
                long long product = sum * nums[i].second;
                if(product > ans){
                    ans = product;
                }
            }
        }

        return ans;
        //Space Complexity: O(n);
        //Time Complexity: O(n*log(n));
    }
};
```
## priority queue: 2462. Total Cost to Hire K Workers
### Problem
In this problem, you are given three inputs:
- costs: An array of integers where costs[i] represents the cost of hiring the i-th worker.
- k: The number of workers you need to hire.
- candidates: An integer that indicates how many workers you can consider from both ends of the costs array.

The goal is to hire exactly k workers with the lowest possible total cost. You can choose workers only from the first candidates workers and the last candidates workers (from the remaining workers, if any, after hiring from the extremes). The workers from the middle of the list cannot be hired until all workers from the extremes are exhausted.
### Solution
```cpp
class Solution {
public:
    long long totalCost(vector<int>& costs, int k, int candidates) {
        long long ans = 0;
        int n = costs.size();

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        int l=candidates-1, r = n-candidates;

        if(l+1<r){
            for(int i=0; i<=l; ++i) pq.push({costs[i],i});
            for(int i=r; i<n; ++i) pq.push({costs[i],i});
        }else{
            for(int i=0; i<n; ++i) pq.push({costs[i],i});
        }

        while(k > 0){
            pair<int, int> pqTop = pq.top();
            pq.pop();
            ans += pqTop.first;
            int index = pqTop.second;

            if(l+1<r){
                if(index<=l){
                    l++;
                    pq.push({costs[l],l});
                }
                if(index>=r){
                    r--;
                    pq.push({costs[r],r});
                }
            }

            k--;
        }

        return ans;
        //Space Complexity: O(candidates);
        //Time Complexity: O((candidates+k)*log(candidates));
    }
};
```
---
## binary search: 374. Guess Number Higher or Lower
### Problem
You are playing a guess game with a system. The system generates a random number between 1 and n. You need to guess the number, and the system will respond with one of three possibilities:
- If your guess is lower than the actual number, it will return -1.
- If your guess is higher than the actual number, it will return 1.
- If your guess is correct, it will return 0.

You must implement a function int guessNumber(int n) that returns the number you are guessing.
### Solution
```c
/** 
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is higher than the picked number
 *			      1 if num is lower than the picked number
 *               otherwise return 0
 * int guess(int num);
 */
int guess(int num);
int guessNumber(int n){
    int l=1, r=n;

    while(l<r){
        int mid = l+(r-l)/2;
        if(guess(mid)==1){
            l = mid+1;
        }else{
            r = mid;
        }
    }

    return l;
    //Space Complexity: O(1);
    //Time Complexity: O(log(n));
}
```
## binary search: 2300. Successful Pairs of Spells and Potions
### Problem
You are given two positive integer arrays spells and potions, where spells[i] represents the strength of the i-th spell and potions[j] represents the strength of the j-th potion. You are also given a positive integer success. A pair of spells and potions is considered successful if spells[i] * potions[j] >= success.

Return an integer array pairs of length n where pairs[i] is the number of potions that will form a successful pair with the i-th spell.
### Solution
```c
int compare(const void * a, const void * b){
    return (*(int *)a)-(*(int *)b);
}

int* successfulPairs(int* spells, int spellsSize, int* potions, int potionsSize, long long success, int* returnSize) {
    //initialization 
    *returnSize = spellsSize;
    int *ans = (int *)malloc(spellsSize*sizeof(int));
    if(ans == NULL){
        return NULL;
    }
    //sort potions array
    qsort(potions, potionsSize, sizeof(int), compare);
    //fill the answer array with binary search
    for(int i=0; i<spellsSize; ++i){
        int l=0, r=potionsSize; //pay attention to the index here
        long long target = (success+spells[i]-1)/spells[i];
        while(l<r){
            int mid = l+(r-l)/2;
            if(potions[mid]>=target){
                r = mid;
            }else{
                l = mid+1;
            }
        }
        ans[i] = potionsSize - l; //pay attention to the calculation here
    }
    
    return ans;
    //Space Complexity: O(spellsSize);
    //Time Complexity: O((potionsSize+pellsSize)*log(potionsSize));
}
```
## binary search: 162. Find Peak Element
### Problem
You are given an integer array nums consisting of n elements, where n > 1. An element is called a peak if it is strictly greater than its neighbors.

Formally, an element nums[i] is a peak if:
- nums[i] > nums[i - 1] (if i > 0)
- nums[i] > nums[i + 1] (if i < n - 1)

Your task is to find any peak element in the array and return its index. You may assume that nums[-1] = nums[n] = -∞.
### Solution
```c
int findPeakElement(int* nums, int numsSize) {
    int l=0, r=numsSize-1; //pay attention to the index here

    while(l<r){
        int mid = l+(r-l)/2;
        if(nums[mid] > nums[mid+1]){
            r = mid;
        }else{
            l = mid+1;
        }
    }

    return l;
    //Space Complexity: O(1);
    //Time Complexity: O(log(n));
}
```
## binary search: 875. Koko Eating Bananas
### Problem
Koko loves bananas. There are n piles of bananas, where the i-th pile has piles[i] bananas. Koko can decide her eating speed k (bananas per hour). Each hour, she chooses one pile and eats k bananas from it. If the pile has fewer than k bananas, she eats all of them instead. Koko wants to eat all the bananas within h hours.

You need to determine the minimum integer value of k such that she can eat all the bananas within h hours.
### Solution
```c
int hoursToFinish(int * piles, int pilesSize, int k){
    int hours = 0;
    for(int i=0; i<pilesSize; ++i){
        hours += (piles[i]+k-1)/k;
    }
    return hours;
}

int minEatingSpeed(int* piles, int pilesSize, int h) {
    int l=1, r=1;
    //calculate upper bound
    for(int i=0; i<pilesSize; ++i){
        if(piles[i]>r){
            r = piles[i];
        }
    }

    while(l<r){
        int mid = l+(r-l)/2;
        if(hoursToFinish(piles, pilesSize, mid)<=h){    
            r = mid;
        }else{
            l = mid+1;
        }
    }

    return l;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## backtracking: 17. Letter Combinations of a Phone Number
### Problem
Given a string containing digits from 2-9 inclusive, return all possible letter combinations that the number could represent. Return the answer in any order.

A mapping of digit to letters (just like on the telephone buttons) is given below:

2 -> "abc"
3 -> "def"
4 -> "ghi"
5 -> "jkl"
6 -> "mno"
7 -> "pqrs"
8 -> "tuv"
9 -> "wxyz"

### Solution
```cpp
class Solution {
private:
    unordered_map<int, string> buttons = {
        {2,"abc"},
        {3,"def"},
        {4,"ghi"},
        {5,"jkl"},
        {6,"mno"},
        {7,"pqrs"},
        {8,"tuv"},
        {9,"wxyz"},
    };

    string tmp;
public:
    void helper(int count, string digits, vector<string> & ans){
        if(count == digits.length()){
            ans.push_back(tmp);
            return;
        }

        for(char c: buttons[digits[count]-'0']){
            tmp.push_back(c);
            helper(count+1, digits, ans);
            tmp.pop_back();
        }
    }

    vector<string> letterCombinations(string digits) {
        vector<string> ans;

        helper(0,digits,ans);

        return ans;
        //Space Complexity: O(m+n); //the length of digits
        //Time Complexity: O((3^m)*(4^n));
    }   
};
```
## backtracking: 216. Combination Sum III
### Problem
Find all valid combinations of k numbers that sum up to n, where the numbers must be chosen from the range [1, 9] and each number can only be used once.

Return a list of all possible valid combinations.
### Solution
```cpp
class Solution {
public:
    void helper(int count, int k, int n, vector<vector<int>>& ans, vector<int> &tmp, int num){
        if(count == k){
            if(n == 0){
                ans.push_back(tmp);
            }
        }

        for(int i=num; i<=9; ++i){
            tmp.push_back(i);
            helper(count+1, k, n-i, ans, tmp, i+1);
            tmp.pop_back();
        }
    }

    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> ans;
        vector<int> tmp;

        helper(0,k,n,ans,tmp,1);

        return ans;
        //Space Complexity: O(k);
        //Time Complexity: O(C(9, k));
    }
};
```
---
## dynamic programming: 1137. N-th Tribonacci Number
### Problem
The Tribonacci sequence T(n) is defined as follows:
- T(0) = 0
- T(1) = 1
- T(2) = 1
- T(n) = T(n - 1) + T(n - 2) + T(n - 3) for n > 2

Given n, return the value of T(n).
### Solution
```c
int tribonacci(int n) {
    if(n == 0){
        return 0;
    }
    if(n < 3){
        return 1;
    }

    int t0 = 0, t1 = 1, t2 = 1, ans=0;

    for(int i=3; i<=n; ++i){
        ans = t0+t1+t2;
        t0 = t1;
        t1 = t2;
        t2 = ans;
    }

    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## dynamic programming: 746. Min Cost Climbing Stairs
### Problem
You are given an integer array cost where cost[i] is the cost of stepping on the i-th step. You can either start from the step with index 0 or 1, and you need to reach the top of the stairs, which is one step beyond the last step (i.e., the length of the array).

To reach the top, you can either:
- Climb one step to i + 1 with a cost of cost[i + 1]
- Climb two steps to i + 2 with a cost of cost[i + 2]

Your goal is to return the minimum cost to reach the top of the stairs.
### Solution
```c
int minCostClimbingStairs(int* cost, int costSize) {
    if(costSize<2){
        return 0;
    }
    //initialize dp array
    int dp[costSize+1];
    dp[0]=0;
    dp[1]=0;
    //dp equation
    for(int i=2; i<=costSize; ++i){
        dp[i]=fmin(dp[i-1]+cost[i-1], dp[i-2]+cost[i-2]);
    }

    return dp[costSize];
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## dynamic programming: 198. House Robber
### Problem
You are a skilled robber planning to rob houses along a street. Each house has a certain amount of money hidden, but you cannot rob two adjacent houses because they have security systems connected.

You need to determine the maximum amount of money you can rob tonight without alerting the police.
### Solution
```c
int rob(int* nums, int numsSize) {
    if(numsSize < 2){
        return nums[0];
    }

    int * dp = (int *)calloc(numsSize, sizeof(int));
    if(dp == NULL){
        return 0; 
    }

    dp[0] = nums[0];
    dp[1] = nums[0] < nums[1] ? nums[1]:nums[0];

    for(int i=2; i<numsSize; ++i){
        dp[i] = fmax(dp[i-1], dp[i-2]+nums[i]);
    }

    int ans = dp[numsSize-1];
    free(dp);
    return ans;
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## dynamic programming: 790. Domino and Tromino Tiling
### Problem
You are given a 2 x n board, and you need to cover it completely with the following tiles:
- Domino: A tile that covers 2 x 1 cells.
- Tromino: A tile that covers 2 x 2 cells (in a L shape) or can be viewed as three squares connected in an L-shape.

Your task is to compute the number of ways to fill the 2 x n board using these tiles. The answer should be returned modulo 10^9 + 7.
### Solution
```c
#define MOD 1000000007;

int numTilings(int n) {
    //initialization
    unsigned dp[4][n+1];
    dp[0][0] = 0;
    dp[1][0] = 0;
    dp[2][0] = 0;
    dp[3][0] = 1;
    //dynamic programming calculation
    for(int i=1; i<n+1; ++i){
        dp[0][i] = dp[3][i-1];
        dp[1][i] = (dp[0][i-1] + dp[2][i-1]) % MOD;
        dp[2][i] = (dp[0][i-1] + dp[1][i-1]) % MOD;
        dp[3][i] = (dp[0][i-1] + dp[1][i-1] + dp[2][i-1] + dp[3][i-1]) % MOD;
    }

    return dp[3][n];
    //space complexity: O(n);
    //time complexity: O(n);

}
```
## dynamic programming: 62. Unique Paths
### Problem
You are given an m x n grid (or matrix) representing a field of obstacles. You start at the top-left corner of the grid and want to reach the bottom-right corner. You can only move either down or right at any point in time. Count how many unique paths there are to reach the bottom-right corner.
### Solution
```c
int uniquePaths(int m, int n) {
    //grid initialization
    int ** grid = (int **)malloc(m*sizeof(int *));
    if(grid == NULL){
        return -1;
    }
    for(int i=0; i<m; ++i){
        grid[i] = (int *)malloc(n*sizeof(int));
        if(grid[i] == NULL){
            for(int j=0; j<i; ++j){
                free(grid[j])
            }
            free(grid);
            return -1;
        }
        //initialize values
        memset(grid[i],0,n*sizeof(int));
        grid[i][0] = 1;
    }
    for(int j=1; j<n; ++j){
        grid[0][j] = 1;
    }
    
    //paths calculation
    for(int i=1; i<m; ++i){
        for(int j=1; j<n; ++j){
            grid[i][j] = grid[i-1][j]+grid[i][j-1];
        }
    }
    //get answer
    int ans = grid[m-1][n-1];
    //free heap memory
    for(int i=0; i<m; ++i){
        free(grid[i]);
    }
    free(grid);
    //return answer
    return ans;
    //Space Complexity: O(m*n);
    //Time Complexity: O(m*n);
}
```
## dynamic programming: 1143. Longest Common Subsequence
### Problem
The Longest Common Subsequence (LCS) problem is a well-known problem in computer science and is commonly encountered in dynamic programming contexts. 

Given two strings text1 and text2, return the length of their longest common subsequence. A subsequence of a string is a new string that is formed from the original string by deleting some (can be none) of the characters without disturbing the relative positions of the remaining characters. (i.e., "ace" is a subsequence of "abcde" while "aec" is not).

If there is no common subsequence, return 0.
### Solution
```c
int longestCommonSubsequence(char* text1, char* text2) {
    //get size of sequences
    int m = strlen(text1), n = strlen(text2);

    //initialization
    int ** dp = (int **)calloc(m+1,sizeof(int *));
    if(dp == NULL){
        return -1;
    }
    for(int i=0; i<m+1; ++i){
        dp[i] = (int *)calloc(n+1, sizeof(int));
        if(dp[i] == NULL){
            for(int j=0; j<i; ++j){
                free(dp[j]);
            }
            free(dp);
            return -1;
        }
    }

    //dynamic programming check
    for(int i=1; i<m+1; ++i){
        for(int j=1; j<n+1; ++j){
            if(text1[i-1] == text2[j-1]){
                dp[i][j] = dp[i-1][j-1]+1;
            }else{
                dp[i][j] = fmax(dp[i-1][j],dp[i][j-1]);
            }
        }
    }

    int ans = dp[m][n];

    for(int i=0; i<m+1; ++i){
        free(dp[i]);
    }
    free(dp);

    return ans;
    //Space Complexity: O(m*n);
    //Time Complexity: O(m*n);
}
```
## dynamic programming: 714. Best Time to Buy and Sell Stock with Transaction Fee
### Problem
You are given an array prices where prices[i] is the price of a given stock on the i-th day, and an integer fee representing a transaction fee.

You want to maximize your profit by choosing a single transaction (buying one and selling one share of the stock) with the constraint that you have to pay the transaction fee when you sell the stock.

Return the maximum profit you can achieve.

Note that you cannot sell a stock before you buy one.
### Solution
```c
int maxProfit(int* prices, int pricesSize, int fee) {
    //initialization
    int dp[2][pricesSize];
    dp[0][0] = 0; //Maximum profit when not holding a stock
    dp[1][0] = -prices[0]; //Maximum profit when holding a stock (initially negative)

    //dynamic programming calculation
    for(int i=1; i<pricesSize; ++i){
        dp[0][i] = fmax(dp[0][i-1], dp[1][i-1]+prices[i]-fee); //sell 
        dp[1][i] = fmax(dp[1][i-1], dp[0][i-1]-prices[i]); //buy
    }

    int ans = dp[0][pricesSize-1]>dp[1][pricesSize-1] ? dp[0][pricesSize-1]:dp[1][pricesSize-1];

    return ans;
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## dynamic programming: 72. Edit Distance
### Problem
Given two strings word1 and word2, return the minimum number of operations required to convert word1 to word2. You can perform the following three operations:
- Insert a character.
- Delete a character.
- Replace a character.
### Solution
```c
int minDistance(char* word1, char* word2) {
    int m = strlen(word1), n = strlen(word2);
    //initialization
    int dp[m+1][n+1];
    for(int i=0; i<m+1; ++i){
        dp[i][0] = i; // If word2 is empty, delete all characters from word1
    }

    for(int j=0; j<n+1; ++j){
        dp[0][j] = j; // If word1 is empty, insert all characters of word2
    }

    for(int i=1; i<m+1; ++i){
        for(int j=1; j<n+1; ++j){
            if(word1[i-1] == word2[j-1]){ //pay attention to the index here
                dp[i][j] = dp[i-1][j-1];
            }else{
                dp[i][j] =  1 + (dp[i - 1][j] < dp[i][j - 1] ? 
                                (dp[i - 1][j] < dp[i - 1][j - 1] ? dp[i - 1][j] : dp[i - 1][j - 1]) : 
                                (dp[i][j - 1] < dp[i - 1][j - 1] ? dp[i][j - 1] : dp[i - 1][j - 1]));
            }
        }
    }

    return dp[m][n];
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
---
## bit operation: 338. Counting Bits
### Problem
Given an integer n, return an array ans of length n + 1 such that for each i (0 <= i <= n), ans[i] is the number of 1s in the binary representation of i.
### Solution
```c
int* countBits(int n, int* returnSize) {
    *returnSize = n+1;

    int * ans = (int *)calloc((*returnSize),sizeof(int));
    if(ans == NULL){
        return NULL;
    }

    for(int i=0; i<*returnSize; ++i){
        ans[i]=ans[i>>1]+(i&1);
    }

    return ans;
    //Space Complexity: O(n);
    //Time Complexity: O(n);
}
```
## bit operation: 136. Single Number I 
### Problem
Given a non-empty array of integers nums, every element appears twice except for one. Find that single one.

You must implement a solution with a linear runtime complexity and use only constant extra space.
### Solution
```c
int singleNumber(int* nums, int numsSize) {
    int ans=0;

    for(int i=0; i<numsSize; ++i){
        ans ^= nums[i];
    }

    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
## bit operation: 1318 Minimum Number of Flips of Or Operation
### Problem
You are given three integers a, b, and c. The goal is to calculate the minimum number of bit flips required to make the bitwise OR of a and b equal to c.

A bit flip means changing a 0 to a 1 or a 1 to a 0.
### Solution 
```c
int minFlips(int a, int b, int c) {
    int ans=0, aBit=0, bBit=0, cBit=0;
    //compare bit by bit, suppose 32 bit integer
    for(int i=0; i<32; ++i){
        aBit = (a>>i) & 1;
        bBit = (b>>i) & 1;
        cBit = (c>>i) & 1;
        if((aBit|bBit)!=cBit){
            if(aBit==1&&bBit==1&&cBit==0){
                ans=ans+2;
            }else{
                ans=ans+1;
            }
        }
    }

    return ans;
    //Space Complexity: O(1);
    //Time Complexity: O(n);
}
```
---
## prefix tree: 208. Implement Trie (Prefix Tree)
### Problem
Implement a trie with insert, search, and startsWith methods. Requirements
- insert: Inserts a word into the trie.
- search: Returns true if the word is in the trie (i.e., was inserted before).
- startsWith: Returns true if there is a previously inserted word that starts with the given prefix.
### Solution
```cpp
class Trie {
private:
    class TreeNode{
    public:
        unordered_map<char, TreeNode *> children;
        bool isEndOfWord;

        TreeNode(): isEndOfWord(false) {}
    };

    TreeNode * root;

public:
    Trie() {
        root = new Trie::TreeNode();
    }

    ~Trie(){
        deleteTrie(root);
    }
    
    void insert(string word) {
        TreeNode * node = root;

        for(char c: word){
            if(node->children.find(c)==node->children.end()){
                node->children[c] = new TreeNode();
            }   
            node = node->children[c];
        }

        node->isEndOfWord = true;
    }
    
    bool search(string word) {
        TreeNode * node = root;

        for(char c: word){
            if(node->children.find(c)!=node->children.end()){
                node = node->children[c];
            }else{
                return false;
            }
        }

        return node->isEndOfWord;
    }
    
    bool startsWith(string prefix) {
        TreeNode * node = root;

        for(char c: prefix){
            if(node->children.find(c)!=node->children.end()){
                node = node->children[c];
            }else{
                return false;
            }
        }

        return true;
    }

    void deleteTrie(TreeNode * node){
        for(auto &pair: node->children){
            delete(pair.second);
        }
        delete(node);
    }
    //space complexity: O(nL); //n is the number of words
    //time complexity: O(L); //L is the average length of words
};
```
## prefix tree: 1268. Search Suggestions System
### Problem
You are given an array of strings products and a string searchWord. You need to implement a function that returns a list of lists of suggestions where each list contains up to three products that begin with the current prefix:
- Sort the products array lexicographically.
- For each character added to the prefix, find all products that start with the prefix and return the first three.
### Solution
```cpp
class Solution {
private:
    const int maxSuggestions = 3;
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        //sort this array
        sort(products.begin(),products.end());

        //initialize variables
        vector<vector<string>> ans;
        string prefix;

        //loop each character in searchWord
        for(char c: searchWord){
            prefix += c;
            vector<string> suggestions;
            //binary search to find the first iterator that is not less than prefix
            auto lb = lower_bound(products.begin(), products.end(), prefix);
            //loop to find suggestions
            for(int i=0; i<maxSuggestions && lb+i!=products.end(); ++i){
                if((lb+i)->find(prefix)==0){
                    suggestions.emplace_back(*(lb+i));            
                }else{
                    break;
                }
            }
            ans.emplace_back(suggestions);
        }

        return ans;
        //space complexity: O(m); //n is the number of products, m is the length of searchWord
        //time complexity: O((n+m)log(n)); //
    }
};
```
---
## interval set: Non-overlapping Intervals
### Problem
### Solution
Given an array of intervals where intervals[i] = [start_i, end_i], remove the minimum number of intervals such that the remaining intervals do not overlap.

Return the minimum number of intervals you need to remove. Note that intervals which only touch at a point are non-overlapping.
```c
int compare(const void * a, const void * b){
    return (*(int ** )a)[1]-(*(int **) b)[1];
}

int eraseOverlapIntervals(int** intervals, int intervalsSize, int* intervalsColSize) {
    //check special case
    if(intervalsSize == 0) return 0;
    //sort the array based on the right end
    qsort(intervals, intervalsSize, sizeof(int *), compare);
    //greedy approach
    int ans = 0, right = intervals[0][1];
    for(int i=1; i<intervalsSize; ++i){
        if(right > intervals[i][0]){
            ans ++; 
        }else{
            right = intervals[i][1];
        }
    }

    return ans;
    //space complexity: O(1);
    //time complexity: O(nlog(n));
}
```
## interval set: Minimum Number of Arrows to Burst Balloons
### Problem
You are given a list of balloons, where each balloon is represented by an interval [start, end]. An arrow can burst all balloons that it passes through (i.e., the balloon’s interval contains the arrow's position).

The task is to find the minimum number of arrows required to burst all the balloons.
### Solution
- greedy: first sort the vectors based on the end of the interval, then shot. 
```c
int compare(const void * a, const void * b){
    if ((*(int **)a)[1] > (*(int **)b)[1]) return 1;
    if ((*(int **)a)[1] < (*(int **)b)[1]) return -1;
    return 0;
}

int findMinArrowShots(int** points, int pointsSize, int* pointsColSize) {
    if(pointsSize == 0) return 0;

    qsort(points, pointsSize, sizeof(int *), compare);

    int ans = 1, arrow = points[0][1];
    for(int i=1; i<pointsSize; ++i){
        if(arrow < points[i][0]){
            ans++;
            arrow = points[i][1];
        }
    }

    return ans;
    //space complexity: O(1);
    //time complexity: O(nlog(n));
}
```
---
## monotonic stack: 739. Daily Temperatures
### Problem
You are given an array temperatures where temperatures[i] is the temperature on the i-th day. You need to return an array answer such that answer[i] is the number of days you have to wait after the i-th day to get a warmer temperature. If there is no future day for which this is possible, keep answer[i] == 0.
### Solution
```cpp
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        vector<int> ans(n,0);

        stack<int> st;

        for(int i=0; i<n; ++i){
            while(!st.empty()){
                int topIdx = st.top();
                if(temperatures[i]>temperatures[topIdx]){
                    ans[topIdx]=i-topIdx;
                    st.pop();
                }else{
                    break;
                }
            }
            st.push(i);
        }

        return ans;
        //space complexity: O(n);
        //time complexity: O(n);
    }
};
```
## monotonic stack: 901. Online Stock Span
### Problem
Implement a class StockSpanner that calculates the span of stock prices for the current day. The stock span is defined as the number of consecutive days (including today) that the stock price has been less than or equal to today's price. You need to design the StockSpanner class with the following methods:
- StockSpanner(): Initializes the stock spanner object.
- next(int price): Returns the stock span of the stock price price today.
### Solution
```cpp
class StockSpanner {
private: 
    stack<pair<int, int>> st;
public:
    StockSpanner() {}
    
    int next(int price) {
        int span = 1;

        while(!st.empty()){
            auto topElement = st.top();
            int topPrice = topElement.first;
            int topSpan = topElement.second;
            if(price >= topPrice){
                span = span + topSpan;
                st.pop();
            }else{
                break;
            }
        }

        st.push({price, span});

        return span;
        //space complexity: O(n);
        //time complexity: O(n);
    }
};
```


