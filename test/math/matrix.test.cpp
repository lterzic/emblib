#include "emblib/math/matrix.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include <cmath>

using namespace emblib::math;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST_CASE("Matrix fill construction", "[math][matrix]")
{
    matrix<int, 2, 3> m(7);
    matrix<int, 2, 3> expected({{7, 7, 7}, {7, 7, 7}});
    REQUIRE((m == expected).all());
}

TEST_CASE("Matrix array construction element access", "[math][matrix]")
{
    matrix<int, 2, 3> m({{1, 2, 3}, {4, 5, 6}});
    REQUIRE(m(0, 0) == 1);
    REQUIRE(m(0, 1) == 2);
    REQUIRE(m(0, 2) == 3);
    REQUIRE(m(1, 0) == 4);
    REQUIRE(m(1, 1) == 5);
    REQUIRE(m(1, 2) == 6);
}

TEST_CASE("Matrix copy construction is independent", "[math][matrix]")
{
    matrix<int, 2, 3> a({{1, 2, 3}, {4, 5, 6}});
    matrix<int, 2, 3> b(a);
    REQUIRE((a == b).all());

    b(0, 0) = 99;
    REQUIRE(a(0, 0) == 1);
}

TEST_CASE("Matrix move construction", "[math][matrix]")
{
    matrix<int, 2, 3> a({{1, 2, 3}, {4, 5, 6}});
    matrix<int, 2, 3> expected({{1, 2, 3}, {4, 5, 6}});
    matrix<int, 2, 3> b(std::move(a));
    REQUIRE((b == expected).all());
}

// ---------------------------------------------------------------------------
// Element access & mutation
// ---------------------------------------------------------------------------

TEST_CASE("Matrix element write and read", "[math][matrix]")
{
    matrix<int, 3, 3> m(0);
    m(1, 2) = 42;
    REQUIRE(m(1, 2) == 42);
    REQUIRE(m(0, 0) == 0);
}

TEST_CASE("Matrix const element access", "[math][matrix]")
{
    const matrix<int, 2, 2> m({{10, 20}, {30, 40}});
    REQUIRE(m(0, 0) == 10);
    REQUIRE(m(1, 1) == 40);
}

// ---------------------------------------------------------------------------
// Assignment from expression
// ---------------------------------------------------------------------------

TEST_CASE("Matrix assignment from expression", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{5, 6}, {7, 8}});
    matrix<int, 2, 2> c(0);
    c = a + b;
    matrix<int, 2, 2> expected({{6, 8}, {10, 12}});
    REQUIRE((c == expected).all());
}

// ---------------------------------------------------------------------------
// Element-wise binary ops: matrix op matrix
// ---------------------------------------------------------------------------

TEST_CASE("Matrix element-wise addition", "[math][matrix]")
{
    matrix<int, 2, 3> a({{1, 2, 3}, {4, 5, 6}});
    matrix<int, 2, 3> b({{10, 20, 30}, {40, 50, 60}});
    matrix<int, 2, 3> expected({{11, 22, 33}, {44, 55, 66}});
    REQUIRE(((a + b) == expected).all());
}

TEST_CASE("Matrix element-wise subtraction", "[math][matrix]")
{
    matrix<int, 2, 2> a({{10, 20}, {30, 40}});
    matrix<int, 2, 2> b({{1, 2}, {3, 4}});
    matrix<int, 2, 2> expected({{9, 18}, {27, 36}});
    REQUIRE(((a - b) == expected).all());
}

TEST_CASE("Matrix element-wise multiplication", "[math][matrix]")
{
    matrix<int, 2, 2> a({{2, 3}, {4, 5}});
    matrix<int, 2, 2> b({{10, 10}, {10, 10}});
    matrix<int, 2, 2> expected({{20, 30}, {40, 50}});
    REQUIRE(((a * b) == expected).all());
}

TEST_CASE("Matrix element-wise division", "[math][matrix]")
{
    matrix<int, 2, 2> a({{10, 20}, {30, 40}});
    matrix<int, 2, 2> b({{2, 4}, {5, 8}});
    matrix<int, 2, 2> expected({{5, 5}, {6, 5}});
    REQUIRE(((a / b) == expected).all());
}

// ---------------------------------------------------------------------------
// Scalar ops
// ---------------------------------------------------------------------------

TEST_CASE("Matrix scalar addition", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> expected({{11, 12}, {13, 14}});
    REQUIRE(((a + 10) == expected).all());
}

TEST_CASE("Matrix scalar subtraction", "[math][matrix]")
{
    matrix<int, 2, 2> a({{10, 20}, {30, 40}});
    matrix<int, 2, 2> expected({{8, 18}, {28, 38}});
    REQUIRE(((a - 2) == expected).all());
}

TEST_CASE("Matrix scalar multiplication", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> expected({{3, 6}, {9, 12}});
    REQUIRE(((a * 3) == expected).all());
}

TEST_CASE("Matrix scalar division", "[math][matrix]")
{
    matrix<int, 2, 2> a({{10, 20}, {30, 40}});
    matrix<int, 2, 2> expected({{5, 10}, {15, 20}});
    REQUIRE(((a / 2) == expected).all());
}

// ---------------------------------------------------------------------------
// Comparison ops
// ---------------------------------------------------------------------------

TEST_CASE("Matrix equality — all equal", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{1, 2}, {3, 4}});
    REQUIRE((a == b).all());
}

TEST_CASE("Matrix equality — not all equal", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{1, 2}, {3, 5}});
    REQUIRE(!(a == b).all());
}

TEST_CASE("Matrix inequality — all different", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{5, 6}, {7, 8}});
    REQUIRE((a != b).all());
}

TEST_CASE("Matrix inequality — one equal element", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{1, 6}, {7, 8}});
    REQUIRE(!(a != b).all());
}

TEST_CASE("Matrix less-than", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{5, 5}, {5, 5}});
    REQUIRE((a < b).all());
    REQUIRE(!(b < a).all());
}

TEST_CASE("Matrix greater-than", "[math][matrix]")
{
    matrix<int, 2, 2> a({{9, 9}, {9, 9}});
    matrix<int, 2, 2> b({{1, 2}, {3, 4}});
    REQUIRE((a > b).all());
    REQUIRE(!(b > a).all());
}

TEST_CASE("Matrix less-than-or-equal", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{1, 3}, {4, 4}});
    REQUIRE((a <= b).all());
}

TEST_CASE("Matrix greater-than-or-equal", "[math][matrix]")
{
    matrix<int, 2, 2> a({{5, 5}, {5, 5}});
    matrix<int, 2, 2> b({{5, 4}, {3, 5}});
    REQUIRE((a >= b).all());
}

// ---------------------------------------------------------------------------
// all() semantics
// ---------------------------------------------------------------------------

TEST_CASE("all() returns false when one element is zero", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 1}, {1, 0}});
    matrix<int, 2, 2> b({{1, 1}, {1, 1}});
    REQUIRE(!(a == b).all());
}

TEST_CASE("all() returns true for 1x1 equal matrices", "[math][matrix]")
{
    matrix<int, 1, 1> a({{5}});
    matrix<int, 1, 1> b({{5}});
    REQUIRE((a == b).all());
}

// ---------------------------------------------------------------------------
// Chained / composed expressions (lazy evaluation)
// ---------------------------------------------------------------------------

TEST_CASE("Matrix chained element-wise ops", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{1, 1}, {1, 1}});
    matrix<int, 2, 2> c({{10, 10}, {10, 10}});
    matrix<int, 2, 2> expected({{20, 30}, {40, 50}});
    REQUIRE(((a + b) * c == expected).all());
}

TEST_CASE("Matrix scalar op chained with element-wise op", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{1, 1}, {1, 1}});
    matrix<int, 2, 2> expected({{10, 11}, {12, 13}});
    REQUIRE((((a + 10) - b) == expected).all());
}

TEST_CASE("Matrix expression result stored via assignment", "[math][matrix]")
{
    matrix<int, 2, 2> a({{4, 8}, {12, 16}});
    matrix<int, 2, 2> result(0);
    result = a / 4;
    matrix<int, 2, 2> expected({{1, 2}, {3, 4}});
    REQUIRE((result == expected).all());
}

// ---------------------------------------------------------------------------
// Submatrix view
// ---------------------------------------------------------------------------

TEST_CASE("Submatrix view bottom-right corner", "[math][matrix]")
{
    matrix<int, 3, 3> m({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    auto sub = m.submatrix<2, 2>(1, 1);
    REQUIRE(sub(0, 0) == 5);
    REQUIRE(sub(0, 1) == 6);
    REQUIRE(sub(1, 0) == 8);
    REQUIRE(sub(1, 1) == 9);
}

TEST_CASE("Submatrix view top-left corner", "[math][matrix]")
{
    matrix<int, 3, 3> m({{10, 20, 30}, {40, 50, 60}, {70, 80, 90}});
    auto sub = m.submatrix<2, 2>(0, 0);
    REQUIRE(sub(0, 0) == 10);
    REQUIRE(sub(1, 1) == 50);
}

TEST_CASE("Submatrix equality against explicit matrix", "[math][matrix]")
{
    matrix<int, 3, 4> m({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}});
    auto sub = m.submatrix<2, 3>(0, 1);
    matrix<int, 2, 3> expected({{2, 3, 4}, {6, 7, 8}});
    REQUIRE((sub == expected).all());
}

// ---------------------------------------------------------------------------
// Matrix multiplication
// ---------------------------------------------------------------------------

TEST_CASE("Matrix matmul square", "[math][matrix]")
{
    matrix<int, 2, 2> a({{1, 2}, {3, 4}});
    matrix<int, 2, 2> b({{5, 6}, {7, 8}});
    matrix<int, 2, 2> result(0);
    result = a.matmul(b);
    matrix<int, 2, 2> expected({{19, 22}, {43, 50}});
    REQUIRE((result == expected).all());
}

TEST_CASE("Matrix matmul non-square (2x3) * (3x2) = (2x2)", "[math][matrix]")
{
    matrix<int, 2, 3> a({{1, 2, 3}, {4, 5, 6}});
    matrix<int, 3, 2> b({{7, 8}, {9, 10}, {11, 12}});
    matrix<int, 2, 2> result(0);
    result = a.matmul(b);
    matrix<int, 2, 2> expected({{58, 64}, {139, 154}});
    REQUIRE((result == expected).all());
}

TEST_CASE("Matrix matmul identity", "[math][matrix]")
{
    matrix<int, 3, 3> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    matrix<int, 3, 3> id({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
    matrix<int, 3, 3> result(0);
    result = a.matmul(id);
    REQUIRE((result == a).all());
}

TEST_CASE("Matrix matmul chained with transpose", "[math][matrix]")
{
    matrix<int, 2, 3> a({{1, 2, 3}, {4, 5, 6}});
    matrix<int, 2, 2> result(0);
    result = a.matmul(a.transpose());
    matrix<int, 2, 2> expected({{14, 32}, {32, 77}});
    REQUIRE((result == expected).all());
}

// ---------------------------------------------------------------------------
// Float matrix
// ---------------------------------------------------------------------------

TEST_CASE("Float matrix scalar multiply", "[math][matrix]")
{
    matrix<float, 2, 2> a({{1.0f, 2.0f}, {3.0f, 4.0f}});
    matrix<float, 2, 2> expected({{2.0f, 4.0f}, {6.0f, 8.0f}});
    REQUIRE(((a * 2.0f) == expected).all());
}

TEST_CASE("Float matrix element-wise add", "[math][matrix]")
{
    matrix<float, 1, 3> a({{0.5f, 1.0f, 1.5f}});
    matrix<float, 1, 3> b({{0.5f, 0.5f, 0.5f}});
    matrix<float, 1, 3> expected({{1.0f, 1.5f, 2.0f}});
    REQUIRE(((a + b) == expected).all());
}

// ---------------------------------------------------------------------------
// Edge-case shapes
// ---------------------------------------------------------------------------

TEST_CASE("1x1 matrix add and subtract", "[math][matrix]")
{
    matrix<int, 1, 1> a({{7}});
    matrix<int, 1, 1> b({{3}});
    matrix<int, 1, 1> sum(10);
    matrix<int, 1, 1> diff(4);
    REQUIRE(((a + b) == sum).all());
    REQUIRE(((a - b) == diff).all());
}

TEST_CASE("Row vector (1xN) element-wise add", "[math][matrix]")
{
    matrix<int, 1, 4> a({{1, 2, 3, 4}});
    matrix<int, 1, 4> b({{4, 3, 2, 1}});
    matrix<int, 1, 4> expected({{5, 5, 5, 5}});
    REQUIRE(((a + b) == expected).all());
}

TEST_CASE("Column vector (Nx1) element-wise add", "[math][matrix]")
{
    matrix<int, 4, 1> a({{1}, {2}, {3}, {4}});
    matrix<int, 4, 1> b({{10}, {10}, {10}, {10}});
    matrix<int, 4, 1> expected({{11}, {12}, {13}, {14}});
    REQUIRE(((a + b) == expected).all());
}

TEST_CASE("Zero-filled matrix equality", "[math][matrix]")
{
    matrix<int, 3, 3> a(0);
    matrix<int, 3, 3> b(0);
    REQUIRE((a == b).all());
}

// ---------------------------------------------------------------------------
// Linear solve: matdivl / matdivr
// ---------------------------------------------------------------------------

TEST_CASE("matdivl identity A returns B", "[math][matrix]")
{
    matrix<float, 2, 2> A({{1.f, 0.f}, {0.f, 1.f}});
    matrix<float, 2, 2> B({{3.f, 5.f}, {7.f, 9.f}});
    matrix<float, 2, 2> X = A.matdivl(B);
    REQUIRE(((X == B).all()));
}

TEST_CASE("matdivl diagonal A", "[math][matrix]")
{
    matrix<float, 2, 2> A({{2.f, 0.f}, {0.f, 4.f}});
    matrix<float, 2, 2> B({{6.f, 8.f}, {12.f, 20.f}});
    matrix<float, 2, 2> X = A.matdivl(B);
    matrix<float, 2, 2> expected({{3.f, 4.f}, {3.f, 5.f}});
    REQUIRE(((X == expected).all()));
}

TEST_CASE("matdivl requires row swap (zero leading pivot)", "[math][matrix]")
{
    // First column pivot is 0 — must swap rows
    matrix<float, 2, 2> A({{0.f, 1.f}, {1.f, 0.f}});
    matrix<float, 2, 1> B({{3.f}, {7.f}});
    matrix<float, 2, 1> X = A.matdivl(B);
    // A·X = B → [x1, x0]^T = [3, 7]^T → x0=7, x1=3
    REQUIRE(X(0, 0) == Catch::Approx(7.f));
    REQUIRE(X(1, 0) == Catch::Approx(3.f));
}

TEST_CASE("matdivl 3x3 round-trip: A \\ (A*X) == X", "[math][matrix]")
{
    matrix<float, 3, 3> A({{2.f, 1.f, 0.f}, {1.f, 3.f, 1.f}, {0.f, 1.f, 4.f}});
    matrix<float, 3, 2> X_orig({{1.f, 2.f}, {3.f, 4.f}, {5.f, 6.f}});
    matrix<float, 3, 2> AX = A.matmul(X_orig);
    matrix<float, 3, 2> X_solved = A.matdivl(AX);
    for (size_t i = 0; i < 3; i++)
        for (size_t k = 0; k < 2; k++)
            REQUIRE(X_solved(i, k) == Catch::Approx(X_orig(i, k)).epsilon(1e-5f));
}

TEST_CASE("matdivl non-square B (3x3 A, 3x2 B)", "[math][matrix]")
{
    matrix<float, 3, 3> A({{1.f, 0.f, 0.f}, {0.f, 2.f, 0.f}, {0.f, 0.f, 3.f}});
    matrix<float, 3, 2> B({{2.f, 4.f}, {6.f, 8.f}, {9.f, 12.f}});
    matrix<float, 3, 2> X = A.matdivl(B);
    matrix<float, 3, 2> expected({{2.f, 4.f}, {3.f, 4.f}, {3.f, 4.f}});
    for (size_t i = 0; i < 3; i++)
        for (size_t k = 0; k < 2; k++)
            REQUIRE(X(i, k) == Catch::Approx(expected(i, k)).epsilon(1e-5f));
}

TEST_CASE("matdivl singular A returns NaN", "[math][matrix]")
{
    matrix<float, 2, 2> A({{1.f, 2.f}, {2.f, 4.f}});
    matrix<float, 2, 2> B({{1.f, 0.f}, {0.f, 1.f}});
    matrix<float, 2, 2> X = A.matdivl(B);
    REQUIRE(std::isnan(X(0, 0)));
}

TEST_CASE("matdivr solves X*A = B", "[math][matrix]")
{
    // B (2x2) / A (2x2) = X (2x2), X*A = B
    matrix<float, 2, 2> A({{2.f, 0.f}, {0.f, 4.f}});
    matrix<float, 2, 2> B({{6.f, 8.f}, {2.f, 4.f}});
    matrix<float, 2, 2> X = B.matdivr(A);
    // Verify X*A == B
    matrix<float, 2, 2> check = X.matmul(A);
    for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 2; j++)
            REQUIRE(check(i, j) == Catch::Approx(B(i, j)).epsilon(1e-5f));
}

TEST_CASE("matdivr non-square B: Kalman gain shape (3x2 result)", "[math][matrix]")
{
    // B is 3x2, A is 2x2 — mirrors Kalman gain: (STATE x OBS).matdivr(OBS x OBS)
    matrix<float, 3, 2> B({{1.f, 0.f}, {0.f, 1.f}, {1.f, 1.f}});
    matrix<float, 2, 2> A({{2.f, 0.f}, {0.f, 2.f}});
    matrix<float, 3, 2> X = B.matdivr(A);
    matrix<float, 3, 2> expected({{0.5f, 0.f}, {0.f, 0.5f}, {0.5f, 0.5f}});
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 2; j++)
            REQUIRE(X(i, j) == Catch::Approx(expected(i, j)).epsilon(1e-5f));
}
