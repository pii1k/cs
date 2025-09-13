pub fn selection_sort(data: &mut [i32]) {
    for i in 0..data.len() {
        for j in (i + 1)..data.len() {
            if data[j] < data[i] {
                data.swap(i, j);
            }
        }
    }
}

pub fn bubble_sort(data: &mut [i32]){
    for i in 0..data.len() {
        for j in 0 .. data.len() -1 -i {
            if data[j + 1] < data[j] {
                data.swap(j, j +1);
            }
        }
    }
}