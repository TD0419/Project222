#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <array>
#include <memory>
#include <map>

#include <conio.h>

using namespace std;

// 定義------------------------------
namespace BOARD_DATA
{
	constexpr int BOARD_X_MAX{ 3 };
	constexpr int BOARD_Y_MAX{ 3 };
	constexpr int ERROR{ -9999 };
}

// 盤に置かれる石の種類
namespace STONE
{
	constexpr unsigned int NONE{ 0 };
	constexpr unsigned int CIRCLE{ 1 };
	constexpr unsigned int PUNISHMENT{ 2 };
}

// ----------------------------------

// 型--------------------------------
namespace TYPE
{
	template <typename T>
	using LINE = array < T, BOARD_DATA::BOARD_X_MAX>;
	using MAP = array < LINE<int>, BOARD_DATA::BOARD_Y_MAX>;

	template <typename T>
	using VEC_LAN = map<int, T>;

	template <typename T>
	using UN_VEC_LAN = unique_ptr<VEC_LAN<T>>;
}
// ----------------------------------

//クラス：位置-------------
class  CPOS
{
public:
	int x;
	int y;
};
using CVEC = CPOS;

class Board
{
public:
	Board();
	void SetStone(int x, int y, unsigned int stone_kind);
	int GetMapStone(int x, int y);
	bool LineCheck();
	bool CheckIfAreaOutside(int x, int y);
	void Draw();
	bool CheckPutPlace();

private:
	TYPE::MAP	m_board;
	TYPE::UN_VEC_LAN<CPOS>	m_vec_lan;
};

Board::Board()
{
	// ボードの初期化
	memset(&m_board, 0x00, sizeof(m_board));

	//探索方向マップ作成
	m_vec_lan.reset(new TYPE::VEC_LAN<CPOS>);
	auto p = m_vec_lan.get();
	p->insert(pair<int, CPOS>{ 0, CPOS{ 0, 1 } }); // 右
	p->insert(pair<int, CPOS>{ 1, CPOS{ 1, 0 } }); // 下
	p->insert(pair<int, CPOS>{ 2, CPOS{ 1, 1 } }); // 右下
	p->insert(pair<int, CPOS>{ 3, CPOS{ -1, 1 } });// 左下
}

void Board::SetStone(int x, int y, unsigned int stone_kind)
{
	// 範囲外に置こうとしていないか
	if (CheckIfAreaOutside(x, y) == false)
	{
		m_board[y][x] = stone_kind;
	}
}

// マップのある位置に何が置かれているか
int Board::GetMapStone(int x, int y)
{
	// 範囲外を見ようとしていないか
	if (CheckIfAreaOutside(x, y) == false)
	{
		return m_board[y][x];
	}

	return BOARD_DATA::ERROR;
}

// 全体を調べて、同じ文字が一列並んでいるかどうかを調べる
// 戻り値  bool : true : 一列に並んでいる false: 並んでいない
bool Board::LineCheck()
{
	auto lan = m_vec_lan.get();
	
	// 探索する位置をここのfor文で決める
	for (int x = 0; x < BOARD_DATA::BOARD_X_MAX; x++)
	{
		CPOS pos{ x, 0 };
		int put_stone_kind;
		// 置かれている石を調べる
		put_stone_kind = m_board[pos.y][pos.x];

		if (put_stone_kind == STONE::NONE)
			continue;

		// 探索する向きをここのfor文で決める
		for (auto p = lan->cbegin(); p != lan->cend(); p++)
		{
			CVEC vec{ p->second };
			int count = 1;

			// 探索を深くしていく
			while (1)
			{
				int new_put_stone_kind;
				new_put_stone_kind = GetMapStone(pos.x + vec.x * count, pos.y + vec.y * count);

				if (put_stone_kind != new_put_stone_kind)
					break;

				// 1列そろっていた
				if (count == BOARD_DATA::BOARD_X_MAX - 1)
					return true;

				count++;
			}
		}
	}

	// xが1なのは上のyの時にxが0を調べてしまっているから
	for (int y = 1; y < BOARD_DATA::BOARD_Y_MAX; y++)
	{
		CPOS pos{ 0, y };
		int put_stone_kind;
		put_stone_kind = m_board[pos.y][pos.x];

		if (put_stone_kind == STONE::NONE)
			continue;
		
		for (auto p = lan->cbegin(); p != lan->cend(); p++)
		{
			CVEC vec{ p->second };
			int count = 1;

			while (1)
			{
				int new_put_stone_kind;
				
				new_put_stone_kind = GetMapStone(pos.x + vec.x * count, pos.y + vec.y * count);

				if (put_stone_kind != new_put_stone_kind)
					break;

				// 1列そろっていた
				if (count == BOARD_DATA::BOARD_X_MAX - 1)
					return true;

				count++;
			}
		}
	}

	return false;
}

void Board::Draw()
{
	system("cls");
	for (int y = 0; y < BOARD_DATA::BOARD_Y_MAX; y++)
	{
		for (int x = 0; x < BOARD_DATA::BOARD_X_MAX; x++)
		{
			switch (m_board[y][x])
			{
				case STONE::NONE:
					cout << "　";
					break;
				case STONE::CIRCLE:
					cout << "○";
					break;
				case STONE::PUNISHMENT:
					cout << "×";
					break;
				default:
					break;
			}
		}
		cout << "\n";
	}

}

// ボードの範囲内かどうかを調べる
// 戻り値 bool  :  trueなら範囲外 falseなら範囲内
bool Board::CheckIfAreaOutside(int x, int y)
{
	if (x >= BOARD_DATA::BOARD_X_MAX) return true;
	if (y >= BOARD_DATA::BOARD_Y_MAX) return true;
	if (x < 0) return true;
	if (y < 0) return true;
	return false;
}

// 石を置ける場所があるか調べる
// 戻り値 bool   : true : 置ける場所がある false : ない
bool Board::CheckPutPlace()
{
	for (int y = 0; y < BOARD_DATA::BOARD_Y_MAX; y++)
	{
		for (int x = 0; x < BOARD_DATA::BOARD_X_MAX; x++)
		{
			// 置ける場所があった場合
			if (m_board[y][x] == STONE::NONE)
				return true;
		}
	}

	return false;
}

// ゲームを管理するクラス
class Game
{
public:
	Game(Board* board);
	void Input(unsigned int stone_kind);
	void GameLoop();
private:
	Board* m_board;
	unsigned int m_turn_stone; // 現在置ける石の種類
	unsigned int m_in_x;
	unsigned int m_in_y;
};

Game::Game(Board* board)
{
	m_board = board;
}

// 入力処理
void Game::Input(unsigned int stone_kind)
{
	while (1)
	{
		if (stone_kind == STONE::CIRCLE)
			cout << "○の番です\n";
		else if (stone_kind == STONE::PUNISHMENT)
			cout << "×の番です\n";

		cout << "〇×を置く場所を入力してください(0～2)\n";
		
		cout << "x : ";
		cin >> m_in_x;
		cout << "y : ";
		cin >> m_in_y;

		if(m_board->GetMapStone(m_in_x, m_in_y) == STONE::NONE)
			break;
	}

	m_board->SetStone(m_in_x, m_in_y, stone_kind);
}

// ゲームループ処理
void Game::GameLoop()
{
	while (1)
	{
		bool end = false;

		m_board->Draw();

		// 置ける場所があるかどうか調べる
		if (m_board->CheckPutPlace() == false)
		{
			cout << "引き分けです";
			_getch();
			break;
		}

		Input(STONE::CIRCLE);
		// 一列出来たら終了
		end = m_board->LineCheck();
		
		if (end == true)
		{
			m_board->Draw();
			cout << "○の勝ちです";
			_getch();
			break;
		}

		m_board->Draw();

		// 置ける場所があるかどうか調べる
		if (m_board->CheckPutPlace() == false)
		{
			cout << "引き分けです";
			_getch();
			break;
		}

		Input(STONE::PUNISHMENT);
		// 一列出来たら終了
		end = m_board->LineCheck();

		if (end == true)
		{
			m_board->Draw();
			cout << "×の勝ちです";
			_getch();
			break;
		}
	}
}

int main()
{
	unique_ptr<Board> board(new Board);
	unique_ptr<Game> game(new Game(board.get()));
	
	// ゲームループ
	game->GameLoop();

	board.release();
	game.release();

	return 0;
}

