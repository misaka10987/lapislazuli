#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

/// @brief LapisLazuli is a collection of utilities for OI.
namespace ll {

    /// @brief Compute factorial for integer.
    /// @param n a positive integer to compute factorial for
    /// @return the result of factorial
    /// 
    /// The behavior is undefined unless `n>0`.
    constexpr int fact(int n) noexcept { return n == 0 ? 1 : n * fact(n - 1); }

    /// @brief Iterator for base-n digits of an integer.
    /// @note The order of which digits come from the iterator is reverse from how they are written,
    /// i.e. the last digit will be the **first** element from the iterator.
    /// 
    /// # Example
    /// 
    /// ```cpp
    /// 
    /// for (auto i : ll::BaseN(8, 123)) std::cout << i; // 371
    /// 
    /// ```
    class BaseN final {
    public:
        /// @brief Base of numeric.
        int const base;
        /// @brief The number to convert into digits.
        int const num;
        constexpr BaseN(int base, int num) noexcept : base(base), num(num) {}
        class Iterator final {
            int const base;
            int curr;
        public:
            constexpr Iterator(int base, int curr) noexcept : base(base), curr(curr) {}
            constexpr int operator*() const noexcept { return this->curr % this->base; }
            constexpr Iterator& operator++() noexcept { this->curr /= base; return *this; }
            constexpr bool operator!=(Iterator const& rhs) const noexcept { return this->curr != rhs.curr; }
        };
        constexpr Iterator begin() const noexcept { return Iterator(base, num); }
        constexpr Iterator end() const noexcept { return Iterator(base, 0); }
        inline operator std::vector<int>() const noexcept {
            auto res = std::vector<int>();
            for (auto i : *this) res.push_back(i);
            return res;
        }
    };

    /// @brief Abort the program execution, printing `msg` to `stderr`.
    /// @tparam T any type that is able to be output with `std::ostream`
    /// @param msg the message to print
    template<typename T>
    constexpr void panic(T msg) noexcept {
        std::cerr << msg << std::endl;
        std::abort();
    }

    /// @brief Abort the program execution. The same as `std::abort`.
    constexpr void panic() noexcept { panic(""); }

    /// @brief An integer range. This is useful when you would like to loop over some consecutive integers.
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
    template<typename T = int>
    class Range final {
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
            constexpr Iterator& operator++() noexcept { ++this->curr; return *this; }
            constexpr bool operator!=(Iterator const& other) const noexcept { return this->curr != other.curr; }
        };
        constexpr Iterator begin() const noexcept { return Iterator(left); }
        constexpr Iterator end() const noexcept { return Iterator(right); }
    };

    /// @brief Create an integer range [left,right).
    /// @tparam T an integer type
    /// @param left left hand side of range
    /// @param right right hand side of range
    /// @return new `Range<T>`
    template<typename T>
    constexpr Range<T> rng(T left, T right) noexcept { return Range(left, right); }

    /// @brief Create an integer range [0,right).
    /// @tparam T an integer type
    /// @param term right hand side of range
    /// @return new `Range<T>`
    template<typename T>
    constexpr Range<T> rng(T term) noexcept { return Range(T(), term); }

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
    class Permut final {
    public:
        /// @brief Elements of the permutation.
        std::vector<int> const el;
        constexpr Permut(std::vector<int> el) noexcept : el(el) {}
        constexpr int cnt() const noexcept { return fact(el.size()); }
        class Iterator final {
        public:
            std::vector<int> const& el;
        private:
            std::vector<int> curr;
            std::vector<int> buf;
            bool next = true;
        public:
            constexpr Iterator(std::vector<int> const& el)
                : el(el), curr(std::vector<int>(el.size())), buf(std::vector<int>(el.size())) {
                for (auto i : rng(el.size())) {
                    this->curr[i] = i;
                    this->buf[i] = el[curr[i]];
                }
            }
            constexpr std::vector<int> const& operator*() const noexcept { return this->buf; }
            constexpr Iterator& operator++() noexcept {
                this->next = std::next_permutation(this->curr.begin(), this->curr.end());
                for (auto i : rng(this->el.size()))  this->buf[i] = this->el[curr[i]];
                return *this;
            }
            constexpr bool operator!=(Iterator const& _) const noexcept { return this->next; }
        };

        constexpr Iterator begin() const noexcept { return Iterator(this->el); }
        constexpr Iterator end() const noexcept { return Iterator(this->el); }
    };

    // template<int W = 1024, int H = 1024>
    class Grid final {
    private:
        static constexpr int W = 1024, H = 1024;
        using T = char;
        static inline T MAP[W][H];
        static inline std::vector<bool> DONE = std::vector(W * H, false);
    public:
        static inline int WIDTH = W, HEIGHT = H;

        /// @brief Set the width and height of the grid.
        /// @param width width, or the first index of array
        /// @param height height, or the second index of array
        static constexpr void set(int width, int height) noexcept { WIDTH = width; HEIGHT = height; }

        /// @brief Initialize the grid reading input from `input`
        /// @param input the stream to read
        static inline void init(std::istream& input) noexcept {
            for (auto y : rng(HEIGHT)) for (auto x : rng(WIDTH)) input >> MAP[x][y];
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
            for (auto _ : rng(WIDTH)) std::cerr << "─";
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
        int const x;

        /// @brief y-coordinate of the cell.
        int const y;

        /// @brief Create an always-invalid cell.
        constexpr Grid() noexcept : x(-1), y(-1) {}

        /// @brief Create a cell with specified coordinates.
        /// @param x x-coordinate of the cell
        /// @param y y-coordinate of the cell
        constexpr Grid(int x, int y) noexcept : x(x), y(y) {}

        /// @brief Calculate new `Grid` with difference `dx` in x coordinate.
        /// @param dx difference in x
        /// @return the translated cell
        constexpr Grid dx(int dx) const noexcept { return Grid(this->x + dx, this->y); }

        /// @brief Calculate new `Grid` with difference `dy` in y coordinate.
        /// @param dy difference in y
        /// @return the translated cell
        constexpr Grid dy(int dy) const noexcept { return Grid(this->x, this->y + dy); }

        /// @brief Check whether two `Grid`s indicates the same position.
        /// @param rhs right-hand side of operator
        /// @return result of equivalance operator
        constexpr bool operator==(Grid rhs) const noexcept { return this->x == rhs.x && this->y == rhs.y; };

        /// @brief Whether this `Grid` cell indicates a valid position in the grid.
        /// @return result of check
        constexpr bool valid() const noexcept {
            return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
        }

        /// @brief Visit the content in specified position.
        /// @return reference to content
        /// @warning This does not perform any boundary check.
        constexpr T& tile() const noexcept {
            return MAP[this->x][this->y];
        }

        /// @brief Output the `Grid` cell in the format "(x, y)".
        /// @param output the stream to output to
        /// @param cell the instatnce to output
        /// @return `output` for chaining
        friend constexpr std::ostream& operator<<(std::ostream& output, Grid cell) noexcept {
            return output << "(" << cell.x << ", " << cell.y << ")";
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
            for (auto cell : { this->dy(1), this->dx(1), this->dx(-1), this->dy(-1) })
                if (cell.valid()) res.push_back(cell);
            return res;
        }

        /// @brief Traverse the grid cells that match `cond` starting from `this`, executing `then` on each.
        /// @param cond the condition function that judges whether to walk into a cell
        /// @param then the function to execute on the cell
        inline void walk(std::function<bool(Grid)> cond, std::function<void(Grid)> then) const noexcept {
            if (!this->valid()) return;
            if (this->done()) return;
            if (!cond(*this)) return;
            this->done() = true;
            for (auto g : this->neighbor()) g.walk(cond, then);
            then(*this);
        }

        /// @brief Connected area from this cell.
        /// @return size of area
        constexpr int conn_area() const noexcept {
            int ans = 0;
            auto cond = [&](Grid map) { return map.tile() == this->tile(); };
            auto then = [&](auto) { ans++; };
            this->walk(cond, then);
            return ans;
        }

        /// @brief Find the next cell that matches `pat`.
        /// @param pat the pattern to match
        /// @return a `Grid` cell, may be invalid for not found
        static inline Grid next(T const& pat) noexcept {
            for (auto y : rng(HEIGHT)) for (auto x : rng(WIDTH))
                if (MAP[x][y] == pat && !Grid(x, y).done()) return Grid(x, y);
            return Grid();
        }

        /// @brief Count how many `pat`s is there in `Grid`.
        /// @param pat the pattern to count
        /// @return number of cells
        static inline int stat(T const& pat) noexcept {
            int ans = 0;
            for (auto y : rng(HEIGHT)) for (auto x : rng(WIDTH))
                if (MAP[x][y] == pat) ans++;
            return ans;
        }
    };

}
