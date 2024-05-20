#include <iostream>
#include <vector>
#include <fstream>

//функция, получающая на вход граф, и возвращающая флаг, полносвязный он или нет
void dfs(std::vector<std::vector<int>> m, int start, std::vector<bool>& visited, std::fstream& out) {

	std::cout << start << " ";
	visited[start] = true;
	out << start << " ";
	for (int i = 0; i < m[start].size(); i++) {

		if (m[start][i] == 1 && (!visited[i])) {
			dfs(m, i, visited, out);
		}
	}

}


std::vector<std::vector<int>> matrix_multiplication(std::vector<std::vector<int>>& m, std::vector<std::vector<int>>& m1) {

	int n = m.size();

	std::vector<std::vector<int>> result(n, std::vector<int>(n,0));

		for (int i = 0; i < result.size(); ++i) {
			for (int j = 0; j < result.size(); ++j) {
				for (int k = 0; k < result.size(); ++k) {
					result[i][j] += m[i][k] * m1[k][j];
				}
			}
		}

	return result;
}

std::vector<std::vector<int>> matrix_power(std::vector<std::vector<int>>& m, int power) {

	int n = m.size();
	std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));
	for (int i = 0; i < n; ++i) {
		result[i][i] = 1;
	}

	while (power > 0) {
		if (power % 2 == 0) {
			m = matrix_multiplication(m, m);
			power /= 2;
		}
		else {
			result = matrix_multiplication(result, m);
			power--;
		}
	}

	return result;
}

bool is_fully_connected_edge_count(std::vector<std::vector<int>> graph) {

	int n = graph.size();

	int count = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (graph[i][j] == 1) {
				count++;
			}
		}
	}
	//((i != j && graph[i][j] == 1) && (i == j && graph[i][j] == 0))
	int needed = n * (n - 1) / 2;
	//std::cout << "\n" << count << " " << needed << "\n";
	if (count == needed) {
		std::cout << "\nGraph is fully connected \n";
		return true;
	}
	std::cout << "Graph is not fully connected \n";
	return false;

}

bool is_fully_connected_matrix(std::vector<std::vector<int>>& graph) {

	int n = graph.size();
	std::vector<std::vector<int>> result = graph;

	result = matrix_power(result, n);

	std::cout << std::endl;
	std::cout << "Adjastency matrix in power n: \n";
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << result[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::vector<int> diag;
	std::vector<int> other;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i == j) {
				diag.push_back(result[i][j]);
			}
			else {
				other.push_back(result[i][j]);
			}
		}
	}
	/*for (int i = 0; i < diag.size(); ++i) {
		std::cout << diag[i] << " ";
	}
	std::cout << " \n";

	for (int i = 0; i < other.size(); ++i) {
		std::cout << other[i] << " ";
	}
	std::cout << " \n";
	*/
	int left = 0;
	int right = diag.size() - 1;
	int right1 = other.size() - 1;

	while (left < right) {
		if (diag[left] == diag[right]) {
			left++;
		}
		else {
			std::cout << "Graph is not fully connected \n";
			return false;
			break;
		}
	}
	while (left < right1) {
		if (other[left] == other[right1]) {
			left++;
		}
		else {
			std::cout << "Graph is not fully connected \n";
			return false;
			break;
		}
	}
	std::cout << "\nGraph is fully connected \n";
	return true;
}

void articulation_dfs(int u, int& time, std::vector<std::vector<int>>& adj_m, std::vector<int>& discovery,
	std::vector<int>& low, std::vector<int> &parent, std::vector<bool> &ap) {

	int children = 0;
	discovery[u] = low[u] = ++time;
	for (int v = 0; v < adj_m.size(); ++v) {

		if (adj_m[u][v] == 1) {
			if (discovery[v] == -1) {
				children++;
				parent[v] = u;
				articulation_dfs(v, time, adj_m, discovery, low, parent, ap);

				low[u] = std::min(low[u], low[v]);
				
				if (parent[u] == -1 && parent[u] > 1) {
					ap[u] = true;
				}

				if (parent[u] != -1 && low[v] >= discovery[u]) {
					ap[u] = true;
				}
			}
			else if (v != parent[u]){
				low[u] = std::min(low[u], discovery[v]);
			}
		}
	}
}

void find_articulation_points(std::vector<std::vector<int>> adj_m, int v, std::fstream& out) {

	int n = adj_m.size();
	std::vector<int> discovery(n, -1);
	std::vector<int> low(n, -1);
	std::vector<int> parent(n, -1);
	std::vector<bool> ap(n, false);

	int time = 0;

	for (int i = 0; i < n; ++i) {

		if (discovery[i] == -1) {

			articulation_dfs(i, time, adj_m, discovery, low, parent, ap);
		}
	}

	out << "Articulation points are: \n";

	for (int i = 0; i < n; ++i) {
		if (ap[i]) {
			out << i << " ";
			std::cout << i << " ";
		}
	}
	out << "\n";
}

int main() {

	std::string path = "input.txt";
	std::ifstream fin;
	fin.open(path);
	int count = 0;


	if (!fin.is_open()) {
		std::cerr << "Error while opening file \n";
	}
	else {
		while (fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
			if (!fin.eof()) {
				count++;
			}
		}
	}

	fin.clear();
	fin.seekg(0, std::ios::beg);

	std::vector<std::vector<int>> adj_m(count, std::vector<int>(count));

	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			fin >> adj_m[i][j];
		}
	}
	std::cout << "adjastency matrix: \n";
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			std::cout << adj_m[i][j] << " ";
		}
		std::cout << std::endl;
	}
	
	int start_vertex = 0;
	fin >> start_vertex;
	std::cout << "\nStart vertex: " << start_vertex << '\n';
	if (start_vertex >= count) {
		std::cerr << "Unexisting vertex\n";
		return -1;
	}

	fin.close();

	std::vector<bool> visited(count, false);

	std::fstream out;
	std::string out_path = "output.txt";
	out.open(out_path);
	if (!out.is_open()) {
		std::cerr << "Error while opening file \n";
	}
	
	out << "DFS result: \n";
	dfs(adj_m, start_vertex, visited, out);
	out << "\n";
	find_articulation_points(adj_m, start_vertex, out);

	out.close();

	is_fully_connected_matrix(adj_m);
	is_fully_connected_edge_count(adj_m);

	return 0;
}