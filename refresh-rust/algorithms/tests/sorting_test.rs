use algorithms::sorting::*;

fn unsorted() -> [i32; 10] {
    [2, 3, 5, 1, 6, 4, 10, 9, 7, 8]
}

fn expected() -> [i32; 10] {
    [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
}

#[test]
fn selection_sort_test() {
    let mut data = unsorted();
    selection_sort(&mut data);
    assert_eq!(data, expected());
}

#[test]
fn bubble_sort_test() {
    let mut data = unsorted();
    bubble_sort(&mut data);
    assert_eq!(data, expected());
}

#[test]
fn insertion_sort_test() {
    let mut data = unsorted();
    insertion_sort(&mut data);
    assert_eq!(data, expected());
}

#[test]
fn quick_sort_lomuto_test() {
    let mut data = unsorted();
    quick_sort(&mut data, QuickSortPartitionType::LomutoPartition, false);
    assert_eq!(data, expected());
}

#[test]
fn quick_sort_hoare_test() {
    let mut data = unsorted();
    quick_sort(&mut data, QuickSortPartitionType::HoarePartition, false);
    assert_eq!(data, expected());
}

#[test]
fn quick_sort_lomuto_median_pivot() {
    let mut data = unsorted();
    quick_sort(&mut data, QuickSortPartitionType::LomutoPartition, true);
    assert_eq!(data, expected());
}

#[test]
fn quick_sort_hoare_median_pivot() {
    let mut data = unsorted();
    quick_sort(&mut data, QuickSortPartitionType::HoarePartition, true);
    assert_eq!(data, expected());
}