#!/usr/bin/env python3
import pandas

files = ["cpp_hyperscan", "rust", "python"]

with open("patterns", "r") as f:
    patterns = f.read().splitlines()

all_results = {}
for file in files:
    with open(f"output/{file}", "r") as f:
        results = f.read().splitlines()
    results = [float(line.split()[-1]) for line in results]
    all_results[file] = results

all_results["regex"] = patterns

pandas.set_option("display.max_colwidth", 80)
df = pandas.DataFrame(all_results)
print(df.to_string(max_colwidth=80))
df.to_json("output/comparison.json")
