pub fn selection_sort<T: Ord> (data: &mut [T]) {
    for i in 0..data.len() {
        let mut min_idx = i;
        for j in (i + 1)..data.len() {
            if data[j] < data[min_idx] {
                min_idx = j;
            }
        }
        if min_idx != i {
            data.swap(i, min_idx);
        }
    }
}

pub fn bubble_sort(data: &mut [i32]) {
    if data.len() < 2 { return; }

    for i in 0..data.len() {
        let mut swapped = false;
        for j in 0..(data.len() - i - 1) {
            if data[j + 1] < data[j] {
                data.swap(j, j + 1);
                swapped = true;
            }
        }
        if !swapped {
            break;
        }
    }
}

pub fn insertion_sort(data: &mut [i32]) {
    if data.len() < 2 { return; }

    for i in 1..data.len() {
        let key = data[i];
        let mut j = i;
        while j > 0 && key < data[j - 1] {
            data[j] = data[j - 1];
            j -= 1;
        }
        data[j] = key;
    }
}

#[derive(Copy, Clone)]
pub enum QuickSortPartitionType {
    LomutoPartition,
    HoarePartition,
}

fn lomuto_partition(data: &mut [i32], low: usize, high: usize, pivot_idx: usize) -> usize {
    data.swap(pivot_idx, high);
    let pivot = data[high];
    let mut iter = low;

    for k in low..high {
        if data[k] < pivot {
            data.swap(iter, k);
            iter += 1;
        }
    }

    data.swap(iter, high);
    iter
}

fn hoare_partition(data: &mut [i32], low: usize, high: usize, pivot_idx: usize) -> usize {
    let pivot = data[pivot_idx];
    let mut left = low as isize - 1;
    let mut right = high as isize + 1;

    loop {
        loop {
            left += 1;
            if data[left as usize] >= pivot {
                break;
            }
        }

        loop {
            right -= 1;
            if data[right as usize] <= pivot {
                break;
            }
        }

        if left >= right {
            return right as usize;
        }

        data.swap(left as usize, right as usize);
    }
}

pub fn quick_sort(data: &mut [i32], partition_type: QuickSortPartitionType, use_median_pivot: bool) {
    if data.len() < 2 { return; }
    quick_sort_impl(data, 0, data.len() - 1, partition_type, use_median_pivot);
}

fn quick_sort_impl(data: &mut [i32], low: usize, high: usize, partition_type: QuickSortPartitionType, use_median_pivot: bool) {
    if data.len() < 2 { return; }
    match partition_type {
        QuickSortPartitionType::LomutoPartition => {
            let pivot_idx = if use_median_pivot {low + (high - low) / 2} else {high};
            let partition_idx = lomuto_partition(data, low, high, pivot_idx);
            if partition_idx > low {
                quick_sort_impl(data, low, partition_idx - 1, partition_type, use_median_pivot);
            }
            if partition_idx + 1 <= high {
                quick_sort_impl(data, partition_idx + 1, high, partition_type, use_median_pivot);
            }
        }
        QuickSortPartitionType::HoarePartition => {
            let pivot_idx = if use_median_pivot {low + (high - low) / 2} else {low};
            let partition_idx = hoare_partition(data, low, high, pivot_idx);
            if partition_idx > low {
                quick_sort_impl(data, low, partition_idx, partition_type, use_median_pivot);
            }
            if partition_idx + 1 <= high {
                quick_sort_impl(data, partition_idx + 1, high, partition_type, use_median_pivot);
            }
        }
    }
}