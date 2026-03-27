#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <windows.h>

using namespace std;

const double EPS = 0.001;
const int MAX_ITERATIONS = 1000;

// Функция для вывода вектора
void printVector(const vector<double>& vec) {
    for (double val : vec) {
        cout << fixed << setprecision(6) << val << " ";
    }
    cout << endl;
}

// Функция для вывода матрицы
void printMatrix(const vector<vector<double>>& mat) {
    for (const auto& row : mat) {
        printVector(row);
    }
}

// Функция для решения системы методом LU-разложения
vector<double> solveLU(const vector<vector<double>>& A, const vector<double>& b) {
    int n = A.size();
    vector<vector<double>> L(n, vector<double>(n, 0.0)), U(n, vector<double>(n, 0.0));

    // LU-разложение
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i <= j) {
                U[i][j] = A[i][j];
                for (int k = 0; k < i; k++) {
                    U[i][j] -= L[i][k] * U[k][j];
                }
            }
            if (i > j) {
                L[i][j] = A[i][j];
                for (int k = 0; k < j; k++) {
                    L[i][j] -= L[i][k] * U[k][j];
                }
                L[i][j] /= U[j][j];
            }
        }
    }

    // Прямой ход
    vector<double> y(n, 0.0);
    for (int i = 0; i < n; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
    }

    // Обратный ход
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }

    return x;
}

// Функция для решения системы методом Зейделя
vector<double> solveGaussSeidel(const vector<vector<double>>& A, const vector<double>& b) {
    int n = A.size();
    vector<double> x(n, 0.0), x_new(n, 0.0);
    double sum;
    double norm_diff;
    int iteration = 0;

    do {
        x = x_new;
        for (int i = 0; i < n; i++) {
            sum = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    sum -= A[i][j] * x[j];
                }
            }
            x_new[i] = sum / A[i][i];
        }
        // Вычисляем норму разности векторов
        norm_diff = 0.0;
        for (int i = 0; i < n; ++i) {
            norm_diff += (x_new[i] - x[i]) * (x_new[i] - x[i]);
        }
        norm_diff = sqrt(norm_diff);
        iteration++;
    } while (norm_diff > EPS && iteration < MAX_ITERATIONS);

    if (iteration == MAX_ITERATIONS) {
        cout << "Previsheno maksimalnoe kolichestvo iteraciy. Metod ne soshelsya." << endl;
    }

    return x_new;
}

// Функция для решения системы методом простых итераций
vector<double> solveSimpleIteration(const vector<vector<double>>& B, const vector<double>& f, const vector<double>& x0) {
    int n = B.size();
    vector<double> x = x0;
    vector<double> x_new(n, 0.0);
    double norm_diff;
    int iteration = 0;

    do {
        for (int i = 0; i < n; ++i) {
            double sum = f[i];
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    sum += B[i][j] * x[j];
                }
            }
            x_new[i] = sum;
        }

        // Вычисляем норму разности векторов
        norm_diff = 0.0;
        for (int i = 0; i < n; ++i) {
            norm_diff += (x_new[i] - x[i]) * (x_new[i] - x[i]);
        }
        norm_diff = sqrt(norm_diff);

        x = x_new;
        iteration++;
    } while (norm_diff > EPS && iteration < MAX_ITERATIONS);

    if (iteration == MAX_ITERATIONS) {
        cout << "Previsheno maksimalnoe kolichestvo iteraciy. Metod ne soshelsya." << endl;
    }

    return x;
}

int main() {
    
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    system("chcp 1251 > nul");
    
    // Инициализация системы уравнений
    vector<vector<double>> A = { 
        {0.89, -0.04, 0.21, -18}, 
        {0.25, -1.23, 0.08, -0.09}, 
        {-0.21, 0.08, 0.8, -0.13}, 
        {0.15, -1.31, 0.06, -1.21} 
    };
    vector<double> b = { -1.24, -1.21, 2.56, 0.89 };

    cout << "========================================" << endl;
    cout << "RESHENIE SISTEM LINEYNYH URAVNENIY" << endl;
    cout << "========================================" << endl;
    
    // Вывод исходной системы
    cout << "\nIshodnaya sistema:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "  ";
        for (int j = 0; j < 4; j++) {
            cout << showpos << A[i][j] << noshowpos << " * x" << j+1;
            if (j < 3) cout << " + ";
        }
        cout << " = " << b[i] << endl;
    }
    cout << endl;

    // Решение системы прямым методом LU-разложения
    vector<double> x_lu = solveLU(A, b);
    cout << "1) Reshenie sistemym pryamym metodom LU-razlozheniya:" << endl;
    cout << "   x1 = " << fixed << setprecision(6) << x_lu[0] << endl;
    cout << "   x2 = " << fixed << setprecision(6) << x_lu[1] << endl;
    cout << "   x3 = " << fixed << setprecision(6) << x_lu[2] << endl;
    cout << "   x4 = " << fixed << setprecision(6) << x_lu[3] << endl;
    cout << endl;

    // Решение системы приближенным методом Зейделя
    vector<double> x_gauss = solveGaussSeidel(A, b);
    cout << "2) Reshenie sistemym priblizhennym metodom Zeydelya:" << endl;
    cout << "   x1 = " << fixed << setprecision(6) << x_gauss[0] << endl;
    cout << "   x2 = " << fixed << setprecision(6) << x_gauss[1] << endl;
    cout << "   x3 = " << fixed << setprecision(6) << x_gauss[2] << endl;
    cout << "   x4 = " << fixed << setprecision(6) << x_gauss[3] << endl;
    cout << endl;

    // Преобразование системы к каноническому виду
    vector<vector<double>> B(4, vector<double>(4, 0.0));
    vector<double> f(4, 0.0);
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            if (i == j) {
                B[i][j] = 0.0;
            }
            else {
                B[i][j] = -A[i][j] / A[i][i];
            }
        }
        f[i] = b[i] / A[i][i];
    }

    // Начальное приближение
    vector<double> x0 = { 0.0, 0.0, 0.0, 0.0 };

    // Решение системы итерационным методом
    vector<double> x_iter = solveSimpleIteration(B, f, x0);
    cout << "3) Reshenie sistemym iteracionnym metodom:" << endl;
    cout << "   x1 = " << fixed << setprecision(6) << x_iter[0] << endl;
    cout << "   x2 = " << fixed << setprecision(6) << x_iter[1] << endl;
    cout << "   x3 = " << fixed << setprecision(6) << x_iter[2] << endl;
    cout << "   x4 = " << fixed << setprecision(6) << x_iter[3] << endl;
    cout << endl;

    // Сравнение результатов
    cout << "========================================" << endl;
    cout << "SRAVNENIE REZULTATOV" << endl;
    cout << "========================================" << endl;
    cout << "Metod\t\tx1\t\tx2\t\tx3\t\tx4" << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "LU-razlozhenie\t";
    for (int i = 0; i < 4; i++) cout << fixed << setprecision(6) << x_lu[i] << "\t";
    cout << endl;
    cout << "Zeydelya\t";
    for (int i = 0; i < 4; i++) cout << fixed << setprecision(6) << x_gauss[i] << "\t";
    cout << endl;
    cout << "Iteracionnyy\t";
    for (int i = 0; i < 4; i++) cout << fixed << setprecision(6) << x_iter[i] << "\t";
    cout << endl;
    cout << "----------------------------------------------------------------" << endl;
    
    // Вычисление погрешности
    cout << "\nPogreshnost metodov:" << endl;
    double err_gauss = 0.0, err_iter = 0.0;
    for (int i = 0; i < 4; i++) {
        err_gauss += (x_lu[i] - x_gauss[i]) * (x_lu[i] - x_gauss[i]);
        err_iter += (x_lu[i] - x_iter[i]) * (x_lu[i] - x_iter[i]);
    }
    err_gauss = sqrt(err_gauss);
    err_iter = sqrt(err_iter);
    cout << "  Pogreshnost metoda Zeydelya: " << fixed << setprecision(8) << err_gauss << endl;
    cout << "  Pogreshnost iteracionnogo metoda: " << fixed << setprecision(8) << err_iter << endl;
    cout << endl;

    cout << "========================================" << endl;
    cout << "KORNI SISTEMY URAVNENIY:" << endl;
    cout << "========================================" << endl;
    cout << "  x1 = " << fixed << setprecision(6) << x_lu[0] << endl;
    cout << "  x2 = " << fixed << setprecision(6) << x_lu[1] << endl;
    cout << "  x3 = " << fixed << setprecision(6) << x_lu[2] << endl;
    cout << "  x4 = " << fixed << setprecision(6) << x_lu[3] << endl;
    cout << "========================================" << endl;

    return 0;
}
