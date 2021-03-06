#ifndef MODEL_H
#define MODEL_H

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <vector>

#include "libqhull_r/libqhull_r.h"
using namespace std;

#if __cplusplus
extern "C" {
  int isatty(int);
}
#endif

#define INF 1e6

class Point3Df {
public:
    float x;
    float y;
    float z;
    Point3Df(float x = 0.0f, float y = 0.0f, float z = 0.0f);
};

class Voronoi {
public:
    Voronoi();
    vector<Point3Df> sites;
    vector<Point3Df> vertices;
    vector<vector<int> > cells;
    int dim;
    int updated = 0;

    void print();
    void compute();
    void generate_by_random(int num_sites, int dim, float bound = 500.0f);
    void generate_by_append(Point3Df p);
    void generate_by_list(vector<Point3Df> input, int dim);
    void generate_by_file(const char file_location[]);
    void save_sites(const char file_location[]);
    void clear();
    void clearResults();
};

#endif // MODEL_H
