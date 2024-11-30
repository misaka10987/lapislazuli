#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <functional>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

/// @brief LapisLazuli is a collection of utilities for OI.
namespace ll {

typedef __int128_t int128_t;

/// @brief Compute factorial for integer.
/// @param n a positive integer to compute factorial for
/// @return the result of factorial
///
/// The behavior is undefined unless `n>0`.
constexpr int32_t fact(int32_t n) noexcept {
    return n == 0 ? 1 : n * fact(n - 1);
}

/// @brief Iterator for base-n digits of an integer.
/// @note The order of which digits come from the iterator is reverse from how
/// they are written, i.e. the last digit will be the **first** element from the
/// iterator.
template <int32_t N, typename T = int32_t> class BaseN final {
  public:
    /// @brief The number to convert into digits.
    int32_t const num;
    constexpr BaseN(int32_t num) noexcept : num(num) {}
    class Iterator final {
        int32_t curr;

      public:
        constexpr Iterator(int32_t curr) noexcept : curr(curr) {}
        constexpr int32_t operator*() const noexcept { return this->curr % N; }
        constexpr Iterator& operator++() noexcept {
            this->curr /= N;
            return *this;
        }
        constexpr bool operator!=(Iterator const& rhs) const noexcept {
            return this->curr != rhs.curr;
        }
    };
    constexpr Iterator begin() const noexcept { return Iterator(num); }
    constexpr Iterator end() const noexcept { return Iterator(0); }
    inline operator std::vector<int32_t>() const noexcept {
        auto res = std::vector<int32_t>();
        for (auto i : *this)
            res.push_back(i);
        return res;
    }
};

/// @brief Abort the program execution, printing `msg` to `stderr`.
/// @tparam T any type that is able to be output with `std::ostream`
/// @param msg the message to print
template <typename T> constexpr void panic(T msg) noexcept {
    std::cerr << msg << std::endl;
    std::abort();
}

/// @brief Abort the program execution. The same as `std::abort`.
constexpr void panic() noexcept { panic(""); }

/// @brief An integer range. This is useful when you would like to loop over
/// some consecutive integers.
/// @tparam T type of integer, `int` by default
///
/// # Example
///
/// ```cpp
///
/// for (int i = 0; i < n; i++) f(i); // you do not need this
///
/// for (auto i : Range(0, n)) f(i); // instead, you could write this
///
/// ```
template <typename T = int32_t> class Range final {
    /// @brief Left hand side of the range. Closed.
    T const left;
    /// @brief Right hand side of the range. Open.
    T const right;

  public:
    /// @brief Create an integer range [left,right).
    /// @param left left hand side of range
    /// @param right right hand side of range
    constexpr Range(T left, T right) noexcept : left(left), right(right) {}
    class Iterator final {
        T curr;

      public:
        constexpr Iterator(T curr) noexcept : curr(curr) {}
        constexpr T operator*() const noexcept { return this->curr; }
        constexpr Iterator& operator++() noexcept {
            ++this->curr;
            return *this;
        }
        constexpr bool operator!=(Iterator const& other) const noexcept {
            return this->curr != other.curr;
        }
    };
    constexpr Iterator begin() const noexcept { return Iterator(left); }
    constexpr Iterator end() const noexcept { return Iterator(right); }
};

/// @brief Create an integer range [left,right).
/// @tparam T an integer type
/// @param left left hand side of range
/// @param right right hand side of range
/// @return new `Range<T>`
template <typename T> constexpr Range<T> rng(T left, T right) noexcept {
    return Range(left, right);
}

/// @brief Create an integer range [0,right).
/// @tparam T an integer type
/// @param term right hand side of range
/// @return new `Range<T>`
template <typename T> constexpr Range<T> rng(T term) noexcept {
    return Range(T(), term);
}

/// @brief An iterator for generating permutations.
/// @note The implementation internally invokes `std::next_permutation`.
///
/// # Example
///
/// ```cpp
///
/// auto el = { 5,1,4 };
///
/// for (auto p : ll::Permut(el)) {
///
///     for (auto i : p) std::cout << i;
///
///     std::cout << " ";
///
/// } // 514 541 154 145 451 415
///
/// ```
template <typename T = int32_t> class Permut final {
  public:
    /// @brief Elements of the permutation.
    std::vector<T> const el;
    constexpr Permut(std::vector<T> el) noexcept : el(el) {}
    constexpr int32_t cnt() const noexcept { return fact(el.size()); }
    class Iterator final {
      public:
        std::vector<T> const& el;

      private:
        std::vector<uintptr_t> curr;
        std::vector<T> buf;
        bool next = true;

      public:
        constexpr Iterator(std::vector<T> const& el)
            : el(el), curr(std::vector<uintptr_t>(el.size())),
              buf(std::vector<T>(el.size())) {
            for (auto i : rng(el.size())) {
                this->curr[i] = i;
                this->buf[i] = el[curr[i]];
            }
        }
        constexpr std::vector<T> const& operator*() const noexcept {
            return this->buf;
        }
        constexpr Iterator& operator++() noexcept {
            this->next =
                std::next_permutation(this->curr.begin(), this->curr.end());
            for (auto i : rng(this->el.size()))
                this->buf[i] = this->el[curr[i]];
            return *this;
        }
        constexpr bool operator!=(Iterator const& _) const noexcept {
            return this->next;
        }
    };

    constexpr Iterator begin() const noexcept { return Iterator(this->el); }
    constexpr Iterator end() const noexcept { return Iterator(this->el); }
};

// template<int W = 1024, int H = 1024>
class Grid final {
  private:
    static constexpr uintptr_t W = 1024, H = 1024;
    using T = char;
    static inline T MAP[W][H];
    static inline std::vector<bool> DONE = std::vector(W * H, false);

  public:
    static inline uintptr_t WIDTH = W, HEIGHT = H;

    /// @brief Set the width and height of the grid.
    /// @param width width, or the first index of array
    /// @param height height, or the second index of array
    static constexpr void set(uintptr_t width, uintptr_t height) noexcept {
        WIDTH = width;
        HEIGHT = height;
    }

    /// @brief Initialize the grid reading input from `input`
    /// @param input the stream to read
    static inline void init(std::istream& input) noexcept {
        for (auto y : rng(HEIGHT))
            for (auto x : rng(WIDTH))
                input >> MAP[x][y];
    }

    /// @brief Output the grid to `output`
    /// @param output the stream to write to
    static inline void output(std::ostream& output) noexcept {
        for (auto y : rng(HEIGHT)) {
            for (auto x : rng(WIDTH))
                output << MAP[x][y];
            output << std::endl;
        }
    }

    /// @brief Pretty print the grid to `stderr`.
    static inline void debug() noexcept {
        std::cerr << std::endl << "┌";
        for (auto _ : rng(WIDTH))
            std::cerr << "─";
        std::cerr << WIDTH << std::endl;
        for (auto y : rng(HEIGHT)) {
            std::cerr << "│";
            for (auto x : rng(WIDTH))
                std::cerr << MAP[x][y];
            std::cerr << std::endl;
        }
        std::cerr << HEIGHT << std::endl << std::endl;
    }

    /// @brief Reset all `.done()` flags.
    static constexpr void refresh() noexcept {
        DONE.assign(DONE.size(), false);
    }

    /// @brief x-coordinate of the cell.
    uintptr_t const x;

    /// @brief y-coordinate of the cell.
    uintptr_t const y;

    /// @brief Create an always-invalid cell.
    constexpr Grid() noexcept : x(-1), y(-1) {}

    /// @brief Create a cell with specified coordinates.
    /// @param x x-coordinate of the cell
    /// @param y y-coordinate of the cell
    constexpr Grid(uintptr_t x, uintptr_t y) noexcept : x(x), y(y) {}

    /// @brief Calculate new `Grid` with difference `dx` in x coordinate.
    /// @param dx difference in x
    /// @return the translated cell
    constexpr Grid dx(intptr_t dx) const noexcept {
        return Grid(this->x + dx, this->y);
    }

    /// @brief Calculate new `Grid` with difference `dy` in y coordinate.
    /// @param dy difference in y
    /// @return the translated cell
    constexpr Grid dy(intptr_t dy) const noexcept {
        return Grid(this->x, this->y + dy);
    }

    /// @brief Check whether two `Grid`s indicates the same position.
    /// @param rhs right-hand side of operator
    /// @return result of equivalance operator
    constexpr bool operator==(Grid rhs) const noexcept {
        return this->x == rhs.x && this->y == rhs.y;
    };

    /// @brief Whether this `Grid` cell indicates a valid position in the grid.
    /// @return result of check
    constexpr bool valid() const noexcept {
        return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
    }

    /// @brief Visit the content in specified position.
    /// @return reference to content
    /// @warning This does not perform any boundary check.
    constexpr T& tile() const noexcept { return MAP[this->x][this->y]; }

    /// @brief Output the `Grid` cell in the format "(x, y)".
    /// @param output the stream to output to
    /// @param cell the instatnce to output
    /// @return `output` for chaining
    friend constexpr std::ostream& operator<<(std::ostream& output,
                                              Grid cell) noexcept {
        return output << "(" << cell.x << "," << cell.y << ")";
    }

    /// @brief Flag that indicates whether a cell has been processed.
    /// @return reference to flag
    inline std::vector<bool>::reference done() const noexcept {
        return DONE[this->y * WIDTH + this->x];
    }

    /// @brief Gets the neighboring cells (if valid) of the given cell.
    /// @return list of cells
    /// @todo constexpr when c++23
    inline std::vector<Grid> neighbor() const noexcept {
        auto res = std::vector<Grid>();
        for (auto cell : {this->dy(1), this->dx(1), this->dx(-1), this->dy(-1)})
            if (cell.valid())
                res.push_back(cell);
        return res;
    }

    /// @brief Traverse the grid cells that match `cond` starting from `this`,
    /// executing `then` on each.
    /// @param cond the condition function that judges whether to walk into a
    /// cell
    /// @param then the function to execute on the cell
    inline void walk(std::function<bool(Grid)> cond,
                     std::function<void(Grid)> then) const noexcept {
        if (!this->valid())
            return;
        if (this->done())
            return;
        if (!cond(*this))
            return;
        this->done() = true;
        for (auto g : this->neighbor())
            g.walk(cond, then);
        then(*this);
    }

    /// @brief Connected area from this cell.
    /// @return size of area
    constexpr uint64_t conn_area() const noexcept {
        uint64_t ans = 0;
        auto cond = [&](Grid map) { return map.tile() == this->tile(); };
        auto then = [&](auto) { ans++; };
        this->walk(cond, then);
        return ans;
    }

    /// @brief Find the next cell that matches `pat`.
    /// @param pat the pattern to match
    /// @return a `Grid` cell, may be invalid for not found
    static inline Grid next(T const& pat) noexcept {
        for (auto y : rng(HEIGHT))
            for (auto x : rng(WIDTH))
                if (MAP[x][y] == pat && !Grid(x, y).done())
                    return Grid(x, y);
        return Grid();
    }

    /// @brief Count how many `pat`s is there in `Grid`.
    /// @param pat the pattern to count
    /// @return number of cells
    static inline uint64_t stat(T const& pat) noexcept {
        uint64_t ans = 0;
        for (auto y : rng(HEIGHT))
            for (auto x : rng(WIDTH))
                if (MAP[x][y] == pat)
                    ans++;
        return ans;
    }
};

/**
 * @brief Read a value from the specified <code>istream</code>.
 *
 * @tparam T type of the value read
 * @param from stream to read from
 * @return the value read
 */
template <typename T> T input(std::istream& from = std::cin) {
    T buf;
    std::cin >> buf;
    return buf;
}

template <> int128_t input(std::istream& from) {
    auto buf = std::string();
    std::cin >> buf;
    int128_t ans = 0;
    for (uintptr_t i = 0; i < buf.size(); i++) {
        int128_t digit = buf[buf.size() - 1 - i] - '0';
        for (auto j : rng(i))
            digit *= 10;
        ans += digit;
    }
    return ans;
}

/**
 * @brief Format a 128-bit integer.
 *
 * @param value the integer
 * @return formatted string
 */
std::string printed(int128_t value) {
    auto buf = std::deque<char>();
    if (value == 0)
        buf.push_front('0');
    else
        for (; value > 0; value /= 10)
            buf.push_front('0' + value % 10);
    if (value < 0)
        buf.push_front('-');
    return std::string(buf.begin(), buf.end());
}

} // namespace ll
