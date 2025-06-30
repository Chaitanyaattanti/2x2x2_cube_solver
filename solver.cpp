#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iostream>
using namespace std;

// Represents a cube's corner configuration (position + orientation)
struct State {
    int val[7];
};

// Encoded form: permutation index + orientation value
struct Encoded {
    int perm, orient;
};

// Each face has 4 stickers; total 6 faces
struct Cube {
    char face[6][4];
};

// Holds all valid color combinations (21 triplets)
struct Triplets {
    string t[21];
};

// Factorial function
int fact(int n) {
    int f = 1;
    for (int i = 2; i <= n; ++i) f *= i;
    return f;
}

// Build 21 valid corner triplets from front stickers of cube
Triplets buildTriplets(const Cube &c) {
    Triplets tripletSet;
    char colors[6] = {c.face[0][0], c.face[1][0], c.face[2][0], c.face[3][0], c.face[4][0], c.face[5][0]};

    int combos[21][3] = {
        {0,3,4},{4,0,3},{3,4,0},
        {0,4,1},{1,0,4},{4,1,0},
        {0,5,3},{3,0,5},{5,3,0},
        {0,1,5},{5,0,1},{1,5,0},
        {2,4,3},{3,2,4},{4,3,2},
        {2,1,4},{4,2,1},{1,4,2},
        {2,5,1},{1,2,5},{5,1,2}
    };

    for (int i = 0; i < 21; i++) {
        string s;
        for (int j = 0; j < 3; j++) {
            s.push_back(colors[combos[i][j]]);
        }
        tripletSet.t[i] = s;
    }
    return tripletSet;
}

// Get State from cube colors using triplet lookup
State getState(const Cube& cube, const Triplets& tripletSet) {
    State state;
    string s;

    // Each corner has a unique triplet of stickers
    vector<tuple<int, int, int, int, int, int>> corners = {
        {0,0, 3,1, 4,2}, {0,1, 4,3, 1,0}, {0,2, 5,0, 3,3},
        {0,3, 1,2, 5,1}, {2,0, 4,0, 3,0}, {2,1, 1,1, 4,1}, {2,3, 5,3, 1,3}
    };

    for (int i = 0; i < 7; i++) {
        s.clear();
        s.push_back(cube.face[get<0>(corners[i])][get<1>(corners[i])]);
        s.push_back(cube.face[get<2>(corners[i])][get<3>(corners[i])]);
        s.push_back(cube.face[get<4>(corners[i])][get<5>(corners[i])]);

        for (int j = 0; j < 21; j++) {
            if (s == tripletSet.t[j]) {
                state.val[i] = (i * 3) + (j % 3);
                break;
            }
        }
    }
    return state;
}

// Get opposite color
char getOpposite(char color) {
    if (color == 'r') return 'o';
    if (color == 'o') return 'r';
    if (color == 'w') return 'y';
    if (color == 'y') return 'w';
    if (color == 'b') return 'g';
    if (color == 'g') return 'b';
    return ' ';
}

// Compute destination cube by applying opposite colors
Cube getTargetCube(const Cube& cube) {
    Cube target;
    for (int i = 0; i < 4; i++) {
        target.face[2][i] = cube.face[2][2];
        target.face[3][i] = cube.face[3][2];
        target.face[5][i] = cube.face[5][2];
        target.face[0][i] = getOpposite(cube.face[2][2]);
        target.face[1][i] = getOpposite(cube.face[3][2]);
        target.face[4][i] = getOpposite(cube.face[5][2]);
    }
    return target;
}

// Read cube input and normalize red/orange
void readCube(Cube& cube) {
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 4; ++j) {
            scanf(" %c", &cube.face[i][j]);
            if (cube.face[i][j] == 'r') cube.face[i][j] = 'o';
            else if (cube.face[i][j] == 'o') cube.face[i][j] = 'r';
        }
}

// Display a cube's face configuration
void printCube(const Cube& cube) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j)
            printf("%c ", cube.face[i][j]);
        printf("\n");
    }
}

// Display a state's encoded values
void printState(const State& s) {
    for (int i = 0; i < 7; ++i)
        printf("%3d", s.val[i]);
    printf("\n");
}

// Define all possible moves
enum Move { R=1, U=2, F=3, R2=4, U2=5, F2=6, R1=7, U1=8, F1=9 };

// Convert State to unique integer (base-21)
int hashState(const State& s) {
    int result = 0;
    for (int i = 0; i < 7; ++i)
        result = result * 21 + s.val[i];
    return result;
}

// Encode a state to permutation and orientation
Encoded encode(const State& s) {
    Encoded enc;
    int perm = 0, orient = 0;
    int base[7];
    for (int i = 0; i < 7; ++i) {
        base[i] = s.val[i] / 3 + 1;
        orient += (s.val[i] % 3) * pow(3, i);
    }

    int seen[8] = {};
    int t = 6;
    for (int i = 0; i < 7; ++i) {
        int v = base[i], less = 0;
        for (int j = 1; j < v; ++j) if (!seen[j]) less++;
        perm += less * fact(t--);
        seen[v] = 1;
    }

    enc.perm = perm;
    enc.orient = orient;
    return enc;
}

// Decode integer to State (inverse of hashState)
State unhash(int h) {
    State s;
    for (int i = 6; i >= 0; --i) {
        s.val[i] = h % 21;
        h /= 21;
    }
    return s;
}

// Rotate front/right/up
State rotateFront(const State& s) {
    int map[21] = {3,4,5,9,10,11,0,1,2,6,7,8,12,13,14,15,16,17,18,19,20};
    State r;
    for (int i = 0; i < 7; ++i) r.val[i] = map[s.val[i]];
    return r;
}
State rotateRight(const State& s) {
    int map[21] = {0,1,2,17,15,16,6,7,8,4,5,3,12,13,14,19,20,18,11,9,10};
    State r;
    for (int i = 0; i < 7; ++i) r.val[i] = map[s.val[i]];
    return r;
}
State rotateUp(const State& s) {
    int map[21] = {14,12,13,1,2,0,6,7,8,9,10,11,16,17,15,5,3,4,18,19,20};
    State r;
    for (int i = 0; i < 7; ++i) r.val[i] = map[s.val[i]];
    return r;
}

// Max size for visited array
#define MAX_PERM 5050
#define MAX_ORIENT 2190

// BFS to find shortest move sequence from start to target
vector<int> bfs(const State& start, const State& target) {
    vector<int> result;
    int visited[MAX_PERM][MAX_ORIENT] = {};
    int parent[MAX_PERM][MAX_ORIENT];

    queue<int> q;
    int startHash = hashState(start);
    int targetHash = hashState(target);

    Encoded startEnc = encode(start);
    visited[startEnc.perm][startEnc.orient] = U;
    q.push(startHash);

    while (!q.empty()) {
        int curHash = q.front(); q.pop();
        if (curHash == targetHash) {
            while (curHash != startHash) {
                State cur = unhash(curHash);
                Encoded e = encode(cur);
                result.push_back(visited[e.perm][e.orient]);
                curHash = parent[e.perm][e.orient];
            }
            reverse(result.begin(), result.end());
            return result;
        }

        State now = unhash(curHash);
        vector<pair<State, int>> nextMoves = {
            {rotateFront(now), F}, {rotateFront(rotateFront(now)), F1}, {rotateFront(rotateFront(rotateFront(now))), F2},
            {rotateRight(now), R}, {rotateRight(rotateRight(now)), R2}, {rotateRight(rotateRight(rotateRight(now))), R1},
            {rotateUp(now), U}, {rotateUp(rotateUp(now)), U2}, {rotateUp(rotateUp(rotateUp(now))), U1}
        };

        for (auto& [next, move] : nextMoves) {
            Encoded e = encode(next);
            if (!visited[e.perm][e.orient]) {
                visited[e.perm][e.orient] = move;
                parent[e.perm][e.orient] = curHash;
                q.push(hashState(next));
            }
        }
    }

    printf("Not possible\n");
    return {};
}

// Print move list
void printMoves(const vector<int>& moves) {
    for (int m : moves) {
        switch (m) {
            case R:  printf("R ");  break;
            case R1: printf("R' "); break;
            case R2: printf("R2 "); break;
            case U:  printf("U ");  break;
            case U1: printf("U' "); break;
            case U2: printf("U2 "); break;
            case F:  printf("F ");  break;
            case F1: printf("F' "); break;
            case F2: printf("F2 "); break;
        }
    }
    printf("\n");
}

// Main function
int main() {
    Cube input;
    readCube(input);

    Cube target = getTargetCube(input);

    Triplets triplets = buildTriplets(target);

    State start = getState(input, triplets);
    State goal = getState(target, triplets);

    printf("\nInitial Cube:\n\n");
    printCube(input);
    printf("\nTarget Cube:\n\n");
    printCube(target);
    printf("\nMoves:\n");

    vector<int> moves = bfs(start, goal);
    printMoves(moves);

    return 0;
}
