use algorithms::binary_search;

#[test]
fn binary_search_test() {
    let v = [1,3,5,7,9];
    assert_eq!(binary_search(&v, 1), Some(0));
    assert_eq!(binary_search(&v, 9), Some(4));
    assert_eq!(binary_search(&v, 4), None);
}