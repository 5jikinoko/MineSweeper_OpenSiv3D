#pragma once
/*
#ifndef ___Constant.h
#define ___Constant.h
インクルードガードはpragma onceがやってくれる
*/
//定数を設定するヘッダファイルです

const Size window_size(960, 660);
const Size game_field(660, 660);
const Size minimum_margin(30, 30);
const Point menu_crnter((window_size.x - game_field.x) / 2 + game_field.x, window_size.y / 2);

//#endif