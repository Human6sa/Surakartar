#pragma once
#include <vector>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <stdio.h>

namespace SingelThreadMCTS {
	const int EMPTY = -1;
	const int WHITE = 0;
	const int BLACK = 1;

	const int Board_len = 6;

	const int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	const int dy[8] = { 1, 0, -1, 1, -1, 1, 0, -1 };

	struct PairHash {
		template <typename T1, typename T2>
		std::size_t operator () (const std::pair<T1, T2>& p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);
			return h1 ^ (h2 << 1);
		}
	};

	std::unordered_map<std::pair<int, int>, std::tuple<int, int, char>, PairHash> Transmit {
		{ {0, 1}, { 1, 0, 'R' }},
		{ {1, 0}, {0, 1, 'D'} },
		{ {0, 2}, {2, 0, 'R'} },
		{ {2, 0}, {0, 2, 'D'} },
		{ {0, 3}, {2, 5, 'L'} },
		{ {2, 5}, {0, 3, 'D'} },
		{ {0, 4}, {1, 5, 'L'} },
		{ {1, 5}, {0, 4, 'D'} },
		{ {3, 0}, {5, 2, 'U'} },
		{ {5, 2}, {3, 0, 'R'} },
		{ {4, 0}, {5, 1, 'U'} },
		{ {5, 1}, {4, 0, 'R'} },
		{ {5, 3}, {3, 5, 'L'} },
		{ {3, 5}, {5, 3, 'U'} },
		{ {5, 4}, {4, 5, 'L'} },
		{ {4, 5}, {5, 4, 'U'} }
	};

	class Board {
	private:
		std::vector<std::vector<int>> board;

		int current_player;

		int WHITE_num;
		int BLACK_num;

		int stage;
		int pre_action;
	public:
		Board();
		Board(const Board& pre_board);

		void init();

		inline bool is_in_board(int x, int y);
		inline bool is_not_corner(int x, int y);

		bool check(int x, int y, int real_x, int real_y, int current_player, int cnt);
		int get_eat(int x, int y, int real_x, int real_y, int current_player, int cnt);

		bool get_select(int x, int y, int current_player);
		std::vector<int> get_choose(int x, int y, int current_player);
		std::vector<int> getAvailable();
	};
}