#include "board.h"
using namespace SingelThreadMCTS;

Board::Board() {
	this->board = std::vector<std::vector<int>>(Board_len, std::vector<int>(Board_len, EMPTY));
	init();
}

Board::Board(const Board& pre_board) {
	this->board = pre_board.board;
	this->current_player = pre_board.current_player;
	this->WHITE_num = pre_board.WHITE_num;
	this->BLACK_num = pre_board.BLACK_num;
	this->stage = pre_board.stage;
	this->pre_action = pre_board.pre_action;
}

void Board::init() {
	current_player = BLACK;

	WHITE_num = BLACK_num = 12;

	stage = 0;
	pre_action = -1;

	for (size_t i = 0; i < Board_len; i++) {
		board[0][i] = BLACK;
		board[1][i] = BLACK;
		board[2][i] = EMPTY;
		board[3][i] = EMPTY;
		board[4][i] = WHITE;
		board[5][i] = WHITE;
	}
}

bool Board::is_in_board(int x, int y) {
	return x >= 0 && x < Board_len && y >= 0 && y < Board_len;
}

bool Board::is_not_corner(int x, int y) {
	if (x == 0 || x == 5) {
		return y != 0 && y != 5;
	}
	return true;
}

bool Board::check(int x, int y, int real_x, int real_y, int current_player, int cnt) {
	if (cnt >= 5) return false;
	auto& [nx, ny, mov] = Transmit[{x, y}];
	if (mov == 'L') {
		for (int i = ny; i >= 0; --i) {
			if (board[nx][i] != EMPTY) {
				if (nx == real_x && i == real_y) continue;
				if (board[nx][i] == current_player) {
					return false;
				} 
				else {
					return true;
				}
			}
			if (i == 0) return check(nx, 0, real_x, real_y, current_player, cnt + 1);
		}
	}
	else if (mov == 'R') {
		for (int i = ny; i < Board_len; ++i) {
			if (board[nx][i] != EMPTY) {
				if (nx == real_x && i == real_y) continue;
				if (board[nx][i] == current_player) {
					return false;
				}
				else {
					return true;
				}
			}
			if (i == Board_len - 1) return check(nx, Board_len - 1, real_x, real_y, current_player, cnt + 1);
		}
	}
	else if (mov == 'U') {
		for (int i = nx; i >= 0; --i) {
			if (board[i][ny] != EMPTY) {
				if (i == real_x && ny == real_y) continue;
				if (board[i][ny] == current_player) {
					return false;
				}
				else {
					return true;
				}
			}
			if (i == 0) return check(0, ny, real_x, real_y, current_player, cnt + 1);
		}
	}
	else {
		for (int i = nx; i < Board_len; ++i) {
			if (board[i][ny] != EMPTY) {
				if (i == real_x && ny == real_y) continue;
				if (board[i][ny] == current_player) {
					return false;
				}
				else {
					return true;
				}
			}
			if (i == 0) return check(Board_len - 1, ny, real_x, real_y, current_player, cnt + 1);
		}
	}
	puts("function check is Wrong!");
	assert(true != true);
	return false;
}

bool Board::get_select(int x, int y, int current_player) {
	for (int i = 0; i < 8; ++i) {
		int nx = x + dx[i], ny = y + dy[i];
		if (is_in_board(nx, ny) && board[nx][ny] == EMPTY) {
			return true;
		}
	}
	if (x != 0 && x != 5) {
		for (int i = y - 1; i >= 0; --i) {
			if (board[x][i] != EMPTY) {
				break;
			}
			if (i == 0 && check(x, 0, x, 0, current_player, 0)) {
				return true;
			}
		}
		for (int i = y + 1; i < Board_len; ++i) {
			if (board[x][i] != EMPTY) {
				break;
			}
			if (i == Board_len - 1 && check(x, Board_len - 1, x, Board_len - 1, current_player, 0)) {
				return true;
			}
		}
	}
	if (y != 0 && y != 5) {
		for (int i = x - 1; i >= 0; --i) {
			if (board[i][y] != EMPTY) {
				break;
			}
			if (i == 0 && check(0, y, 0, y, current_player, 0)) {
				return true;
			}
		}
		for (int i = x + 1; i < Board_len; ++i) {
			if (board[i][y] != EMPTY) {
				break;
			}
			if (i == Board_len - 1 && check(Board_len - 1, y, Board_len - 1, y, current_player, 0)) {
				return true;
			}
		}
	}
	if (Transmit.count({ x, y }) && check(x, y, x, y, current_player, 0)) {
		return true;
	}
	return false;
}

int Board::get_eat(int x, int y, int real_x, int real_y, int current_player, int cnt) {
	if (cnt >= 5) return -1;
	auto& [nx, ny, mov] = Transmit[{x, y}];
	if (mov == 'L') {
		for (int i = ny; i >= 0; --i) {
			if (board[nx][i] != EMPTY) {
				if (nx == real_x && i == real_y) continue;
				if (board[nx][i] == current_player) {
					return -1;
				}
				else {
					return nx * Board_len + i;
				}
			}
			if (i == 0) return get_eat(nx, 0, real_x, real_y, current_player, cnt + 1);
		}
	}
	else if (mov == 'R') {
		for (int i = ny; i < Board_len; ++i) {
			if (board[nx][i] != EMPTY) {
				if (nx == real_x && i == real_y) continue;
				if (board[nx][i] == current_player) {
					return -1;
				}
				else {
					return nx * Board_len + i;
				}
			}
			if (i == Board_len - 1) return get_eat(nx, Board_len - 1, real_x, real_y, current_player, cnt + 1);
		}
	}
	else if (mov == 'U') {
		for (int i = nx; i >= 0; --i) {
			if (board[i][ny] != EMPTY) {
				if (i == real_x && ny == real_y) continue;
				if (board[i][ny] == current_player) {
					return -1;
				}
				else {
					return i * Board_len + ny;
				}
			}
			if (i == 0) return get_eat(0, ny, real_x, real_y, current_player, cnt + 1);
		}
	}
	else {
		for (int i = nx; i < Board_len; ++i) {
			if (board[i][ny] != EMPTY) {
				if (i == real_x && ny == real_y) continue;
				if (board[i][ny] == current_player) {
					return -1;
				}
				else {
					return i * Board_len + ny;
				}
			}
			if (i == 0) return get_eat(Board_len - 1, ny, real_x, real_y, current_player, cnt + 1);
		}
	}
	puts("function gwt_eat is Wrong!");
	assert(true != true);
	return -1;
}

std::vector<int> Board::get_choose(int x, int y, int current_player) {
	std::vector<int> res;
	for (int i = 0; i < 8; ++i) {
		int nx = x + dx[i], ny = y + dy[i];
		if (is_in_board(nx, ny) && board[nx][ny] == EMPTY) {
			res.emplace_back(nx * Board_len + ny);
		}
	}
	if (x != 0 && x != 5) {
		for (int i = y - 1; i >= 0; --i) {
			if (board[x][i] != EMPTY) {
				break;
			}
			if (i == 0) {
				int mid = get_eat(x, 0, x, 0, current_player, 0);
				if (mid != -1) {
					res.emplace_back(mid);
				}
			}
		}
		for (int i = y + 1; i < Board_len; ++i) {
			if (board[x][i] != EMPTY) {
				break;
			}
			if (i == Board_len - 1) {
				int mid = get_eat(x, Board_len - 1, x, Board_len - 1, current_player, 0);
				if (mid != -1) {
					res.emplace_back(mid);
				}
			}
		}
	}
	if (y != 0 && y != 5) {
		for (int i = x - 1; i >= 0; --i) {
			if (board[i][y] != EMPTY) {
				break;
			}
			if (i == 0) {
				int mid = get_eat(0, y, 0, y, current_player, 0);
				if (mid != -1) {
					res.emplace_back(mid);
				}
			}
		}
		for (int i = x + 1; i < Board_len; ++i) {
			if (board[i][y] != EMPTY) {
				break;
			}
			if (i == Board_len - 1) {
				int mid = get_eat(Board_len - 1, y, Board_len - 1, y, current_player, 0);
				if (mid != -1) {
					res.emplace_back(mid);
				}
			}
		}
	}
	if (Transmit.count({ x, y })) {
		int mid = get_eat(x, y, x, y, current_player, 0);
		if (mid != -1) {
			res.emplace_back(mid);
		}
	}
	return res;
}

std::vector<int> Board::getAvailable() {
	std::vector<int> available;
	if (stage == 0) {
		for (int i = 0; i < Board_len; ++i) {
			for (int j = 0; j < Board_len; ++j) {
				if (board[i][j] == current_player && get_select(i, j, current_player)) {
					available.emplace_back(i * Board_len + j);
				}
			}
		}
	}
	else {
		for (int i = 0; i < Board_len; ++i) {
			for (int j = 0; j < Board_len; ++j) {
				if (board[i][j] == current_player) {
					std::vector<int> mid = get_choose(i, j, current_player);
					available.insert(available.end(), mid.begin(), mid.end());
				}
			}
		}
	}
	std::sort(available.begin(), available.end());
	auto it = std::unique(available.begin(), available.end());
	available.erase(it, available.end());
	return available;
}