#include "libqhull_r/libqhull_r.h"

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <vector>
using namespace std;

#if __cplusplus
extern "C" {
  int isatty(int);
}
#endif

typedef struct {
    float x;
    float y;
    float z;
} Point3Df;

typedef struct {
    vector<Point3Df> vertices;
    vector<vector<int> > regions;
} Results;

void debug_results(Results res) {
    for (int i = 0; i < res.vertices.size(); ++i) {
        cout << res.vertices[i].x << " " << res.vertices[i].y << " " << res.vertices[i].z << endl;
    }
    for (int i = 0; i < res.regions.size(); ++i) {
        for (int j = 0; j < res.regions[i].size(); ++j) {
            cout << res.regions[i][j] << " ";
        }
        cout << endl;
    }
}

Results compute_voronoi(vector<Point3Df> input, int origin_dim, int argc, char** argv) {
    Results res;
    res.regions.clear();
    res.vertices.clear();

    if (origin_dim != 2 && origin_dim != 3)
        return res;

    int numpoints =input.size();
    int dim = origin_dim;
    char flags[] = "voronoi o";
    char hidden_flags[] = " ";
    coordT *points;
    boolT ismalloc = True;
    qhT qh_qh;
    qhT *qh= &qh_qh;

    QHULL_LIB_CHECK /* Check for compatible library */

    FILE* fp_in = fopen("inputs.txt", "w");
    FILE* fp_o = fopen("outputs.txt", "w");
    fprintf(fp_in, "%d\n%d\n", dim, numpoints);
    for (int i = 0; i < numpoints; ++i) {
        if (dim == 2) {
            fprintf(fp_in, "%f %f\n", input[i].x, input[i].y);
        }
        else {
            fprintf(fp_in, "%f %f %f\n", input[i].x, input[i].y, input[i].z);
        }
    }
    fclose(fp_in);
    fp_in = fopen("inputs.txt", "r");
    qh_init_A(qh, fp_in, fp_o, stderr, argc, argv);  /* sets qh->qhull_command */
    qh->NOerrexit = False;
    qh_option(qh, "voronoi  _bbound-last  _coplanar-keep", NULL, NULL);
    qh->DELAUNAY= True;     /* 'v'   */
    qh->VORONOI= True;
    qh->SCALElast= True;    /* 'Qbb' */
    qh_checkflags(qh, flags, hidden_flags);
    qh_initflags(qh, flags);
    points= qh_readpoints(qh, &numpoints, &dim, &ismalloc);
    qh_init_B(qh, points, numpoints, dim, ismalloc);
    qh_qhull(qh);
    qh_check_output(qh);
    qh_produce_output(qh);
    qh_freeqhull(qh, !qh_ALL);
    int curlong, totlong;
    qh_memfreeshort(qh, &curlong, &totlong);

    fclose(fp_o);
    fp_o = fopen("outputs.txt", "r");
    int num_vertices, num_regions, dummy;
    float x, y, z;
    fscanf(fp_o, "%d%d%d%d", &dummy, &num_vertices, &num_regions, &dummy);
    for (int i = 0; i < num_vertices; ++i) {
        if (origin_dim == 2) {
            fscanf(fp_o, "%f%f", &x, &y);
            Point3Df p;
            p.x = x;
            p.y = y;
            p.z = 0;
            res.vertices.push_back(p);
        }
        else if (origin_dim == 3) {
            fscanf(fp_o, "%f%f%f", &x, &y, &z);
            Point3Df p;
            p.x = x;
            p.y = y;
            p.z = z;
            res.vertices.push_back(p);
        }
    }
    for (int i = 0; i < num_regions; ++i) {
        int num_v, id_v;
        fscanf(fp_o, "%d", &num_v);
        vector<int> r_vs; r_vs.clear();
        for (int j = 0; j < num_v; ++j) {
            fscanf(fp_o, "%d", &id_v);
            r_vs.push_back(id_v);
        }
        res.regions.push_back(r_vs);
    }
    return res;
}

int main(int argc, char *argv[]) {
  int numpoints = 4;
  int dim = 2;
  float points[] = {2,0, -2,0, 0,1, 0,-1};

  vector<Point3Df> input;
  for (int i = 0; i < numpoints; ++i) {
      Point3Df p;
      p.x = points[i * dim];
      p.y = points[i * dim + 1];
      input.push_back(p);
  }

  auto res = compute_voronoi(input, 2, argc, argv);
  debug_results(res);

  return 0;
} /* main */

