#include <iostream>
#include "vector"
#include "limits"
#include "fstream"
#include "string"

using namespace std;


int n;
vector <vector<int>> M;      // Матрица эффективности M[разраб][задача]
vector<int> xy, yx;             // Паросочетания: xy[разраб], yx[задача]
vector<bool> vertexX, vertexY;            // Альтернирующее дерево vx[разраб], vy[задача]
vector<int> skill, understanding;// Способности, изученность

vector<string> worker; //имена сотрудников
vector<string> tasking;//задачи


void add_info() {
    ifstream inf("Worker_and_task");
    inf>>n;
    for (int i = 0; i < n; ++i) {
        string str;
        inf>>str;
        tasking.push_back(str);
    }

    M.resize(n);
    for (int i = 0; i < n; ++i) {
        string worker_name;
        inf>>worker_name;
        worker.push_back(worker_name);
        M[i].resize(n);
        for (int j = 0; j < n; ++j) {
            inf>>M[i][j];
        }
    }
}



bool findMaxMatching (int i) {
    if (vertexX[i]) {
        return false;
    }

    vertexX[i] = true;
    for (int j = 0; j < n; ++j) {
        if ((M[i][j] - (skill[i] + understanding[j])) == 0) {
            vertexY[j] = true;
        }
    }
    for (int j = 0; j < n; ++j) {
        if (M[i][j] - (skill[i] + understanding[j]) == 0 && yx[j] == -1) {
            xy[i] = j;
            yx[j] = i;
            return true;
        }
    }
    for (int j = 0; j < n; ++j) {
        if (M[i][j] - skill[i] - understanding[j] == 0 && findMaxMatching(yx[j])) {
            xy[i] = j;
            yx[j] = i;
            return true;
        }
    }
    return false;
}

int main() {
    add_info();

    understanding.assign(n, 0);
    skill.assign(n, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            skill[i] = max(skill[i], M[i][j]);
        }
    }
    xy.assign (n, -1);
    yx.assign (n, -1);


    for (int c = 0; c < n; ) {
        vertexX.assign (n, false);
        vertexY.assign (n, false);
        int k = 0;
        for (int i = 0; i < n; ++i)
            if (xy[i] == -1 && findMaxMatching(i)) ++k;
        c += k;
        if (k == 0) {
            int z = numeric_limits<int>::max();
            for (int i = 0; i < n; ++i)
                if (vertexX[i]) {
                    for (int j = 0; j < n; ++j) {
                        if (!vertexY[j]) {
                            z = min(z, skill[i] + understanding[j] - M[i][j]);
                        }
                    }
                }
            for (int i = 0; i < n; ++i) {
                if (vertexX[i]) skill[i] -= z;
                if (vertexY[i]) understanding[i] += z;
            }
        }
    }

    int ans = 0;
    for (int i = 0; i < n; ++i) {
        ans += M[i][xy[i]];
    }
    ofstream fout("Result");

    for (int i = 0; i < n; ++i) {
        fout<< "Task for worker "<<worker[i]<<" "<<tasking[xy[i]]<<"\n";
        cout<<"Task for worker "<<worker[i]<<" "<<tasking[xy[i]]<<"\n";
    }

}
