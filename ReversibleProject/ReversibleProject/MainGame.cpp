#include<iostream>
#include<string>
#include<vector>

using namespace std;

// 盤面の高さ(BOARD_HEIGHT)と横幅(BOARD_WIDTH)
#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8

// マス目の状態のラベル
// self : 自分　enemy : 相手　empty : 空き　can : 置ける
enum ObjLabel
{
	self = -1,
	empty,
	enemy,
	can,
};

// ターンの状況を保存している変数
int turn = -1;

// 盤面上にあるオブジェクトの数を保存しておく変数（初期値は４）
int objCount = 4;

class Board
{
private:
	// 自分(selfObj)と相手(enemyObj)と空き(emptyObj)と置ける(cabObj)のオブジェクト
	string selfObj = " ○ ";
	string enemyObj = " ● ";
	string emptyObj = " 　 ";
	string canObj = " × ";
public:
	// 盤面のコンストラクタ
	Board(int** boardObj)
	{
		// 全てのマス目の状態を空きにする
		for (int x = 0; x < BOARD_HEIGHT; x++)
		{
			for (int y = 0; y < BOARD_WIDTH; y++)
			{
				boardObj[x][y] = ObjLabel::empty;
			}
		}
	}

	// 盤面を表示する
	void ShowBoard(int** const boardObj)
	{
		// 盤面がメモリ確保されていなかった場合
		if (boardObj == nullptr)
		{
			cout << boardObj << "null" << endl;
		}
		else
		{
			// 盤面の表示
			for (int x = 0; x < BOARD_HEIGHT; x++)
			{
				cout << "+----+----+----+----+----+----+----+----+" << endl;
				for (int y = 0; y < BOARD_WIDTH; y++)
				{
					cout << "|";
					// マス目の二次元配列の値によって表示するコマを変える
					switch (boardObj[x][y])
					{
					case ObjLabel::self: // 相手のコマ
						cout << enemyObj;
						break;
					case ObjLabel::empty:  // 空きのコマ
						cout << emptyObj;
						break;
					case ObjLabel::enemy:  // 自分のコマ
						cout << selfObj;
						break;
					case ObjLabel::can:
						cout << canObj;
						break;
					default: // 例外
						break;
					}
				}
				cout << "|" << endl;
			}
			cout << "+----+----+----+----+----+----+----+----+" << endl;
		}
	}

	// 盤面の状態を表示するデバッグ用関数
	void DebugBoardValue(int** boardObj)
	{
		cout << endl;
		cout << "------------------------";
		cout << "Debug";
		cout << "------------------------" << endl;
		cout << " 　　0   １   ２   ３   ４   ５   ６   ７" << endl;
		for (int x = 0; x < BOARD_HEIGHT; x++)
		{
			cout << " 　+----+----+----+----+----+----+----+----+" << endl;
			cout <<" "<< x << " ";
			for (int y = 0; y < BOARD_WIDTH; y++)
			{
				cout << "|";
				cout <<"  "<< boardObj[x][y]<<" ";
			}
			cout << "|" << endl;
		}
		cout << " 　+----+----+----+----+----+----+----+----+" << endl;
		cout << "------------------------------------------------------" << endl;

	}
};

// オブジェクトの縦と横の位置のvectorを持つクラス
class StoneList
{
public:
	// 縦の位置を保存するvector
	vector<int> indexL;
	// 横の位置を保存するvector
	vector<int> indexR;
};

// 盤面を調べるクラス
class PlayPhase
{
private:

public:
	// 自分のオブジェクトの位置情報
	StoneList selfStone;
	// 相手のオブジェクトの位置情報
	StoneList enemyStone;

	// 盤面を調べるコンストラクタ
	PlayPhase(int** boardObj)
	{
		// 中央に自分のオブジェクトと相手のオブジェクトを設置する
		boardObj[3][3] = ObjLabel::self;
		boardObj[3][4] = ObjLabel::enemy;
		boardObj[4][3] = ObjLabel::enemy;
		boardObj[4][4] = ObjLabel::self;

		// オブジェクトの位置情報を保存する
		selfStone.indexL={ 3,4 };
		selfStone.indexR = { 3,4 };
		enemyStone.indexL = { 3,4 };
		enemyStone.indexR = { 4,3 };
	}

	// オブジェクトの周りに空いているマス目がないか調べる
	void FoundEmptyStone(int** boardObj,vector<int> vecL,vector<int> vecR)
	{
		// 盤面にある数だけ調べる
		for (int index = 0; index < objCount; index++)
		{
			// ※関数でまとめる
			// オブジェクトの左上から３列分
			for (int row = -1; row <= 1; row++)
			{
				// 調べようとしている行が範囲外かを調べる
				if (vecL[index] + row < 0 || vecL[index] + row >= BOARD_HEIGHT)
				{
					cout << "Error : 盤面の幅行　範囲外" << endl;

					// 処理を飛ばす
					continue;
				}

				// オブジェクトの左上から３行分
				for (int col = -1; col <= 1; col++)
				{
					// 調べようとしている列が範囲外かを調べる
					if (vecR[index] + col < 0 || vecR[index] + col >= BOARD_WIDTH)
					{
						cout << "Error : 盤面の高行　範囲外" << endl;

						// 処理を飛ばす
						continue;
					}
					// マス目が空きの状態である場合
					else if (boardObj[vecL[index] + row][vecR[index] + col] == ObjLabel::empty)
					{
						// ※関数でまとめる
						// --- 空いているマス目から相手のオブジェクトがある方向に自分のオブジェクトがあるかを調べる
						// マス目を置けるの状態にする
						//boardObj[vecL[index] + row][vecR[index] + col] = ObjLabel::can;
					}
				}
			}
		}
	}
};

// メインの処理
void main()
{
	// 一つ目の配列のメモリを確保
	int** square = new int* [BOARD_HEIGHT];

	// 二つ目の配列のメモリを確保
	for (int index = 0; index < BOARD_HEIGHT; index++)
	{
		square[index] = new int[BOARD_WIDTH];
	}

	// Boardクラスのインスタンスを生成
	Board board(square);
	
	// PlayPhaseクラスのインスタンスを生成
	PlayPhase playPhase(square);

	// 盤面を表示
	board.ShowBoard(square);

	// 空いているマスを探す
	//playPhase.FoundEmpty(square);

	// 盤面を表示
	//board.ShowBoard(square);

	// 確保したメモリを解放する
	for (int index = 0; index < BOARD_HEIGHT; index++) {
		delete[] square[index];
		square[index] = 0;	
	}
	delete[] square;
	square = 0;
}