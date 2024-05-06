# Writeup

To pass the first group of tests, it is sufficient to implement the process described in the problem statement. Since at each step at least one is added, the total number of steps will not exceed $Y − X$.

For $B =1; W = 1$, the level rises by $1 + P + Q$ each day, so the answer will be the rounded up value
$ \frac{Y − X}{1 + P + Q}  $

For $B = W$, the problem is essentially one-dimensional. At the very beginning, the level will
increase by 1 each day until it reaches $Y$ or a value divisible by $W$. Then if
$1 + P + Q$ is divisible by $W$, then all subsequent days the level will increase by $1 + P + Q$. If not
so, then there will be repeated $1 + W − (1 + P + Q) \mod W$ days further, where on the very first day the level
increases by $(1 + P + Q)$, and on all subsequent days — by 1.
To avoid fully outputting the mathematical solution, one can compute only the number of such periods that give slightly less than $Y − X$ levels, and calculate the remaining days and levels
in naive simulation mode of the process (group 1 solution).

Let $LCM(B; W)$ be the least common multiple of $B$ and $W$, then if we take
some $X$ and add $LCM(B; W)$ to it, the remainders of division by $B$ and $W$ will not change. And
since the further path and the associated level increases depend only on these remainders,
let's say that these two states are identical in terms of process development. The next step (and all subsequent steps) of each number $X$ and each pair of remainders ($X \mod W$; $X \mod B$)
are deterministic. Therefore, let's call all such different pairs of states. From each state
there will be exactly one transition to another (or to the same) state. This can be considered as a functional graph — a graph with one outgoing edge for each vertex. To each edge of the graph
assign a weight — the level increase it gives.

Now our task is to find the minimum length of the path in the weighted functional graph from
a given vertex ($X \mod W$; $X \mod B$), such that the total weight of the path is greater than or equal to $Y − X$. Let $N$ be the size of the graph — the number of states. A path of arbitrary length from any vertex

in the functional graph consists of three (potentially empty) parts:
1. A segment leading into a cycle — this part does not repeat and can be traversed by naive iterations in $O(N)$.
2. Some number of iterations of the cycle — a looped sequence of states. It can be detected when naive iterations first lead to a visited vertex, after
which the cycle itself can be identified in $O(N)$ and its total contribution to the level calculated, and then
the number of full cycles passed in our path.
3. The remaining part of the cycle — the part that did not enter a full iteration of the cycle, it also
will not repeat and can be naively traversed in $O(N)$ time.
Thus, a path of arbitrary length can be traversed in $O(N)$ time = $O(B \cdot W)$.

Note that if our current state $X$ is not divisible by either $W$ or $B$, then the transition
from this state ($X \mod W$; $X \mod B$) will be easily predictable to the state ($(X + 1) \mod W$; $(X + 1) \mod B$).
In this case, if the new value $X + 1$ is also not divisible by either $W$ or $B$, then we will continue to $X + 2$. Thus, it is possible to quickly understand in which state, which is divisible by at least one of the numbers, we will end up and in how many moves: \
$d = min(W − W \mod X; B − B \mod X)$ \
thus we can compress all chains of states inside which events that add $P$ or $Q$ do not occur,
and thus we can obtain a condensed functional graph, in which there will be already $O(B + W)$ states.
Solving the problem on such a graph can be done similarly to the previous one, however, it is necessary to additionally
consider that the starting and ending vertices may be absent in this condensed graph, i.e.,
lie inside the edge. However, additionally considering transitions across two edges by a naive
algorithm will still cost $O(min(B; W))$ operations.
The time complexity of this solution will be $O(B + W)$ operations.
