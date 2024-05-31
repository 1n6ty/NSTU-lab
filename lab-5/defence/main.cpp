#include <iostream>

void swap_rows(int *mat, int m, int n, int fst, int sec){
    if(fst == sec) return;
    for(int i = 0; i < n; i++){
        mat[fst * n + i] += mat[sec * n + i];
        mat[sec * n + i] = mat[fst * n + i] - mat[sec * n + i];
        mat[fst * n + i] -= mat[sec * n + i];
    }
}

void swap_cols(int *mat, int m, int n, int fst, int sec){
    if(fst == sec) return;
    for(int i = 0; i < m; i++){
        mat[i * n + fst] += mat[i * n + sec];
        mat[i * n + sec] = mat[i * n + fst] - mat[i * n + sec];
        mat[i * n + fst] -= mat[i * n + sec];
    }
}

void print_matrix(int *mat, int m, int n){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            std::cout << mat[i * n + j] << ' ';
        }
        std::cout << '\n';
    }
}

char is_row_less(int *mat, int m, int n, int fst, int sec){
    for(int i = 0; i < n; i++){
        if(mat[fst * n + i] < mat[sec * n + i]) return 1;
        else if(mat[fst * n + i] > mat[sec * n + i]) return 0;
    }
    return 0;
}

char is_col_less(int *mat, int m, int n, int fst, int sec){
    for(int i = 0; i < m; i++){
        if(mat[i * n + fst] < mat[i * n + sec]) return 1;
        else if(mat[i * n + fst] > mat[i * n + sec]) return 0;
    }
    return 0;
}

void straight_choose_row_sort(int *mat, int m, int n, int &changes, int &checks){
    changes = 0;
    checks = 0;

    int min_row_id;
    for(int i = 0; i < m - 1; i++){
        min_row_id = i;
        for(int j = i + 1; j < m; j++){
            if(is_row_less(mat, m, n, j, min_row_id)){
                checks ++;
                min_row_id = j;
            }
        }
        swap_rows(mat, m, n, i, min_row_id);
        if(min_row_id != i) changes ++;
    }
}

void straight_choose_col_sort(int *mat, int m, int n, int &changes, int &checks){
    changes = 0;
    checks = 0;

    int min_col_id;
    for(int i = 0; i < n - 1; i++){
        min_col_id = i;
        for(int j = i + 1; j < n; j++){
            if(is_col_less(mat, m, n, j, min_col_id)){
                checks ++;
                min_col_id = j;
            }
        }
        swap_cols(mat, m, n, i, min_col_id);
        if(min_col_id != i) changes ++;
    }
}

int main(){
    //Reading matrix
    FILE *mat_f = fopen("matrix.txt", "r");
    if(mat_f == NULL){
        std::cout << "File did\'t open\n";
        return 0;
    }

    int m, n; // rows, cols
    if(fscanf(mat_f, "%d %d", &m, &n) < 2){
        std::cout << "File read error\n";
        fclose(mat_f);
        return 0;
    }

    int *mat = (int *) malloc(sizeof(int) * m * n);

    for(int i = 0; i < m * n; i++){
        if(fscanf(mat_f, "%d", mat + i) < 1){
            std::cout << "File read error\n";
            fclose(mat_f);
            return 0;
        }
    }
    fclose(mat_f);

    int option = -1, changes = 0, checks = 0;
    while(option != 3){
        print_matrix(mat, m, n);
        std::cout << "Choose option (1 - sort by row, 2 - sort by column, 3 - exit):\n";
        if(scanf("%d", &option) < 1){
            std::cout << "scanf error\n";
            return 0;
        }

        switch(option){
            case 1:
                straight_choose_row_sort(mat, m, n, changes, checks);
                printf("%d checks, %d changes\n", checks, changes);
            break;
            case 2:
                straight_choose_col_sort(mat, m, n, changes, checks);
                printf("%d checks, %d changes\n", checks, changes);
            break;
        }
    }
    return 0;
}
