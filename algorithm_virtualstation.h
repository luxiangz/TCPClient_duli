#ifndef ALGORITHM_VIRTUALSTATION_H
#define ALGORITHM_VIRTUALSTATION_H
///*class createvirtualstation
//{
//public:
//   void Creat_virtualstation(float *estimate_distance);//���ݹ��ƾ�����������վ��λ��
//   ~Creat_virtualstation();//һ����ֻ��һ������������������������������ͷ��ڴ�
//};*/

//class createvirtualstation
//{
//public:
//    createvirtualstation(float *estimate_distance);//���ݹ��ƾ�����������վ��λ��
//    ~createvirtualstation();//һ����ֻ��һ������������������������������ͷ��ڴ�
//};
//class creatcoordinatesystem
//{
//public:
//    creatcoordinatesystem();
//    ~creatcoordinatesystem();
//};

#include<cmath>
//���������
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
//���Է�������
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
