编译 C 程序

1.单击“开始”，指向“所有程序”、“Microsoft Visual Studio 2010”和“Visual Studio 工具”，然后单击“Visual Studio 2010 命令提示”。

根据计算机上的 Windows 版本和系统安全配置，可能必须右击“Visual Studio 2008 命令提示”，然后单击“以管理员身份运行”，才能成功运行按下列步骤创建的应用程序。

注意 
“Visual Studio 2010 命令提示”会自动设置 C 编译器和所需的任何库的正确路径。 应使用它而不是使用普通的“命令提示符”窗口。 有关更多信息，请参见为命令行生成设置路径和环境变量。 
 
2.在命令提示符下，键入 notepad simple.c，并按 Enter。

在系统提示是否创建文件时，单击“是”。

3.在记事本中，键入下列各行。

复制代码 #include <stdio.h>

int main()
{
    printf("This is a native C program.\n");
    return 0;
}4.在“文件”菜单上，单击“保存”，以创建 C 源文件。

5.关闭记事本。

6.在命令提示符下，键入 cl simple.c，并按 Enter。

cl.exe 编译器将生成一个可执行程序 Simple.exe。

您可以在编译器显示的多行输出信息中看到可执行程序的名称。

复制代码 Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 16.00 for 80x86
Copyright (C) Microsoft Corporation.  All rights reserved.

simple.c
Microsoft (R) Incremental Linker Version 10.00
Copyright (C) Microsoft Corporation.  All rights reserved.

/out:simple.exe
simple.obj7.若要查看 \simple\ 目录中的所有文件的列表，请键入 dir simple.* 并按 Enter。

.obj 文件是一个中间格式文件，可以安全地忽略它。

8.若要运行 Simple.exe，请键入 simple 并按 Enter。

该程序显示以下文本并退出：

This is a native C program.

9.若要关闭命令提示符窗口，请键入 exit 并按 Enter。

