#include <stdio.h>
#include <stack>
#include <cstdlib>  // Adicionado para usar a função system("clear")

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

        maze[current_position.i][current_position.j] = '.';
        system("clear");
        print_maze();

        pos_t new_position;

        new_position.i = current_position.i + 1;
        new_position.j = current_position.j;
        if (new_position.i >= 0 && new_position.i < num_rows && new_position.j >= 0 && new_position.j < num_cols && maze[new_position.i][new_position.j] == 'x') {
            if (maze[new_position.i][new_position.j] == 's') {
                return true;
            }
            valid_positions.push(new_position);
        }

        // Implementar verificação para outras direções (acima, à direita, à esquerda)

    }

    return false;
}

int main(int argc, char* argv[]) {
    pos_t initial_pos = load_maze(argv[1]);
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
