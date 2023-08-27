#include <stdio.h>
#include <stack>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t { //A posição é representada por um vetor de posição contendo a coordenada linha(i) e coluna(j)
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;

/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();
*/

// (FEITO) Função que le o labirinto de um arquivo texto, carrega em memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE* inFile = fopen(file_name, "r");

	// Le o numero de linhas e colunas (fscanf) e salva em num_rows e num_cols
	fscanf(inFile, "%d %d", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
  	maze = new char*[num_rows];
	// Aloca cada linha da matriz
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = new char[num_cols];
    }
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			fscanf(inFile, " %c", &maze[i][j]);

			// Se o valor for 'e' salvar o valor em initial_pos
			if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
		}
	}
	
}
	return initial_pos;



// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {

	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	while (!valid_positions.empty()) {
    	pos_t current_position = valid_positions.top();
        valid_positions.pop();

		// Marcar a posição atual com o símbolo '.'
		maze[current_position.i][current_position.j] = '.';

		// Limpa a tela
		system("clear");

		// Imprime o labirinto
		print_maze();

		// Verificar as posições vizinhas e inseri-las na pilha
		
		pos_t new_position;

		// Verificar a posição abaixo
		new_position.i = current_position.i + 1;
		new_position.j = current_position.j;
		if (new_position.i >= 0 && new_position.i < num_rows && maze[new_position.i][new_position.j] == 'x') {
			valid_positions.push(new_position);
		}

		// Verificar a posição acima
		new_position.i = current_position.i - 1;
		new_position.j = current_position.j;
		if (new_position.i >= 0 && new_position.i < num_rows && maze[new_position.i][new_position.j] == 'x') {
			valid_positions.push(new_position);
		}

		// Verificar a posição à direita
		new_position.i = current_position.i;
		new_position.j = current_position.j + 1;
		if (new_position.j >= 0 && new_position.j < num_cols && maze[new_position.i][new_position.j] == 'x') {
			valid_positions.push(new_position);
		}

		// Verificar a posição à esquerda
		new_position.i = current_position.i;
		new_position.j = current_position.j - 1;
		if (new_position.j >= 0 && new_position.j < num_cols && maze[new_position.i][new_position.j] == 'x') {
			valid_positions.push(new_position);
		}

		// Verificar se alguma das posições válidas é igual a 's'
		if (maze[new_position.i][new_position.j] == 's') {
    		return true; // Saída encontrada!
		}

		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		if (!valid_positions.empty()) {
			pos_t next_position = valid_positions.top();
			valid_positions.pop();
		}
	return false;
	}
}
int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	 if (exit_found) {
        printf("Saída encontrada!\n");
    } else {
        printf("Saída não encontrada.\n");
    }
	return 0;
}
