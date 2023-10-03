#include "board.h"

using namespace SingelThreadAlphaBetaPro;

long long SingelThreadAlphaBetaPro::CompressedMatrix(const std::vector<std::vector<int>>& board, const int& color) {
	long long res = 0;
	for (int i = 0; i < Board_len; ++i) {
		for (int j = 0; j < Board_len; ++j) {
			if (board[i][j] == color) {
				res |= (1LL << (i * Board_len + j));
			}
		}
	}
	return res;
}

std::vector<std::vector<int>> SingelThreadAlphaBetaPro::decompression(const long long& MatrixWHITE, const long long& MatrixBLACK) {
	std::vector<std::vector<int>> board(Board_len, std::vector<int>(Board_len, -1));
	for (int i = 0; i < Board_len; ++i) {
		for (int j = 0; j < Board_len; ++j) {
			if (check(MatrixWHITE, i, j)) {
				board[i][j] = WHITE;
			}
			else if (check(MatrixBLACK, i, j)) {
				board[i][j] = BLACK;
			}
		}
	}
	return board;
}

bool SingelThreadAlphaBetaPro::check(const long long& Matrix, int& x, int& y) {
	return (Matrix >> (x * Board_len + y)) & 1LL;
}

bool SingelThreadAlphaBetaPro::check(const long long& Matrix, const std::pair<int, int>& pd) {
	return (Matrix >> (pd.first * Board_len + pd.second)) & 1LL;
}

long long SingelThreadAlphaBetaPro::Transform(const std::pair<int, int>& pos) {
	return 1LL << (pos.first * Board_len + pos.second);
}

void SingelThreadAlphaBetaPro::change_bit(long long& Matrix, int& x, int& y) {
	Matrix ^= (1LL << (x * Board_len + y));
}
void SingelThreadAlphaBetaPro::change_bit(long long& Matrix, const std::pair<int, int>& pd) {
	Matrix ^= (1LL << (pd.first * Board_len + pd.second));
}

int SingelThreadAlphaBetaPro::fb(const int& x, const int& y) {
	return x >= y ? x - y : y - x;
}

int SingelThreadAlphaBetaPro::get_dis(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
	return fb(p1.first, p2.first) + fb(p1.second, p2.second);
}

void SingelThreadAlphaBetaPro::init() {
	for (int i = 0; i < (road_len << 1); ++i) {
		inside[i] = SingelThreadAlphaBetaPro::Transform(inside_original[i]);
		outside[i] = SingelThreadAlphaBetaPro::Transform(outside_original[i]);
	}

	for (int i = 0; i < road_len; ++i) {
		if (!get_pos[0].count(inside_original[i])) {
			get_pos[0][inside_original[i]] = i;
		}
		else {
			get_pos[1][inside_original[i]] = i;
		}
		if (!get_pos[4].count(outside_original[i])) {
			get_pos[4][outside_original[i]] = i;
		}
		else {
			get_pos[5][outside_original[i]] = i;
		}
	}
	for (int i = (road_len << 1) - 1; i >= road_len; --i) {
		if (!get_pos[2].count(inside_original[i])) {
			get_pos[2][inside_original[i]] = i;
		}
		else {
			get_pos[3][inside_original[i]] = i;
		}
		if (!get_pos[6].count(outside_original[i])) {
			get_pos[6][outside_original[i]] = i;
		}
		else {
			get_pos[7][outside_original[i]] = i;
		}
	}

	for (int i = 1; i < (road_len << 1); ++i) {
		if (get_dis(inside_original[i], inside_original[i - 1]) > 1) along[i] = true;
	}
	for (int i = (road_len << 1) - 2; i >= 0; --i) {
		if (get_dis(inside_original[i], inside_original[i + 1]) > 1) against[i] = true;
	}

	SingelThreadAlphaBetaPro::init_WHITE_board = CompressedMatrix(init_board, WHITE);
	SingelThreadAlphaBetaPro::init_BLACK_board = CompressedMatrix(init_board, BLACK);
}

Board::Board() {
	current_player = BLACK;

	WHITE_num = BLACK_num = 12;

	WHITE_board = init_WHITE_board;
	BLACK_board = init_BLACK_board;
}

Board::Board(const Board& pre_board) {
	this->current_player = pre_board.current_player;
	this->WHITE_num = pre_board.WHITE_num;
	this->BLACK_num = pre_board.BLACK_num;
	this->WHITE_board = pre_board.WHITE_board;
	this->BLACK_board = pre_board.BLACK_board;
}

Board::Board(int cp, int wn, int bn, long long wb, long long bd) : current_player(cp), WHITE_num(wn), BLACK_num(bn), WHITE_board(wb), BLACK_board(bd) {}

bool Board::is_in_board(int x, int y) {
	return x >= 0 && x < Board_len && y >= 0 && y < Board_len;
}

bool Board::is_not_corner(int x, int y) {
	if (x == 0 || x == 5) {
		return y != 0 && y != 5;
	}
	return true;
}

std::vector<Board> Board::get_move(int x, int y, int color) {
	std::vector<Board> res;
	for (int i = 0; i < 8; ++i) {
		int nx = x + dx[i], ny = y + dy[i];
		if (is_in_board(nx, ny) && !check(WHITE_board, nx, ny) && !check(BLACK_board, nx, ny)) {
			long long wd = this->WHITE_board, bd = this->BLACK_board;
			if (color == WHITE) {
				change_bit(wd, x, y);
				change_bit(wd, nx, ny);
			}
			else {
				change_bit(bd, x, y);
				change_bit(bd, nx, ny);
			}
			res.push_back({ this->current_player ^ 1, this->WHITE_num, this->BLACK_num, wd, bd });
		}
	}
	return res;
}

std::vector<Board> Board::get_attack(int x, int y, int color) {
	if (!is_not_corner(x, y)) return {};
	std::vector<Board> res;
	if (get_pos[0].count({ x, y })) {
		bool through_ring = false, origin_point = false;
		for (int i = get_pos[0][{x, y}] + 1, j = 1; j < road_len; ++i, ++j) {
			if (inside_original[i].first == x && inside_original[i].second == y) {
				origin_point = true;
				continue;
			}
			if (along[i]) {
				through_ring = true;
			}
			bool is_black = check(BLACK_board, inside_original[i]), is_white = check(WHITE_board, inside_original[i]);
			if (is_black) {
				if (color == WHITE && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(wd, x, y);
					change_bit(wd, inside_original[i]);
					change_bit(bd, inside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
				}
				break;
			}
			else if (is_white) {
				if (color == BLACK && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(bd, x, y);
					change_bit(bd, inside_original[i]);
					change_bit(wd, inside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
				}
				break;
			}
		}
		if (!origin_point && get_pos[1].count({ x, y })) {
			through_ring = false;
			for (int i = get_pos[1][{x, y}] + 1, j = 1; j < road_len; ++i, ++j) {
				if (along[i]) {
					through_ring = true;
				}
				bool is_black = check(BLACK_board, inside_original[i]), is_white = check(WHITE_board, inside_original[i]);
				if (is_black) {
					if (color == WHITE && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(wd, x, y);
						change_bit(wd, inside_original[i]);
						change_bit(bd, inside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
					}
					break;
				}
				else if (is_white) {
					if (color == BLACK && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(bd, x, y);
						change_bit(bd, inside_original[i]);
						change_bit(wd, inside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
					}
					break;
				}
			}
		}
		origin_point = through_ring = false;
		for (int i = get_pos[2][{x, y}] - 1, j = 1; j < road_len; --i, ++j) {
			if (inside_original[i].first == x && inside_original[i].second == y) {
				origin_point = true;
				continue;
			}
			if (against[i]) {
				through_ring = true;
			}
			bool is_black = check(BLACK_board, inside_original[i]), is_white = check(WHITE_board, inside_original[i]);
			if (is_black) {
				if (color == WHITE && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(wd, x, y);
					change_bit(wd, inside_original[i]);
					change_bit(bd, inside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
				}
				break;
			}
			else if (is_white) {
				if (color == BLACK && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(bd, x, y);
					change_bit(bd, inside_original[i]);
					change_bit(wd, inside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
				}
				break;
			}
		}
		if (!origin_point && get_pos[3].count({ x, y })) {
			through_ring = false;
			for (int i = get_pos[3][{x, y}] - 1, j = 1; j < road_len; --i, ++j) {
				if (against[i]) {
					through_ring = true;
				}
				bool is_black = check(BLACK_board, inside_original[i]), is_white = check(WHITE_board, inside_original[i]);
				if (is_black) {
					if (color == WHITE && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(wd, x, y);
						change_bit(wd, inside_original[i]);
						change_bit(bd, inside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
					}
					break;
				}
				else if (is_white) {
					if (color == BLACK && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(bd, x, y);
						change_bit(bd, inside_original[i]);
						change_bit(wd, inside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
					}
					break;
				}
			}
		}
	}
	if (get_pos[4].count({ x, y })) {
		bool through_ring = false, origin_point = false;
		for (int i = get_pos[4][{x, y}] + 1, j = 1; j < road_len; ++i, ++j) {
			if (outside_original[i].first == x && outside_original[i].second == y) {
				origin_point = true;
				continue;
			}
			if (along[i]) {
				through_ring = true;
			}
			bool is_black = check(BLACK_board, outside_original[i]), is_white = check(WHITE_board, outside_original[i]);
			if (is_black) {
				if (color == WHITE && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(wd, x, y);
					change_bit(wd, outside_original[i]);
					change_bit(bd, outside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
				}
				break;
			}
			else if (is_white) {
				if (color == BLACK && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(bd, x, y);
					change_bit(bd, outside_original[i]);
					change_bit(wd, outside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
				}
				break;
			}
		}
		if (!origin_point && get_pos[5].count({ x, y })) {
			through_ring = false;
			for (int i = get_pos[5][{x, y}] + 1, j = 1; j < road_len; ++i, ++j) {
				if (along[i]) {
					through_ring = true;
				}
				bool is_black = check(BLACK_board, outside_original[i]), is_white = check(WHITE_board, outside_original[i]);
				if (is_black) {
					if (color == WHITE && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(wd, x, y);
						change_bit(wd, outside_original[i]);
						change_bit(bd, outside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
					}
					break;
				}
				else if (is_white) {
					if (color == BLACK && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(bd, x, y);
						change_bit(bd, outside_original[i]);
						change_bit(wd, outside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
					}
					break;
				}
			}
		}
		origin_point = through_ring = false;
		for (int i = get_pos[6][{x, y}] - 1, j = 1; j < road_len; --i, ++j) {
			if (outside_original[i].first == x && outside_original[i].second == y) {
				origin_point = true;
				continue;
			}
			if (against[i]) {
				through_ring = true;
			}
			bool is_black = check(BLACK_board, outside_original[i]), is_white = check(WHITE_board, outside_original[i]);
			if (is_black) {
				if (color == WHITE && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(wd, x, y);
					change_bit(wd, outside_original[i]);
					change_bit(bd, outside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
				}
				break;
			}
			else if (is_white) {
				if (color == BLACK && through_ring) {
					long long wd = WHITE_board, bd = BLACK_board;
					change_bit(bd, x, y);
					change_bit(bd, outside_original[i]);
					change_bit(wd, outside_original[i]);
					res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
				}
				break;
			}
		}
		if (!origin_point && get_pos[7].count({ x, y })) {
			through_ring = false;
			for (int i = get_pos[7][{x, y}] - 1, j = 1; j < road_len; --i, ++j) {
				if (against[i]) {
					through_ring = true;
				}
				bool is_black = check(BLACK_board, outside_original[i]), is_white = check(WHITE_board, outside_original[i]);
				if (is_black) {
					if (color == WHITE && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(wd, x, y);
						change_bit(wd, outside_original[i]);
						change_bit(bd, outside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num, BLACK_num - 1, wd, bd });
					}
					break;
				}
				else if (is_white) {
					if (color == BLACK && through_ring) {
						long long wd = WHITE_board, bd = BLACK_board;
						change_bit(bd, x, y);
						change_bit(bd, outside_original[i]);
						change_bit(wd, outside_original[i]);
						res.push_back({ current_player ^ 1, WHITE_num - 1, BLACK_num, wd, bd });
					}
					break;
				}
			}
		}
	}
	return res;
}

std::vector<Board> Board::get_children() {
	std::vector<Board> res;
	for (int i = 0; i < Board_len; ++i) {
		for (int j = 0; j < Board_len; ++j) {
			if (this->current_player == BLACK) {
				if (check(this->BLACK_board, i, j)) {
					std::vector<Board> mc = get_move(i, j, BLACK);
					std::vector<Board> ac = get_attack(i, j, BLACK);
					std::sort(ac.begin(), ac.end());
					auto mid = std::unique(ac.begin(), ac.end());
					res.insert(res.end(), ac.begin(), mid);
					res.insert(res.end(), mc.begin(), mc.end());
				}
			} else {
				if (check(this->WHITE_board, i, j)) {
					std::vector<Board> mc = get_move(i, j, WHITE);
					std::vector<Board> ac = get_attack(i, j, WHITE);
					std::sort(ac.begin(), ac.end());
					auto mid = std::unique(ac.begin(), ac.end());
					res.insert(res.end(), ac.begin(), mid);
					res.insert(res.end(), mc.begin(), mc.end());
				}
			}
		}
	}
	return res;
}

void Board::print_board() {
	std::vector<std::vector<int>> bd = decompression(this->WHITE_board, this->BLACK_board);
	for (int i = 0; i < Board_len; ++i) {
		for (int j = 0; j < Board_len; ++j) {
			printf("%3d ", bd[i][j]);
		}
		putchar('\n');
	}
}