#include <iostream>
#include <string>
#include <algorithm>
#include "Archiver.h"
#include "Unarchiver.h"

using namespace std;

string int_to_string(int value) {
    string result;
    while (value > 0) {
        result += value % 10 + '0';
        value /= 10;
    }
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    string work_case;
    cin >> work_case;
    for (int i = 1; i < 2; ++i) {
        cout << "Test #" << i << endl;
        string data_file_name = "Тест_" + int_to_string(i) + ".txt";
        string archive_file_name = "archive_" + int_to_string(i);
        string unarchive_file_name = "unarchive_" + int_to_string(i) + ".txt";
        if (work_case == "a") {
            Archiver archiver(data_file_name, archive_file_name);
            archiver.archive();
        } else if (work_case == "u") {
            Unarchiver unarchiver(archive_file_name, unarchive_file_name);
            unarchiver.unarchive();
        } else {
            cout << "Unknown command" << endl;
        }
    }
    return 0;
}
