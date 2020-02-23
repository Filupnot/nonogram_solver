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
//const int LEN = 5;
const int LEN = 15;

struct header {
    vector<int> value; // can contain any number of values
    vector<vector<int> > options; // filled with getOptions()
    bool solved = false;
};

class Nonogram {
    header row[LEN], col[LEN];
    int field[LEN][LEN];
    
public:
    /* Correct field for game1 */
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
        
        solve();
        
        cout << "Solved!" << endl;
        print();
        
    }
    
    void readFile() {
        ifstream input;
        
        if (LEN == 10) {
            input.open("/Users/filupnot/Desktop/nonogram_solver/nonogram_solver/game1.txt");
        }
        else if (LEN == 5) {
            input.open("/Users/filupnot/Desktop/nonogram_solver/nonogram_solver/game2.txt");
        }
        else if (LEN == 15) {
            input.open("/Users/filupnot/Desktop/nonogram_solver/nonogram_solver/game3.txt");
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
        
        // Column headers
        cout << endl;
        for (int i = max_x; i >= 0; i--) { // iterate through each number
             
//            // start over the right distance away
            for (int s = 0; s < max_y; s++) cout << "   ";
            cout << "     ";
            
            for (header h : col) {
                if (h.value.size() - 1 >= i) {
                    cout << h.value.at(h.value.size() - 1 - i) << " "; // outputs number in correct order
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
                    cout << h.value.at(h.value.size() - 1 - i) << " "; // outputs number in correct order
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
        for (header r : row) if (!r.solved) return false;
        for (header c : col) if (!c.solved) return false;
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
    
    /* Solves game (hopefully)
     How it works:
        -Starts by examining all options
        -Compares all options to what's currently on the field. Options that contradict
    what's on the field are erased.
        -Take the options left and create a 'super' case, containing all the overlapping
    values from each option
        -Copy what's on the super class onto the line (1s and -1s, or Os and Xs)
        -Repeat until each header has only one option left / isComlpete() returns true!
     */
    void solve() {
        int overflow_ctr = 0;
        
        while (1) {
            if (overflow_ctr == 50) throw -1; // unsolvable without guesses
            if (isComplete()) break; // done!
            
            // cover rows, then columns, then repeat
            // first, rows
            for (int r = 0; r < LEN; r++) {
                
                // if it's already solved, move on
                if (row[r].solved) continue;
                
                // easily-accessible options vector
                vector<vector<int> > *options = &row[r].options;
                
                // create vector for current 'line' being observed
                vector<int> line;
                for (int c = 0; c < LEN; c++) {
                    line.push_back(field[r][c]);
                }
                
                // get rid of options that don't support line
                for (int i = 0; i < (int)options->size(); i++) {
                    
                    vector<int> *v = &row[r].options.at(i);

                    for (int j = 0; j < LEN; j++) {
                        if ((line.at(j) == 1 && v->at(j) == 0) ||
                            (line.at(j) == -1 && v->at(j) == 1)) {
                            options->erase(options->begin() + i);
                            i--;
                            break;
                        }
                    }
                }
                
                // create "super" line, or line with shared values throughout all options
                vector<int> super = row[r].options.at(0);
                for (int i = 0; i < LEN; i++) if (super.at(i) == 0) super.at(i) = -1;
                
                // iterate through all options, save shared values to super
                for (vector<int> v : *options) {
                    for (int i = 0; i < LEN; i++) {
                        if (v.at(i) == 0 && super.at(i) == 1) {
                            super.at(i) = 0;
                        }
                        else if (v.at(i) == 1 && super.at(i) == -1) {
                            super.at(i) = 0;
                        }
                    }
                }
                
                // add super to line
                for (int i = 0; i < LEN; i++) {
                    if (super.at(i) != 0) line.at(i) = super.at(i);
                    
                    // push line to board
                    field[r][i] = line.at(i);
                }
                
                if (row[r].options.size() == 1) row[r].solved = true;
            }
            
            // now, columns
            for (int c = 0; c < LEN; c++) {
                            
                // if it's already solved, move on
                if (col[c].solved) continue;
                
                // easily-accessible options vector
                vector<vector<int> > *options = &col[c].options;
                
                // create vector for current 'line' being observed
                vector<int> line;
                for (int r = 0; r < LEN; r++) {
                    line.push_back(field[r][c]);
                }
                
                // get rid of options that don't support line
                for (int i = 0; i < (int)options->size(); i++) {
                    
                    vector<int> *v = &col[c].options.at(i);

                    for (int j = 0; j < LEN; j++) {
                        if ((line.at(j) == 1 && v->at(j) == 0) ||
                            (line.at(j) == -1 && v->at(j) == 1)) {
                            options->erase(options->begin() + i);
                            i--;
                            break;
                        }
                    }
                }
                
                // create "super" line, or line with shared values throughout all options
                vector<int> super = col[c].options.at(0);
                for (int i = 0; i < LEN; i++) if (super.at(i) == 0) super.at(i) = -1;
                
                // iterate through all options, save shared values to super
                for (vector<int> v : *options) {
                    for (int i = 0; i < LEN; i++) {
                        if (v.at(i) == 0 && super.at(i) == 1) {
                            super.at(i) = 0;
                        }
                        else if (v.at(i) == 1 && super.at(i) == -1) {
                            super.at(i) = 0;
                        }
                    }
                }
                
                // add super to line
                for (int i = 0; i < LEN; i++) {
                    if (super.at(i) != 0) line.at(i) = super.at(i);
                    
                    // push line to board
                    field[i][c] = line.at(i);
                }
                
                if (col[c].options.size() == 1) col[c].solved = true;
            }
        }
        // repeat process
    }
};

int main(int argc, char * argv[]) {
    
    Nonogram gram;
    
//    gram.print();
    
//    cout << (gram.isComplete() ? "Completed!\n" : "");
    
    return 0;
}
