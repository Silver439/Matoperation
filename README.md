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
  		counter.push_back(1);
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

* **首先先简单的检测一下内存管理机制**：

  在main函数写下如下代码：

  ```c++
  int main()
  {
  	matrix A(2, 2);
  	matrix B(2, 2);
  	A = B;
  }
  ```

  运行结果：

  ![Screenshot 2020-11-27 113035](https://github.com/Silver439/Matoperation/blob/main/picture/Screenshot%202020-11-27%20113035.png)

  可以看到A,B的内存均被正确释放，由于存在两个指针指向B所对应的内存块，所以该内存释放时先“wait”再释放。

* **再看一个较复杂的情况**：

  ```c++
  int main()
  {
  	matrix A(2, 2);
  	matrix B(2, 2);
  	matrix C(A);
  	A = B;
  	B = C;
  }
  ```

  运行结果：

  ![Screenshot 2020-11-27 113754](https://github.com/Silver439/Matoperation/blob/main/picture/Screenshot%202020-11-27%20113754.png)

  这里进行了更多更为复杂的赋值构造操作，可以看到最终内存依旧被正确释放。

* **接下来测试一些运算符**：（简单起见，将所有矩阵的元素都设置为1）

  首先创建3个二维动态数组，元素全部初始化为1:

  ```c++
  	float** p1;
  	float** p2;
  	float** p3;
  	p1 = new float* [2];
  	p2 = new float* [2];
  	p3 = new float* [3];
  	for (int i = 0; i < 2; i++) {
  		p1[i] = new float[3];
  		p2[i] = new float[3];
  	}
  	for (int i = 0; i < 3; i++) {
  		p3[i] = new float[4];
  	}
  	for(int i=0;i<2;i++)
  	{
  		for (int j = 0; j < 3; j++) {
  			p1[i][j] = 1;
  			p2[i][j] = 1;
  		}
  	}
  	for (int i = 0; i < 3; i++)
  	{
  		for (int j = 0; j < 4; j++) {
  			p3[i][j] = 1;
  		}
  	}
  ```

  然后将这些数组的数据传递到矩阵中：

  ```c++
        matrix A(2, 3, p1);
  	matrix B(2, 3, p2);
  	matrix C(3, 4, p3);
  ```

  **开始测试各个运算符功能**：（同时演示了<<重载后的功能）

  ```c++
        cout << "A+B: " << endl;
  	cout << A + B << endl;
  	cout << endl;
  	cout << "A-B: " << endl;
  	cout << A - B << endl;
  	cout << endl;
  	cout << "A*C: " << endl;
  	cout << A*C << endl;
  	cout << endl;
  ```

  输出结果：

  ![Screenshot 2020-11-28 101413](https://github.com/Silver439/Matoperation/blob/main/picture/Screenshot%202020-11-28%20101413.png)

可以看到结果均正确，且内存均被正确释放。特别的，由于动态申请的二维数组的内存由析构函数释放，所以在delete这些指针之前需要先将它们转化为空指针，具体如下：

```c++
        p1 = NULL;
	p2 = NULL;
	p3 = NULL;
	delete[] p1;
	delete[] p2;
	delete[] p3;

	return 0;
```

这样便可避免程序崩溃。

* 至此我们已经展示了所有被重载过的运算符。对于它们我们还可以**做更为复杂的符合运算**，例如：

  ```c++
        float a, b;
  	a = 2.1f;
  	b = 3.2f;
  
  	cout << "a*B-A " << endl;
  	cout << a * B - A << endl;
  	cout << endl;
  	cout << "(A*b)*C " << endl;
  	cout << (A * b)* C << endl;
  	cout << endl;
  	cout << "(A+B)*C: " << endl;
  	cout << (A + B) * C << endl;
  	cout << endl;
  ```

  这段代码**还展示了矩阵与标量相乘的两种*的重载方法**。运行结果如下：

 ![Screenshot 2020-11-28 102256](https://github.com/Silver439/Matoperation/blob/main/picture/Screenshot%202020-11-28%20102256.png)

  结果均正确，且内存均被正确释放。

* 以上是程序展示部分。接下来我会将该代码用arm板运行。

### 四：Arm板运行测试：

* 首先写cmakelist文件后用cmake指令生成makefile，如图：

 ![2020-11-28-112344_1024x600_scrot](https://github.com/Silver439/Matoperation/blob/main/picture/2020-11-28-112344_1024x600_scrot.png)

* 再用make指令生成可执行程序文件Mat：

  ![2020-11-28-112536_1024x600_scrot](https://github.com/Silver439/Matoperation/blob/main/picture/2020-11-28-112536_1024x600_scrot.png)

* 执行可执行程序Mat，运行结果如图：

  ![2020-11-28-112822_1024x600_scrot](https://github.com/Silver439/Matoperation/blob/main/picture/2020-11-28-112822_1024x600_scrot.png)
  ![2020-11-28-112848_1024x600_scrot](https://github.com/Silver439/Matoperation/blob/main/picture/2020-11-28-112848_1024x600_scrot.png)

  与windows下运行结果完全相同。

### 五.总结：

* 以上便是本次报告的全部内容。个人认为本次作业最大的难点在于内存管理，解决了内存的释放问题后，其他的任务都显得较为轻松。由于代码文件较少，所以用cmake管理代码显得并不必要，因此我目前对cmake的理解与运用还处在较为粗浅的阶段，今后需要进一步深入学习。
* **感谢阅读！**
