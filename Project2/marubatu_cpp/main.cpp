#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <array>
#include <memory>
#include <map>

#include <conio.h>

using namespace std;

// ��`------------------------------
namespace BOARD_DATA
{
	constexpr int BOARD_X_MAX{ 3 };
	constexpr int BOARD_Y_MAX{ 3 };
	constexpr int ERROR{ -9999 };
}

// �Ղɒu�����΂̎��
namespace STONE
{
	constexpr unsigned int NONE{ 0 };
	constexpr unsigned int CIRCLE{ 1 };
	constexpr unsigned int PUNISHMENT{ 2 };
}

// ----------------------------------

// �^--------------------------------
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

//�N���X�F�ʒu-------------
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
	// �{�[�h�̏�����
	memset(&m_board, 0x00, sizeof(m_board));

	//�T�������}�b�v�쐬
	m_vec_lan.reset(new TYPE::VEC_LAN<CPOS>);
	auto p = m_vec_lan.get();
	p->insert(pair<int, CPOS>{ 0, CPOS{ 0, 1 } }); // �E
	p->insert(pair<int, CPOS>{ 1, CPOS{ 1, 0 } }); // ��
	p->insert(pair<int, CPOS>{ 2, CPOS{ 1, 1 } }); // �E��
	p->insert(pair<int, CPOS>{ 3, CPOS{ -1, 1 } });// ����
}

void Board::SetStone(int x, int y, unsigned int stone_kind)
{
	// �͈͊O�ɒu�����Ƃ��Ă��Ȃ���
	if (CheckIfAreaOutside(x, y) == false)
	{
		m_board[y][x] = stone_kind;
	}
}

// �}�b�v�̂���ʒu�ɉ����u����Ă��邩
int Board::GetMapStone(int x, int y)
{
	// �͈͊O�����悤�Ƃ��Ă��Ȃ���
	if (CheckIfAreaOutside(x, y) == false)
	{
		return m_board[y][x];
	}

	return BOARD_DATA::ERROR;
}

// �S�̂𒲂ׂāA����������������ł��邩�ǂ����𒲂ׂ�
// �߂�l  bool : true : ���ɕ���ł��� false: ����ł��Ȃ�
bool Board::LineCheck()
{
	auto lan = m_vec_lan.get();
	
	// �T������ʒu��������for���Ō��߂�
	for (int x = 0; x < BOARD_DATA::BOARD_X_MAX; x++)
	{
		CPOS pos{ x, 0 };
		int put_stone_kind;
		// �u����Ă���΂𒲂ׂ�
		put_stone_kind = m_board[pos.y][pos.x];

		if (put_stone_kind == STONE::NONE)
			continue;

		// �T�����������������for���Ō��߂�
		for (auto p = lan->cbegin(); p != lan->cend(); p++)
		{
			CVEC vec{ p->second };
			int count = 1;

			// �T����[�����Ă���
			while (1)
			{
				int new_put_stone_kind;
				new_put_stone_kind = GetMapStone(pos.x + vec.x * count, pos.y + vec.y * count);

				if (put_stone_kind != new_put_stone_kind)
					break;

				// 1�񂻂���Ă���
				if (count == BOARD_DATA::BOARD_X_MAX - 1)
					return true;

				count++;
			}
		}
	}

	// x��1�Ȃ̂͏��y�̎���x��0�𒲂ׂĂ��܂��Ă��邩��
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

				// 1�񂻂���Ă���
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
					cout << "�@";
					break;
				case STONE::CIRCLE:
					cout << "��";
					break;
				case STONE::PUNISHMENT:
					cout << "�~";
					break;
				default:
					break;
			}
		}
		cout << "\n";
	}

}

// �{�[�h�͈͓̔����ǂ����𒲂ׂ�
// �߂�l bool  :  true�Ȃ�͈͊O false�Ȃ�͈͓�
bool Board::CheckIfAreaOutside(int x, int y)
{
	if (x >= BOARD_DATA::BOARD_X_MAX) return true;
	if (y >= BOARD_DATA::BOARD_Y_MAX) return true;
	if (x < 0) return true;
	if (y < 0) return true;
	return false;
}

// �΂�u����ꏊ�����邩���ׂ�
// �߂�l bool   : true : �u����ꏊ������ false : �Ȃ�
bool Board::CheckPutPlace()
{
	for (int y = 0; y < BOARD_DATA::BOARD_Y_MAX; y++)
	{
		for (int x = 0; x < BOARD_DATA::BOARD_X_MAX; x++)
		{
			// �u����ꏊ���������ꍇ
			if (m_board[y][x] == STONE::NONE)
				return true;
		}
	}

	return false;
}

// �Q�[�����Ǘ�����N���X
class Game
{
public:
	Game(Board* board);
	void Input(unsigned int stone_kind);
	void GameLoop();
private:
	Board* m_board;
	unsigned int m_turn_stone; // ���ݒu����΂̎��
	unsigned int m_in_x;
	unsigned int m_in_y;
};

Game::Game(Board* board)
{
	m_board = board;
}

// ���͏���
void Game::Input(unsigned int stone_kind)
{
	while (1)
	{
		if (stone_kind == STONE::CIRCLE)
			cout << "���̔Ԃł�\n";
		else if (stone_kind == STONE::PUNISHMENT)
			cout << "�~�̔Ԃł�\n";

		cout << "�Z�~��u���ꏊ����͂��Ă�������(0�`2)\n";
		
		cout << "x : ";
		cin >> m_in_x;
		cout << "y : ";
		cin >> m_in_y;

		if(m_board->GetMapStone(m_in_x, m_in_y) == STONE::NONE)
			break;
	}

	m_board->SetStone(m_in_x, m_in_y, stone_kind);
}

// �Q�[�����[�v����
void Game::GameLoop()
{
	while (1)
	{
		bool end = false;

		m_board->Draw();

		// �u����ꏊ�����邩�ǂ������ׂ�
		if (m_board->CheckPutPlace() == false)
		{
			cout << "���������ł�";
			_getch();
			break;
		}

		Input(STONE::CIRCLE);
		// ���o������I��
		end = m_board->LineCheck();
		
		if (end == true)
		{
			m_board->Draw();
			cout << "���̏����ł�";
			_getch();
			break;
		}

		m_board->Draw();

		// �u����ꏊ�����邩�ǂ������ׂ�
		if (m_board->CheckPutPlace() == false)
		{
			cout << "���������ł�";
			_getch();
			break;
		}

		Input(STONE::PUNISHMENT);
		// ���o������I��
		end = m_board->LineCheck();

		if (end == true)
		{
			m_board->Draw();
			cout << "�~�̏����ł�";
			_getch();
			break;
		}
	}
}

int main()
{
	unique_ptr<Board> board(new Board);
	unique_ptr<Game> game(new Game(board.get()));
	
	// �Q�[�����[�v
	game->GameLoop();

	board.release();
	game.release();

	return 0;
}

