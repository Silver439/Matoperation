# Matoperation

Assignment4

### 一：本次作业实现功能如下：

* 定义了矩阵类，成员变量包含矩阵的行数，列数，以及数据。
* 重载定义了+，-，*，=，<<运算符，其中 ” *“定义了三种重载，即A * B, a *B，A * b（小写代表标量）。
* 能够判断运算是否合理，并且能够指出不合理运算的错误所在。
* 向函数传递矩阵类参数是全部使用引用传递，尽可能减小内存消耗。
* 通过使用vector，利用OpenCV的CV_XADD函数的思路，实现了内存维护功能，避免了因多个指针指向同一地址的内存块导致的delete时出现的程序崩溃问题。
* 将程序通过arm板成功运行。
* 编写了cmakelist，通过cmake生成makefile，最后通过编译makefile成功运行程序。

### 二：代码分析：

* **首先是矩阵类**：

  ```c++
  class matrix {
  private:
  	int row=0;
  	int col=0;
  	float** data;
  	void init();
  	void check1();
  	bool check2();
  public:
  	matrix(const matrix& B);
  	matrix(int m, int n);
  	matrix(int m, int n,float** element);
  
  	matrix& operator=(const matrix& B);
  	matrix operator+(const matrix& B);
  	matrix operator-(const matrix& B);
  	matrix operator*(const matrix& B);
  	matrix operator*(const float& b);
  
  	~matrix();
  
  	friend matrix operator*(const float a, const matrix& B);
  	friend ostream& operator<<(ostream& os,const matrix& B);
  };
  ```

  private中包含行数row，列数col，数据data，初始化函数init（），以及两个用于内存维护的检查函数。

  public中包含三种构建函数，析构函数，以及各类符号的重载函数接口。

* 接下来是各个函数接口的具体实现：

  首先定义两个vector，一个用来储存内存地址，另一个用来储存相应内存所对应指针的个数（即有多少指针指向这一内存块）

  ```c++
  static vector<int> address;
  static vector<int> counter;
  ```

* **初始化函数init（）**：初始化row，col，data以及两个vector。

  ```c++
  void matrix::init()
  {
  	data = new float*[row];
  	for (int i = 0; i < row; i++)
  	{
  		data[i] = new float[col] {0};
  	}
  	if (row * col != 0)
  	{
  		address.push_back((int)&(data[0][0]));
  		counter.push_back(1);
  	}
  }
  
  ```

* **两个检查函数**：

  ```c++
  void matrix:: check1()
  {
  	bool b = 0;
  	for (int i = 0; i < address.size(); i++)
  	{
  		if ((int)&(data[0][0]) == address[i]) {
  			counter[i] += 1;
  			b = 1;
  			break;
  		}
  	}
  	if (b == 0) {
  		address.push_back((int)&(data[0][0]));
  		counter.push_back(1);
  	}
  }
  
  bool matrix::check2()
  {
  	int i;
  	for (i = 0; i < address.size(); i++)
  	{
  		if ((int)&(data[0][0]) == address[i]) {
  			break;
  		}
  	}
  	if (counter[i] == 1) {
  		return 1;
  	}
  	else {
  		counter[i] -= 1;
  		return 0;
  	}
  }
  
  ```

  其中，check1用于检查指针所指向的内存块是否已经被其他指针所使用，若有其他指针则负责计数的vector对应位置元素加一，若无则向负责记录地址的vector中添加该内存地址，并且在负责计数的vector末尾添加元素“1”。

  check2负责在调用析构函数时检查内存块的内存是否能够被释放。若计数vector对应的元素为一，则说明该内存块可以释放，若大于一则说明还有多个指针在使用，此时vector对应元素减一，内存不进行释放。

* **三个构造函数**：（支持三种类型的构造方式，包括直接传递二维动态数组）

  ```c++
  matrix::matrix(const matrix& B)
  {
  	row = B.row;
  	col = B.col;
  	data = B.data;
  	check1();
  }
  
  matrix::matrix(int m, int n)
  {
  	row = m;
  	col = n;
  	init();
  }
  
  matrix::matrix(int m, int n,float** element)
  {
  	bool b = 0;
  	for (int i = 0; i < address.size(); i++)
  	{
  		if ((int)&(element[0][0]) == address[i]) {
  			counter[i] += 1;
  			b = 1;
  			break;
  		}
  	}
  	if (b == 0) {
  		address.push_back((int)&(element[0][0]));
  		counter.push_back(2);
  	}
  	row = m;
  	col = n;
  	data = element;
  }
  ```

* **各类符号的重载函数**：

  1.**等号**：进行了内存管理操作，判断是否需要释放被赋值的矩阵类的内存。

```c++
matrix& matrix::operator=(const matrix& B)
{
	if (row * col != 0) {
		if (check2()) {
			cout << row << "," << col << ":" << "Done!" << endl;
			delete[] data;
		}
		else {
			cout << row << "," << col << ":" << "Wait!" << endl;
		}
	}
	row = B.row;
	col = B.col;
	data = B.data;
	check1();
	return *this;
}
```

​      **2.  加减号**：能够判断两个矩阵是否能进行加减操作，若不能则会报错。

```c++
matrix matrix::operator+(const matrix& B)
{
	if (row == B.row && col == B.col) {
		int m = row;
		int n = B.col;
		matrix C(m, n);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				C.data[i][j] = data[i][j] + B.data[i][j];
			}
		}
		return C;
	}
	else {
		cout << "You can't add two matrix with different size!";
		return *this;
	}
}
matrix matrix::operator-(const matrix& B)
{
	if (row == B.row && col == B.col) {
		int m = row;
		int n = B.col;
		matrix C(m, n);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				C.data[i][j] = data[i][j] - B.data[i][j];
			}
		}
		return C;
	}
	else {
		cout << "You can't minus two matrix with different size!";
		return *this;
	}
}
```

​		**3.矩阵相乘**：能够判断两个矩阵是否能够相乘，若不能则会报错。

```c++
matrix matrix::operator*(const matrix &B)
{
	if (col == B.row) {
		matrix C(row, B.col);
		for (int i = 0; i < row; i++)
		{
			for (int k = 0; k < col; k++)
			{
				float v = data[i][k];
				for (int j = 0; j < B.col; j++)
				{
					C.data[i][j] += v * B.data[k][j];
				}
			}
		}
		return C;
	}
	else {
		cout << "Please multiply two matrix that the first matrix's col is equal to the second matrix's row.";
		return *this;
	}
}
```

这里采取的乘法算法在最简单的算法上做了一点改进，即更改循环次序以增加访存的连续性。这个代码较为简单，且效率良好。在O3优化的条件下进行两个1000*1000大小的矩阵乘法只需200ms，这个适用范围足够满足一般的运算需求。至于标量与矩阵的乘法较为简单，这里不再赘述。

​		**4.矩阵与标量间的乘法**：分为a * B和A * b两种情况。

```c++
matrix matrix::operator*(const float& b)
{
	matrix C(row, col);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++)
		{
			C.data[i][j] = b * data[i][j];
		}
	}
	return C;
}

matrix operator*(const float a, const matrix& B)
{
	matrix C(B.row, B.col);
	for (int i = 0; i < B.row; i++) {
		for (int j = 0; j < B.col; j++)
		{
			C.data[i][j] = a * B.data[i][j];
		}
	}
	return C;
}
```

* **最后是析构函数**：

```c++
matrix::~matrix()
{
	if (check2()) {
		cout <<row<<","<<col<<":"<< "Done!" << endl;
		delete[] data;
	}
	else {
		cout <<row<<","<<col<<":"<< "Wait!" << endl;
	}
}
```

  为了方便显示功能，该函数在调用时若成功释放内存则会输出"Done！"并输出释放内存所对应的矩阵大小。若未释放内存则会输出“Wait”并输出矩阵大小。在接下来的程序展示中可以看到，它能够很好的完成内存管理的任务。

### 三：程序展示：

