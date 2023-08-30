#include <stdio.h>
#include <stack>
#include <cstdlib>  // Adicionado para usar a função system("clear")
#include <thread>

// Matriz de char representando o labirinto
char** maze;

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
    int i;
    int j;
};

std::stack<pos_t> valid_positions;

pos_t load_maze(const char* file_name) {
    pos_t initial_pos;
    FILE* inFile = fopen(file_name, "r");

    fscanf(inFile, "%d %d", &num_rows, &num_cols);

    maze = new char*[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = new char[num_cols];
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            fscanf(inFile, " %c", &maze[i][j]);
            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
    }

    fclose(inFile);
    return initial_pos;
}

void print_maze() {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
}

bool walk(pos_t pos) {
    valid_positions.push(pos);

    

    while (!valid_positions.empty()) {
        pos_t current_position = valid_positions.top();
        valid_positions.pop();

    if (maze[pos.i][pos.j] == 's') {
        return true;
    }

        maze[current_position.i][current_position.j] = '.';

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        system("clear");
        print_maze();

        pos_t new_position;

//Verificar abaixo
new_position.i = current_position.i + 1;
new_position.j = current_position.j;
if (new_position.i >= 0 && new_position.i < num_rows && new_position.j >= 0 && new_position.j < num_cols && maze[new_position.i][new_position.j] == 'x') {
    if (maze[new_position.i][new_position.j] == 's') {
        return true;
    }
    valid_positions.push(new_position);
}

// Verificar acima
pos_t new_position_above;
new_position_above.i = current_position.i - 1;
new_position_above.j = current_position.j;
if (new_position_above.i >= 0 && new_position_above.i < num_rows && new_position_above.j >= 0 && new_position_above.j < num_cols && maze[new_position_above.i][new_position_above.j] == 'x') {
    valid_positions.push(new_position_above);
    if (maze[new_position_above.i][new_position_above.j] == 's') {
        return true;
    }
}

// Verificar direita
pos_t new_position_right;
new_position_right.i = current_position.i;
new_position_right.j = current_position.j + 1;
if (new_position_right.i >= 0 && new_position_right.i < num_rows && new_position_right.j >= 0 && new_position_right.j < num_cols && maze[new_position_right.i][new_position_right.j] == 'x') {
    valid_positions.push(new_position_right);
    if (maze[new_position_right.i][new_position_right.j] == 's') {
        return true;
    }
}

// Verificar esquerda
pos_t new_position_left;
new_position_left.i = current_position.i;
new_position_left.j = current_position.j - 1;
if (new_position_left.i >= 0 && new_position_left.i < num_rows && new_position_left.j >= 0 && new_position_left.j < num_cols && maze[new_position_left.i][new_position_left.j] == 'x') {
    valid_positions.push(new_position_left);
    if (maze[new_position_left.i][new_position_left.j] == 's') {
        return true;
    }
}


    }
//Verifica se a pilha de posições não está vazia
if (!valid_positions.empty()){
pos_t next_position = valid_positions.top();
valid_positions.pop();
return walk(next_position);

}

    return false;
}


int main() {

    pos_t initial_pos = load_maze("../data/maze.txt");   
    print_maze();
    bool exit_found = walk(initial_pos);
    
    if (exit_found) {
        printf("Saída encontrada!\n");
    } else {
        printf("Saída não encontrada.\n");
    }

    // Libera a memória alocada para o labirinto
    for (int i = 0; i < num_rows; ++i) {
        delete[] maze[i];
    }
    delete[] maze;

    return 0;
}
