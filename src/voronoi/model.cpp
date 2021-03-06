#include "model.h"
#include "libqhull_r/libqhull_r.h"
using namespace std;

Point3Df::Point3Df(float x, float y, float z) :
    x(x), y(y), z(z) {
}

Voronoi::Voronoi() {
    dim = 2;
    sites.clear();
    vertices.clear();
    cells.clear();
}

void Voronoi::print() {
    cout << "Sites:" << endl;
    for (int i = 0; i < sites.size(); ++i) {
        cout << sites[i].x << " " << sites[i].y << " " << sites[i].z << endl;
    }
    cout << "Vertices:" << endl;
    for (int i = 0; i < vertices.size(); ++i) {
        cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
    }
    cout << "Cells:" << endl;
    for (int i = 0; i < cells.size(); ++i) {
        for (int j = 0; j < cells[i].size(); ++j) {
            cout << cells[i][j] << " ";
        }
        cout << endl;
    }
}

void Voronoi::compute() {
    clearResults();

    /* Bounding box */
    generate_by_append(Point3Df(-INF, -INF));
    generate_by_append(Point3Df(-INF, INF));
    generate_by_append(Point3Df(INF, -INF));
    generate_by_append(Point3Df(INF, INF));


    int num_sites = sites.size();
    int dim = this->dim;
    char flags[] = "voronoi o";
    char hidden_flags[] = " ";
    coordT *points;
    boolT ismalloc = True;
    qhT qh_qh;
    qhT *qh= &qh_qh;

    QHULL_LIB_CHECK /* Check for compatible library */

    FILE* fp_in = fopen("inputs.txt", "w");
    FILE* fp_o = fopen("outputs.txt", "w");
    fprintf(fp_in, "%d\n%d\n", dim, num_sites);
    for (int i = 0; i < num_sites; ++i) {
        if (dim == 2) {
            fprintf(fp_in, "%f %f\n", sites[i].x, sites[i].y);
        }
        else {
            fprintf(fp_in, "%f %f %f\n", sites[i].x, sites[i].y, sites[i].z);
        }
    }
    fclose(fp_in);
    fp_in = fopen("inputs.txt", "r");
    qh_init_A(qh, fp_in, fp_o, stderr, 0, NULL);  /* sets qh->qhull_command */
    qh->NOerrexit = False;
    qh_option(qh, "voronoi  _bbound-last  _coplanar-keep", NULL, NULL);
    qh->DELAUNAY= True;     /* 'v'   */
    qh->VORONOI= True;
    qh->SCALElast= True;    /* 'Qbb' */
    qh_checkflags(qh, flags, hidden_flags);
    qh_initflags(qh, flags);
    points = qh_readpoints(qh, &num_sites, &dim, &ismalloc);
    qh_init_B(qh, points, num_sites, dim, ismalloc);
    qh_qhull(qh);
    qh_check_output(qh);
    qh_produce_output(qh);
    qh_freeqhull(qh, !qh_ALL);
    int curlong, totlong;
    qh_memfreeshort(qh, &curlong, &totlong);

    fclose(fp_o);
    fp_o = fopen("outputs.txt", "r");
    int num_vertices, num_cells, dummy_int;
    float x, y, z;
    fscanf(fp_o, "%d%d%d%d", &dummy_int, &num_vertices, &num_cells, &dummy_int);
    for (int i = 0; i < num_vertices; ++i) {
        if (this->dim == 2) {
            fscanf(fp_o, "%f%f", &x, &y);
            vertices.push_back(Point3Df(x, y));
        }
        else {
            fscanf(fp_o, "%f%f%f", &x, &y, &z);
            vertices.push_back(Point3Df(x, y, z));
        }
    }
    for (int i = 0; i < num_cells; ++i) {
        int num_v, id_v;
        fscanf(fp_o, "%d", &num_v);
        vector<int> r_vs; r_vs.clear();
        for (int j = 0; j < num_v; ++j) {
            fscanf(fp_o, "%d", &id_v);
            r_vs.push_back(id_v);
        }
        cells.push_back(r_vs);
    }
    updated = 1;
    return;
}

void Voronoi::generate_by_random(int num_sites, int dim, float bound) {
    sites.clear();
    for (int i = 0; i < num_sites; ++i) {
        Point3Df site;
        site.x = (float(rand()) / RAND_MAX - 0.5f) * bound;
        site.y = (float(rand()) / RAND_MAX - 0.5f) * bound;
        site.z = (dim == 2) ? 0.f : (float(rand()) / RAND_MAX * bound);
        sites.push_back(site);
    }
    this->dim = dim;
    updated = 0;
}

void Voronoi::generate_by_list(vector<Point3Df> input, int dim) {
    sites = input;
    this->dim = dim;
    updated = 0;
}

void Voronoi::generate_by_append(Point3Df p) {
    sites.push_back(p);
    updated = 0;
}

void Voronoi::generate_by_file(const char file_location[]) {
    sites.clear();
    FILE* fp_in = fopen(file_location, "r");
    int num_sites;
    fscanf(fp_in, "%d\n%d\n", &dim, &num_sites);
    float x, y, z;
    for (int i = 0; i < num_sites; ++i) {
        if (dim == 2) {
            fscanf(fp_in, "%f%f", &x, &y);
            sites.push_back(Point3Df(x, y, 0.0f));
        }
        else {
            fscanf(fp_in, "%f%f%f", &x, &y, &z);
            sites.push_back(Point3Df(x, y, z));
        }
    }
    fclose(fp_in);
    updated = 0;
}

void Voronoi::save_sites(const char file_location[])
{
    FILE* fp_in = fopen(file_location, "w");
    fprintf(fp_in, "%d\n%d\n", dim, sites.size());
    for (int i = 0; i < sites.size(); ++i) {
        if (dim == 2) {
            fprintf(fp_in, "%f %f\n", sites[i].x, sites[i].y);
        }
        else {
            fprintf(fp_in, "%f %f %f\n", sites[i].x, sites[i].y, sites[i].z);
        }
    }
    fclose(fp_in);
}

void Voronoi::clear() {
    sites.clear();
    vertices.clear();
    cells.clear();
    updated = 0;
}

void Voronoi::clearResults() {
    vertices.clear();
    cells.clear();
    updated = 0;
}
