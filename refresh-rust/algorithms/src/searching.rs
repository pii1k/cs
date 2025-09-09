pub fn binary_search(sorted: &[i32], target: i32) -> Option<usize>
{
    if sorted.is_empty() {
        return None;
    }

    let mut left: isize = 0;
    let mut right: isize = sorted.len() as isize -1 ;

    while left <= right
    {
        let mid = left + ((right - left) >> 1);
        let m = mid as usize;
        let v = sorted[m];

        if v == target {
            return Some(m);
        }

        if v < target {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    None
}
