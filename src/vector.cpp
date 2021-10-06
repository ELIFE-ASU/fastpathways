#include <fastpathways.h>

static auto sum(std::vector<int64_t> const &xs) -> int64_t {
    auto s = int64_t{};
    for (auto const &x : xs) {
        s += x;
    }
    return s;
}

auto isbelow(std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> bool {
    if (v == w || std::size(v) != std::size(w)) {
        return false;
    }
    for (std::size_t i = 0; i < std::size(v); ++i) {
        if (v.at(i) > w.at(i)) {
            return false;
        }
    }
    return true;
}

auto operator<=>(std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> std::partial_ordering {
    if (std::size(v) != std::size(w)) {
        return std::partial_ordering::unordered;
    }
    for (size_t i = 0; i < std::size(v); ++i) {
        if (v.at(i) < w.at(i)) {
            return std::partial_ordering::less;
        } else if (v.at(i) > w.at(i)) {
            return std::partial_ordering::greater;
        }
    }
    return std::partial_ordering::equivalent;
}

auto isbasic(std::vector<int64_t> const &xs) -> bool {
    auto s = int64_t{};
    for (auto const &x : xs) {
        s += x;
        if (x < 0) {
            return false;
        }
    }
    return s == 1;
}

auto operator+(std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> std::vector<int64_t> {
    if (std::size(v) != std::size(w)) {
        throw std::invalid_argument{"vectors must have the same size"};
    }
    auto x = std::vector<int64_t>(std::size(v));
    for (std::size_t i = 0; i < std::size(v); ++i) {
        x.at(i) = v.at(i) + w.at(i);
    }
    return x;
}

auto basic(std::vector<int64_t> const &xs) -> std::vector<std::vector<int64_t>> {
    auto const N = std::size(xs);
    auto bs = std::vector<std::vector<int64_t>>{};
    bs.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        auto b = std::vector<int64_t>(N);
        b.at(i) = 1;
        bs.push_back(b);
    }
    return bs;
}

auto lowerbound(std::vector<int64_t> const &xs) -> int64_t {
    return lowerbound(sum(xs));
}

auto ord(std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> bool {
    auto const a = sum(v);
    auto const b = sum(w);
    return a < b || (a == b && v < w);
}

auto stackchildren(std::vector<int64_t> const &x, std::vector<std::vector<std::vector<int64_t>>> &stack) -> void {
    auto const a = stack.back().back();
    auto const N = std::size(stack);
    auto segment = std::vector<std::vector<int64_t>>{};
    segment.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i; j < N; ++j) {
            auto const anext = stack.at(i).back() + stack.at(j).back();
            if (!isbelow(anext, a) && anext <= x) {
                segment.push_back(anext);
            }
        }
    }
    std::sort(std::begin(segment), std::end(segment), ord);
    auto const last = std::unique(std::begin(segment), std::end(segment));
    segment.erase(last, std::end(segment));
    stack.push_back(segment);
}

auto thurber(std::vector<int64_t> const &x) -> int64_t {
    if (sum(x) == 0) {
        throw std::domain_error{"vector is not in the space"};
    } else if (isbasic(x)) {
        return 0;
    }

    auto stack = std::vector<std::vector<std::vector<int64_t>>>{};
    stack.reserve(std::size(x));
    for (auto &&e: basic(x)) {
        stack.push_back({e});
    }
    auto lb = lowerbound(x);
    auto N = std::size(stack);

    auto loop = 1;
    std::vector<int64_t> vertical, slant;
    while (true) {
        if (std::size(stack) == N) {
            stackchildren(x, stack);
        }
        std::tie(vertical, slant) = bounds(sum(x), lb + 1);
        while (true) {
            auto const i = static_cast<int64_t>(std::size(stack));
            if (i - static_cast<int64_t>(N) <= lb) {
                auto const aprev = stack.at(i-2).back();
                auto const a = stack.at(i-1).back();

                if (a == x) {
                    return i - N;
                } else if (retain(sum(x), lb, vertical.at(i - N - 1), slant.at(i - N), i - N - 1, sum(aprev), sum(a))) {
                    stackchildren(x, stack);
                } else if (backup(stack, N)) {
                    loop += 1;
                    break;
                }
            } else {
                stack.pop_back();
                if (std::size(stack) == N || backup(stack, N)) {
                    loop += 1;
                    break;
                }
            }
            loop += 1;
        }
        lb += 1;
    }
}

auto vectorspace(std::size_t n, int64_t max) -> std::vector<std::vector<int64_t>> {
    auto space = std::vector<std::vector<int64_t>>{};
    space.reserve(1 << n);
    auto x = std::vector<int64_t>(n);
    x.at(0) = 1;
    space.push_back(x);
    while (true) {
        std::size_t i = 0;
        while (i < std::size(x) && x.at(i) == max) {
            x.at(i++) = 0;
        }
        if (i == std::size(x)) {
            break;
        }
        x.at(i) += 1;
        space.push_back(x);
    }
    return space;
}