def print_matrix(matrix):
    size = len(matrix)**2
    center_row = len(matrix) // 2 
    center_col = len(matrix[0]) // 2 
    counter = 0
    step = 1
    diff = 1

    print("Элементы матрицы:", end = " ") 
    
    while counter <= size:
        for col in range(step):
            counter += 1
            if counter > size: break
            print(matrix[center_row][center_col], end =" ")
            center_col += diff

            
        for row in range(step):
            counter += 1
            if counter > size: break
            print(matrix[center_row][center_col], end =" ")
            center_row += diff
            
        step += 1
        diff *= -1


matrix = [[0]*13 for _ in range(13)] #создание и заполнение матрицы 13 на 13 числами от 1 до 169
count = 1
for i in range(13):
    for j in range(13):
        matrix[i][j] = count
        count += 1

print_matrix(matrix) #вывод матрицы по спирали, начиная с центра

