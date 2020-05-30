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



//マウスの座標からブロック番地をPoint(幅, 高さ)の形で返す　ブロックをクリックしていないならPoint(-1,-1)を返す
Point Blocks::convert(Point p) const {

}

//クリックしたブロック以外にランダムに爆弾を置いたうえで開く
//マウスの座標を引数にすること
//マウスがブロックの上になかったらfalseを返す
bool Blocks::first_open(const Point p) {

	Point address(convert(p));

	//マウスがブロックを押してなかったらfalseを返す
	if () return false
		//クリックしたブロック以外のランダムなブロックbombs_個に重複しないよう爆弾を入れればここまでのプログラムは何でもよい
	else {
		
		
		open(address);
		return true;
	}
}

//block_配列の番地を引数にする　マウスの座標を引数にしないこと
void Blocks::open(const Point& address) {

	//ブロックをクリックしていなかったらこの関数を終了
	if () {

	}

	//？ブロックか開いていないブロックを左クリックしたときの処理
	if () {
		//ブロックを開く
		//隣接するブロックに爆弾がなかったら隣接するブロックも開く（再帰）
	}

	//開いてるブロック（数字のあるブロック）をクリックしたときの処理
	else if () {
		
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
	if () return;

	//開いてないブロックか？ブロックならフラグを立てる
	if () {

	}
	//既にフラグが立ってるなら消す
	else if () {

	}
}

//開いていないブロックとフラグブロックを？ブロックに、？ブロックを開いていないブロックにする
//マウスの座標を引数にすること
void Blocks::make_question(Point p) {
	Point address(convert(p));
	//ブロックをクリックしていなかったらこの関数を終了
	if () return;

	//開いてないブロックなら？ブロックにする
	if () {
		
	}
	//フラグブロックなら？ブロックにしてflags_を1減らす
	else if () {
	
	}
	//既に？ブロックなら？ブロックを開いていないブロックにする
	else if () {
		
	}
}

//開いていないブロックを爆弾があるブロックなら爆弾ブロックに、爆弾がないブロックなら開いたブロックに
void Blocks::make_answer() {
	
}
