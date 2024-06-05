#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    int n = 5;

    // تعریف آرایه از اشاره‌گرها به void
    void **arr = (void **)malloc(n * sizeof(void *));

    // مقداردهی به اشاره‌گرها برای ذخیره کاراکتر و عدد
    char ch = 'A';
    int num = 10;

    *(arr) = &ch;  // اشاره به کاراکتر
    *(arr + 1) = &num; // اشاره به عدد

    // چاپ کاراکتر
    printf("%c\n", *(char *)(arr));

    // چاپ عدد
    printf("%d\n", *(int *)(arr + 1));

    free(arr);

    return 0;
}
