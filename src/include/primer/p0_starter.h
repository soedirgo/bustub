//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "common/exception.h"

namespace bustub {

/**
 * The Matrix type defines a common
 * interface for matrix operations.
 */
template <typename T>
class Matrix {
 protected:
  /**
   * TODO(P0): Add implementation
   *
   * Construct a new Matrix instance.
   * @param rows The number of rows
   * @param cols The number of columns
   *
   */
  Matrix(int rows, int cols) {
    rows_ = rows;
    cols_ = cols;
    linear_ = reinterpret_cast<T *>(std::malloc(rows * cols * sizeof(T)));
  }

  /** The number of rows in the matrix */
  int rows_;
  /** The number of columns in the matrix */
  int cols_;

  /**
   * TODO(P0): Allocate the array in the constructor.
   * TODO(P0): Deallocate the array in the destructor.
   * A flattened array containing the elements of the matrix.
   */
  T *linear_;

 public:
  /** @return The number of rows in the matrix */
  virtual int GetRowCount() const = 0;

  /** @return The number of columns in the matrix */
  virtual int GetColumnCount() const = 0;

  /**
   * Get the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @return The (i,j)th matrix element
   * @throws OUT_OF_RANGE if either index is out of range
   */
  virtual T GetElement(int i, int j) const = 0;

  /**
   * Set the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @param val The value to insert
   * @throws OUT_OF_RANGE if either index is out of range
   */
  virtual void SetElement(int i, int j, T val) = 0;

  /**
   * Fill the elements of the matrix from `source`.
   *
   * Throw OUT_OF_RANGE in the event that `source`
   * does not contain the required number of elements.
   *
   * @param source The source container
   * @throws OUT_OF_RANGE if `source` is incorrect size
   */
  virtual void FillFrom(const std::vector<T> &source) = 0;

  /**
   * Destroy a matrix instance.
   * TODO(P0): Add implementation
   */
  virtual ~Matrix() { std::free(linear_); };
};

/**
 * The RowMatrix type is a concrete matrix implementation.
 * It implements the interface defined by the Matrix type.
 */
template <typename T>
class RowMatrix : public Matrix<T> {
 public:
  /**
   * TODO(P0): Add implementation
   *
   * Construct a new RowMatrix instance.
   * @param rows The number of rows
   * @param cols The number of columns
   */
  RowMatrix(int rows, int cols) : Matrix<T>(rows, cols) {}

  /**
   * TODO(P0): Add implementation
   * @return The number of rows in the matrix
   */
  int GetRowCount() const override { return this->rows_; }

  /**
   * TODO(P0): Add implementation
   * @return The number of columns in the matrix
   */
  int GetColumnCount() const override { return this->cols_; }

  /**
   * TODO(P0): Add implementation
   *
   * Get the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @return The (i,j)th matrix element
   * @throws OUT_OF_RANGE if either index is out of range
   */
  T GetElement(int i, int j) const override {
    if (i < 0 || i >= this->rows_ || j < 0 || j >= this->cols_) {
      throw Exception(ExceptionType::OUT_OF_RANGE,
                      "i: " + std::to_string(i) + ", rows: " + std::to_string(this->rows_) +
                          ", j: " + std::to_string(j) + ", cols: " + std::to_string(this->cols_));
    }
    return this->linear_[i * this->cols_ + j];
  }

  /**
   * Set the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @param val The value to insert
   * @throws OUT_OF_RANGE if either index is out of range
   */
  void SetElement(int i, int j, T val) override {
    if (i < 0 || i >= this->rows_ || j < 0 || j >= this->cols_) {
      throw Exception(ExceptionType::OUT_OF_RANGE,
                      "i: " + std::to_string(i) + ", rows: " + std::to_string(this->rows_) +
                          ", j: " + std::to_string(j) + ", cols: " + std::to_string(this->cols_));
    }
    this->linear_[i * this->cols_ + j] = val;
  }

  /**
   * TODO(P0): Add implementation
   *
   * Fill the elements of the matrix from `source`.
   *
   * Throw OUT_OF_RANGE in the event that `source`
   * does not contain the required number of elements.
   *
   * @param source The source container
   * @throws OUT_OF_RANGE if `source` is incorrect size
   */
  void FillFrom(const std::vector<T> &source) override {
    if (source.size() != static_cast<size_t>(this->rows_ * this->cols_)) {
      throw Exception(ExceptionType::OUT_OF_RANGE, "len: " + std::to_string(source.size()) +
                                                       ", rows: " + std::to_string(this->rows_) +
                                                       ", cols: " + std::to_string(this->cols_));
    }

    std::memcpy(this->linear_, source.data(), source.size() * sizeof(T));
  }

  /**
   * TODO(P0): Add implementation
   *
   * Destroy a RowMatrix instance.
   */
  ~RowMatrix() override = default;

 private:
  /**
   * A 2D array containing the elements of the matrix in row-major format.
   *
   * TODO(P0):
   * - Allocate the array of row pointers in the constructor.
   * - Use these pointers to point to corresponding elements of the `linear` array.
   * - Don't forget to deallocate the array in the destructor.
   */
  T **data_;
};

/**
 * The RowMatrixOperations class defines operations
 * that may be performed on instances of `RowMatrix`.
 */
template <typename T>
class RowMatrixOperations {
 public:
  /**
   * Compute (`matrixA` + `matrixB`) and return the result.
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @return The result of matrix addition
   */
  static std::unique_ptr<RowMatrix<T>> Add(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB) {
    // TODO(P0): Add implementation
    int rows = matrixA->GetRowCount();
    int cols = matrixA->GetColumnCount();

    if (rows != matrixB->GetRowCount() || cols != matrixB->GetColumnCount()) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    auto m = std::make_unique<RowMatrix<T>>(rows, cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        m->SetElement(i, j, matrixA->GetElement(i, j) + matrixB->GetElement(i, j));
      }
    }
    return m;
  }

  /**
   * Compute the matrix multiplication (`matrixA` * `matrixB` and return the result.
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @return The result of matrix multiplication
   */
  static std::unique_ptr<RowMatrix<T>> Multiply(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB) {
    // TODO(P0): Add implementation
    int rows = matrixA->GetRowCount();
    int cols = matrixB->GetColumnCount();
    int stride = matrixA->GetColumnCount();

    if (stride != matrixB->GetRowCount()) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    auto m = std::make_unique<RowMatrix<T>>(rows, cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int sum = 0;
        for (int k = 0; k < stride; k++) {
          sum += matrixA->GetElement(i, k) * matrixB->GetElement(k, j);
        }
        m->SetElement(i, j, sum);
      }
    }
    return m;
  }

  /**
   * Simplified General Matrix Multiply operation. Compute (`matrixA` * `matrixB` + `matrixC`).
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @param matrixC Input matrix
   * @return The result of general matrix multiply
   */
  static std::unique_ptr<RowMatrix<T>> GEMM(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB,
                                            const RowMatrix<T> *matrixC) {
    // TODO(P0): Add implementation
    return std::unique_ptr<RowMatrix<T>>(nullptr);
  }
};
}  // namespace bustub
