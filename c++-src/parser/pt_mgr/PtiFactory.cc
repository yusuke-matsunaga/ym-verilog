
/// @file PtiFactory.cc
/// @brief PtiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/PtiFactory.h"
#include "parser/CptFactory.h"
#include "parser/SptFactory.h"
#include "ym/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtiFactory の実装クラスのオブジェクトを作る関数
// @param[in] type 実装クラスを指定する文字列
// @param[in] alloc メモリアロケータ
PtiFactory*
PtiFactory::make_obj(const string& type,
		     PtAlloc& alloc)
{
  if ( type == "spt" ) {
    return new SptFactory(alloc);
  }
  return new CptFactory(alloc);
}

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
PtiFactory::PtiFactory(PtAlloc& alloc) :
  mAlloc{alloc}
{
}

// @brief デストラクタ
PtiFactory::~PtiFactory()
{
}

// @brief パス記述の生成
// @param[in] fr ファイル位置の情報
// @param[in] edge
// @param[in] input_array
// @param[in] input_pol
// @param[in] op
// @param[in] output_array
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
// @return 生成されたパス記述
const PtPathDecl*
PtiFactory::new_PathDecl(const FileRegion& fr,
			 int edge,
			 const PtExprArray* input_array,
			 int input_pol,
			 VpiPathType op,
			 const PtExpr* output,
			 int output_pol,
			 const PtExpr* expr,
			 const PtPathDelay* path_delay)
{
  auto output_array{new_PtExprArray(output)};
  return new_PathDecl(fr, edge, input_array, input_pol,
		      op, output_array, output_pol,
		      expr, path_delay);
}

// @brief constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index インデックス
// @return 生成された const primary
const PtExpr*
PtiFactory::new_CPrimary(const FileRegion& fr,
			 const char* name,
			 const PtExpr* index)
{
  auto index_array{new_PtExprArray(index)};
  return new_CPrimary(fr, name, index_array);
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @return 生成されたイベントコントロール
const PtControl*
PtiFactory::new_EventControl(const FileRegion& fr)
{
  auto event_array{new_PtExprArray()};
  return new_EventControl(fr, event_array);
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] event イベント
// @return 生成されたイベントコントロール
const PtControl*
PtiFactory::new_EventControl(const FileRegion& fr,
			     const PtExpr* event)
{
  auto event_array{new_PtExprArray(event)};
  return new_EventControl(fr, event_array);
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @return 生成されたリピートコントロール
const PtControl*
PtiFactory::new_RepeatControl(const FileRegion& fr,
			      const PtExpr* expr)
{
  auto event_array{new_PtExprArray()};
  return new_RepeatControl(fr, expr, event_array);
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @param[in] event 繰り返しの単位となるイベント
// @return 生成されたリピートコントロール
const PtControl*
PtiFactory::new_RepeatControl(const FileRegion& fr,
			      const PtExpr* expr,
			      const PtExpr* event)
{
  auto event_array{new_PtExprArray(event)};
  return new_RepeatControl(fr, expr, event_array);
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     const PtExpr* expr1)
{
  return new_Inst(fr, new_PtConArray(expr1));
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     const PtExpr* expr1,
		     const PtExpr* expr2)
{
  return new_Inst(fr, new_PtConArray(expr1, expr2));
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     const PtExpr* expr1,
		     const PtExpr* expr2,
		     const PtExpr* expr3)
{
  return new_Inst(fr, new_PtConArray(expr1, expr2, expr3));
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     const PtExpr* expr1,
		     const PtExpr* expr2,
		     const PtExpr* expr3,
		     const PtExpr* expr4)
{
  return new_Inst(fr, new_PtConArray(expr1, expr2, expr3, expr4));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      const PtExpr* expr1)
{
  return new_InstN(fr, name, new_PtConArray(expr1));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      const PtExpr* expr1,
		      const PtExpr* expr2)
{
  return new_InstN(fr, name, new_PtConArray(expr1, expr2));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      const PtExpr* expr1,
		      const PtExpr* expr2,
		      const PtExpr* expr3)
{
  return new_InstN(fr, name, new_PtConArray(expr1, expr2, expr3));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      const PtExpr* expr1,
		      const PtExpr* expr2,
		      const PtExpr* expr3,
		      const PtExpr* expr4)
{
  return new_InstN(fr, name, new_PtConArray(expr1, expr2, expr3, expr4));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      const PtExpr* left,
		      const PtExpr* right,
		      const PtExpr* expr1)
{
  return new_InstV(fr, name, left, right, new_PtConArray(expr1));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      const PtExpr* left,
		      const PtExpr* right,
		      const PtExpr* expr1,
		      const PtExpr* expr2)
{
  return new_InstV(fr, name, left, right, new_PtConArray(expr1, expr2));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      const PtExpr* left,
		      const PtExpr* right,
		      const PtExpr* expr1,
		      const PtExpr* expr2,
		      const PtExpr* expr3)
{
  return new_InstV(fr, name, left, right, new_PtConArray(expr1, expr2, expr3));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      const PtExpr* left,
		      const PtExpr* right,
		      const PtExpr* expr1,
		      const PtExpr* expr2,
		      const PtExpr* expr3,
		      const PtExpr* expr4)
{
  return new_InstV(fr, name, left, right,
		   new_PtConArray(expr1, expr2, expr3, expr4));
}

// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
const PtConnectionArray*
PtiFactory::new_PtConArray(const PtExpr* expr1)
{
  auto con{new_OrderedCon(expr1)};
  void* p{mAlloc.get_memory(sizeof(PtConnectionArray))};
  return new (p) PtConnectionArray(mAlloc, con);
}

// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
// @param[in] expr2 結合する式
const PtConnectionArray*
PtiFactory::new_PtConArray(const PtExpr* expr1,
			   const PtExpr* expr2)
{
  auto con1{new_OrderedCon(expr1)};
  auto con2{new_OrderedCon(expr2)};
  void* p{mAlloc.get_memory(sizeof(PtConnectionArray))};
  return new (p) PtConnectionArray(mAlloc, con1, con2);
}

// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
// @param[in] expr2 結合する式
// @param[in] expr3 結合する式
const PtConnectionArray*
PtiFactory::new_PtConArray(const PtExpr* expr1,
			   const PtExpr* expr2,
			   const PtExpr* expr3)
{
  auto con1{new_OrderedCon(expr1)};
  auto con2{new_OrderedCon(expr2)};
  auto con3{new_OrderedCon(expr3)};
  void* p{mAlloc.get_memory(sizeof(PtConnectionArray))};
  return new (p) PtConnectionArray(mAlloc, con1, con2, con3);
}

// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
// @param[in] expr2 結合する式
// @param[in] expr3 結合する式
// @param[in] expr4 結合する式
const PtConnectionArray*
PtiFactory::new_PtConArray(const PtExpr* expr1,
			   const PtExpr* expr2,
			   const PtExpr* expr3,
			   const PtExpr* expr4)
{
  auto con1{new_OrderedCon(expr1)};
  auto con2{new_OrderedCon(expr2)};
  auto con3{new_OrderedCon(expr3)};
  auto con4{new_OrderedCon(expr4)};
  void* p{mAlloc.get_memory(sizeof(PtConnectionArray))};
  return new (p) PtConnectionArray(mAlloc, con1, con2, con3, con4);
}

const PtExprArray*
PtiFactory::new_PtExprArray()
{
  void* p{mAlloc.get_memory(sizeof(PtExprArray))};
  auto expr_array{new (p) PtExprArray()};
  return expr_array;
}

const PtExprArray*
PtiFactory::new_PtExprArray(const PtExpr* expr)
{
  void* p{mAlloc.get_memory(sizeof(PtExprArray))};
  auto expr_array{new (p) PtExprArray(mAlloc, expr)};
  return expr_array;
}

END_NAMESPACE_YM_VERILOG
