#!/bin/bash
set -e
set -x

. "$HOME/.cargo/env"

cd /src/

rm -rf output
mkdir -p output

rustc --version >> output/lang_versions
gcc --version >> output/lang_versions
python3.11 --version >> output/lang_versions
python3.11 -m pip freeze >> output/pip_versions

pushd python
cythonize -3 -b bench.pyx
python3.11 main.py > ../output/python
popd

pushd rust
rm -rf target/criterion
cargo bench > ../output/rust.raw
for dir in target/criterion/*; do
  measure=$(cat $dir/base/estimates.json | jq '.mean.point_estimate')
  echo "$(basename $dir) ${measure}" >> ../output/rust
done
popd

pushd cpp_hyperscan
g++ -O3 main.cpp -lhs -o bench
./bench > ../output/cpp_hyperscan
popd

python3.11 compile_results.py | tee output/comparison

chown -R $TARGET_UID output/
