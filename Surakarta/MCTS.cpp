#include "MCTS.h"

using namespace MCTS;

unsigned long long Node::cnt = 0;

Node::Node() : father(nullptr), game_num(0), win_num(0), depth(0) {
	bd = SingelThreadAlphaBetaPro::Board();
	id = cnt++;
}

Node::Node(Node* ft, SingelThreadAlphaBetaPro::Board Bd, int Depth) : father(ft), bd(Bd), game_num(0), win_num(0), depth(Depth) {
	id = cnt++;
}

Node::~Node() {
	for (auto& child : children) {
		delete child;
	}
}

bool Node::is_leaf(bool lock) {
	if (lock) Node_lock.lock_shared();
	bool is_empty = this->children.empty();
	if (lock) Node_lock.unlock_shared();
	return is_empty;
}

void Node::expend() {
	if (!this->children.empty()) return;
	std::vector<SingelThreadAlphaBetaPro::Board> Children_board = this->bd.get_children();
	for (auto& child : Children_board) {
		children.push_back(new Node(this, child, this->depth + 1));
	}
	children.shrink_to_fit();
}

void Node::back_ground(bool is_win) {
	spinLock.lock();
	//--vitrual_loss, 
	++game_num;
	if(is_win) ++win_num;
	spinLock.unlock();
	if (!father) return;
	father->back_ground(!is_win);
}

void Node::is_Draw() {
	spinLock.lock();
	++game_num;
	spinLock.unlock();
	if (!father) return;
	father->is_Draw();
}

double Node::uct() { 
	std::lock_guard<SpinLock> this_lk(spinLock);
	std::lock_guard<SpinLock> father_lk(father->spinLock);
	if (!game_num) return std::numeric_limits<double>::max();
	double ln_value = static_cast<double>(std::log(static_cast<double>(this->game_num))) / static_cast<double>(father->game_num);// +this->vitrual_loss);
	if (ln_value < 0) printf("666666\n");
	return C * std::sqrt(ln_value);// 1.0 * this->win_num / this->game_num + C * std::sqrt(ln_value);
}

Node* Node::get_best_node() {
	//std::unique_lock<std::shared_mutex> lk(Node_lock);
	if (is_leaf()) expend();
	if (children.empty()) {
		printf("error");
		exit(0);
	}
	Node* best_Node = nullptr;
	double max_score = -10000.0;//std::numeric_limits<double>::min();
	for (auto& child : children) {
		double score = child->uct();
		if (score > max_score) {
			max_score = score;
			best_Node = child;
		}
	}
	//if(best_Node) ++(best_Node->vitrual_loss);
	if (!best_Node) {
		printf("error");
		for (auto& child : children) { 
			double score = child->uct();
			printf("%lf\n", score);
		}
		exit(0);
	}
	//printf("%lf\n", max_score);
	return best_Node;
}

int Node::who_win() {
	if (this->bd.WHITE_num == 0) return BLACK_win;
	if (this->bd.BLACK_num == 0) return WHITE_win;
	if (this->depth == 200) {
		if (this->bd.BLACK_num > this->bd.WHITE_num) return BLACK_win;
		if (this->bd.WHITE_num > this->bd.BLACK_num) return WHITE_win;
		return DRAW;
	}
	return Not_Player_win;
}

void MCTS::init() {
	root = new Node();
}

void MCTS::simulate() {
	Node* now = root;
	while (true) {
		now = now->get_best_node();
		int win = now->who_win();
		//printf("%d\n", now->bd.current_player);
		if (win == BLACK_win) { 
			now->bd.current_player == SingelThreadAlphaBetaPro::BLACK ? now->back_ground(true) : now->back_ground(false);
			break;
		}
		else if (win == WHITE_win) {
			now->bd.current_player == SingelThreadAlphaBetaPro::WHITE ? now->back_ground(true) : now->back_ground(false);
			break;
		}
		else if (win == DRAW) {
			now->is_Draw();
			break;
		}
	}
}

void Node::print() {
	printf("%lld %lld\n", this->win_num.load(), this->game_num.load());
}

void MCTS::work(int x) {
	for (int i = 0; i < N; ++i) {
		printf("µÚ%d´ÎµÚ%dÂÖ\n", x + 1, i + 1);
		simulate();
		/*std::vector<std::thread> mutiThread;
		for (int i = 0; i < thread_size; ++i) {
			mutiThread.push_back(std::thread(simulate));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(Sleep_time));
		for (int i = 0; i < thread_size; ++i) {
			mutiThread[i].join();
		}*/

		root->print();
	}
	for (auto& i : root->children) {
		i->print();
	}
}