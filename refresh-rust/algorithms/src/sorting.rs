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

pub fn bubble_sort(data: &mut [i32]){
    let mut swap_flag = false;
    for i in 0..data.len() {
        for j in 0 .. data.len() -1 -i {
            if data[j + 1] < data[j] {
                data.swap(j, j +1);
                swap_flag = true;
            }
        }
        if !swap_flag { break; }
    }
}

pub fn insertion_sort(data: &mut[i32]) {

    for i in 0..data.len() {
        let mut j = i;
        let key = data[i];

        while 0 < j && key < data[j - 1] {
            data[j] = data[j - 1];
            j -= 1;
        }
        data[j] = key;
    }
}