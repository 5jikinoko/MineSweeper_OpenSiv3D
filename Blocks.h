#pragma once
/*
#ifndef ___Blocks.h
#define ___Blocks.h
インクルードガードはpragma onceがやってくれる
*/
#include<Siv3D.hpp>
#include"Constant.h"

struct block {
	int state = 1;	//-2:爆弾　-1:爆発した　0:開いている　1:開いていない　2:フラグ　3:？
	bool isbomb = false;	//爆弾があるブロックならtrue
	int neighbor_bombs = 0;	//隣接する爆弾の数
};

class Blocks {
	const int width_;
	const int height_;
	int life_;
	int bombs_;
	int flags_;
	int unopened_blocks_;
	const int block_size_;
	const int shadow_size_;
	const int margin_h;
	const int margin_w;
	std::vector<std::vector<block>> block_;
	//テクスチャとフォントを宣言
	const Texture bomb_texture;
	const Texture flag_texture;
	const Texture explosion_texture;
	const Texture question_texture;
	const Font font;
public:
	Blocks(int w, int h, int b, int hp);
	Point convert(Point p) const;
	bool first_open(const Point p);
	void open(const Point& address);
	void print_map() const;
	void make_flag(const Point p);
	void make_question(const Point p);
	void make_answer();
	//ここからゲッタ関数
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