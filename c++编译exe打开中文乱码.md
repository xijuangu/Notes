**解决**

在头文件处添加<br>
__#include <windows.h>__

设置main函数中第一条语句为<br>
__SetConsoleOutputCP(65001);__

也即更改后的程序如下：
```
#include <iostream>
#include <windows.h>  
using namespace std;  
int main()  
{  
    SetConsoleOutputCP(65001);  
    cout<<"这是中文"<<endl;  
    system("pause");  
    return 0;  
}
```
