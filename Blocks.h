#pragma once
/*
#ifndef ___Blocks.h
#define ___Blocks.h
インクルードガードはpragma onceがやってくれる
*/
#include<Siv3D.hpp>
#include"Constant.h"

struct block {
	int state = 1;	//-3:間違ったところにフラグを立てていた　-2:爆弾　-1:爆発した　0:開いている　1:開いていない　2:フラグ　3:？
	bool isbomb = false;	//爆弾があるブロックならtrue
	int neighbor_bombs = 0;	//隣接する爆弾の数
};

class Blocks {
	const int width_;	//ブロックの横の数
	const int height_;	//ブロックの縦の数
	int life_;			//残機
	int bombs_;			//爆弾の数
	int flags_;			//フラグの数
	int unopened_blocks_;	//開いてないブロックの数
	const int block_size_;	//ブロックの大きさ
	const int shadow_size_;	//ブロックの影の大きさ
	const int margin_w;		//横の余白
	const int margin_h;		//縦の余白
	//ブロックの配列block_.at(y).at(x)で上からy+1番目、左からx+1番目のブロックにアクセス
	std::vector<std::vector<block>> block_;	

	//テクスチャとフォントを宣言
	const Texture bomb_texture;
	const Texture flag_texture;
	const Texture explosion_texture;
	const Texture question_texture;
	const Texture cross_texture;
	const Font font;
public:
	//コンストラクタ
	Blocks(int w, int h, int b, int hp);
	//マウスの座標をブロック配列の番地に変換
	Point convert(Point p) const;
	//clickしたブロック以外のブロックbombs_個に爆弾を入れて、clickされたマスを開ける
	bool first_open(const Point p);
	//引数が示すブロックの番地を開ける
	void open(const Point& address);
	//全てのブロックを画面に表示
	void print_map() const;
	//引数が示すブロックの番地にフラグを立てる。既にフラグが立っているならフラグを消す
	void make_flag(const Point p);
	//引数が示すブロックの番地を？ブロックにする。既に？ブロックなら？を消す
	void make_question(const Point p);
	//開ていないブロックをすべて開ける。
	void make_answer();

	//ここからゲッタ関数を記述してください
	int life() const { return life_; };
	int bombs() const { return bombs_; };
	int flags() const { return flags_; };
	int unopened_blocks() const { return unopened_blocks_; };
	/*
	いらなかった
	int margin() const { return margin_h; };
	int block_size() const { return block_size_; };
	*/
};

//#endif