#pragma once

#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <iostream>
#include <algorithm>

namespace SingelThreadAlphaBetaPro {
	static const int EMPTY = -1;
	static const int WHITE = 0;
	static const int BLACK = 1;

	static const int Board_len = 6;
	static const int road_len = 24;

	static const int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	static const int dy[8] = { 1, 0, -1, 1, -1, 1, 0, -1 };

	static const std::vector<std::vector<int>> init_board = {
		{BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
		{BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
		{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
		{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
		{WHITE, WHITE, WHITE, WHITE, WHITE, WHITE},
		{WHITE, WHITE, WHITE, WHITE, WHITE, WHITE},
	};

	static long long init_WHITE_board;
	static long long init_BLACK_board;

	struct ScoreHashFunction {
		size_t operator()(const std::pair<long long, long long>& p) const {
			const long long prime = 2654435769ULL;
			long long hashValue = 0;
			hashValue ^= p.first + prime + (hashValue << 6) + (hashValue >> 2);
			hashValue ^= p.second + prime + (hashValue << 6) + (hashValue >> 2);
			return hashValue;
		}
	};

	struct PosHashFunction {
		size_t operator()(const std::pair<int, int>& p) const {
			return (p.first << 3) | p.second;
		}
	};

	struct ScoreEqualFunction {
		bool operator()(const std::pair<long long, long long>& lhs,
			const std::pair<long long, long long>& rhs) const {
			return lhs.first == rhs.first && lhs.second == rhs.second;
		}
	};

	struct PosEqualFunction {
		bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const {
			return lhs.first == rhs.first && lhs.second == rhs.second;
		}
	};

	static std::unordered_map<std::pair<long long, long long>, int, ScoreHashFunction, ScoreEqualFunction> Scores;
	static std::unordered_map<std::pair<int, int>, int, PosHashFunction, PosEqualFunction> get_pos[8];

	static const std::pair<int, int> inside_original[road_len << 1] = {
		{1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4},
		{4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1},
		{1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4},
		{4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}
	};
	static const std::pair<int, int> outside_original[road_len << 1] = {
		{2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3},
		{3, 5}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2},
		{2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3},
		{3, 5}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}
	};

	static std::vector<int> along(road_len << 1), against(road_len << 1);

	static std::vector<long long> inside(road_len << 1, false), outside(road_len << 1, false);

	void init();

	inline int fb(const int& x, const int& y);
	inline int get_dis(const std::pair<int, int>& p1, const std::pair<int, int>& p2);

	inline long long CompressedMatrix(const std::vector<std::vector<int>>& board, const int& color);
	inline std::vector<std::vector<int>> decompression(const long long& MatrixWHITE, const long long& MatrixBLACK);
	inline bool check(const long long& Matrix, int& x, int& y);
	inline bool check(const long long& Matrix, const std::pair<int, int>& pd);
	inline long long Transform(const std::pair<int, int>& pos);
	inline void change_bit(long long& Matrix, int& x, int& y);
	inline void change_bit(long long& Matrix, const std::pair<int, int>& pd);

	class Board {
	public:
		int current_player;

		int WHITE_num;
		int BLACK_num;

		long long WHITE_board;
		long long BLACK_board;

		Board();
		Board(const Board& pre_board);
		Board(int cp, int wn, int bn, long long wb, long long bd);

		inline bool is_in_board(int x, int y);
		inline bool is_not_corner(int x, int y);

		std::vector<Board> get_move(int x, int y, int color);
		std::vector<Board> get_attack(int x, int y, int color);
		std::vector<Board> get_children();

		bool operator == (const Board& other) const {
			return WHITE_board == other.WHITE_board &&
				BLACK_board == other.BLACK_board;
		}

		bool operator != (const Board& other) const {
			return !(*this == other);
		}

		bool operator < (const Board& other) const {
			if (WHITE_num != other.WHITE_num) {
				return WHITE_num < other.WHITE_num;
			}
			if (BLACK_num != other.BLACK_num) {
				return BLACK_num < other.BLACK_num;
			}
			if (WHITE_board != other.WHITE_board) {
				return WHITE_board < other.WHITE_board;
			}
			if (BLACK_board != other.BLACK_board) {
				return BLACK_board < other.BLACK_board;
			}
			return false;
		}

		void print_board();
	};
}