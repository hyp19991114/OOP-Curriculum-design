/* *******************************************************
|               面向对象程序设计综合应用
|     选    题  画图软件的设计与实现
|     姓    名  黄远鹏      学    号  183401050225
|     班    级  软件1802
|     完成时间  2019年7月3日
\  ******************************************************* */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

const double PI=acos(-1.0);     // PI 圆周率
// equals0：判断浮点数是否为0     返回值：true（为0），false（非0）
bool equals0(double num)
{
	return fabs(num) < 1e-8;
}
class Shape     // Shape类：抽象类，作为其他类的基类
{
protected:
	double perimeter,area;     // perimeter 周长,area 面积
public:
	Shape() {}
	Shape(double p,double a):perimeter(p),area(a) {}
	// output：输出函数
	// 传入参数及返回值：输出流的引用std::ostream &os,默认参数为std::cout
	virtual ostream &output(ostream &os = cout)
	{
		os << "周长 = " << perimeter << "，面积 = " << area;
		return os;
	}
};

vector<Shape*> addr;     // 存放画过的图形的地址

class Point : public Shape     // 点类
{
private:
	double x,y;
public:
	Point() {}
	Point(double a,double b):Shape(0,0),x(a),y(b) {}
	// dist：计算两点间的距离
	// 传入参数：另一个点b(Point常量引用)   返回值：a,b间距离(double)
	double dist(const Point &b)
	{
		double dx=x-b.x,dy=y-b.y;
		return sqrt(dx*dx + dy*dy);
	}
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << "( " << x << " , " << y << " )";
		return os;
	}
};
class Line : public Shape     // 线段类
{
private:
	Point m,n;      // 线段的两个端点
	double A,B,C;   // 直线方程的三个参数
public:
	Line() {}
	Line(double mx,double my,double nx,double ny):Shape(),m(mx,my),n(nx,ny),
		A(ny-my),B(mx-nx),C(nx*my-mx*ny)
	{
		area = 0;
		perimeter = m.dist(n);
		if(A<0)   // 让第一项为正
			A *= -1, B *= -1, C *= -1;
		else if(equals0(A) && B<0)
			B *= -1, C *= -1;
		if(equals0(A) && equals0(C)) B=1;   // A,C都为0，B恒为1,例如3y=0即y=0
		if(equals0(B) && equals0(C)) A=1;   // B,C都为0，A恒为1
	}
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << "线段：方程：";
		if(!equals0(A))     // A为0时不输出此项
		{
			if(equals0(A-1)) os << "x";   // A为正1不输出参数
			else if(equals0(A+1)) os << "-x";   // A为负1不输出1只输出负号
			else  os << A << "x";
		}
		if(!equals0(B))     // B为0时不输出此项
		{
			if(!equals0(A))   // 前一项非0时输出+或-号
			{
				if(equals0(B-1))  // B为正1,例如2x+y
					os << "+y";
				else if(equals0(B+1))   // B为负1,例如2x-y
					os << "-y";
				else
					os << showpos << B << "y" << noshowpos;
			}
			else   // 前项为0，不输出+或-号
			{
				if(equals0(B-1))  // y
					os << "y";
				else if(equals0(B+1))   // -y
					os << "-y";
				else
					os << B << "y";
			}
		}
		if(!equals0(C))   // C为0时不输出
			os << showpos << C << noshowpos;
		os << "=0，端点坐标 ";
		m.output(os) << " ";
		n.output(os);
		return os;
	}
};
// make_Line：根据用户输入，生成一条线段并添加其地址到addr中
void make_Line()
{
	system("title Draw a Line");
	double x1,y1,x2,y2;
	cout << "请输入线段两个端点的坐标，用空格隔开：" << endl <<endl;
	cout << "第一个点x,y坐标:" << ends;
	cin >> x1 >> y1;
	cout << "第二个点x,y坐标:" << ends;
	cin >> x2 >> y2;
	while(fabs(x1 - x2) < 1e-8 && fabs(y1 - y2) < 1e-8)     // 两点重合，不能构成线段
	{
		cout << "输入的两点重合，不能构成线段。请重新输入：" << endl;
		cout << "第一个点x,y坐标:" << ends;
		cin >> x1 >> y1;
		cout << "第二个点x,y坐标:" << ends;
		cin >> x2 >> y2;
	}
	Line *L=new Line(x1,y1,x2,y2);
	addr.push_back(L);     // 新生成的线段添加到地址序列中
	cout << endl << "添加线段成功：" << endl;
	L->output() << endl;
	system("pause");
	return;
}

class Ellipse : public Shape     // 椭圆类
{
private:
	double a,b,c,e;     
	// a长半轴，b短半轴，c焦半径，e离心率
public:
	Ellipse() {}
	Ellipse(double a_,double b_):Shape(),a(a_),b(b_),c(sqrt(a_*a_ - b_*b_))
	{
		// 椭圆周长无法用初等函数表达，这里略去
		area=PI*a*b;
		e=c/a;
	}
	ostream &output(ostream &os = cout)   // 输出函数
	{

	}

};
// printDirection：输出提示信息
inline void printDirection()
{
	cout << "   Draw It!" << endl << endl;
	cout << "        1 - 画一条线段"   << endl;
	cout << "        2 - 画一个圆"     << endl;
	cout << "        3 - 画一个椭圆"   << endl;
	cout << "        4 - 画个多边形"   << endl;
	cout << "        5 - 画一条星形线" << endl;
	cout << "        6 - 画一条心形线" << endl;
	cout << "        7 - 查看已经画过的图形" << endl;
	cout << "        0 - 保存并退出"   << endl;
	cout << "请选择:" << ends;
}
// saveAndExit：输出所有图形到指定文件,释放申请的内存
void saveAndExit()
{
	system("title Save and Exit");
	if(addr.empty())   // addr为空
	{
		cerr << "未添加任何图形，无需保存。" << endl;
		system("pause");
		return;
	}
	cout << "请输入文件名，以回车结束：";
	string outFile;
	getline(cin,outFile);
	while(outFile.empty())
	{
		cerr << "文件名不能为空，请重新输入：";
		getline(cin,outFile);
	}
	ofstream fstrm(outFile);
	int cnt = 0;     // 保存输出信息条数
	for(auto it=addr.begin(),end=addr.end(); it!=end; ++it)
	{
		(*it)->output(fstrm) << endl;
		++cnt;
	}
	cout << "成功保存 " << cnt << " 条信息到 " << outFile << " 。" << endl;
	fstrm.close();
	for(auto it=addr.begin(),end=addr.end(); it!=end; ++it)
		delete *it;
	system("pause");
	return;
}
int main()
{
	system("color fc");
	string choice;     // choice接收用户的指令
	while(true)
	{
		system("cls");     // 清屏
		system("title Draw It!");     // 设置窗口标题
		fflush(stdin);
		printDirection();

		getline(cin,choice);
		if(choice.size()!=1)   // 输入字符过多，说明输入有误
		{
			cerr << "输入错误，请重新输入。" << endl;
			system("pause");
			continue;
		}
		switch(choice[0])
		{
		case '1':system("cls"); make_Line(); break;   // 画一条线段
		case '2':system("cls"); break;   // 画一个圆
		case '3':system("cls"); break;   // 画一个椭圆
		case '4':system("cls"); break;   // 画个多边形
		case '5':system("cls"); break;   // 画一条星形线
		case '6':system("cls"); break;   // 画一条心形线
		case '7':system("cls"); break;   // 查看已经画过的图形
		case '0':system("cls"); saveAndExit(); return 0;   // 保存并退出

		default:
			{
				cerr << "输入错误，请重新输入。" << endl;
				system("pause");
				continue;
			}
		}
	}

	return 0;
}
