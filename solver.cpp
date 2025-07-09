#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <array>
#include <algorithm>
#include <map>
#include <cstdint>

using namespace std;

// All possible moves for the 2x2x2 cube
const vector<string> MOVES = {
    "F", "F'", "F2", "B", "B'", "B2", "R", "R'", "R2",
    "L", "L'", "L2", "U", "U'", "U2", "D", "D'", "D2"
};

struct Cube {
    array<char, 24> state;

    Cube() = default;

    Cube(string s) {
        for (int i = 0; i < 24; ++i)
            state[i] = s[i];
    }

    string to_string() const {
        return string(state.begin(), state.end());
    }

    // Checks if each face has the same color
    bool is_solved() const {
        for (int i = 0; i < 24; i += 4)
            if (!(state[i] == state[i+1] && state[i] == state[i+2] && state[i] == state[i+3]))
                return false;
        return true;
    }

    // Encodes the state as a unique 64-bit integer
    uint64_t encode() const {
        uint64_t code = 0;
        for (char c : state)
            code = (code << 3) | (c - 'A');
        return code;
    }

    // Applies a move and returns the resulting cube
    Cube move(const string& mv) const {
        static const unordered_map<string, vector<int>> turns = {
            {"F",  {8, 9, 11, 10, 2, 6, 13, 17}},
            {"F'", {8, 10, 11, 9, 2, 17, 13, 6}},
            {"F2", {8, 11, 10, 9, 2, 13, 17, 6}},
            {"B",  {20, 21, 23, 22, 0, 16, 15, 5}},
            {"B'", {20, 22, 23, 21, 0, 5, 15, 16}},
            {"B2", {20, 23, 22, 21, 0, 15, 5, 16}},
            {"U",  {0, 1, 3, 2, 20, 8, 16, 4}},
            {"U'", {0, 2, 3, 1, 20, 4, 16, 8}},
            {"U2", {0, 3, 2, 1, 20, 16, 4, 8}},
            {"D",  {12, 13, 15, 14, 10, 6, 18, 22}},
            {"D'", {12, 14, 15, 13, 10, 22, 18, 6}},
            {"D2", {12, 15, 14, 13, 10, 18, 22, 6}},
            {"R",  {4, 5, 7, 6, 1, 9, 13, 21}},
            {"R'", {4, 6, 7, 5, 1, 21, 13, 9}},
            {"R2", {4, 7, 6, 5, 1, 13, 21, 9}},
            {"L",  {16, 17, 19, 18, 0, 20, 12, 8}},
            {"L'", {16, 18, 19, 17, 0, 8, 12, 20}},
            {"L2", {16, 19, 18, 17, 0, 12, 8, 20}},
        };

        Cube next = *this;
        const vector<int>& sw = turns.at(mv);
        char tmp = next.state[sw[0]];
        for (int i = 0; i < 7; ++i)
            next.state[sw[i]] = next.state[sw[i + 1]];
        next.state[sw[7]] = tmp;
        return next;
    }
};

// Bidirectional BFS to find the shortest solution
string bidirectional_bfs(Cube start) {
    if (start.is_solved())
        return "";

    unordered_map<uint64_t, string> visited_f, visited_b;
    queue<pair<Cube, string>> q_f, q_b;

    Cube goal("WWWWRRRRGGGGYYYYOOOOBBBB"); // Solved cube
    visited_f[start.encode()] = "";
    visited_b[goal.encode()] = "";

    q_f.push({start, ""});
    q_b.push({goal, ""});

    while (!q_f.empty() && !q_b.empty()) {
        if (q_f.size() > q_b.size()) {
            swap(q_f, q_b);
            swap(visited_f, visited_b);
        }

        int sz = q_f.size();
        while (sz--) {
            Cube cube = q_f.front().first;
            string path = q_f.front().second;
            q_f.pop();

            for (const string& mv : MOVES) {
                Cube next = cube.move(mv);
                uint64_t code = next.encode();

                if (visited_f.count(code)) continue;

                string next_path = path.empty() ? mv : path + " " + mv;
                visited_f[code] = next_path;

                if (visited_b.count(code)) {
                    string rev_path = visited_b[code];
                    reverse(rev_path.begin(), rev_path.end());
                    return next_path + " " + rev_path;
                }

                q_f.push({next, next_path});
            }
        }
    }

    return "Unsolvable";
}

// Checks if each color appears exactly 4 times
bool validate_color_counts(const string& s) {
    map<char, int> freq;
    for (char c : s)
        freq[toupper(c)]++;
    return freq['W'] == 4 && freq['R'] == 4 && freq['O'] == 4 &&
           freq['Y'] == 4 && freq['G'] == 4 && freq['B'] == 4;
}

string to_uppercase(string s) {
    for (char& c : s)
        c = toupper(c);
    return s;
}

int main() {
    cout << "Enter 24 characters for cube in the order:\n";
    cout << "Top(4), Right(4), Left(4), Bottom(4), Front(4), Back(4):\n";

    string input;
    cin >> input;

    input = to_uppercase(input);

    if (input.length() != 24) {
        cout << "Invalid input length. Must be exactly 24 characters.\n";
        return 1;
    }

    if (!validate_color_counts(input)) {
        cout << "Invalid color count. Each of W, R, O, Y, G, B must appear exactly 4 times.\n";
        return 1;
    }

    // Rearranged to internal face order: U, R, F, D, L, B
    string rearranged = input.substr(0, 4)   // U (Top)
                      + input.substr(4, 4)   // R (Right)
                      + input.substr(16, 4)  // F (Front)
                      + input.substr(12, 4)  // D (Bottom)
                      + input.substr(8, 4)   // L (Left)
                      + input.substr(20, 4); // B (Back)

    Cube scrambled(rearranged);
    string solution = bidirectional_bfs(scrambled);

    cout << "Solution: " << solution << "\n";
    return 0;
}
