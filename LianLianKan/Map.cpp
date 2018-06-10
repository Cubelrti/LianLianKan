#include "Map.h"

#define WIDTH 18
#define HEIGHT 10

using namespace std;

vector<int> props = { 28, 9, 26, 30 ,29, 44 ,42 ,41 };

Map::Map() {

}

Map::~Map() {

}

int Map::get_direction_lightning(int pre, int post) {
	if (abs(pre - post) <= 1) {
		if (pre == D || pre == U)
			return UD;
		else
			return LR;
	}
	else if ((pre == R && post == U) || (pre == D && post == L))
		return RD;
	else if ((pre == R && post == D) || (pre == U && post == L))
		return RU;
	else if ((pre == L && post == U) || (pre == D && post == R))
		return LD;
	else if ((pre == L && post == D) || (pre == U && post == R))
		return LU;
	return -1;
}

int Map::get_direction_point(vector<int> vct) {
	if (vct[0] == 1 && vct[1] == 0) return D;
	if (vct[0] == -1 && vct[1] == 0) return U;
	if (vct[0] == 0 && vct[1] == 1) return R;
	if (vct[0] == 0 && vct[1] == -1) return L;
	return -1;
}

vector<int> Map::getDirections(vector<vector<int>> route) {
	vector<int> directionVct;
	vector<int> order;
	if (route.empty())
		return directionVct;
	int diff_x;
	int diff_y;
	for (int i = 0; i < route.size() - 1; i++) {
		diff_x = route[i + 1][0] - route[i][0];
		diff_y = route[i + 1][1] - route[i][1];
		order.push_back(get_direction_point({ diff_x, diff_y }));
	}
	for (int i = 0; i < order.size() - 1; i++) {
		directionVct.push_back(get_direction_lightning(order[i], order[i + 1]));
	}
	return directionVct;
}

bool Map::full_true(vector<bool> flags) {
	for (auto flag : flags)
		if (!flag) return false;
	return true;
}

bool Map::in_props(int pos)
{
	for (int i = 0; i < props.size(); i++)
		if (pos == props[i])
			return true;
	return false;
}

bool Map::around_block(int x, int y)
{
	if (map[x + 1][y] || map[x - 1][y] || map[x][y + 1] || map[x][y - 1])
		return true;
	if (map[x + 1][y + 1] || map[x - 1][y - 1] || map[x + 1][y - 1] ||
		map[x - 1][y + 1])
		return true;
	return false;
}

bool Map::isLineLinkable(int x1, int y1, int x2, int y2) {
	if (x1 == x2) {
		if (y1 > y2) std::swap(y1, y2);
		for (y1 += 1; y1 < y2; ++y1)
			if (map[x1][y1]) return false;

	}
	else if (y1 == y2) {
		if (x1 > x2) std::swap(x1, x2);
		for (x1 += 1; x1 < x2; ++x1)
			if (map[x1][y1]) return false;
	}
	else
		return false;

	return true;
}

vector<vector<int>> Map::makeMap(int order) {
	int number_of_props = 0;
	if (order >= 1 && order <= 4)
		number_of_props = 3;
	else if (order >= 5 && order <= 12)
		number_of_props = 4;
	else if (order >= 13)
		number_of_props = 5;
	map.clear();
	vector<bool> flags;
	vector<vector<int>> position;
	vector<int> frequency(45, 0);
	srand((unsigned)time(NULL));

	string filename = "./Maps/Map" + std::to_string(order) + ".txt";
	fstream readMap;
	readMap.open(filename.c_str());
	int row = 0;
	int col = 0;
	if (!readMap.is_open()) {
		throw "MAP NOT FOUND";
	}
	else {
		vector<int> elements;
		while (!readMap.eof()) {
			char i;
			if (col < WIDTH) {
				readMap.get(i);
				elements.push_back((int)i - '0');
				int a = (int)i - '0';
				if (i == '1') {
					position.push_back({ row, col });
					flags.push_back(false);
				}
				col++;
			}
			else {
				col = 0;
				row++;
				map.push_back(elements);
				elements.clear();
				readMap.get(i);
			}
		}
		readMap.close();
	}
	int len = position.size();
	int tmp_len = len;
	while (number_of_props) {
		int element1 = rand() % len;
		while (flags[element1])
			element1 = rand() % len;
		int element2 = rand() % len;
		while (flags[element2] || element1 == element2)
			element2 = rand() % len;
		flags[element1] = true;
		flags[element2] = true;
		int rand_props = rand() % props.size();
		map[position[element1][0]][position[element1][1]] = props[rand_props];
		map[position[element2][0]][position[element2][1]] = props[rand_props];
		number_of_props--;
	}
	for (int i = 0; i < len; i++) {
		if (flags[i]) {
			tmp_len--;
			continue;
		};
		int j = i + (rand() % (tmp_len - 1)) + 1;
		while (flags[j])
			j = i + (rand() % (tmp_len - 1)) + 1;
		flags[i] = true;
		flags[j] = true;
		tmp_len--;
		int rand_element = 1 + (rand() % 44);
		int maxProps = 3;
		while (1) {
			//if (in_props(rand_element))
			//	maxProps = 1;
			//else
			//	maxProps = 3;
			if (!in_props(rand_element)) {
				if (frequency[rand_element] < maxProps) {
					frequency[rand_element]++;
					break;
				}
				else
					rand_element = 1 + (rand() % 44);
			}
			else
				rand_element = 1 + (rand() % 44);
		}
		map[position[i][0]][position[i][1]] = rand_element;
		map[position[j][0]][position[j][1]] = rand_element;
		if (full_true(flags)) break;
	}
	return map;
}

vector<vector<int>> Map::mirror()
{
	bool flags[10][18] = { false };
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			if (map[i][j] && !flags[i][j]) {
				std::swap(map[i][j], map[i][WIDTH - j - 1]);
				flags[i][j] = true;
				flags[i][WIDTH - j - 1] = true;
			}
	return map;
}

vector<vector<int>> Map::makeMap(Difficulty level) {
	srand((unsigned)time(NULL));
	int order = 0;
	if (level == easy)
		order = 1 + (rand() % 4);
	else if (level == normal)
		order = 5 + (rand() % 8);
	else if (level == hard)
		order = 13 + (rand() % 4);
	return makeMap(order);
}

vector<vector<int>> Map::connection(int x1, int y1, int x2, int y2, bool promptFlag) {
	vector<vector<int>> route;
	int tmp_x1 = x1;
	int tmp_x2 = x2;
	int tmp_y1 = y1;
	int tmp_y2 = y2;
	if (map[x1][y1] != map[x2][y2] || (x1 == x2 && y1 == y2)) return route;
	if (x1 == x2 && isLineLinkable(x1, y1, x2, y2)) {
		if (y1 > y2) swap(y1, y2);
		for (y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
		if (!promptFlag) {
			map[tmp_x1][tmp_y1] = 0;
			map[tmp_x2][tmp_y2] = 0;
		}
		return route;
	}
	if (y1 == y2 && isLineLinkable(x1, y1, x2, y2)) {
		if (x1 > x2) swap(x1, x2);
		for (x1; x1 <= x2; x1++) route.push_back({ x1, y1 });
		if (!promptFlag) {
			map[tmp_x1][tmp_y1] = 0;
			map[tmp_x2][tmp_y2] = 0;
		}
		return route;
	}
	for (int i = 0; i < WIDTH; i++) {
		if (!map[x1][i] && isLineLinkable(x1, i, x1, y1)) {
			if (isLineLinkable(x1, i, x2, y2)) {
				if (i >= y1) {
					for (y1; y1 <= i; y1++) route.push_back({ x1, y1 });
					y1--;
				}
				else {
					for (y1; y1 >= i; y1--) route.push_back({ x1, y1 });
					y1++;
				}
				if (x1 < x2)
					for (++x1; x1 <= x2; x1++) route.push_back({ x1, y1 });
				else
					for (--x1; x1 >= x2; x1--) route.push_back({ x1, y1 });
				if (!promptFlag) {
					map[tmp_x1][tmp_y1] = 0;
					map[tmp_x2][tmp_y2] = 0;
				}
				return route;
			}
		}
	}
	for (int i = 0; i < HEIGHT; i++) {
		if (!map[i][y1] && isLineLinkable(i, y1, x1, y1)) {
			if (isLineLinkable(i, y1, x2, y2)) {
				if (i >= x1) {
					for (x1; x1 <= i; x1++) route.push_back({ x1, y1 });
					x1--;
				}
				else {
					for (x1; x1 >= i; x1--) route.push_back({ x1, y1 });
					x1++;
				}
				if (y1 < y2)
					for (++y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
				else
					for (--y1; y1 >= y2; y1--) route.push_back({ x1, y1 });
				if (!promptFlag) {
					map[tmp_x1][tmp_y1] = 0;
					map[tmp_x2][tmp_y2] = 0;
				}
				return route;
			}
		}
	}
	int alpha = -1;
	int beta = -1;
	for (int i = x1 + 1; i < HEIGHT; i++)
		if (!map[i][y1] && isLineLinkable(i, y1, x1, y1) && !map[i][y2] && isLineLinkable(i, y2, x2, y2) &&
			isLineLinkable(i, y1, i, y2)) {
			alpha = i;
			break;
		}
	for (int i = x1 - 1; i >= 0; i--)
		if (!map[i][y1] && isLineLinkable(i, y1, x1, y1) && !map[i][y2] && isLineLinkable(i, y2, x2, y2) &&
			isLineLinkable(i, y1, i, y2)) {
			beta = i;
			break;
		}
	if (alpha != -1 || beta != -1) {
		int i = find_right_number(alpha, beta, x1, -1);
		if (i >= x1) {
			for (x1; x1 <= i; x1++) route.push_back({ x1, y1 });
			x1--;
		}
		else {
			for (x1; x1 >= i; x1--) route.push_back({ x1, y1 });
			x1++;
		}
		if (y1 > y2) {
			for (--y1; y1 >= y2; y1--) route.push_back({ x1, y1 });
			y1++;
		}
		else {
			for (++y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
			y1--;
		}
		if (x1 > x2)
			for (--x1; x1 >= x2; x1--) route.push_back({ x1, y1 });
		else
			for (++x1; x1 <= x2; x1++) route.push_back({ x1, y1 });
		if (!promptFlag) {
			map[tmp_x1][tmp_y1] = 0;
			map[tmp_x2][tmp_y2] = 0;
		}
		return route;
	}
	alpha = -1;
	beta = -1;
	for (int i = y1 + 1; i < WIDTH; i++) {
		if (!map[x1][i] && isLineLinkable(x1, i, x1, y1) && !map[x2][i] && isLineLinkable(x2, i, x2, y2) &&
			isLineLinkable(x1, i, x2, i)) {
			alpha = i;
			break;
		}
	}
	for (int i = y1 - 1; i >= 0; i--) {
		if (!map[x1][i] && isLineLinkable(x1, i, x1, y1) && !map[x2][i] && isLineLinkable(x2, i, x2, y2) &&
			isLineLinkable(x1, i, x2, i)) {
			beta = i;
			break;
		}
	}
	if (alpha != -1 || beta != -1) {
		int i = find_right_number(alpha, beta, -1, y1);
		if (i >= y1) {
			for (y1; y1 <= i; y1++) route.push_back({ x1, y1 });
			y1--;
		}
		else {
			for (y1; y1 >= i; y1--) route.push_back({ x1, y1 });
			y1++;
		}
		if (x1 < x2) {
			for (++x1; x1 <= x2; x1++) route.push_back({ x1, y1 });
			x1--;
		}
		else {
			for (--x1; x1 >= x2; x1--) route.push_back({ x1, y1 });
			x1++;
		}
		if (y1 > y2)
			for (--y1; y1 >= y2; y1--) route.push_back({ x1, y1 });
		else
			for (++y1; y1 <= y2; y1++) route.push_back({ x1, y1 });
		if (!promptFlag) {
			map[tmp_x1][tmp_y1] = 0;
			map[tmp_x2][tmp_y2] = 0;
		}
		return route;
	}

	return route;
}

vector<vector<int>> Map::rearrange() {
	srand((unsigned)time(NULL));
	vector<int> frequencies(45, 0);
	vector<vector <int>> position;
	vector<bool> flags;
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++) {
			if (map[i][j]) {
				position.push_back({ i,j });
				flags.push_back(false);
				frequencies[map[i][j]]++;
			}
		}

	int len = position.size();
	int tmp_len = len;
	for (int i = 1; i < 45; i++) {
		while (frequencies[i]) {
			int count = 0;
			int tmp = 0;
			int dis = 1;
			int element1 = rand() % len;
			tmp = element1;
			while (flags[element1]) {
				if (count <= 1) {
					element1 = abs((tmp + (dis * (int)pow(-1, count))) % len);
					count++;
					continue;
				}
				count = 0;
				dis++;
				element1 = element1 >= 0 ? element1 : 0;
			}
			count = 0;
			dis = 1;
			int element2 = rand() % len;
			tmp = element2;
			while (element2 == element1 || flags[element2]) {
				if (count <= 1) {
					element2 = abs((tmp + (dis * (int)pow(-1, count))) % len);
					count++;
					continue;
				}
				count = 0;
				dis++;
			}
			flags[element1] = true;
			flags[element2] = true;
			map[position[element1][0]][position[element1][1]] = i;
			map[position[element2][0]][position[element2][1]] = i;
			frequencies[i] -= 2;
		}
	}
	return map;
}

vector<vector<int>> Map::prompt() {
	vector<vector <int>> route;
	vector<vector <int>>position;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			if (map[i][j]) {
				position.push_back({ map[i][j],i,j });
			}
	}
	for (int i = 0; i < position.size(); i++) {
		for (int j = i + 1; j < position.size(); j++)
			if (position[i][0] == position[j][0]) {
				route = connection(position[i][1], position[i][2], position[j][1], position[j][2], true);
				if (route.size())
					return route;
			}
	}
	return route;
}

vector<vector<int>> Map::obstacle()
{
	srand((unsigned)time(NULL));
	vector<vector<int>> position;
	vector<bool> props_flags(45, false);
	vector<bool> blank_flags;
	int total = 6;
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			if (!map[i][j]) {
				position.push_back({ i, j });
				blank_flags.push_back(false);
			}
	int len = position.size();
	int tmp_len = len;
	while (total && tmp_len) {
		int element1 = rand() % len;
		while (blank_flags[element1] &&
			!around_block(position[element1][0], position[element1][1]))
			element1 = rand() % len;
		int element2 = rand() % len;
		while (blank_flags[element2] && element2 == element1 &&
			!around_block(position[element2][0], position[element2][1]))
			element2 = rand() % len;
		int rand_element = 1 + (rand() % 44);
		while (in_props(rand_element) || props_flags[rand_element])
			rand_element = 1 + (rand() % 44);
		props_flags[rand_element] = true;
		map[position[element1][0]][position[element1][1]] = rand_element;
		map[position[element2][0]][position[element2][1]] = rand_element;
		total--;
		tmp_len -= 2;
	}
	return map;
}

int Map::find_right_number(int alpha, int beta, int x, int y) {
	int tmp_alpha = alpha;
	int tmp_beta = beta;
	if (tmp_alpha == -1 && tmp_beta != -1)
		return beta;
	if (tmp_alpha != -1 && tmp_beta == -1)
		return alpha;
	if (tmp_alpha != -1 && tmp_beta != -1) {
		if (x != -1 && y == -1) {
			if (abs(tmp_alpha - x) > abs(tmp_beta - x))
				swap(tmp_alpha, tmp_beta);
			return tmp_alpha;
		}
		if (x == -1 && y != -1) {
			if (abs(tmp_alpha - y) > abs(tmp_beta - y))
				swap(tmp_alpha, tmp_beta);
			return tmp_alpha;
		}

	}
	return 0;
}