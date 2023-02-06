#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

vector<string> splitString(string str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string token;
    while(getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

int main() {
    string testString = "This is a test string";
    char delimiter = ' ';
    vector<string> result = splitString(testString, delimiter);

    cout << "Size = "<<result.size()<<endl;
    cout << result[0]<<endl;
    for(string s : result) {
        cout << s << endl;
    }
    return 0;
}
