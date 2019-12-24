// Philip Knott
// Nonogram Solver
// Started October 1st, 2019

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

//const int LEN = 10;
const int LEN = 5;


struct header {
    vector<int> value; // can contain any number of values
    vector<string> options; // filled with getOptions()
};

class nonogram {
public:
    header r[LEN], c[LEN];
//    int field[LEN][LEN];
    
    int field[5][5] = {
        {0, 1, 1, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}
    };
    
    void readFile() {
        ifstream input;
        input.open("/Users/filupnot/Desktop/2275/independent_stuff/nonogram_solver/nonogram_solver/game2.txt");
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
                r[i].value.insert(r[i].value.begin(), stoi(temp));
            }
            i++;
        }
        
        i = 0;
        while (getline(input, line)) {
            stringstream ss(line);
            
            string temp;
            while (ss >> temp) {
                c[i].value.insert(c[i].value.begin(), stoi(temp));
            }
            i++;
        }
    }
    
    /* Long & complicated function to print out field */
    void print() {
        int start_x = 0, // how many spaces to put before the column
            max_x = 0, // biggest final index for columns
            start_y = 0, // how many spaces to put before the row (probably not needed...)
            max_y = 0; // biggest final index for rows
        
        // find start_x and max_x
        for (header h : c) {
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
        for (header h : r) {
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
            
            for (header h : c) {
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
        for (header h : r) {
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
            header h = c[c_idx];
            int fc = 0; // num of spaces filled consecutively
            int r_idx = 0; // current row index
            
//            for (int num_idx = 0; num_idx < h.value.size(); num_idx++) {
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
            header h = r[r_idx];
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
    
    /* Returns vector of all possible line options */
    vector<int> getAllOptions(vector<int> header) {
        vector<int> options;
        int start = 0, // index to start
            end, // index to end
            iter; // number of iterations
        
        // <OOO-------> / start = 0, end = 9, iter = 8
        if (header.size() == 1) {
            end = LEN - 1;
            iter = LEN - header.at(0) + 1;
        }
        // <OOO-OO-O--> / (1): start = 0, end = 9, iter = 3
        //                (2): start = 4, end = 7, iter = 3
        //                (3): start = 0, end = 4, iter = 3
        else if (header.size() > 1) {
            
            for (int i = 0; i < header.size() - 2; i++) {
                start += header.at(i);
            }
            start += header.size() - 1;
        }
        
        
        
        
        return options;
    }
};

int main(int argc, char * argv[]) {
    
    nonogram gram;
    gram.readFile();
    gram.print();
    
    cout << gram.isComplete() << endl;
    
    return 0;
}
