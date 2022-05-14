//
// Created by Will McGlaughlin on 14/5/2022.
//

int GetSumIntArray(const int *array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

int GetNonZeroCountArray(const int *array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] > 0) {
            sum += 1;
        }
    }
    return sum;
}