// Philip Knott
// Nonogram Solver
// Started October 1st, 2019

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <numeric>
using namespace std;

//const int LEN = 10;
const int LEN = 5;

struct header {
    vector<int> value; // can contain any number of values
    vector<vector<int> > options; // filled with getOptions()
};

class Nonogram {
    header row[LEN], col[LEN];
    int field[LEN][LEN];
    
public:
    /* Correct field */
//    int field[5][5] = {
//        {0, 1, 1, 1, 0},
//        {1, 1, 1, 1, 0},
//        {1, 1, 1, 1, 0},
//        {0, 1, 1, 1, 1},
//        {1, 1, 1, 1, 1}
//    };
    
    /* Constructor */
    Nonogram() {
        // initialize field
        for (int r = 0; r < LEN; r++) {
            for (int c = 0; c < LEN; c++) {
                field[r][c] = 0;
            }
        }
        
        readFile();
        
//        solve();
        
    }
    
    void readFile() {
        ifstream input;
        
        if (LEN == 10) {
            input.open("/Users/filupnot/Desktop/nonogram_solver/nonogram_solver/game1.txt");
        }
        else if (LEN == 5) {
            input.open("/Users/filupnot/Desktop/nonogram_solver/nonogram_solver/game2.txt");
        }
            
        if (!input.is_open()) {
            cout << "Error reading file." << endl;
            exit(0);
        }
        
        string line;
        int i = 0;
        while (getline(input, line)) {
            if (line == "-") break;
            
            stringstream ss(line);
            string temp;
            while (ss >> temp) {
                row[i].value.push_back(stoi(temp));
            }
            row[i].options = getOptions(row[i].value);
            
            i++;
        }
        
//        for (int i = 0; i < LEN; i++) {
//            for (int j : row[i].value) cout << j << ' ';
//            cout << endl;
//        }
//        cout << endl;
        
        i = 0;
        while (getline(input, line)) {
            if (line == "-") break;
            
            stringstream ss(line);
            string temp;
            while (getline(ss, temp, ' ')) {
                col[i].value.push_back(stoi(temp));
            }
            col[i].options = getOptions(col[i].value);
            
            i++;
        }
        
//        for (int i = 0; i < LEN; i++) {
//            for (int j : col[i].value) cout << j << ' ';
//            cout << endl;
//        }
    }
    
    void print(vector<int> v) {
        for (int i : v) cout << i << ' ';
        cout << endl;
    }
    
    /* Long & complicated function to print out field */
    void print() {
        int start_x = 0, // how many spaces to put before the column
            max_x = 0, // biggest final index for columns
            start_y = 0, // how many spaces to put before the row (probably not needed...)
            max_y = 0; // biggest final index for rows
        
        // find start_x and max_x
        for (header h : col) {
            int temp = 0;
            
            // save largest value for max_x
            if (h.value.size() - 1 > max_x) {
                max_x = (int)h.value.size() - 1;
            }
            
            // add a space for each digit
            for (int i : h.value) {
                if (i < 10) temp++;
                if (i >= 10) temp += 2;
            }
            
            // add a space for between each number
            temp += h.value.size() - 1;
            
            // save the value of 'temp' with the most spaces
            if (temp > start_x) start_x = temp;
        }
        
        // find start_y and max_y
        for (header h : row) {
            int temp = 0;
            
            // save largest value for max_x
            if (h.value.size() - 1 > max_y) {
                max_y = (int)h.value.size() - 1;
            }
            
            // add a space for each digit
            for (int i : h.value) {
                if (i < 10) temp++;
                if (i >= 10) temp += 2;
            }
            
            // add a space for between each number
            temp += h.value.size() - 1;
            
            // save the value of 'temp' iwth the most spaces
            if (temp > start_y) start_y = temp;
        }
        
//        cout << "start_x: " << start_x << endl;
//        cout << "max_x: " << max_x << endl;
//        cout << "start_y: " << start_y << endl;
//        cout << "max_y: " << max_y << endl;
        
        // Column headers
        cout << endl;
        for (int i = max_x; i >= 0; i--) { // iterate through each number
             
//            // start over the right distance away
//            for (int s = 0; s < start_x; s++) cout << "   ";
            for (int s = 0; s < max_y; s++) cout << "   ";
            cout << "     ";
            
            for (header h : col) {
                if (h.value.size() - 1 >= i) {
                    cout << h.value.at(i) << " "; // outputs number
                    if (h.value.at(i) < 10) cout << " "; // adds an extra space if the number is only one digit
                }
                else cout << "   "; // no number to output => fill spaces
            }
            cout << endl;
        }
        
        // underscores
        for (int s = 0; s < max_y; s++) cout << "   ";
        cout << "     ";
        for (int u = 0; u < LEN; u++) cout << "___";
        cout << endl;
        
        // Row headers and field values
        int r_idx = 0;
        for (header h : row) {
            for (int i = max_y; i >= 0; i--) {
                if (h.value.size() - 1 >= i) {
                    cout << h.value.at(i) << " "; // outputs number
                    if (h.value.at(i) < 10) cout << " "; // adds an extra space if the number is only one digit
                }
                else cout << "   "; // no number to output => fill spaces
            }
            cout << "| ";
            
            for (int c_idx = 0; c_idx < LEN; c_idx++) {
                if (field[r_idx][c_idx] == 1) cout << 'O' << "  ";
                else cout << "   ";
            }
            r_idx++;
            cout << endl;
        }
    }
    
    /* Returns true if the game is completed */
    bool isComplete() {

        // check columns
        for (int c_idx = 0; c_idx < LEN; c_idx++) {
            header h = col[c_idx];
            int fc = 0; // num of spaces filled consecutively
            int r_idx = 0; // current row index
            
            for (int num_idx = (int)h.value.size() - 1; num_idx >= 0; num_idx--) {
                while (true) {
                    if (r_idx == LEN) { // reached end of column
                        if (fc != h.value.at(num_idx) || num_idx != 0) {
                            return false; // wrong length of stream at end => incomplete
                        }
                        else {
                            r_idx++;
                            break;
                        }
                    }
                    if (field[r_idx][c_idx] == 1) {
                        fc++;
                    }
                    else {
                        if (fc != h.value.at(num_idx) && fc > 0) { // wrong length of stream => incomplete
                            return false;
                        }
                        else if (fc == h.value.at(num_idx)) { // correct length => set fc to zero and move onto next num
                            fc = 0;
                            r_idx++;
                            break;
                        }
                    }
                    r_idx++;
                }
            }
        }
        
        // check rows
        for (int r_idx = 0; r_idx < LEN; r_idx++) {
            header h = row[r_idx];
            int fc = 0; // number of spaces filled consecutively
            int c_idx = 0; // current row index
            
            for (int num_idx = (int)h.value.size() - 1; num_idx >= 0; num_idx--) {
                while (true) {
                    if (c_idx == LEN) {
                        if (fc != h.value.at(num_idx) || num_idx != 0) {
                            return false; // wrong length of stream at end => incomplete
                        }
                        else {
                            c_idx++;
                            break;
                        }
                    }
                    if (field[r_idx][c_idx] == 1) {
                        fc++;
                    }
                    else {
                        if (fc != h.value.at(num_idx) && fc > 0) { // wrong length of stream => incomplete
                            return false;
                        }
                        else if  (fc == h.value.at(num_idx)) { // correct length => set fc to zero and move onto next num
                            fc = 0;
                            c_idx++;
                            break;
                        }
                    }
                    c_idx++;
                }
            }
        }
        return true;
    }
    
    /* Reverses a vector passed */
    void reverse(vector<int> &vec) {
        vector<int> vec2;
        for (int i = (int)vec.size() - 1; i >= 0; i--) {
            vec2.push_back(vec.at(i));
        }
        vec.clear();
        for (int i : vec2) vec.push_back(i);
    }
    
    /* Returns vector of all possible line options */
    vector<vector<int> > getOptions(vector<int> hdr) {
        vector<vector<int> > options;
        
        // begin indices
        vector<int> start_idx;
        for (int i = 0; i < (int)hdr.size(); i++) {
            int s = 0;
            for (int j = i - 1; j >= 0; j--) s += hdr.at(j);
            s = s + i;
            start_idx.push_back(s);
        }
        
        // base of reference numbers (how many spots each chunk will/can move to)
        int base = LEN - accumulate(hdr.begin(), hdr.end(), 0) - ((int)hdr.size() - 1) + 1;
        
        // number of elements in outer 'ref' vector, or how many cases (will) exist
        int ref_size = pow(base, (int)hdr.size());
        
        // vector holding case vectors
        vector<vector<int> > ref;
        
        // go through loop for each case, max cases = ref_size
        for (int i = 0; i < ref_size; i++) {
            // convert base
            int temp = i;
            vector<int> value;
            int counter = (int)hdr.size();
            while (counter > 0) {
                value.push_back(temp % base);
                temp /= base;
                counter--;
            }
            
            // pop cases who's digits decrease
            bool good = true;
            int max = value.at(0);
            for (int j = 0; j < (int)hdr.size(); j++) {
                if (value.at(j) > max) max = value.at(j);
                if (value.at(j) < max) {
                    good = false;
                    break;
                }
            }
            
            // if digits decrease, don't add and move on. otherwise, it's good.
            if (good) {
                
                // add starting values to convert to starting indices
                for (int j = 0; j < (int)hdr.size(); j++) {
                    value.at(j) = value.at(j) + start_idx.at(j);
                }
                ref.push_back(value);
            }
        }
        
        // convert from reference beginning indices to actual arrays
        for (vector<int> v : ref) {
            // empty vector
            vector<int> base;
            for (int i = 0; i < LEN; i++) base.push_back(0);
            
            for (int i = 0; i < (int)hdr.size(); i++) {
                int counter = hdr.at(i);
                int j = v.at(i);
                while (counter > 0) {
                    base.at(j) = 1;
                    j++;
                    counter--;
                }
            }
            options.push_back(base);
        }
        return options;
    }
    
    void solve() {
        while (1) {
            // cover rows, then columns, then repeat
            for (int r = 0; r < LEN; r++) {
                vector<int> h = row[r].value;
                cout << h.size();
//                for (int i : h) cout << i << endl;
            }
            
        }
    }
};

int main(int argc, char * argv[]) {
    
    Nonogram gram;
    
//    gram.print();
    
    cout << (gram.isComplete() ? "Completed!\n" : "");
    
    return 0;
}
