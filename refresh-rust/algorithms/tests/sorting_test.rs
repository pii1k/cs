use algorithms::sorting::*;

#[test]
fn selection_sort_test() {
    let mut test_vec = [2,3,5,1,6,4,10,9,7,8];
    let expect_result = [1,2,3,4,5,6,7,8,9,10];

    selection_sort(&mut test_vec);
    assert_eq!(test_vec, expect_result);
}

#[test]
fn selection_sort_test2() {
    let mut test_vec = [2.1,3.3,5.4,1.1,6.2,4.3,10.2,9.4,7.5,8.6];
    let expect_result = [1,2,3,4,5,6,7,8,9,10];

    selection_sort(&mut test_vec);
    assert_eq!(test_vec, expect_result);
}

#[test]
fn bubble_sort_test() {
    let mut test_vec = [2,3,5,1,6,4,10,9,7,8];
    let expect_result = [1,2,3,4,5,6,7,8,9,10];

    bubble_sort(&mut test_vec);
    assert_eq!(test_vec, expect_result);
}

#[test]
fn insertion_sort_test() {
    let mut test_vec = [2,3,5,1,6,4,10,9,7,8];
    let expect_result = [1,2,3,4,5,6,7,8,9,10];

    insertion_sort(&mut test_vec);
    assert_eq!(test_vec, expect_result);
}