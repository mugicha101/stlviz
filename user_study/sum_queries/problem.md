Given an array of integers `input` and a set of range queries stored as `queries`,
output the results of each query into `output` such that `output[i]` is the sum of integers from index `queries[i].first` to `queries[i].second` inclusive.

Input: [ 92, 89, 74, 100, 61, 70, -63, 70, -75, 75 ]
Queries: [ [ 0, 2 ], [ 7, 9 ], [ 4, 4 ] ]
Expected Output: [ 255, 70, 61 ]