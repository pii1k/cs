use algorithms::binary_search;
use criterion::{criterion_group, criterion_main, Criterion, BatchSize, black_box};

fn bench_binary_search(c: &mut Criterion) {
    let mut g = c.benchmark_group("binary_search");

    for &n in &[1_000usize, 10_000, 100_000, 1_000_000] {
        g.bench_function(format!("n={}", n), |b| {
            b.iter_batched(
                || {
                    let data: Vec<i32> = (0..n as i32).collect();
                    let target = (n as i32) / 2;
                    (data, target)
                },
                |(data, target)| {
                    let _ = black_box(binary_search(&data, target));
                },
                BatchSize::SmallInput,
            );
        });
    }

    g.finish();
}

criterion_group!(benches, bench_binary_search);
criterion_main!(benches);
