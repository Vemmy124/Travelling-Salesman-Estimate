# Travelling-Salesman-Estimate
### Description
This code estimates the hamiltonial cycle on the Earth coordinates (lon, lat), so it uses spherical distance between points. Firstly, the Delaunay triangulation is built (taken from my [post on habr](https://habr.com/ru/post/445048/)). Then we build a minimal spanning tree on the abovementioned triangulation via Kruskal algorithm and finally obtain the estimate for the hamiltonial cycle with DFS on a tree. That gives us a 2x weight approximation in O(nlogn) complexity.
### Build
```
git clone ... TSE
cd TSE/Travelling-Salesman
mkdir build
cd build
cmake ..
make Travelling_salesman
(put input.txt as shown later)
./Travelling_salesman
```

Running tests from build directory (gtest must be installed):
```
make Test
./Test
```
(Test 003 includes the geolocation of all russian cities)
### Input format
Sequence of coordinate pairs (lon, lat) in input.txt file located nearby the executable

Example:
```
44.8783715 39.190172
44.6098268 40.1006527
51.9582681 85.9602957
52.4920914 82.7794145
53.3481145 83.7798362
51.9960534 84.9840343
```

### Output format
The length of the hamiltonial path in meters in stdout

Example:
```
97766.7 is the estimated hamiltonial path length
```

### Correctness
##### Claim: Euclidean minimal spanning tree is a subgraph of the Delaunay triangulation
Proof: we use a global criterion for an edge in the Delaunay triangulation: DT contains the edge iff there exists a circle with the edge as a chord, which doesn't contain any other point from the initial set. Now, suppose the edge, connecting pi and pj, is in EMST, but not in DT. Then there is a point pk in the circle built on pi, pj as a diameter. Remove the edge pi, pj, then we have 2 components in MST. Connecting them by pi, pk or pj, pk we obtain spanning tree with the lower weight. Contradiction.

##### Claim: The hamiltonial cycle weight is lower than 2x MST weight
Proof: The sequence, achieved in DFS on MST is a hamiltonial cycle. Every edge in DFS is visited 2 times, so 2x MST weight is an upper bound for a minimal hamiltonial cycle weight.

##### Remarks
1) Actually, the earth surface is not a Euclidean space, but we suppose that the spanning tree, obtained by DT built on the points like they're on the plane, is a good approximation for MST.
2) After we get a sequence of points in DFS, we connect them directly which, of course, lowers the estimated weight.

### Results
Here I present precisions, achieved in tests with ~10 points from testcase

##### Uniform Distribution
```
Results (presicion in comparison to the best possible value):
1.31533497, 1.16205201, 1.33534821, 1. , 1.1853046,  1.10410965, 1.17274662, 1.08610774, 1.18575894
Mean: 1.17186
Standard deviation: 0.0995
Maximum value: 1.33534
```

##### Normal distribution
```
Results (presicion in comparison to the best possible value):
1.26893405, 1.05213293, 1.20544529, 1.19, 1.17527988, 1.11597565, 1.11880122, 1.23227422, 1.08819433
Mean: 1.16078
Standard deviation: 0.06725
Maximum value: 1.26893
```

##### Results on the real data
```
Estimated hamiltonial path for the cities graphs:
1112 Russian cities: 97766.7km
366 German cities: 12635.28km;
28890 USA cities (extended): 447245.23km;
78 Japanese cities: 7395.43km
```
