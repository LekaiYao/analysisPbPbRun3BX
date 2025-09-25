#include <iostream>
using namespace std;

// 函数名和文件名保持一致：plot_var
void test(int mode) {
    cout << "mode = " << mode << endl;

    if (mode == 0) {
        cout << ">>> mode==0 branch entered <<<" << endl;
    } else {
        cout << ">>> mode!=0 branch entered <<<" << endl;
    }
}
