#include "Blocks.h"

//コンストラクタ
Blocks::Blocks(int w, int h, int b, int hp) :
	width_(w),
	height_(h),
	bombs_(b),
	life_(hp),
	unopened_blocks_(h* w),
	block_size_(w* game_field.y > h* game_field.x ? (game_field.x - minimum_margin.x * 2) / w : (game_field.y - minimum_margin.y * 2) / h),
	shadow_size_(block_size_ / 10),
	margin_h((game_field.y - h * block_size_) / 2),
	margin_w((game_field.x - w * block_size_) / 2),
	flags_(0),
	block_(height_, std::vector<block>(width_)),
	//ここからはテクスチャとフォントの生成
	bomb_texture(Emoji(U"💣")),
	flag_texture(Emoji(U"🚩")),
	explosion_texture(Emoji(U"💥")),
	question_texture(Emoji(U"❓")),
	font(block_size_)
{

}

//デストラクタ
//newで確保したヒープ領域は使わなくなったら解放
//Blocks::~Blocks() { delete[] block_; }

//マウスの座標からブロックの配列を指定する　ブロックをクリックしていないならPoint(-1,-1)を返す
Point Blocks::convert(Point p) const {
	p.x -= margin_w;
	p.y -= margin_h;
	if (0 < p.x && p.x < width_ * block_size_ && 0 < p.y && p.y < height_ * block_size_) {
		return Point(p.x / block_size_, p.y / block_size_);
	}
	else {
		return Point(-1, -1);
	}
}

//クリックしたブロック以外にランダムに爆弾を置いたうえで開く
//マウスの座標を引数にすること
//マウスがブロックの上になかったらfalseを返す
bool Blocks::first_open(const Point p) {

	Point address(convert(p));

	//マウスがブロックを押してなかったらfalseを返す
	if (address.x < 0 || width_ <= address.x || address.y < 0 || height_ <= address.y) {
		return false;
	}
	else {
		//block_[target]はクリックされたブロック
		int clicked = address.x + width_ * address.y;

		//配列にtargetを除く0からブロックの数-1の数を順番に入れる
		std::vector<int> order(width_ * height_ - 1);
		for (int i = 0, num = 0; i < width_ * height_ - 1; ++i, ++num) {
			if (i == clicked) ++num;
			order.at(i) = num;
		}

		//デバイスの状態から乱数rdを生成
		std::random_device rd;
		//rdを初期シードにして疑似乱数生成期クラスを初期化
		std::mt19937 mt(rd());
		//配列をFisher-Yatesシャッフル
		unsigned int j = 0;
		int temp = 0;
		for (int i = width_ * height_ - 2; i > 0; --i) {
			j = mt() % (i + 1);
			temp = order.at(j);
			order.at(j) = order.at(i);
			order.at(i) = temp;
		}

		for (int i = 0; i < bombs_; ++i) {
			//order[0]からorder[bombs_(爆弾の数)]までの数が示すブロックに爆弾を入れる
			int w = order.at(i) % width_;
			int h = order.at(i) / width_;
			block_.at(h).at(w).isbomb = true;
			//隣接するブロックのneighbor_bombsを+1
			for (int y = h - 1; y <= h + 1; ++y) {
				for (int x = w - 1; x <= w + 1; ++x) {
					if (0 <= x && x < width_ && 0 <= y && y < height_) {
						block_.at(y).at(x).neighbor_bombs += 1;
					}
				}
			}
		}
		//クリックしたブロック以外のランダムなブロックbombs_個に重複しないよう爆弾を入れればここまでのプログラムは何でもよい
		//数字をシャッフルして先頭のbombs_個を取り出すアルゴリズムを使いました
		//ちゃんとした乱数を生成するためrandom_deviceを使用

		open(address);
		return true;
	}
}

//block_配列の番地を引数にする　マウスの座標を引数にしないこと
void Blocks::open(const Point& address) {

	//ブロックをクリックしていなかったらこの関数を終了
	if (address.x < 0 || width_ <= address.x || address.y < 0 || height_ <= address.y) return;

	//？ブロックか開いていないブロックを左クリックしたときの処理
	if (block_.at(address.y).at(address.x).state == 1 || block_.at(address.y).at(address.x).state == 3) {
		//地雷を踏んだら
		if (block_.at(address.y).at(address.x).isbomb) {
			block_.at(address.y).at(address.x).state = -1;
			--life_;
			--bombs_;
			--unopened_blocks_;
		}
		//地雷を踏まなかったら
		else {
			block_.at(address.y).at(address.x).state = 0;
			--unopened_blocks_;

			int neighbor_EaF = 0;
			//隣接する爆発済みの爆弾とフラグを数えてneighbor_EaFに代入
			for (int y = address.y - 1; y <= address.y + 1; ++y) {
				for (int x = address.x - 1; x <= address.x + 1; ++x) {
					if (0 <= x && x < width_ && 0 <= y && y < height_ && (block_.at(y).at(x).state == 2 || block_.at(y).at(x).state == -1))
						++neighbor_EaF;
				}
			}
			//隣接するブロックに爆弾がなかったら隣接するブロックも開く
			if (block_.at(address.y).at(address.x).neighbor_bombs == neighbor_EaF) {
				for (int y = address.y - 1; y <= address.y + 1; ++y) {
					for (int x = address.x - 1; x <= address.x + 1; ++x) {
						if (0 <= x && x < width_ && 0 <= y && y < height_)
							open(Point(x, y));
					}
				}
			}
		}
	}

	//開いてるブロック（数字のあるブロック）をクリックしたときの処理
	else if (block_.at(address.y).at(address.x).state == 0) {
		int neighbor_FAE = 0;
		//隣接するフラグと爆発済みの爆弾を数えてneighbor_FAEに代入
		for (int y = address.y - 1; y <= address.y + 1; ++y) {
			for (int x = address.x - 1; x <= address.x + 1; ++x) {
				if (0 <= x && x < width_ && 0 <= y && y < height_ && (block_.at(y).at(x).state == 2 || block_.at(y).at(x).state == -1))
					++neighbor_FAE;
			}
		}
		//(隣接するフラグの数+爆発済みの爆弾)と隣接する爆弾の数が同じなら　隣接するフラグが立っておらず開いていないマスを開ける
		if (block_.at(address.y).at(address.x).neighbor_bombs == neighbor_FAE) {
			for (int y = address.y - 1; y <= address.y + 1; ++y) {
				for (int x = address.x - 1; x <= address.x + 1; ++x) {
					if (0 <= x && x < width_ && 0 <= y && y < height_ && (block_.at(y).at(x).state == 1 || block_.at(y).at(x).state == 3))
						open(Point(x, y));
				}
			}
		}
	}
}

void Blocks::print_map() const {

	//ブロックを描画
	for (int i = 0; i < height_; ++i) {
		for (int j = 0; j < width_; j++) {
			switch (block_.at(i).at(j).state) {
			case -2:Rect(margin_w + block_size_ * j, margin_h + block_size_ * i, block_size_).draw(HSV(0.0, 0.0, 0.8)).drawFrame(1, 1, HSV(0.0, 0.0, 0.6));
				bomb_texture.resized(block_size_).draw(margin_w + block_size_ * j, margin_h + block_size_ * i); break;
			case -1:Rect(margin_w + block_size_ * j, margin_h + block_size_ * i, block_size_).draw(HSV(0.0, 0.0, 0.8)).drawFrame(1, 1, HSV(0.0, 0.0, 0.6));
				explosion_texture.resized(block_size_).draw(margin_w + block_size_ * j, margin_h + block_size_ * i); break;
			case 0: Rect(margin_w + block_size_ * j, margin_h + block_size_ * i, block_size_).draw(HSV(0.0, 0.0, 0.8)).drawFrame(1, 1, HSV(0.0, 0.0, 0.6));
				//フォントとrectに同じ座標を渡して描画しても何故かズレるのでshadow_size_を流用して凌ぐ　助けて
				font(U"{}"_fmt(block_.at(i).at(j).neighbor_bombs)).draw(margin_w + block_size_ * j + shadow_size_ * 2, margin_h + block_size_ * i - shadow_size_ * 2, HSV(0.0, 0.0, 0.0)); break;
			case 1: Triangle(margin_w + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ * j, margin_h + block_size_ + block_size_ * i).draw(HSV(0.0, 0.0, 0.9));
				Triangle(margin_w + block_size_ + block_size_ * j, margin_h + block_size_ + block_size_ * i, margin_w + block_size_ + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ * j, margin_h + block_size_ + block_size_ * i).draw(HSV(0.0, 0.0, 0.7));
				Rect(margin_w + shadow_size_ + block_size_ * j, margin_h + shadow_size_ + block_size_ * i, block_size_ - shadow_size_ * 2).draw(HSV(0.0, 0.0, 0.8)); break;
			case 2: Triangle(margin_w + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ * j, margin_h + block_size_ + block_size_ * i).draw(HSV(0.0, 0.0, 0.9));
				Triangle(margin_w + block_size_ + block_size_ * j, margin_h + block_size_ + block_size_ * i, margin_w + block_size_ + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ * j, margin_h + block_size_ + block_size_ * i).draw(HSV(0.0, 0.0, 0.7));
				Rect(margin_w + shadow_size_ + block_size_ * j, margin_h + shadow_size_ + block_size_ * i, block_size_ - shadow_size_ * 2).draw(HSV(0.0, 0.0, 0.8));
				flag_texture.resized(block_size_ - shadow_size_ * 2).draw(margin_w + shadow_size_ + block_size_ * j, margin_h + shadow_size_ + block_size_ * i); break;
			case 3: Triangle(margin_w + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ * j, margin_h + block_size_ + block_size_ * i).draw(HSV(0.0, 0.0, 0.9));
				Triangle(margin_w + block_size_ + block_size_ * j, margin_h + block_size_ + block_size_ * i, margin_w + block_size_ + block_size_ * j, margin_h + block_size_ * i, margin_w + block_size_ * j, margin_h + block_size_ + block_size_ * i).draw(HSV(0.0, 0.0, 0.7));
				Rect(margin_w + shadow_size_ + block_size_ * j, margin_h + shadow_size_ + block_size_ * i, block_size_ - shadow_size_ * 2).draw(HSV(0.0, 0.0, 0.8));
				question_texture.resized(block_size_ - shadow_size_ * 2).draw(margin_w + shadow_size_ + block_size_ * j, margin_h + shadow_size_ + block_size_ * i); break;
			}
		}
	}
	//ブロック全体を囲む縁を描画
	Rect(margin_w, margin_h, width_ * block_size_, height_ * block_size_).drawFrame(0, 1, HSV(0.0, 0.0, 0.6));
}

//開いていないブロックと？ブロックをフラグブロックに、フラグブロックを開いていないブロックにする
//マウスの座標を引数にすること
void Blocks::make_flag(Point p) {
	Point address(convert(p));
	//ブロックをクリックしていなかったらこの関数を終了
	if (address.x < 0 || width_ <= address.x || address.y < 0 || height_ <= address.y) return;

	//開いてないブロックか？ブロックならフラグを立てる
	if (block_.at(address.y).at(address.x).state == 1 || block_.at(address.y).at(address.x).state == 3) {
		block_.at(address.y).at(address.x).state = 2;
		++flags_;
	}
	//既にフラグが立ってるなら消す
	else if (block_.at(address.y).at(address.x).state == 2) {
		block_.at(address.y).at(address.x).state = 1;
		--flags_;
	}
}

//開いていないブロックとフラグブロックを？ブロックに、？ブロックを開いていないブロックにする
//マウスの座標を引数にすること
void Blocks::make_question(Point p) {
	Point address(convert(p));
	//ブロックをクリックしていなかったらこの関数を終了
	if (address.x < 0 || width_ <= address.x || address.y < 0 || height_ <= address.y) return;

	//開いてないブロックなら？ブロックにする
	if (block_.at(address.y).at(address.x).state == 1) {
		block_.at(address.y).at(address.x).state = 3;
	}
	//フラグブロックなら？ブロックにしてflags_を1減らす
	else if (block_.at(address.y).at(address.x).state == 2) {
		block_.at(address.y).at(address.x).state = 3;
		--flags_;
	}
	//既に？ブロックなら？ブロックを開いていないブロックにする
	else if (block_.at(address.y).at(address.x).state == 3) {
		block_.at(address.y).at(address.x).state = 1;
	}
}

//開いていないブロックを爆弾があるブロックなら爆弾ブロックに、爆弾がないブロックなら開いたブロックに
void Blocks::make_answer() {
	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			if (block_.at(y).at(x).state == 1) {
				if (block_.at(y).at(x).isbomb)
					block_.at(y).at(x).state = -2;
				else
					block_.at(y).at(x).state = 0;
			}
		}
	}
}
