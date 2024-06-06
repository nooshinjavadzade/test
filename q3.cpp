#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
using namespace std;

bool ToF(char x){
    if (x == 'T')
        return true;
    if (x == 'F')
        return false;
}

int chap_adad(int* in){
    int size = *(in);
    int sum = 0;
    for (int i = 1; i <= size; ++i) {
        if (*(in+i) > 0)
            sum += *(in+i);
    }
    return sum;
}

char chap_char(char* in){
    int size = *(in);
    for (int i = 1; i <= size; ++i) {
        if (isalpha(*(in+i)))
            *(in+i) = tolower(*(in+i));
    }
    char max = *(in+1);
    for (int i = 1; i <= size; ++i) {
        if (*(in+i) > max)
            max = *(in+i);
    }
    return max;
}

int main() {
    int m, n, l;
    char input;
    cin >> n >> m >> l;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 2*m; j+=2) {
            cin >> input;
            char some_char;
            int some_int;
            char* in_char = (char*)malloc(l*sizeof(char));
            int* in_int = (int*)malloc(l*sizeof(int));

            if (ToF(input)){
                *(in_int) = l;
                for (int k = 1; k <= l; ++k) {
                    cin >> *(in_int+k);
                }
                cout << chap_adad(in_int);
            }
            else if (!ToF(input)){
                *(in_char) = l;
                for (int k = 1; k <= l; ++k) {
                    cin >> *(in_char+k);
                }
                cout << chap_char(in_char);
            }
            cout << " ";
        }
        cout << endl;
    }

    return 0;
}
