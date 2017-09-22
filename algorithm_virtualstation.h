#ifndef ALGORITHM_VIRTUALSTATION_H
#define ALGORITHM_VIRTUALSTATION_H
///*class createvirtualstation
//{
//public:
//   void Creat_virtualstation(float *estimate_distance);//根据估计距离产生虚拟基站的位置
//   ~Creat_virtualstation();//一个类只有一个析构函数，程序结束产生析构，释放内存
//};*/

//class createvirtualstation
//{
//public:
//    createvirtualstation(float *estimate_distance);//根据估计距离产生虚拟基站的位置
//    ~createvirtualstation();//一个类只有一个析构函数，程序结束产生析构，释放内存
//};
//class creatcoordinatesystem
//{
//public:
//    creatcoordinatesystem();
//    ~creatcoordinatesystem();
//};

#include<cmath>
//定义矩阵类
class Matrixvs
{
public:
    Matrixvs(int dims=2)
    {
        index=dims;
        MatrixvsA=new float[index*index];
    }
    ~Matrixvs()
    {
        delete MatrixvsA;
    }
    void setMatrixvs(float *rmatr)
    {
        for(int i;i<index*index;i++)
        {
           *(MatrixvsA+i)=rmatr[i];
        }
    }
    void printMvs();
protected:
    int index;
    float *MatrixvsA;
};
//线性方程组类
class Linequ:public Matrixvs
{
public:
    Linequ(int dims=2):Matrixvs(dims)
    {
        sums=new float[dims];
        solu=new float[dims];
    }
    ~Linequ();
    void setLinequ(float *a,float *b);
    void printL();
//    int Solve();
//    void showX();

protected:
    float *sums;
    float *solu;
};


#endif // ALGORITHM_VIRTUALSTATION_H
