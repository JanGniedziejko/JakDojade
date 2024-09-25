#include <iostream>
#include <stdlib.h>
#define MAXSIZE 100
#define INT_MAX 2147483647
using namespace std;

// Structures
struct City {
	char* name;
	int x;
	int y;
};
struct CityNode {
	City city;
	CityNode* next;
};
struct QueueNode {
	int x, y;
	QueueNode* next;
};
struct Connection {
	char* start;
	char* destination;
	int mode = 0;
};
struct Connections {
	char* name;
	int duration = INT_MAX;
};
struct NameNode {
	char* name;
	NameNode* next;
};
struct AdjacencyList {
	Connections start_city;
	int index;
	struct RelationNode {
		Connections airway;
		AdjacencyList* address;
		RelationNode* next;
	}* relations;
	AdjacencyList* prev_vert;		// an address to the previous vertex ( which it came from)
	AdjacencyList* next;
};
// Useful functions
void print(char* name) {
	int i = 0;
	while (name[i] != '\0') {
		cout << name[i++];
	}
}
bool comparison(char* input, char* input2, int start) {
	int i = 0;
	while (input[i] != '\0') {
		if (input[i++] != input2[start + i]) return false;
	}
	if (input2[start + i] == '\0' || input2[start + i] == ' ') return true;
	return false;
}

// Functions regarding Lists
void Add_New_Name(NameNode*& head, char* name) {
	NameNode* NewNode = new NameNode;
	NewNode->next = nullptr;
	NewNode->name = name;
	if (head == nullptr) head = NewNode;
	else {
		NewNode->next = head;
		head = NewNode;
	}
}
void printTransferCities(NameNode* head) {
	NameNode* current = head;
	while (current != nullptr) {
		int i = 0;
		while (current->name[i] != '\0') {
			cout << current->name[i++];
		}
		cout << " ";
		current = current->next;
	}
}
// City List
void AddNewCity(CityNode*& head, char* name, int x, int y, int size) {
	CityNode* newNode = new CityNode;
	newNode->city.name = new char[size + 1];
	for (int i = 0; i < size + 1; i++) {
		newNode->city.name[i] = name[i];
	}
	newNode->city.x = x;
	newNode->city.y = y;
	newNode->next = nullptr;
	if (head == nullptr) {
		head = newNode;
		return;
	}
	CityNode* current = head;
	while (current->next != nullptr) {
		current = current->next;
	}
	current->next = newNode;
}
void AddNewCityOn1stPos(CityNode*& head, CityNode* city) {
	CityNode* newNode = new CityNode;
	newNode->city = city->city; // Copy the data from city to the new node
	if (head == nullptr) {
		head = newNode;
		head->next = nullptr;
		return;
	}
	newNode->next = head;
	head = newNode;
}
void printCityList(CityNode* head) {
	CityNode* curr = head;
	while (curr != nullptr) {
		int i = 0;
		while (curr->city.name[i] != '\0') {
			cout << curr->city.name[i++];
		}
		cout << " ";
		curr = curr->next;
	}
	cout << endl;
}

// Queue List
void AddNewQueueNode(QueueNode*& head, int x, int y) {
	QueueNode* newNode = new QueueNode;
	newNode->x = x;
	newNode->y = y;
	newNode->next = nullptr;
	if (head == nullptr) {
		head = newNode;
		return;
	}
	QueueNode* current = head;
	while (current->next != nullptr) {
		current = current->next;
	}
	current->next = newNode;
}
void DeleteFirstQueue(QueueNode*& head) {
	if (head != nullptr && head->next != nullptr) {
		QueueNode* current = head;
		current = current->next;
		delete head;
		head = current;
		return;
	}
	if (head->next == nullptr) { delete[] head;  head = nullptr; }
}

// Setting coordinates of '*' 
void Searching(char** map, int x, int y, City* city, int width, int height) {
	if (x > 0) {
		if (map[y][x - 1] == '*') {
			city->x = x - 1;
			return;
		}
		else if (y < height - 1 && map[y + 1][x - 1] == '*') {
			city->x = x - 1;
			city->y = y + 1;
			return;
		}
		else if (y > 0 && map[y - 1][x - 1] == '*') {
			city->x = x - 1;
			city->y = y - 1;
			return;
		}
	}
	while ((map[y][x] != '*' && map[y][x] != '#' && map[y][x] != '.') && x <= width - 1) {
		if (y > 0 && map[y - 1][x] == '*') {
			city->x = x;
			city->y = y - 1;
			return;
		}
		else if (y < height - 1 && map[y + 1][x] == '*') {
			city->x = x;
			city->y = y + 1;
			return;
		}
		x++;
	}
	if (x <= width - 1) {
		if (map[y][x] == '*') {
			city->x = x;
			return;
		}
		else if (y < height - 1 && map[y + 1][x] == '*') {
			city->x = x;
			city->y = y + 1;
			return;
		}
		else if (y > 0 && map[y - 1][x] == '*') {
			city->x = x;
			city->y = y - 1;
			return;
		}
	}
}
void Adjustments(CityNode* head, char** map, int width, int height) {
	CityNode* current = head;
	while (current != nullptr) {
		Searching(map, current->city.x, current->city.y, &current->city, width, height);
		current = current->next;
	}
}

// Allocating flights into adjency list and use dijkstra's algorithm
void delete_vertex(AdjacencyList*& head, AdjacencyList* to_delete) {
	// Handle the case where the vertex to delete is the head of the list
	if (head == to_delete) {
		head = head->next;
	}

	// Update the pointers of adjacent vertices
	AdjacencyList::RelationNode* relation = to_delete->relations;
	while (relation != nullptr) {
		AdjacencyList* adjacent_vertex = relation->address;
		AdjacencyList::RelationNode* adjacent_relation = adjacent_vertex->relations;

		// Remove the relation to the deleted vertex from the adjacent vertex
		if (adjacent_relation->address == to_delete) {
			adjacent_vertex->relations = adjacent_relation->next;
		}
		else {
			while (adjacent_relation->next != nullptr) {
				if (adjacent_relation->next->address == to_delete) {
					adjacent_relation->next = adjacent_relation->next->next;
					break;
				}
				adjacent_relation = adjacent_relation->next;
			}
		}

		relation = relation->next;
	}

	// Delete the relations of the vertex to delete
	AdjacencyList::RelationNode* current_relation = to_delete->relations;
	while (current_relation != nullptr) {
		AdjacencyList::RelationNode* next_relation = current_relation->next;
		delete current_relation;
		current_relation = next_relation;
	}

	// Update the pointers of adjacent vertices to point to the previous vertex of the deleted vertex
	relation = to_delete->relations;
	while (relation != nullptr) {
		AdjacencyList* adjacent_vertex = relation->address;
		if (adjacent_vertex != to_delete->prev_vert) {
			relation->address->prev_vert = to_delete->prev_vert;
		}
		relation = relation->next;
	}

	// Remove the vertex from the list
	if (to_delete->prev_vert != nullptr) {
		to_delete->prev_vert->next = to_delete->next;
	}
	if (to_delete->next != nullptr) {
		to_delete->next->prev_vert = to_delete->prev_vert;
	}

	// Delete the vertex
	delete to_delete;
}
void Add_New_Adjacency_Vertex(AdjacencyList*& head, char* name, int size, int index) {
	AdjacencyList* NewNode = new AdjacencyList;
	NewNode->index = index;
	NewNode->relations = nullptr;
	NewNode->prev_vert = nullptr;
	NewNode->next = nullptr;
	NewNode->start_city.name = new char[size + 1];
	for (int i = 0; i < size + 1; i++) {
		NewNode->start_city.name[i] = name[i];
	}
	if (head == nullptr) {
		head = NewNode;
	}
	else {
		AdjacencyList* current = head;
		while (current->next != nullptr) {
			current = current->next;
		}
		current->next = NewNode;
	}
}
void Add_New_Relation(AdjacencyList::RelationNode** head, char* line, int space1, int space2, int i, AdjacencyList* Main_head) {
	// creating new node
	AdjacencyList::RelationNode* newNode = new AdjacencyList::RelationNode;
	newNode->airway.duration = 0;
	newNode->airway.name = new char[space2 - space1];
	for (int j = 0; j < space2 - space1; j++) {
		newNode->airway.name[j] = line[space1 + j + 1];
	}
	newNode->airway.name[space2 - space1 - 1] = '\0';
	int power = 1;
	for (int j = 0; j < i - space2 - 1; j++) {
		newNode->airway.duration += (int(line[i - j - 1]) - 48) * power;
		power *= 10;
	}
	AdjacencyList* current = Main_head;
	while (current != nullptr) {
		if (comparison(current->start_city.name, line, space1 + 1)) {
			newNode->address = current;
			break;
		}
		current = current->next;
	}
	newNode->next = nullptr;
	// finding a place to allocate this node
	if (*head == nullptr) *head = newNode;
	else {
		AdjacencyList::RelationNode* current = *head;
		while (current->next != nullptr) {
			if (comparison(current->airway.name, line, space1 + 1)) {
				if (newNode->airway.duration < current->airway.duration) {
					current->airway.duration = newNode->airway.duration;
					delete newNode;
				}
				return;
			}
			current = current->next;
		}
		if (comparison(current->airway.name, line, space1 + 1)) {
			if (newNode->airway.duration < current->airway.duration) {
				current->airway.duration = newNode->airway.duration;
				delete newNode;
			}
			return;
		}
		else {
			current->next = newNode;
		}
	}

}
void CreatingAdjacencyList(AdjacencyList*& head) {
	char line[MAXSIZE];
	int i = 0, step = 0, space1 = 0, space2 = 0;
	char c;
	cin >> c;
	while (c != '\n' && i < MAXSIZE - 1) {
		if (c == ' ') {
			if (space1 == 0) space1 = i;
			else space2 = i;
		}
		line[i++] = c;
		c = getchar();
	}
	AdjacencyList* current = head;
	while (current != nullptr) {
		if (comparison(current->start_city.name, line, 0)) {
			Add_New_Relation(&current->relations, line, space1, space2, i,head);
			return;
		}
		current = current->next;
	}
	return;
}
void PrintAdjacencyList(AdjacencyList* head) {
	AdjacencyList* current = head;
	while (current != nullptr) {
		cout << "From " << current->start_city.name << " to:\n";
		AdjacencyList::RelationNode* rel = current->relations;
		while (rel != nullptr) {
			cout << "\t" << rel->airway.name << " (" << rel->airway.duration << ")\n";
			rel = rel->next;
		}
		current = current->next;
	}
}


// dijkstra algorithm
bool find_smallest(AdjacencyList* head, bool* visited, AdjacencyList*& smallest) {
	AdjacencyList* current = head;
	int min = INT_MAX;
	while (current != nullptr) {
		if (!visited[current->index]) {
			if (current->start_city.duration < min) {
				min = current->start_city.duration;
				smallest = current;
			}
		}
		current = current->next;
	}
	if (min == INT_MAX) return false;
	return true;
}
bool Dijkstra_i_guess(AdjacencyList* head, char* start, char* end, int vertices, int mode, int shortest) {
	AdjacencyList* smallest = head;
	while (smallest != nullptr) {
		if (comparison(smallest->start_city.name, start, 0)) {
			smallest->start_city.duration = 0;
			break;
		}
		smallest = smallest->next;
	}
	if (smallest == nullptr || smallest->relations == nullptr) return false;
	bool* visited = new bool[vertices];
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
	}
	AdjacencyList::RelationNode* rel_first = smallest->relations;
	while (rel_first != nullptr) {
		rel_first->address->start_city.duration = rel_first->airway.duration;
		rel_first->address->prev_vert = smallest;
		rel_first = rel_first->next;
	}
	visited[smallest->index] = true;
	while (find_smallest(head, visited, smallest)) {
		if (comparison(smallest->start_city.name, end, 0)) {
			delete[] visited;
			if (smallest->start_city.duration < shortest) {
				cout << smallest->start_city.duration<<" ";
				if (mode == 1) {
					NameNode* Transfer_head = nullptr;
					smallest = smallest->prev_vert;
					while (smallest->start_city.duration != 0) {
						Add_New_Name(Transfer_head, smallest->start_city.name);
						smallest = smallest->prev_vert;
					}
					if(Transfer_head != nullptr) printTransferCities(Transfer_head);
					NameNode* current = Transfer_head;
					while (current != nullptr) {
						NameNode* temp = current;
						current = current->next;
						delete[] temp->name;
						delete temp;
					}
				}
				cout <<endl;
				return true;
			}
			return false;
		}
		rel_first = smallest->relations;
		while (rel_first != nullptr) {
			if (rel_first->airway.duration + smallest->start_city.duration < rel_first->address->start_city.duration) {
				rel_first->address->start_city.duration = rel_first->airway.duration + smallest->start_city.duration;
				rel_first->address->prev_vert = smallest;
			}
			rel_first = rel_first->next;
		}
		visited[smallest->index] = true;
	}
	delete[] visited;
	return false;
}

void Allocate(Connection* Connection) {
	char line[MAXSIZE];
	int i = 0, step = 0, space1 = 0, space2 = 0;
	char c;
	cin >> c;
	while (c != '\n' && i < MAXSIZE - 1) {
		if (c == ' ') {
			if (space1 == 0) space1 = i;
			else space2 = i;
		}
		line[i++] = c;
		c = getchar();
	}
	Connection->start = new char[space1 + 1];
	for (int j = 0; j < space1; j++) {
		Connection->start[j] = line[j];
	}
	Connection->start[space1] = '\0';
	Connection->destination = new char[space2 - space1];
	for (int j = 0; j < space2 - space1; j++) {
		Connection->destination[j] = line[space1 + j + 1];
	}
	Connection->destination[space2 - space1 - 1] = '\0';
	int power = 1;
	for (int j = 0; j < i - space2 - 1; j++) {
		Connection->mode += (int(line[i - j - 1]) - 48) * power;
		power *= 10;
	}
}

// Finding possibly shortest way from one point to another
void find(CityNode* head, char* name, City* re) {
	CityNode* current = head;
	while (current != nullptr) {
		if (comparison(current->city.name, name,0)) {
			re->name = current->city.name;
			re->x = current->city.x;
			re->y = current->city.y;
			return;
		}
		current = current->next;
	}
	return;
}
int add_adjacency_points(char** map, int** visited, QueueNode* queue_head, int x, int y, int end_x, int end_y, int width, int height) {
	int possibilities = 0;
	if (x == end_x && y == end_y) return 1;
	else {
		int temp_x, temp_y;
		int Horiz[4] = { 1, 0, 0, -1 };
		int Vert[4] = { 0, 1, -1, 0 };
		for (int i = 0; i < 4; i++) {
			temp_x = x + Horiz[i];
			temp_y = y + Vert[i];
			if (temp_x < 0 || temp_y < 0 || temp_x > width - 1 || temp_y > height - 1) continue;
			else {
				if (visited[temp_y][temp_x] == -1 && (map[temp_y][temp_x] == '#' || map[temp_y][temp_x] == '*')) {
					AddNewQueueNode(queue_head, temp_x, temp_y);
					visited[temp_y][temp_x] = visited[y][x] + 1;
					possibilities += 1;
				}
			}
		}
	}
	return 0;
}
void setfalse(int** map, int height, int width) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map[i][j] = -1;
		}
	}
}
void checkin(CityNode* head, CityNode*& sec, int x, int y) {
	CityNode* current = head;
	while (current != nullptr) {
		if (x == current->city.x && y == current->city.y) {
			AddNewCityOn1stPos(sec, current);
			return;
		}
		current = current->next;
	}
}
void goingback(char** map, int** visited, CityNode* head, int y, int x, int width, int height) {
	int temp_x, temp_y, first = 1;
	CityNode* secondhead = nullptr;
	int Horiz[4] = { 1, 0, 0, -1 };
	int Vert[4] = { 0, 1, -1, 0 };
	while (visited[y][x] != 1) {
		if (first) first = 0;
		else if (map[y][x] == '*') checkin(head, secondhead, x, y);
		for (int i = 0; i < 4; i++) {
			temp_x = x + Horiz[i];
			temp_y = y + Vert[i];
			if (temp_x < 0 || temp_y < 0 || temp_x > width - 1 || temp_y > height - 1) continue;
			else {
				if (visited[temp_y][temp_x] == visited[y][x] - 1) {
					y = temp_y;
					x = temp_x;
					break;
				}
			}
		}
	}
	printCityList(secondhead);
}
void shortestWayPossible(char** map, CityNode* head, Connection connections, int width, int height, AdjacencyList* Connections_head, int num) {
	if (connections.mode == 1 || connections.mode == 0) {
		City star_point, end_point;
		find(head, connections.start, &star_point);
		find(head, connections.destination, &end_point);
		int x = star_point.x, y = star_point.y;
		int end_x = end_point.x, end_y = end_point.y;
		int duration;
		QueueNode* queue_head = nullptr;
		int** visited = new int* [height];
	 	for (int i = 0; i < height; i++) {
			visited[i] = new int[width];
		}
		setfalse(visited, height, width);
		AddNewQueueNode(queue_head, x, y);
		visited[y][x] = 0;
		int reached_dest = 0, first = 1;
		while (reached_dest != 1) {
			if (first) first = 0;
			else DeleteFirstQueue(queue_head);
			if (queue_head == nullptr) { reached_dest = -1; break; }
			reached_dest = add_adjacency_points(map, visited, queue_head, queue_head->x, queue_head->y, end_x, end_y, width, height);
		}
		if (queue_head != nullptr) {
			if (!Dijkstra_i_guess(Connections_head, star_point.name, end_point.name, num, connections.mode, visited[queue_head->y][queue_head->x])) {
				cout << visited[queue_head->y][queue_head->x] << " ";
				if (connections.mode == 1) {
					if (visited[queue_head->y][queue_head->x] != 0) {
						goingback(map, visited, head, queue_head->y, queue_head->x, width, height);
					}
				}
			}
			cout << endl;
			return;
		}
		Dijkstra_i_guess(Connections_head, star_point.name, end_point.name, num, connections.mode, INT_MAX);
		return;
	}
}



int main() {
	int w, h, k, q, cnt = 0, index = 0;
	CityNode* head = nullptr;
	AdjacencyList* Connections_head = nullptr;
	cin >> w >> h;
	char** board;
	board = new char* [h];
	bool adding = false;
	for (int i = 0; i < h; i++) {
		board[i] = new char[w];
	}
	for (int i = 0; i < h; i++) {
		int x, y;
		char name[MAXSIZE];
		for (int j = 0; j < w; j++) {
			cin >> board[i][j];
			if (!adding && (board[i][j] != '*' && board[i][j] != '#' && board[i][j] != '.')) {
				adding = true;
				index = 0;
				name[index++] = board[i][j];
				x = j;
				y = i;
				if (x == w - 1) {
					name[index] = '\0';
					AddNewCity(head, name, x, y, index);
					Add_New_Adjacency_Vertex(Connections_head, name, index,cnt);
					cnt++;
				}
			}
			else if (adding) {
				if (board[i][j] != '*' && board[i][j] != '#' && board[i][j] != '.') {
					name[index++] = board[i][j];
				}
				else {

					adding = false;
					name[index] = '\0';
					AddNewCity(head, name, x, y, index);
					Add_New_Adjacency_Vertex(Connections_head, name, index,cnt);
					cnt++;
				}
			}
		}
	}
	Adjustments(head, board, w, h);
	cin >> q;
	for (int i = 0; i < q; i++) {
		CreatingAdjacencyList(Connections_head);
	}
	cin >> k;
	Connection* travels = new Connection[k];
	for (int i = 0; i < k; i++) {
		Allocate(&travels[i]);
		shortestWayPossible(board, head, travels[i], w, h, Connections_head,cnt);
	}
	for (int i = 0; i < h; i++) {
		delete[] board[i];
		board[i] = NULL;
	}
	delete[] board;
	board = NULL;
	/*
	while (Connections_head != nullptr) {
		AdjacencyList* to_delete = Connections_head;
		head = head->next;
		delete_vertex(Connections_head, to_delete);
	}
	*/
	return 0;
}
