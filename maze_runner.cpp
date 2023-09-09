#include <stdio.h>
#include <stack>
#include <cstdlib>
#include <thread>
#include <vector>
#include <mutex>

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
std::mutex stack_mutex;

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
    std::vector<std::thread> threads;
    std::vector<bool> thread_results;

    while (!valid_positions.empty()) {
        pos_t current_position = valid_positions.top();
        valid_positions.pop();

        if (maze[current_position.i][current_position.j] == 's') {
            return true;
        }

        maze[current_position.i][current_position.j] = '.';
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        system("clear");
        print_maze();

        std::vector<pos_t> new_positions_to_check;

        // Verificar abaixo
        pos_t new_position;
        new_position.i = current_position.i + 1;
        new_position.j = current_position.j;
        if (new_position.i >= 0 && new_position.i < num_rows && new_position.j >= 0 && new_position.j < num_cols && maze[new_position.i][new_position.j] != '#' && maze[new_position.i][new_position.j] != '.') {
            new_positions_to_check.push_back(new_position);
        }

        // Verificar acima
        pos_t new_position_acima;
        new_position_acima.i = current_position.i - 1;
        new_position_acima.j = current_position.j;
        if (new_position_acima.i >= 0 && new_position_acima.i < num_rows && new_position_acima.j >= 0 && new_position_acima.j < num_cols && maze[new_position_acima.i][new_position_acima.j] != '#' && maze[new_position_acima.i][new_position_acima.j] != '.') {
            new_positions_to_check.push_back(new_position_acima);
             if (maze[new_position_acima.i][new_position_acima.j] == 's') {
                 return true;
            }
        }

        // Verificar direita
        pos_t new_position_direita;
        new_position_direita.i = current_position.i;
        new_position_direita.j = current_position.j + 1;
        if (new_position_direita.i >= 0 && new_position_direita.i < num_rows && new_position_direita.j >= 0 && new_position_direita.j < num_cols && maze[new_position_direita.i][new_position_direita.j] != '#' && maze[new_position_direita.i][new_position_direita.j] != '.') {
            new_positions_to_check.push_back(new_position_direita);
             if (maze[new_position_direita.i][new_position_direita.j] == 's') {
                 return true;
            }
        }

        // Verificar esquerda
        pos_t new_position_esquerda;
        new_position_esquerda.i = current_position.i;
        new_position_esquerda.j = current_position.j - 1;
        if (new_position_esquerda.i >= 0 && new_position_esquerda.i < num_rows && new_position_esquerda.j >= 0 && new_position_esquerda.j < num_cols && maze[new_position_esquerda.i][new_position_esquerda.j] != '#' && maze[new_position_esquerda.i][new_position_esquerda.j] != '.') {
            new_positions_to_check.push_back(new_position_esquerda);
             if (maze[new_position_esquerda.i][new_position_esquerda.j] == 's') {
                 return true;
            }
        }

        // Criar threads adicionais
        for (const pos_t& new_pos : new_positions_to_check) {
            threads.emplace_back([new_pos, &thread_results]() {
                bool result = walk(new_pos);
                thread_results.push_back(result);
            });
        }
    }

     for (std::thread& thread : threads) {
        thread.join(); // Espera que todas as threads terminem
    }

    // Verifique os resultados das threads
    for (bool result : thread_results) {
        if (result) {
            return true;
        }
    }

    return false;
}

int main() {

    pos_t initial_pos = load_maze("../data/maze6.txt");   
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
