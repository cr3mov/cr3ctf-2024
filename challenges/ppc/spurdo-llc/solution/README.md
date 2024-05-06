# Writeup


You can take the naive solution as a basis and optimize the search for the minimum element in it. We will maintain a directed set or priority queue, in which we will store all elements in decreasing order of values (and in case of equal values, in increasing order of indices). Put all elements into this structure, $k$ times take out the maximum element with the minimum index, decrease it by one, and insert it back into the structure. The time complexity of this solution is $O(k \log n)$.
Alternate solution:
Sort the array in descending order of elements, remembering the indices. We will consider all prefixes in increasing order. We need to find a prefix $p$ for which
$ \sum_{i=1}^{p} a_i - a_p + 1 * p \geq k $
then the elements in this prefix will be reduced during reductions, and all other elements will not. Knowing this, we can quickly calculate the values of the elements after reduction and return them to the original order. The time complexity of this solution will be $O(n \log n)$.

