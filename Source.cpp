#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
using namespace std;

const int maxpop = 100; // �������� � ���������
const int maxstring = 4; // ������ � ������ ��������
const int lchrom = 4; // ����� �����
const int popsize = 6; //����� ��������� 
const double pmutation = 0.1; // ����������� �������
const double pcross = 0.5;  // ����������� �����������
double min0; // ������������ ����������������� (�������� � ������ )
int p1 = 0;
int p2 = 1; // ��� ����������� 
double sred;

class individual //�����
{
public:

    double chrom[maxstring];  // ������ ������������ �����
    double fenotype; //������ ��������� (�)
    double fitness;  // ����������� ������������ (�������� �������) 

};

individual oldpop[maxpop]; //������ ��������� ���������
individual newpop[maxpop]; // ����� ��������� ���������
individual intpop[maxpop]; // ��������� ��� ���������� 

const double xmax = 4;
const double xmin = -4;

double randomV(double a, double b)
{
    //������������ �����
    double m;
    m = rand() % 6500 + 0;
    double n;
    n = m / 6500;
    //����� �����
    int c = b - a;
    double x;
    x = rand() % c + 0;
    double z;
    z = a + x + n;
    return z;
}
double random_() //����������� (�� 0 �� 1)
{
    double a;
    a = rand() % 6500 + 0;
    double b;
    b = a / 6500;

    return  b;
}

// ���� ������� �������� ����� 1, �� true, ���� ���, �� ������
bool flip(double probability)
{
    double x;
    x = random_();
    return (x <= probability); // ��� ������ probability, ��� ������ ����������� ��������� true
}

//�������� ������� y(x)=(x^2)+4
double objfunc(double x)
{
    return (x*x+4);
}

//����� � ����� ��������� �������� �������� �� ����� ����� 
double decode(double* chrom)
{
    double summ = 0;
    for (int i = 0; i < maxstring; i++)
    {
        summ = summ + chrom[i];
    }
    summ = summ / maxstring;
    return summ;
}

// ����� �������� ������� ������������ � ���������
double statistics(individual* pop)
{
    min0 = pop[0].fitness;
    sred = pop[0].fitness;

    for (int j = 1; j < popsize; j++)
    {
        sred = sred + pop[j].fitness;
        if (pop[j].fitness < min0)
            min0 = pop[j].fitness; //  ������� �������� � ���������
    }
    sred = sred / popsize;
    return min0;
}

// ������� ������ ��������� ���������
void initpop(individual* oldpop)
{

    for (int j = 0; j < popsize; j++) {

        for (int i = 0; i < lchrom; i++)
        {
            oldpop[j].chrom[i] = randomV(xmin, xmax); // ��������� �������� �� -4 �� 0

        }
        oldpop[j].fenotype = decode(oldpop[j].chrom);
        oldpop[j].fitness = objfunc(oldpop[j].fenotype); // ��������� ��� ���������
    }
}


// ���������� �� ����������� ��������� (�������������� � ������) ��������� ����� t=2
void select()
{
    int j1, j2;
    int ipick = 0;

    j1 = ipick;
    j2 = ipick + 1;
    for (int j = 0; j < popsize / 2; j++)
    {
        if (oldpop[j2].fitness < oldpop[j1].fitness) //�������� ����� ��������������� (������ ����� �����) �������- � ������, ������ � �����
        {
            intpop[j] = oldpop[j2];
            intpop[popsize - j - 1] = oldpop[j1];
        }
        else
        {
            intpop[j] = oldpop[j1];
            intpop[popsize - j - 1] = oldpop[j2];
        }
        ipick = ipick + 2;

        j1 = ipick;
        j2 = ipick + 1;

    }

    for (int i = 0; i < popsize; i++)
        oldpop[i] = intpop[i];  // "��������������� ��������� � �������� ������������ � ������ � ������" 
}

//������� � ����������� ��������� 0,01
double mutation(double alleleval)
{
    bool mutate;
    double m;
    mutate = flip(pmutation);
    if (mutate)
    {
        m = alleleval + randomV(-0.5, 0.5);
        if (m >= xmin && m <= xmax)
            return m;
        else return alleleval;
    }
    else
        return alleleval;

}

//�����������
void crossover(int par1, int par2, int ch1, int ch2)
{
    // ���������� ���� ����� �������
    int l1 = rand() % (lchrom - 2) + 1; // ��������� ����� ������� 1
    int l2 = rand() % (lchrom - 2) + 1; // ��������� ����� ������� 2
    int raz1, raz2;
    if (l1 > l2)
    {
        raz1 = l2;
        raz2 = l1;
    }
    if (l1 < l2)
    {
        raz1 = l1;
        raz2 = l2;
    }
    if (l1 = l2)
    {
        if (l1 > lchrom / 2)
        {
            l1 = rand() % (lchrom / 2) + 1;
            raz1 = l1;
            raz2 = l2;
        }
        if (l1 < lchrom / 2)
        {
            l1 = rand() % (lchrom - 1) + lchrom / 2;
            raz1 = l2;
            raz2 = l1;
        }
        if (l1 = lchrom / 2)
        {
            l1 = rand() % (lchrom / 2) + 1;
            raz1 = l1;
            l2 = rand() % (lchrom - 1) + lchrom / 2;
            raz2 = l2;
        }
    }

    int p = rand() % 2 + 1;  // ��������� ���������� 
    if (flip(pcross)) // ����������� ����������� 
    {

        for (int j = 0; j < raz1; j++) // ���� �� 1 ����� �������
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par2].chrom[j]);

            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par1].chrom[j]);
            }
        }
        for (int j = raz1; j < raz2; j++) // ���� �� 1 �� 2 ����� �������
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par1].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par2].chrom[j]);
            }
        }
        for (int j = raz2; j < lchrom; j++) // ���� �� 2  ����� �������
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par2].chrom[j]);

            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par1].chrom[j]);
            }
        }
    }
    else //���� �� �����������, �� ������� ������������ ����� ��������������� �����
    {
        for (int j = 0; j < raz1; j++) // ���� �� 1 ����� �������
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p2].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p1].chrom[j]);
            }
        }
        for (int j = raz1; j < raz2; j++) // ���� �� 1 �� 2 ����� �������
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p1].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p2].chrom[j]);
            }
        }
        for (int j = raz2; j < lchrom; j++) // ���� �� 1 ����� �������
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p2].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p1].chrom[j]);
            }
        }
        //������� � ��������� ���� ������ ��� �����������
        p1 = p1 + 2;
        p2 = p2 + 2;
    }

}

//��������� ������ ��������� 
void generation()
{
    int j, mate1, mate2;
    select(); // ��������� ������ ���������

    cout << "��������������� ���������: " << endl << endl;
    for (int i = 0; i < popsize; i++) //�������� ������ ���������
    {
        cout << "����� " << i + 1 << endl;
        for (int j = 0; j < maxstring; j++)
            cout << oldpop[i].chrom[j] << " ";
        cout << endl;
        cout << "x= " << oldpop[i].fenotype << endl;
        cout << "y(x)= " << oldpop[i].fitness << endl;
    }
    j = 0;
    do {
        mate1 = j;     // ����� ���� ����� ������� ������ 
        mate2 = j + 1;

        // ����������, ��������� ��������� ����� ��������� 
        crossover(mate1, mate2, j, j + 1);

        // ������������ ��� ����� � ������� �������
        newpop[j].fenotype = decode(newpop[j].chrom);
        newpop[j].fitness = objfunc(newpop[j].fenotype);

        // ������������ ��� ����� � ������� �������
        newpop[j + 1].fenotype = decode(newpop[j + 1].chrom);
        newpop[j + 1].fitness = objfunc(newpop[j + 1].fenotype);

        j = j + 2;  //��������� � ��������� ����
    } while (j < popsize);
    cout << "����� ���������:  " << endl << endl;
    for (int i = 0; i < popsize; i++) //�������� ������ ���������
    {
        cout << "����� " << i + 1 << endl;
        for (int j = 0; j < maxstring; j++)
            cout << newpop[i].chrom[j] << " ";
        cout << endl;
        cout << "x= " << newpop[i].fenotype << endl;
        cout << "y(x)= " << newpop[i].fitness << endl;
    }
    //�������� ��� �����������
    p1 = 0;
    p2 = 1;
}


int main()
{
    setlocale(LC_ALL, "rus");
    int gen; //����� ���������
    int maxgen; // ����� ���������
    int nmutation;
    int ncross;
    double t; //���� ����������� ������������ � ���������

    cout << "������� ����� ���������  ";
    cin >> maxgen;

    nmutation = 0;
    ncross = 0;

    initpop(oldpop); //������� ������ ��������� ���������
    t = statistics(oldpop); // ������� �������� ���� ������������ � ������� ��������� (min0 ��������� ������ ����������)
      //������ ��� ������� ����������� ��������
    double* minmass;
    minmass = new double[maxgen];
    // ������ ��� �������� ������� ��������
    double* sredmass;
    sredmass = new double[maxgen];
    minmass[0] = min0;
    sredmass[0] = sred;
    cout << "��������� ���������: " << endl << endl;
    for (int i = 0; i < popsize; i++) //�������� ������ ���������
    {
        cout << "����� " << i + 1 << endl;
        for (int j = 0; j < maxstring; j++)
            cout << oldpop[i].chrom[j] << " ";
        cout << endl;
        cout << "x= " << oldpop[i].fenotype << endl;
        cout << "y(x)= " << oldpop[i].fitness << endl;
    }


    cout << endl;
    cout << "����������� y(x) � ���������: " << minmass[0] << endl;
    cout << "������� y(x) � ���������: " << sredmass[0] << endl;
    gen = 0;
    do
    {
        gen++;
        generation(); // ������� ����� ��������� 
        statistics(newpop); // ������� ������� ����������� ������������ � ���������
        cout << min0 << endl;
        if (min0 < t)
            t = min0;
        for (int i = 0; i < maxpop; i++) // ����� ��������� ���������� � ������ 
            oldpop[i] = newpop[i];
        minmass[gen] = min0;
        sredmass[gen] = sred;
    } while (gen < maxgen); // ������� ��� ���� ������ 

    cout << "�����: " << t;
    cout << endl;

    ofstream Result("d:\\ResultV.txt"); //��������� ����  � �����������

    if (Result.is_open()) // �������� �� ��������
        std::cout << "����� ������� � ���� ResultV!\n\n" << std::endl;
    else
    {
        std::cout << "������ ������ � ����" << std::endl;
        return -1;
    }
    Result << "����������� � ���������: " << endl;
    for (int i = 0; i < maxgen; i++)
    {
        Result << minmass[i] << endl;
    }
    Result << endl;
    Result << "������� � ���������: " << endl;

    for (int i = 0; i < maxgen; i++)
    {
        Result << sredmass[i] << endl;
    }
    Result.close();
}