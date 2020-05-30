#include"Blocks.h"
#include"Constant.h"
#include<random>
#include<Siv3D.hpp> // OpenSiv3D v0.4.2


//画面右のメニューを表示
bool print_menu(const Blocks& b, int t, bool restart=false) {
	/*	フォントを用意
	・毎フレーム絵文字を生成しない
	・関数のスコープ内に変数定義する
	二つの条件を満たそうと関数外でテクスチャーを定義を試したができなかった（？）
	関数内でstaticをつけ定義した
	もっといい方法がありそう
	
	*/
	static Font font(window_size.x/20);
	font(U"H P").draw(game_field.x + 30, window_size.y / 10);
	font(U":{}"_fmt(b.life())).draw(game_field.x + (window_size.x - game_field.x) * 2 / 3, window_size.y / 10);
	font(U"爆 弾").draw(game_field.x + 30, window_size.y / 5);
	font(U":{}"_fmt(b.bombs())).draw(game_field.x + (window_size.x - game_field.x) * 2 / 3, window_size.y / 5);
	font(U"フラグ").draw(game_field.x + 30, window_size.y * 3 / 10);
	font(U":{}"_fmt(b.flags())).draw(game_field.x + (window_size.x - game_field.x) * 2 / 3, window_size.y * 3 / 10);
	font(U"Time").drawAt(menu_crnter.x, window_size.y * 3 / 5);
	//(t % 1000)でミリ秒を取得できるが、取得できる桁が変動する。表示を安定させるため下記のような表示法にしました　C言語の%3dみたいにフィード幅指定できる方法あるのかな？
								//分			2桁目秒			1桁目秒			3桁目ミリ秒		2桁目秒
	font(U"{}:{}{}:{}{}"_fmt(t / 60000, (t % 60000) / 10000, (t / 1000) % 10, (t / 100) % 10, (t / 10) % 10)).drawAt(menu_crnter.x, window_size.y * 7 / 10);

	if (restart) {
		if (SimpleGUI::ButtonAt(U"リスタート", Vec2(menu_crnter.x, window_size.y * 8.5 / 10) )) {
			return true;
		}
	}
	return false;
}



void Main() {
	//ウィンドウの大きさを設定
	Window::Resize(window_size);
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.0, 1.0, 0.0));
	//フォントを用意
	Font font(window_size.x/12);
	
	bool complete = false;	//ゲームをクリアしたならtrue
	//タイマーを生成
	Stopwatch timer(false);
	
	start:	//goto文のラベル
	
	Blocks blocks(25, 20, 50, 3);

	int t = timer.ms();
	while (System::Update()) {
		if (MouseL.down()) {
			if (blocks.first_open(Cursor::Pos()))
				break;
		}
		blocks.print_map();
		print_menu(blocks, t);
	}


	timer.restart();	//タイマースタート

	while (System::Update()) {
		if (MouseL.down()) {
			blocks.open(blocks.convert(Cursor::Pos()));
		}
		if (MouseR.down()) {
			blocks.make_flag(Cursor::Pos());
		}
		if (MouseM.down()) {
			blocks.make_question(Cursor::Pos());
		}

		if (blocks.life() <= 0) {
			complete = false;
			break;
		}
		if (blocks.bombs() == blocks.unopened_blocks()) {
			complete = true;
			break;
		}
		blocks.print_map();
		print_menu(blocks, timer.ms());
	}
	timer.pause();	//タイマーストップ
	t = timer.ms();	

	if (complete) {
		const String text = U"クリア！";
		while (System::Update()) {
			blocks.print_map();
			if (print_menu(blocks, t, true)) {
				goto start;
			}
			font(text).drawAt(game_field.x / 2, game_field.y / 2);
		}
	}
	else {
		blocks.make_answer();
		const String text = U"ゲームオーバー";
		while (System::Update()) {
			blocks.print_map();
			if (print_menu(blocks, t, true)) {
				goto start;
			}
			font(text).drawAt(game_field.x / 2, game_field.y / 2);
		}
	}
}
//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D Slack (ユーザコミュニティ) への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
