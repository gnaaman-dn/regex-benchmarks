import re
import timeit

ROUND_COUNT = 1_000_000

with open("../patterns", "r") as f:
    PATTERNS = list(map(re.compile, f.read().splitlines()))


with open("../input.txt", "r") as f:
    INPUT = f.read().splitlines()


def benchmark():
    for idx, pattern in enumerate(PATTERNS):
        matches = 0

        def _time_it():
            nonlocal matches
            for line in INPUT:
                if pattern.match(line):
                    matches += 1
        overall_time = timeit.timeit(_time_it, number=ROUND_COUNT)
        time_per_round = overall_time / ROUND_COUNT
        time_per_round_ns = str(time_per_round * 10 ** 9)[:10]
        print(f'regex_{idx:02} {matches} {time_per_round_ns}')
