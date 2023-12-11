#include <iostream>
#include <map>
#include <vector>

unsigned long _hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

struct Call{
    char *date, *city, *phone_from, *phone_to;
    int minutes;
};

Call init_call(){
    Call n_call;
    n_call.date = (char*) malloc(sizeof(char) * 10),
    n_call.city = (char*) malloc(sizeof(char) * 6), 
    n_call.phone_from = (char*) malloc(sizeof(char) * 11), 
    n_call.phone_to = (char*) malloc(sizeof(char) * 11);
    n_call.minutes = 0;

    return n_call;
}



struct Call_a{
    Call call;
    int count, all_minutes, least, most;
};

Call_a init_call_a(){
    Call_a call_a;
    call_a.call = init_call();
    call_a.all_minutes = call_a.count = call_a.least = call_a.most = 0;

    return call_a;
}

void printMostWanted(FILE *f){
    std::map<unsigned long, Call_a> count;
    int max_count = 0;
    unsigned long hash;

    Call_a call_a = init_call_a();

    while(fscanf(f, "%10s, %6s, %11s, %11s, %d\n", call_a.call.date, call_a.call.city, call_a.call.phone_from, call_a.call.phone_to, &call_a.call.minutes) == 5){
        hash = _hash(call_a.call.city);
        if(!count.count(hash)){
            count[hash] = call_a;
            
            count[hash].least = count[hash].most = call_a.call.minutes;
        }
        
        count[hash].count ++;
        max_count = std::max(max_count, count[hash].count);

        count[hash].all_minutes += call_a.call.minutes;
        count[hash].least = std::min(call_a.call.minutes, count[hash].least);
        count[hash].most = std::max(call_a.call.minutes, count[hash].most);

        call_a = init_call_a();
    }
    
    for(std::map<unsigned long, Call_a>::const_iterator it = count.begin(); it != count.end(); it++){
        if(it->second.count == max_count){
            printf("%s, %d, %d, %d, %d\n", it->second.call.city, max_count, count[it->first].all_minutes, count[it->first].least, count[it->first].most);
        }

        free(it->second.call.city);
        free(it->second.call.date);
        free(it->second.call.phone_from);
        free(it->second.call.phone_to);
    }

    count.clear();
    fseek(f, 0, SEEK_SET);
}



struct Call_b{
    char *city;
    int all_minutes;
};

Call_b init_call_b(){
    Call_b call_b;
    call_b.all_minutes = 0;
    call_b.city = (char*) malloc(sizeof(char) * 6);

    return call_b;
}

std::pair<Call_b*, size_t> getCityMinutes(FILE *f){
    std::map<unsigned long, Call_b> count;
    unsigned long hash;

    Call buff_call = init_call();

    Call_b call_b = init_call_b();
    size_t city_count = 0;

    while(fscanf(f, "%10s, %6s, %11s, %11s, %d\n", buff_call.date, call_b.city, buff_call.phone_from, buff_call.phone_to, &buff_call.minutes) == 5){
        hash = _hash(call_b.city);
        if(!count.count(hash)){
            count[hash] = call_b;
            city_count ++;
        }

        count[hash].all_minutes += buff_call.minutes;
        call_b = init_call_b();
    }

    Call_b *arr = (Call_b*) malloc(sizeof(Call_b) * city_count), buff, buff2;
    size_t arr_size = 0;
    bool put_f = false;
    for(std::map<unsigned long, Call_b>::const_iterator it = count.begin(); it != count.end(); it++){
        put_f = false;
        for(size_t i = 0; i < arr_size; i++){
            if(put_f){
                buff2 = arr[i];
                arr[i] = buff;
                buff = buff2;
            }
            if(!put_f && arr[i].all_minutes >= it->second.all_minutes){
                put_f = true;

                buff = arr[i];
                arr[i] = it->second;
            }
        }
        if(!put_f){
            arr[arr_size] = it->second;
        } else{
            arr[arr_size] = buff;
        }
        arr_size ++;
    }

    fseek(f, 0, SEEK_SET);

    free(buff_call.date);
    free(buff_call.phone_from);
    free(buff_call.phone_to);

    return {arr, arr_size};
}



struct Call_c{
    char *date, *city, *phone_from;
    int cost, minutes;
};

Call_c init_call_c(){
    Call_c call_c;
    call_c.cost = 0;
    call_c.date = (char*) malloc(sizeof(char) * 10);
    call_c.city = (char*) malloc(sizeof(char) * 6);
    call_c.phone_from = (char*) malloc(sizeof(char) * 11);
    call_c.minutes = 0;

    return call_c;
}

std::pair<Call_c *, size_t> getReceipt(FILE *T1, FILE *T2){
    unsigned long hash;
    std::map<unsigned long, int> cost;

    char *city = (char*) malloc(sizeof(char) * 6);
    int call_cost;
    while (fscanf(T2, "%6s, %d\n", city, &call_cost) == 2){
        hash = _hash(city);
        cost[hash] = call_cost;
    }

    size_t arr_size = 0, arr_cap = 16;
    Call_c call_c = init_call_c(), *arr = (Call_c *) malloc(sizeof(Call_c) * arr_cap);

    char *phone_to = (char*) malloc(sizeof(char) * 11);
    while(fscanf(T1, "%10s, %6s, %11s, %11s, %d\n", call_c.date, call_c.city, call_c.phone_from, phone_to, &call_c.minutes) == 5){
        hash = _hash(call_c.city);

        call_c.cost = cost[hash] * call_c.minutes;

        arr[arr_size] = call_c;
        arr_size ++;

        if(arr_cap == arr_size){
            arr_cap *= 2;
            if((arr = (Call_c *) realloc(arr, arr_cap)) == NULL){
                printf("error\n");
                return {NULL, 0};
            }
        }

        call_c = init_call_c();
    }
    
    fseek(T1, 0, SEEK_SET);
    fseek(T2, 0, SEEK_SET);

    free(phone_to);
    free(city);

    return {arr, arr_size};
}

int main(){

    FILE *T1, *T2;
    if((T1 = fopen("lab-6/T1", "r")) == NULL || (T2 = fopen("lab-6/T2", "r")) == NULL){
        printf("fopen error\n");
        return EXIT_FAILURE;
    }

    printf("\ntask_a\n");
    printMostWanted(T1); // task_a

    printf("\ntask_b\n");
    std::pair<Call_b *, size_t> task_b = getCityMinutes(T1); // task_b

    for(size_t i = 0; i < task_b.second; i++){
        std::cout << task_b.first[i].city << ' ' << task_b.first[i].all_minutes << '\n';
    }

    printf("\ntask_c\n");
    std::pair<Call_c *, size_t> task_c = getReceipt(T1, T2); //task_c

    for(size_t i = 0; i < task_c.second; i++){
        std::cout << task_c.first[i].city << ' ' << task_c.first[i].date << ' ' << task_c.first[i].phone_from << ' ' << task_c.first[i].minutes << ' ' << task_c.first[i].cost << '\n';
    }

    fclose(T1);
    fclose(T2);

    return EXIT_SUCCESS;
}