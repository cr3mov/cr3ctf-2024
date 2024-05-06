# Writeup

To solve the first group of tests, you can implement any algorithm. Let's iterate over
$s$ — this is the sum we want to obtain. Check if it is a full square,
this can be done by simply iterating through all numbers from 1 to $s$ and checking if the square of such
a number equals $s$. Here's an example in C++:
```cpp
for(int s = 1; s <= 100; s += 1) {
    bool is_full_square = false;
    for(int x = 1; x <= s; x += 1) {
        if(x * x == s) {
            is_full_square = true;
        }
    }
}
```

The value of $s$ is also iterated through a for loop. Now you can iterate over two integers from
$l$ to $r$ using a for loop and check if their sum equals $s$.
You can iterate over the values of the required sum directly by iterating over two integers $x; y$ from $l$ to $r$ using two nested loops. Then, somehow check if $x + y$ is a full square. For example, by binary search.
To further speed up the solution, let's look at the pair $(x; y)$ such that $x + y$ is some full
square. If $(x; y)$ is a solution, then we can obtain the solution $(x − 1; y + 1)$. This is true because
the sum hasn't changed. This can be done until one of the numbers $x$ or $y$ hits one of the
boundaries $l$ or $r$, then the numbers can't be moved further. Therefore, let's fix $x = l$ and
iterate over $y$. Then fix $y = r$ and iterate over $x$.
For the complete solution, there's a bit left. Let's notice the obvious fact that by choosing a pair
$(x; y)$ we can obtain any sum from the range $[2l + 1; 2r − 1]$. Then, let's find any full square in this range, let it be $s$. Now, we can proceed as follows:
Fix $x = l$ and $y = s − x$, check if such a pair of numbers fits the answer. Similarly, fix $y = r; x = s − y$.
