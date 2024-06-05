#include <iostream>
#include <vector>
#include <utility>

int main() {
    // تعریف وکتور دو بعدی از جفت‌های مرتب
    std::vector<std::pair<int, char>> vec;

    // مقداردهی به وکتور
    vec.push_back(std::make_pair(10, 'A'));
    vec.push_back(std::make_pair(20, 'B'));
    vec.push_back(std::make_pair(30, 'C'));

    // چاپ عناصر وکتور
    for (const auto& pair : vec) {
        std::cout << "Number: " << pair.first << ", Character: " << pair.second << std::endl;
    }

    return 0;
}
