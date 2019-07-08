/* *******************************************************
|               面向对象程序设计综合应用
|     选    题  画图软件的设计与实现 Draw It
|     姓    名  黄远鹏      学    号  183401050225
|     班    级  软件1802
|     完成时间  2019年7月7日
\  ******************************************************* */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <regex>
using namespace std;

const double PI=acos(-1.0);     // PI 圆周率
// equals0：判断浮点数是否为0     返回值：true（为0），false（非0）
inline bool equals0(double num)
{
	return fabs(num) < 1e-8;
}
class Shape     // Shape类：作为其他类的基类
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
	virtual ~Shape() {}
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
	double getX() { return x; }
	double getY() { return y; }
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << "( " << x << " , " << y << " )";
		return os;
	}
	~Point() {}
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
		os << "=0，长度 = " << m.dist(n) << "，端点坐标 ";
		m.output(os) << " ";
		n.output(os);
		return os;
	}
	~Line() {}
};
// make_Line：根据用户输入，生成一条线段并添加其地址到addr中
void make_Line()
{
	system("title Draw a Line");
	double x1,y1,x2,y2;
	cout << "请输入线段两个端点的坐标，用空格隔开：\n" << endl;
	cout << "第一个点x,y坐标:" << ends;
	cin >> x1 >> y1;
	cout << "第二个点x,y坐标:" << ends;
	cin >> x2 >> y2;
	while(fabs(x1 - x2) < 1e-8 && fabs(y1 - y2) < 1e-8)     // 两点重合，不能构成线段
	{
		cout << "\n输入的两点重合，不能构成线段。请重新输入：" << endl;
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
class Circle : public Shape     // 圆类
{
private:
	Point o;    // 圆心
	double r;   // 半径
public:
	Circle() {}
	Circle(double x_,double y_,double r_):Shape(PI*r_*2,PI*r_*r_),o(x_,y_),r(r_) {}
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << "圆：方程：" << showpos;
		if(equals0(o.getX())) os << "x^2";   // 圆心在y轴上，不输出横坐标
		else os << "(x"  << -o.getX() << ")^2";
		if(equals0(o.getY())) os << " + y^2";   // 圆心在x轴上，不输出纵坐标
		else os << " + (y" << -o.getY() << ")^2";
		os << noshowpos << " = " << r*r << "，圆心 ";
		o.output(os) << "，半径 r = " << r << "，";
		Shape::output(os);
		return os;
	}
	~Circle() {}
};
// make_Circle：根据用户输入，生成一个圆并添加其地址到addr中
void make_Circle()
{
	system("title Draw a Circle");
	double x,y,r;
	cout << "请输入圆心的横纵坐标，用空格隔开:" << ends;
	cin >> x >> y;
	cout << "请输入圆的半径:" << ends;
	cin >> r;
	while(r<=0)  // 半径必须大于0
	{
		cout << "\n输入错误，圆的半径应大于0，请重新输入：\n" << endl;
		cout << "请输入圆心的横纵坐标，用空格隔开:" << ends;
		cin >> x >> y;
		cout << "请输入圆的半径:" << ends;
		cin >> r;
	}
	Circle *c = new Circle(x,y,r);
	addr.push_back(c);
	cout << endl << "添加圆成功：" << endl;
	c->output() << endl;
	system("pause");
	return;
}
class Ellipse : public Shape     // 椭圆类
{
private:
	double a,b,c,e;   // a长半轴，b短半轴，c焦半径，e离心率
public:
	Ellipse() {}
	Ellipse(double a_,double b_):Shape(),a(a_),b(b_)
	{
		double p = a>b ? a : b,q = a>b ? b : a;
		c=sqrt(p*p - q*q);
		perimeter=PI*(b+a)*(1+3*pow((b-a)/(b+a),2)/(10+sqrt(4-3*pow((b-a)/(b+a),2))));
		area=PI*a*b;
		e=c/p;
	}
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << "椭圆：方程：";
		os << "x^2";
		if(!equals0(a-1)) os << "/" << a*a;   // a=1时不输出a^2
		os << " + y^2";
		if(!equals0(b-1)) os << "/" << b*b;   // b=1时不输出b^2
		os << " = 1，长轴长 = " << ( (a>b) ? a*2 : b*2 ) 
			<< "，短轴长 = " << ( (a>b) ? b*2 : a*2 )
			<< "，焦距 = " << c*2 << "，离心率 = " << e
			<< "，周长约为" << perimeter << "，面积 = " << area;
		return os;
	}
	~Ellipse() {}
};
// make_Ellipse：根据用户输入，生成一个椭圆并添加其地址到addr中
void make_Ellipse()
{
	system("title Draw a Ellipse");
	double a,b;
	cout << "请输入椭圆方程的参数：\n" << endl;
	cout << "参数a:" << ends;
	cin >> a;
	cout << "参数b:" << ends;
	cin >> b;
	while(a<=0 || b<=0)   // 输入参数非法
	{
		cout << "\n输入错误，请重新输入：\n" << endl;
		cout << "请输入椭圆方程的参数：" << endl;
		cout << "参数a:" << ends;
		cin >> a;
		cout << "参数b:" << ends;
		cin >> b;
	}
	if(equals0(a-b))   // a==b,其实是个圆
	{
		cout << "您输入的a,b值相等，请问是否要画个圆？" << endl;
		cout << "        1 - 画个圆\n        2 - 返回主菜单" << endl;
		cout << "请选择:" << ends;
		int choice1;
		cin >> choice1;
		if(choice1==1)
		{
			Circle *c = new Circle(0,0,a);
			addr.push_back(c);
			cout << endl << "添加圆成功：" << endl;
			c->output() << endl;
			system("pause");
		}
		return;
	}
	Ellipse *e = new Ellipse(a,b);
	addr.push_back(e);
	cout << endl << "添加椭圆成功：" << endl;
	e->output() << endl;
	system("pause");
	return;
}
class Astroid : public Shape     // 星形线
{
private:
	double a;   // 星形线方程唯一的参数
public:
	Astroid() {}
	Astroid(double a_):Shape(a_*6,(PI*a_*a_*3)/8),a(a_) {}
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << "星形线：参数方程：";
		os << "x = " << a << " cos^3(t) , y = " << a << " sin^3(t)，";
		Shape::output(os);
		return os;
	}
	~Astroid() {}
};
// make_Astroid：根据用户输入，生成一条星形线并添加其地址到addr中
void make_Astroid()
{
	system("title Draw a Astroid");
	double a;
	cout << "请输入星形线的参数a:" << ends;
	cin >> a;
	while(a<=0)   // 输入参数非法
	{
		cout << "\n输入错误，请重新输入：\n" << endl;
		cout << "请输入星形线的参数a:" << ends;
		cin >> a;
	}
	Astroid *as = new Astroid(a);
	addr.push_back(as);
	cout << endl << "添加星形线成功：" << endl;
	as->output() << endl;
	system("pause");
	return;
}
class Cardioid : public Shape     // 心形线
{
private:
	double a;
public:
	Cardioid() {}
	Cardioid(double a_):Shape(a_*8,(PI*a_*a_*3)/2),a(a_) {}
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << "心形线：极坐标方程：";
		os << "r = " << a << "(1+cos t)，";
		Shape::output(os);
		return os;
	}
	~Cardioid() {}
};
// make_Cardioid：根据用户输入，生成一条心形线并添加其地址到addr中
void make_Cardioid()
{
	system("title Draw a Cardioid");
	double a;
	cout << "请输入心形线的参数a:" << ends;
	cin >> a;
	while(a<=0)   // 输入参数非法
	{
		cout << "\n输入错误，请重新输入：\n" << endl;
		cout << "请输入心形线的参数a:" << ends;
		cin >> a;
	}
	Cardioid *c = new Cardioid(a);
	addr.push_back(c);
	cout << endl << "添加心形线成功：" << endl;
	c->output() << endl;
	system("pause");
	return;
}
class Polygon : public Shape     // 多边形
{
private:
	int nLength;     // 边的数目
	vector<Point> vertex;     // 多边形的每个顶点
public:
	Polygon() {}
	Polygon(int n,vector<Point> v):Shape(0,0),nLength(n),vertex(v)
	{
		perimeter += v[n-1].dist(v[0]);
		area += v[n-1].getX()*v[0].getY() - v[0].getX()*v[n-1].getY();
		for(int i=0;i<nLength-1;i++)
		{
			perimeter += v[i].dist(v[i+1]);
			area += v[i].getX()*v[i+1].getY() - v[i+1].getX()*v[i].getY();
		}
		area /= 2;
	}
	ostream &output(ostream &os = cout)   // 输出函数
	{
		os << nLength << "边形：顶点坐标：";
		for(auto v=vertex.begin();v!=vertex.end();v++)
			v->output(os) << ",";
		Shape::output(os);
		return os;
	}
	~Polygon() {}
};
// make_Polygon：根据用户输入，生成一个多边形并添加其地址到addr中
void make_Polygon()
{
	system("title Draw a Polygon");
	int n;
	double x,y;
	cout << "您要画几边形:" << ends;
	cin >> n;
	while(n<3)   // 输入非法
	{
		cout << "\n输入错误，请重新输入：\n" << endl;
		cout << "您要画几边形:" << ends;
		cin >> n;
	}
	cout << "请按逆时针顺序输入 " << n << " 个顶点：" << endl;
	vector<Point> v;
	for(int i=1;i<=n;i++)
	{
		cout << "第 " << i << " 个顶点横纵坐标:" << ends;
		cin >> x >> y;
		v.push_back(Point(x,y));
	}
	Polygon *p = new Polygon(n,v);
	addr.push_back(p);
	cout << endl << "添加 " << n << " 边形成功：" << endl;
	p->output() << endl;
	system("pause");
	return;
}
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
// display：查看已经画过的图形
void display()
{
	system("title Display");
	if(addr.empty())   // addr为空
	{
		cerr << "未添加任何图形。" << endl;
		system("pause");
		return;
	}
	cout << "共有 " << addr.size() << " 个图形：" << endl << endl;
	for(auto it=addr.begin(),end=addr.end(); it!=end; ++it)
		(*it)->output() << endl;
	cout << endl;
	system("pause");
	return;
}
// saveAndExit：输出所有图形到指定文件,释放申请的内存
void saveAndExit()
{
	system("title Save and Exit");
	if(addr.empty())   // addr为空
	{
		cerr << "未添加任何图形，无需保存。\n" << endl;
		system("pause");
		return;
	}
	cout << "请输入文件名，以回车结束：";
	string outFile;
	getline(cin,outFile);
	while(outFile.empty())
	{
		cerr << "\n文件名不能为空，请重新输入：";
		getline(cin,outFile);
	}
	regex r("^[^/\\\\:\\*\\?\\<\\>\\|\"]{1,255}$");
	while(!regex_match(outFile,r))     // 输入的文件名非法
	{
		cerr << "\n文件名不能包含下列任何字符  \\ / : * ? \" < > |   也不能超过255个字符" << endl;
		cerr << "请重新输入:" << ends;
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
		case '1':system("cls"); make_Line();     break;   // 画一条线段
		case '2':system("cls"); make_Circle();   break;   // 画一个圆
		case '3':system("cls"); make_Ellipse();  break;   // 画一个椭圆
		case '4':system("cls"); make_Polygon();  break;   // 画个多边形
		case '5':system("cls"); make_Astroid();  break;   // 画一条星形线
		case '6':system("cls"); make_Cardioid(); break;   // 画一条心形线
		case '7':system("cls"); display();       break;   // 查看已经画过的图形
		case '0':system("cls"); saveAndExit();   return 0;   // 保存并退出

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
