#include <chrono>
#include <fstream>
#include <hs/hs.h>
#include <hs/hs_runtime.h>
#include <vector>

std::vector<hs_database_t *> get_patterns() {
    std::ifstream infile("../patterns");
    std::vector<hs_database_t *> patterns{};
    std::string line{};
    while (std::getline(infile, line)) {
        hs_database_t *database;
        hs_compile_error_t *compile_err;

        if (hs_compile(line.c_str(),
                       HS_FLAG_SINGLEMATCH | HS_FLAG_ALLOWEMPTY | HS_FLAG_UTF8,
                       HS_MODE_BLOCK, NULL, &database,
                       &compile_err) != HS_SUCCESS) {
            fprintf(stderr, "ERROR: Unable to compile magic_str \"%s\": %s\n",
                    line.c_str(), compile_err->message);
            hs_free_compile_error(compile_err);
            database = NULL;
            abort();
        }
        patterns.push_back(database);
    }

    return patterns;
}

std::vector<std::string> get_input() {
    std::vector<std::string> input_lines{};
    std::ifstream infile("../input.txt");
    std::string line{};
    while (std::getline(infile, line)) {
        input_lines.push_back(line);
    }
    return input_lines;
}

static int on_match(__attribute__((unused)) unsigned int id,
                    __attribute__((unused)) unsigned long long from,
                    __attribute__((unused)) unsigned long long to,
                    __attribute__((unused)) unsigned int flags, void *ctx) {
    *((size_t *)ctx) += 1;
    return 0;
}

static constexpr size_t ROUND_COUNT = 1000000;

int main() {
    auto patterns = get_patterns();
    auto input = get_input();
    size_t idx = 0;
    for (const auto &pattern : patterns) {
        size_t matches = 0;

        hs_scratch_t *scratch_space = nullptr;
        auto err = hs_alloc_scratch(pattern, &scratch_space);
        if (err != HS_SUCCESS) {
            fprintf(stderr, "FAILED TO ALLOCATE SCRATCH_SPACE");
            abort();
        }

        auto start = std::chrono::steady_clock::now();
        for (size_t i = 0; i < ROUND_COUNT; i++) {
            for (const auto &input_line : input) {
                auto foo =
                    hs_scan(pattern, input_line.c_str(), input_line.size(), 0,
                            scratch_space, on_match, (void *)&matches);
                if (foo != HS_SUCCESS) {
                    fprintf(stderr, "Failed to SCAN=%d", err);
                    abort();
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        auto delta = end - start;
        auto ns = std::chrono::nanoseconds(delta).count();
        auto iter = double(ns) / double(ROUND_COUNT);

        printf("regex_%02zu %lf\n", idx, iter);

        hs_free_scratch(scratch_space);
        idx++;
    }
    return 0;
}
