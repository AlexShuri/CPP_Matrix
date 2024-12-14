#ifndef CPP1_S21_MATRIXPLUS_SRC_S21_MATRIX_OOP_H_
#define CPP1_S21_MATRIXPLUS_SRC_S21_MATRIX_OOP_H_

class S21Matrix {
 public:
  S21Matrix(){};
  ~S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);

  // Funсtions
  int GetRows() const noexcept { return rows_; }
  int GetCols() const noexcept { return cols_; }
  void SetRows(int new_rows) { Set(new_rows, cols_); }
  void SetCols(int new_cols) { Set(rows_, new_cols); }
  void Set(int rows, int cols);
  bool EqMatrix(const S21Matrix& other) noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() noexcept;
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  // Operators
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix operator*(const double num);
  S21Matrix& operator*=(const double num);
  S21Matrix& operator=(const S21Matrix& other);
  friend S21Matrix operator*(const double num, const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  bool operator==(const S21Matrix& other);
  double& operator()(int i, int j);
  double& operator()(int i, int j) const;

 private:
  // Atributes
  int rows_{0};
  int cols_{0};
  double** matrix_ = nullptr;
  // Functions
  void CreateMatrix(int rows, int cols);
  void RemoveMatrix();
  void SumSub(const S21Matrix& other, int znak);
  S21Matrix Minor(int rows_zero, int cols_zero, const S21Matrix& matrix);
};

#endif  // CPP1_S21_MATRIXPLUS_SRC_S21_MATRIX_OOP_H_