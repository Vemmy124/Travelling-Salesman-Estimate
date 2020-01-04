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
