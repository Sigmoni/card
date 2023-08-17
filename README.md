# The Cardinity Estimation Experiment Platform

## Updata

[Report of the newest NN solution](./python/report.md)

## Algorithms
### How to allocate buckets
The greedy algorithm always allocates bucket to the column that need more.
```cpp
// To allocate bits for each column
for (int i = 0; i < bits; i++) {
    if (dv1 * 1.0 / bkt1 > dv2 * 1.0 / bkt2) {
        alloc1++;
        bkt1 *= 2;
    } else {
        alloc2++;
        bkt2 *= 2;
    }
}
```
### How to do quantization
Use greedy algorithm which tends to make every bucket hold distinct values of a same number.
```cpp
// To build up a lookup table for bounds
for (int i = 1; i < val.size(); i++) {
    if (val[i] != val[i - 1]) {
        cnt++;
        if (std::trunc(cnt * 1.0 / capacity) != id) {
            id++;
            lookup.push_back(val[i]);
        }
    }
}
```
### The uniform assumption
We assume that the elements in a single bucket is uniformly distributed.
```cpp
// To quantize the query 
int id = FindID(lookup, query);
float width = (id == lookup.size() - 1) 
    ? max - lookup[id] 
    : lookup[id + 1] - lookup[id];

return id + (query - lookup[id]) / width;
```

Once we get the quantized query, we can make use of the prefix-sum matrix:
```cpp
// Any float number can be expressed as Q + R,
// where Q is the integer part and
// R is the remnant
int qx = std::trunc(x);
int qy = std::trunc(y);
float rx = x - qx;
float ry = y - qy;

float ans = 
    (1 - rx) * (1 - ry) * GetPrefixSum(prefix, qx, qy) +
    rx * (1 - ry) * GetPrefixSum(prefix, qx + 1, qy) +
    ry * (1 - rx) * GetPrefixSum(prefix, qx, qy + 1) +
    rx * ry * GetPrefixSum(prefix, qx + 1, qy + 1);
```
To avoid the edge effect when the query bounds get too close, we use a  hyper parameter ($\epsilon = 0.25$) to contorl it:
```cpp
ub_fst = (ub_fst - lb_fst > epsilon) ? ub_fst : lb_fst + epsilon;
```

## Build the project
This project is built via `CMake` and use `Vcpkg` for package management.
Please ensure you have installed `CMake` and `Vcpkg`.

Please install the necessary packages before you build the project:
```shell
vcpkg install rapidcsv
vcpkg install tabulate
```
## How to use
The executable takes only one parameter which is the dataset (in `.csv` format). 

For example:
```shell
./card ../data/household_power_consumption.csv
```

## Datasets
Please download from [UCI](http://archive.ics.uci.edu/dataset/235/individual+household+electric+power+consumption)

## Results
The experiment report can be seen [here](./results/report1.md).