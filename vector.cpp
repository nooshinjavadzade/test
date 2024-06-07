#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<vector<int>> in; // ایجاد یک وکتور دو بعدی خالی
    
    int rows, cols;
    cin >> rows >> cols;
    for (int i = 0; i < rows; i++) {
        vector<int> temp;
        for (int j = 0; j < cols; j++) {
            int element;
            cin >> element;
            temp.push_back(element);
        }
        in.push_back(temp);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << in.at(i).at(j) << " ";
        }
        cout << endl;
    }
    
    return 0;
}
