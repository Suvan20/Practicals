#include <iostream>
#include <chrono>
#include <omp.h>
#include <queue>
using namespace std;

int nodes;
int adj_mat[20][20];
int visited_s[20]; // for BFS
int visited_p[20]; // for BFS

void s_BFS(int s) // function for BFS traversal
{
    queue<int> q; // initiating queue
    q.push(s);
    visited_s[s] = 1; // marking source node as visited

    while (!q.empty())
    {
        int n = q.front();
        cout << n << " ";
        q.pop();

        for (int i = 0; i < nodes; i++)
        {
            if (adj_mat[n][i] && !visited_s[i])
            {
                visited_s[i] = 1;
                q.push(i);
            }
        }
    }
}

void p_BFS(int s) // function for BFS traversal
{
    queue<int> q; // initiating queue
    q.push(s);
    visited_p[s] = 1; // marking source node as visited

    while (!q.empty())
    {
        int n = q.front();
        q.pop();

        #pragma omp parallel for
        for (int i = 0; i < nodes; i++)
        {
            if (adj_mat[n][i] && !visited_p[i])
            {
                visited_p[i] = 1;
                q.push(i);
            }
        }
    }
}

int main()
{
    cout << "Enter the number of nodes: "; // taking user input for nodes
    cin >> nodes;

    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < nodes; j++)
        {
            char edge;
            cout << "\nIs there any route from " << i << " to " << j << " (Enter: y/n) : ";
            cin >> edge;

            if (edge == 'y')
            {
                adj_mat[i][j] = 1;
            }
            else if (edge == 'n')
            {
                adj_mat[i][j] = 0;
            }
        }
    }

    cout << "The BFS traversal for the graph is: ";
    
    auto start = chrono::high_resolution_clock::now();
    s_BFS(0);
    auto stop = chrono::high_resolution_clock::now();
    auto result = stop - start;
    auto duration = chrono::duration_cast<chrono::nanoseconds>(result);
    cout << "\nTime taken by sequential BFS: " << duration.count() << " nanoseconds";

    auto start2 = chrono::high_resolution_clock::now();
    #pragma omp parallel num_threads(4)
    {
        #pragma omp single
        {
            p_BFS(0);
        }
    }
    auto stop2 = chrono::high_resolution_clock::now();
    auto result2 = stop2 - start2;
    auto duration2 = chrono::duration_cast<chrono::nanoseconds>(result2);
    cout << "\nTime taken by parallel BFS: " << duration2.count() << " nanoseconds";

    return 0;
}

