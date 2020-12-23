
// #include <iostream>

// using namespace std;

// #define AA(B, C) int B##C = 3
// #define BB(B,C) AA(B,C)
// #define DD BB(A, __LINE__)

// #define BB(C) cout << C << endl;
// #define DD BB(__LINE__)

#define BB(C) int b##C = 3;
#define DD BB(__LINE__)

int main()
{
    DD
    DD

    // cout << A12 << endl;
    // cout << A13 << endl;
    return 0;
}