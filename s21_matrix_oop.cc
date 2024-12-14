#include "s21_matrix_oop.h"

#include <cmath>
#include <iostream>

S21Matrix::~S21Matrix() { RemoveMatrix(); }

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows < 0 || cols < 0) {
    throw std::out_of_range("rows or cols less '0'");
  }
  CreateMatrix(rows, cols);
}

void S21Matrix::CreateMatrix(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;
  this->matrix_ = new double*[rows_];
  if (this->matrix_ == nullptr) std::bad_alloc();
  for (int i = 0; i < rows_; ++i) this->matrix_[i] = new double[cols_]();
  if (this->matrix_ == nullptr) {
    std::bad_alloc();
    for (int j = 0; j < rows_; ++j) delete[] this->matrix_[j];
  }
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  CreateMatrix(rows_, cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) this->matrix_[i][j] = other.matrix_[i][j];
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) {
  std::swap(other.rows_, rows_);
  std::swap(other.cols_, cols_);
  std::swap(other.matrix_, matrix_);
}

void S21Matrix::RemoveMatrix() {
  if (this->matrix_ != nullptr) {
    for (int i = 0; i < rows_; ++i) delete[] this->matrix_[i];
  }
  delete[] this->matrix_;
  rows_ = 0;
  cols_ = 0;
  matrix_ = nullptr;
}

bool S21Matrix::EqMatrix(const S21Matrix& other) noexcept {
  bool flag = true;
  if (matrix_ == nullptr || other.matrix_ == nullptr) flag = false;
  if (rows_ != other.rows_ || cols_ != other.cols_) flag = false;
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (std::fabs(matrix_[i][j] - other.matrix_[i][j]) > 1e-7) flag = false;
    }
  }
  return flag;
}
void S21Matrix::SumSub(const S21Matrix& other, int znak) {
  if (rows_ == other.rows_ && cols_ == other.cols_) {
    for (int i = 0; i < other.rows_; ++i) {
      for (int j = 0; j < other.cols_; ++j)
        matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j] * znak;
    }
  } else {
    throw std::out_of_range("rows and cols should be equal");
  }
}

void S21Matrix::SumMatrix(const S21Matrix& other) { SumSub(other, 1); }

void S21Matrix::SubMatrix(const S21Matrix& other) { SumSub(other, -1); }

void S21Matrix::MulNumber(const double num) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j)
      this->matrix_[i][j] = this->matrix_[i][j] * num;
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) throw std::invalid_argument("cols != other.rows");
  S21Matrix matrix_mul(rows_, other.cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      for (int y = 0; y < cols_; ++y)
        matrix_mul.matrix_[i][j] += matrix_[i][y] * other.matrix_[y][j];
    }
  }
  *this = std::move(matrix_mul);
}
//  Операторы
S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix result{*this};
  return result += other;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix result{*this};
  return result -= other;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix result(*this);
  return result *= other;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

S21Matrix S21Matrix::operator*(const double num) {
  S21Matrix result{*this};
  result.MulNumber(num);
  return result;
}

S21Matrix operator*(const double num, const S21Matrix& other) {
  S21Matrix result{other};
  return result *= num;
}

double& S21Matrix::operator()(int i, int j) {
  if (i >= this->rows_ || j >= this->cols_ || i < 0 || j < 0) {
    throw std::out_of_range("Index out of range");
  }
  return this->matrix_[i][j];
}

double& S21Matrix::operator()(int i, int j) const {
  if (i >= this->rows_ || j >= this->cols_ || i < 0 || j < 0) {
    throw std::out_of_range("Index out of range");
  }
  return this->matrix_[i][j];
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    this->RemoveMatrix();
    CreateMatrix(other.rows_, other.cols_);
    for (int i = 0; i < rows_; ++i) {
      std::copy(other.matrix_[i], other.matrix_[i] + cols_, matrix_[i]);
    }
  }
  return *this;
}

bool S21Matrix::operator==(const S21Matrix& other) { return EqMatrix(other); }

void S21Matrix::Set(int new_rows, int new_cols) {
  if (new_rows < 0 || new_cols < 0) {
    throw std::out_of_range("newRows or newCols should be less than 0");
  }
  S21Matrix new_matrix(new_rows, new_cols);
  int min_rows = 0;
  int min_cols = 0;
  if (new_rows > rows_) {
    min_rows = rows_;
  } else {
    min_rows = new_rows;
  }
  if (new_cols > cols_) {
    min_cols = cols_;
  } else {
    min_cols = new_cols;
  }
  for (int i = 0; i < min_rows; ++i) {
    for (int j = 0; j < min_cols; ++j) {
      new_matrix.matrix_[i][j] = matrix_[i][j];
    }
  }
  *this = std::move(new_matrix);
}

S21Matrix S21Matrix::Transpose() noexcept {
  S21Matrix new_matrix(this->cols_, this->rows_);
  for (int i = 0; i < this->rows_; ++i) {
    for (int j = 0; j < this->cols_; ++j) {
      new_matrix.matrix_[j][i] = this->matrix_[i][j];
    }
  }
  return new_matrix;
}

double S21Matrix::Determinant() {
  double result = 0.0;
  if (this->matrix_ == nullptr) {
    throw std::invalid_argument("Matrix is empty");
  } else if (this->rows_ == this->cols_) {
    if (this->rows_ == 1) {
      result = this->matrix_[0][0];
    } else if (this->rows_ == 2) {
      result = this->matrix_[0][0] * this->matrix_[1][1] -
               this->matrix_[0][1] * this->matrix_[1][0];
    } else {
      S21Matrix minor_matrix(this->rows_ - 1, this->cols_ - 1);
      for (int i = 0; i < this->cols_; ++i) {
        minor_matrix.Minor(0, i, *this);
        result += this->matrix_[0][i] * minor_matrix.Determinant() * pow(-1, i);
      }
    }
  } else {
    throw std::invalid_argument("rows and cols should be equal");
  }
  return result;
}

S21Matrix S21Matrix::Minor(int rows_zero, int cols_zero,
                           const S21Matrix& matrix) {
  for (int i = 0; i < matrix.rows_; ++i) {
    for (int j = 0; j < matrix.cols_; ++j) {
      if (i != rows_zero && j != cols_zero) {
        int i_m = i;
        int j_m = j;
        if (i > rows_zero) --i_m;
        if (j > cols_zero) --j_m;
        matrix_[i_m][j_m] = matrix.matrix_[i][j];
      }
    }
  }
  return *this;
}

S21Matrix S21Matrix::CalcComplements() {
  if (this->rows_ != this->cols_) {
    throw std::invalid_argument("rows and cols should be equal");
  }
  S21Matrix result(this->rows_, this->cols_);
  S21Matrix minor_matrix(this->rows_ - 1, this->cols_ - 1);
  for (int i = 0; i < this->rows_; ++i) {
    for (int j = 0; j < this->cols_; ++j) {
      double determinant = 0.0;
      minor_matrix.Minor(i, j, *this);
      determinant = minor_matrix.Determinant();
      result.matrix_[i][j] = determinant * pow(-1, (i + j));
    }
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() {
  double determinant = 0.0;
  determinant = this->Determinant();
  if (std::fabs(determinant) < 1e-7) {
    throw std::invalid_argument("determinant can't be 0");
  }
  S21Matrix result(this->rows_, this->cols_);
  if (rows_ == 1) {
    result(0, 0) = 1 / matrix_[0][0];
  } else {
    S21Matrix calc_matrix = this->CalcComplements();
    result = calc_matrix.Transpose();
    result.MulNumber(1 / determinant);
  }
  return result;
}