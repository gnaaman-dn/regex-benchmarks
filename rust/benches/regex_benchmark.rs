use criterion::{criterion_group, criterion_main, Criterion};
use regex;

const PATTERNS: &str = include_str!("../../patterns");
const INPUT: &str = include_str!("../../input.txt");

fn criterion_benchmark(c: &mut Criterion) {
    let patterns: Vec<_> = PATTERNS
        .lines()
        .map(|line| regex::Regex::new(line).unwrap())
        .collect();
    let inputs: Vec<_> = INPUT.lines().collect();
    let mut matches = 0;
    for (idx, pattern) in patterns.iter().enumerate() {
        let name = format!("regex_{idx:02}");

        c.bench_function(&name, |b| {
            b.iter(|| {
                for line in inputs.iter() {
                    if pattern.is_match(line) {
                        matches += 1
                    }
                }
            })
        });
    }
}

criterion_group!(benches, criterion_benchmark);
criterion_main!(benches);
