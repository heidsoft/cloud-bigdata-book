创建标准 C++ 程序 (C++)

创建项目并添加源文件
1.通过以下方式创建一个项目：指向“文件”菜单上的“新建”，然后单击“项目”。

2.在“Visual C++”项目类型窗格中，单击“Win32”，然后单击“Win32 控制台应用程序”。

3.键入项目名称。

默认情况下，包含项目的解决方案与项目同名，但您可以键入其他名称。 您也可以为项目键入其他位置。 

单击“确定”创建项目。

4.在“Win32 应用程序向导”中，单击“下一步”，选择“空项目”，然后单击“完成”。

5.如果未显示“解决方案资源管理器”，请在“视图”菜单上，单击“解决方案资源管理器”。

6.将一个新源文件添加到项目，如下所示。

a.在“解决方案资源管理器”中，右击“源文件”文件夹，指向“添加”，然后单击“新建项”。

b.在“代码”节点中单击“C++ 文件(.cpp)”，为文件键入名称，然后单击“添加”。

该 .cpp 文件即显示在“解决方案资源管理器”中的“源文件”文件夹中，并且文件将在 Visual Studio 编辑器中打开。

7.在编辑器内的文件中，键入使用标准 C++ 库的有效 C++ 程序，或者复制示例程序之一并将其粘贴在文件中。

例如，您可以使用 set::find (STL Samples)示例程序，该程序是帮助中附带的标准模板库示例之一。
// SetFind.cpp
// compile with: /EHsc
//
//      Illustrates how to use the find function to get an iterator
//      that points to the first element in the controlled sequence
//      that has a particular sort key.
//
// Functions:
//
//    find         Returns an iterator that points to the first element
//                 in the controlled sequence that has the same sort key
//                 as the value passed to the find function. If no such
//                 element exists, the iterator equals end().
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)
#include <set>
#include <iostream>

using namespace std ;

typedef set<int> SET_INT;

void truefalse(int x)
{
  cout << (x?"True":"False") << endl;
}

int main() {
  SET_INT s1;
  cout << "s1.insert(5)" << endl;
  s1.insert(5);
  cout << "s1.insert(8)" << endl;
  s1.insert(8);
  cout << "s1.insert(12)" << endl;
  s1.insert(12);

  SET_INT::iterator it;
  cout << "it=find(8)" << endl;
  it=s1.find(8);
  cout << "it!=s1.end() returned ";
  truefalse(it!=s1.end());  //  True

  cout << "it=find(6)" << endl;
  it=s1.find(6);
  cout << "it!=s1.end() returned ";
  truefalse(it!=s1.end());  // False
}
/////////////////////////////////////////////////

如果使用该示例程序，请注意 using namespace std; 指令。 此指令使程序能够使用 cout 和 endl，而无需完全限定名（std::cout 和 std::endl）。 

8.保存该文件。

9.在“生成”菜单上，单击“生成解决方案”。

“输出”窗口显示有关编译过程的信息，例如，生成日志的位置，以及指示生成状态的消息。

10.在“调试”菜单上，单击“开始执行(不调试)”。

如果使用了示例程序，将显示一个命令窗口，其中显示是否在集合中找到了特定的整数。

