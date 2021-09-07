#include <iostream>
#include<algorithm>
#include<cmath>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <stdexcept>

using namespace std;

double **A, *B, *X;
int n;
const double eps = 0.000000001;

wchar_t digitToSuperscript(unsigned int digit)
{
	if (digit >= 10)
		throw std::out_of_range("digit");
	switch (digit)
	{
	case 1:
		return 0x00B9;
	case 2:
		return 0x00B2;
	case 3:
		return 0x00B3;
	default:
		return 0x2070 + digit;
	}
}

double* Gauss(double **, double *, int);
void Print_system(double**, double*, int);
void Input_system();
void Solve();

int main(){
	setlocale(LC_ALL, "RUS");
	
	Input_system();
	/*Print_system(A, B, n);*/
	Solve();
	return 0;
}

double * Gauss(double **A, double *B, int n){
	double max_elem;
	int t, max_elemi;
	X = new double[n];
	t = 0;
	while (t < n)
	{
		// Поиск строки с максимальным по модулю элементом A[i][t]
		max_elem = abs(A[t][t]);
		max_elemi = t;
		for (int i = t + 1; i < n; i++)
		{
			if (abs(A[i][t]) > max_elem)
			{
				max_elem = abs(A[i][t]);
				max_elemi = i;
			}
		}
		// Перестановка максимальной строки 
		if (max_elem < eps){
			cout << "Решение получить невозможно";
			return 0;
		}
		for (int j = 0; j < n; j++){
			swap(A[t][j],A[max_elemi][j]);
		}
		swap(B[t], B[max_elemi]);

		// Нормируем 
		for (int i = t; i < n; i++){
			double temp = A[i][t];
			if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
			for (int j = 0; j < n; j++)
				A[i][j] = A[i][j] / temp;
			B[i] = B[i] / temp;
			if (i == t)  continue; // уравнение не вычитать само из себя
			for (int j = 0; j < n; j++)
				A[i][j] = A[i][j] - A[t][j];
			B[i] = B[i] - B[t];
		}
		t++;
	}
	// обратная подстановка
	for (int p = n - 1; p >= 0; p--){
		X[p] = B[p];
		for (int i = 0; i < p; i++)
			B[i] = B[i] - A[i][p] * X[p];
	}

	return X;
}
void Input_system(){
	double x;
	cout << "Введите количество уравнений: ";
	cin >> n;
	A = new double*[n];
	B = new double[n];
	for (int i = 0; i < n; i++){
		A[i] = new double[n];
		cout << "X[" << i+1 << "] = ";
		cin >> x;
			A[i][0] = 1;
			for (int j = 1; j < n; j++){
				A[i][j] = pow(x, j);
			}
	}
	for (int i = 0; i < n; i++){
		cout << "F(X[" << i + 1 << "]) = ";
		cin >> B[i];
	}
}

void Print_system(double** A, double* B, int n)
{
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			cout << A[i][j] << " * X[" << j+1<<"]";
			if (j < n - 1)
				cout << " + ";
		}
		cout << " = " << B[i] << endl;
	}
	return;
}

void Solve(){
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	X = Gauss(A, B, n);
	wcout << endl;
	wcout << "         ";
	if (X != NULL){
		for (int i = n - 1; i >= 0; i--){
			if (abs(X[i]) > eps){
				wcout << X[i];
			}
			else{
				continue;
			}
			if (i != 0){
				std::wcout << L"x";
			}
			if (i != 1 && i != 0){
				std::wcout << digitToSuperscript(i);
			}
			if (i != 0){
				if (X[i - 1] > 0){
					wcout << "+";
				}
			}
		}
	}
	wcout << endl << endl;

	return;
}