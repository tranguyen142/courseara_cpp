#include <iostream>
#include <queue>
#include <vector>
#include <random>

using namespace std;
typedef pair<pair<int, int>, int> pd;

// Structure of the condition
// for sorting the pair by its
// second elements
struct compare_cost {
    bool operator()(

        pair<pair<int, int>, int> const& a,
        pair<pair<int, int>, int> const& b)
    {
        return a.second > b.second;
    }
};

class graph {
public:
    graph() {
        int **data = new int *[1];
        data[0] = new int[1];
        data[0][0] = 0;
        this->data = data;
        this->num_v = 1;
        print_graph();
    }

    // Constructor for graph with all distance 0
    graph(int num_v) {
        this->num_v = num_v;
        int **data = new int *[num_v];
        for (int i = 0; i < num_v; ++i) {
            data[i] = new int[num_v];
            for (int j = 0; j < num_v; ++j) {
                data[i][j] = 0;
            }
        }
        this->data = data;
        print_graph();
    }

    // Constructor for random graph
    graph(const int num_v, double density, int min_d, int max_d) {
        this->num_v = num_v;

        srand(time(0)); //seed rand()

        // 2d matrix to store edges
        int** data = new int *[num_v];
        for (int i = 0; i < num_v; ++i) {
            data[i] = new int[num_v];         //empty graph
        }

        for (int i = 0; i < num_v; ++i) {
            for (int j = 0; j < num_v; ++j) {
                if (i == j) data[i][j] = 0;
                else {
                    bool flag = prob() <= density*100;
                    if (flag) {
                        int val = rand()%max_d;
                        data[i][j] = data[j][i] = val;
                    } else  data[i][j] = data[j][i] = 0;

                }
            }
        }
        this->data = data;
    }

    double prob() {
        return rand()%100;

    }

    // return number of vertices in this graph
    int v() {
        return this->num_v;
    }

    // return number of edges in this graph
    int e() {
        int **data = this->data;
        int num_of_e = 0;
        for (int i = 0; i < this->num_v; ++i) {
            for (int j = 0; j < this->num_v; ++j) {
                if (data[i][j] > 0) {
                    num_of_e += 1;
                }
            }
        }
        return num_of_e/2;
    }

    // add edge between vertex n1 & n2
    void add(int n1, int n2, int cost) {
        if(n1 < num_v && n2 < num_v) {
            if(n1 >= 0 && n2 >= 0 && n1 != n2 && cost > 0) {
                this->data[n1][n2] = cost;
                this->data[n2][n1] = cost;
            }
        }
        this->num_e += 1;
    }

    // add to the queue
    //ref https://www.geeksforgeeks.org/priority-queue-of-pairs-in-c-with-ordering-by-first-and-second-element/?ref=rp
    void add_q(int from, int to, int cost, priority_queue<pd, vector<pd>, compare_cost> &q) {
        q.push(make_pair(make_pair(from,to), cost + this->data[from][to]));
    }

    // print priority queue
    void show_q(priority_queue<pd, vector<pd>, compare_cost> g) {
        while (!g.empty()) {
            cout << g.top().first.first << "-"<< g.top().first.second
                 << ": " << g.top().second
                << endl;
            g.pop();
        }
        cout << endl;
    }

    // print vector
    void print(vector<int> vec) {
        //cout << "size: " << vec.size() << endl;
        for ( int i = 0; i < vec.size(); ++i) {
            cout << vec[i] << ", " ;
        }
        cout << endl;
    }

    // print graph
    void print_graph() {
        cout << "      ";
        for (int i = 0; i < this->num_v; ++i)
            cout << i << "   |   ";
        cout << endl;
        for (int i = 0; i < this->num_v; ++i) {
            cout << i << "  |  " ;
            for (int j = 0; j < this->num_v; ++j) {
                cout << this->data[i][j] << "       ";
            }
            cout << endl;
        }
        cout << endl;
    }

    double shortest_path(int node) {
        int start = node;
        int *path = new int[num_v]; // to keep track of the distance
        bool *visited = new bool[num_v];
        int visited_size = 0;
        priority_queue<pd, vector<pd>, compare_cost> q; // priority queue
        int neighbor;
        int cost;
        int from, to;

        // initialize path & visited array
        for (int i = 0; i < num_v; ++i) {
            path[i] = 0;
            visited[i] = false;
        }

        vector<int> my_neighbors = find_neighbors(start, visited);

        if (my_neighbors.size() == 0) {
            cout << "node " << start << " is not connected" << endl;
            cout << "==============\n" << endl;

        }
        else { // this node is connected
        while (visited_size < num_v) {

            // not visited this node yet
            if (!visited[start]) {
                // find neighbors of this node
                vector<int> neighbors = find_neighbors(start, visited);

                // mark this vertex visited
                visited[start] = true;
                visited_size += 1;

                if ((neighbors.size() > 0) or (q.size() > 0)) {
                    for (int i = 0; i < neighbors.size(); ++i) {
                        neighbor = neighbors[i];
                        add_q(start, neighbor, 0, q);
                    }

                    from = q.top().first.first;
                    to = q.top().first.second;
                    cost = q.top().second + path[from];
                    if (path[to] == 0 || cost < path[to]) {
                        path[to] = cost;
                    }
                    q.pop();
                    start = to;
                }
            }
            else { // already visited before
                // find another node not visited yet
                for (int i = 0; i < num_v; ++i) {
                    if (!visited[i]) {
                        start = i;
                    }
                }
            }
        }
    }
        // print distances
        int sum = 0;

        for (int i = 0; i < num_v ; ++i) {
            sum += path[i];
        }
        return sum/num_v;
    }


    // find all neighbor vertices of v
    vector<int> find_neighbors(int v, bool* visited) {
        vector<int> neighbors;
        for (int i = 0; i < this->num_v; ++i) {
            if (this->data[v][i] > 0) {
                if (!visited[i]) {
                    neighbors.push_back(i);
                }
            }
        }
        return neighbors;
    }


private:
    int **data;   // adjacent matrix
    int num_v; // number of vertices
    int num_e; // number of edges
};



int main()
{
    int num_v = 50;
    double sum = 0;

    graph g1 = *(new graph(num_v, .2, 1, 10));

    cout << "This graph has " << g1.e() << " edges\n" << endl;
    cout << "Density: 0.2" << endl;


    for (int i = 0; i < num_v; ++i) {
        sum += g1.shortest_path(i);
    }
    cout << "Average path length: " << sum/num_v << endl;

}