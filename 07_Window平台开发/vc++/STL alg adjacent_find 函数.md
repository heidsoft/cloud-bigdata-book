STL alg adjacent_find 函数

template <class ForwardIterator>
   ForwardIterator adjacent_find ( ForwardIterator first, ForwardIterator last );

template <class ForwardIterator, class BinaryPredicate>
   ForwardIterator adjacent_find ( ForwardIterator first, ForwardIterator last,
                                   BinaryPredicate pred );
邻近查找容器元素

adjacent_find算法用于查找相等或满足条件的邻近元素对。其有两种函数原型：一种是在迭代器区间[first , last)上查找两个连续的元素相等时，另一种是使用二元谓词判断pred，查找迭代器区间[first , last)上满足pred条件的邻近元素对 。 

函数原型： 
template <class ForwardIterator>
   ForwardIterator adjacent_find ( ForwardIterator first, ForwardIterator last )
{
  ForwardIterator next=first; ++next;
  if (first != last)
    while (next != last)
      if (*first++ == *next++)  // or: if (pred(*first++,*next++)), for the pred version
        return first;
  return last;
}

 



参数
first, last 
指出要查找的迭代器区间[first,last) 
pred 
用二元谓词pred作为参数，查找迭代器区间[first , last)上满足pred条件判断的邻近元素对，它可以是一个函数或对象内的类重载operator()。 

返回值
找到则返回元素对中第一个元素的迭代器位置，未找到则返回迭代器末位置last。 

示例代码
// adjacent_find example
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

bool myfunction (int i, int j) {
  return (i==j);
}

int main () {
  int myints[] = {10,20,30,30,20,10,10,20};
  vector<int> myvector (myints,myints+8);
  vector<int>::iterator it;

  // using default comparison:
  it = adjacent_find (myvector.begin(), myvector.end());

  if (it!=myvector.end())
    cout << "the first consecutive repeated elements are: " << *it << endl;

  //using predicate comparison:
  it = adjacent_find (++it, myvector.end(), myfunction);

  if (it!=myvector.end())
    cout << "the second consecutive repeated elements are: " << *it << endl;
  
  return 0;
}

 

输出:
the first consecutive repeated elements are: 30the second consecutive repeated elements are: 10
 


