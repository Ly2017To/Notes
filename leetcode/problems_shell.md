# Solution of LeetCode Shell Problems

## 192. Word Frequency
### Problem
You are given a text file words.txt that contains several lines of words. Your goal is to count the frequency of each word and return the words in descending order of their frequency.
- Input Format: The input consists of multiple lines, with each line possibly containing one or more space-separated words.
- Output Format: Print out the top k words along with their frequencies, formatted as word frequency.
### Solution
```bash
# Read from the file words.txt and output the word frequency list to stdout.
cat words.txt | tr ' ' '\n' | grep -v '^$' | sort | uniq -c | sort -nr | awk '{print $2, $1}'
```

## 193. Valid Phone Numbers
### Problem
A valid phone number must must be in the format of either:
(xxx) xxx-xxxx
xxx-xxx-xxxx
Your task is to write a bash command that filters out the valid phone numbers from a file called file.txt.
### Solution
```bash
# Read from the file file.txt and output all valid phone numbers to stdout.
grep -P '^\(\d{3}\) \d{3}-\d{4}$|^\d{3}-\d{3}-\d{4}$' file.txt
```

## 194. Transpose File
### Problem
You are given a file with multiple lines of text, where each line contains space-separated integers. Your task is to write a script that transposes the contents of the file. In other words, the first row of the original file becomes the first column of the transposed file, the second row becomes the second column, and so on.
### Solution
```bash
# Read from the file file.txt and print its transposed content to stdout.
awk '
{
    for(i=1;i<=NF;i++){
        arr[i][NR]=$i
    }
}
NF>0 {
    maxNF=NF
}
END{
    for(i=1;i<=maxNF;i++){
        for(j=1;j<=NR;j++){
            printf "%s", arr[i][j]
            if(j<NR) printf " "
        }
        print ""
    }
}' file.txt
```

## 195. Tenth Line
### Problem
Given a text file file.txt, print just the 10th line of the file.
### Solution
```bash
sed -n '10p' file.txt
# awk 'NR==10' file.txt
```