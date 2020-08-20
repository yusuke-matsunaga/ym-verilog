
/// @file PtiFactory.cc
/// @brief PtiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "parser/PtiFactory.h"
#include "parser/CptFactory.h"
#include "parser/SptFactory.h"
#include "parser/PuHierName.h"
#include "alloc/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtiFactory の実装クラスのオブジェクトを作る関数
// @param[in] type 実装クラスを指定する文字列
// @param[in] alloc メモリアロケータ
PtiFactory*
PtiFactory::make_obj(const string& type,
		     Alloc& alloc)
{
  if ( type == "spt" ) {
    return new SptFactory(alloc);
  }
  return new CptFactory(alloc);
}

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
PtiFactory::PtiFactory(Alloc& alloc) :
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
			 const vector<const PtExpr*>& input_array,
			 int input_pol,
			 VpiPathType op,
			 const PtExpr* output,
			 int output_pol,
			 const PtExpr* expr,
			 const PtPathDelay* path_delay)
{
  return new_PathDecl(fr, edge, input_array, input_pol,
		      op, vector<const PtExpr*>{output}, output_pol,
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
  return new_CPrimary(fr, name, vector<const PtExpr*>{index});
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @return 生成されたイベントコントロール
const PtControl*
PtiFactory::new_EventControl(const FileRegion& fr)
{
  return new_EventControl(fr, vector<const PtExpr*>());
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] event イベント
// @return 生成されたイベントコントロール
const PtControl*
PtiFactory::new_EventControl(const FileRegion& fr,
			     const PtExpr* event)
{
  return new_EventControl(fr, vector<const PtExpr*>{event});
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @return 生成されたリピートコントロール
const PtControl*
PtiFactory::new_RepeatControl(const FileRegion& fr,
			      const PtExpr* expr)
{
  return new_RepeatControl(fr, expr, vector<const PtExpr*>());
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
  return new_RepeatControl(fr, expr, vector<const PtExpr*>{event});
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1 ポート割り当て
// @return 生成された module instance/UDP instance の要素
const PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     const PtExpr* expr1)
{
  return new_Inst(fr,
		  vector<const PtConnection*>{new_OrderedCon(expr1)});
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
  return new_Inst(fr,
		  vector<const PtConnection*>{new_OrderedCon(expr1),
 					      new_OrderedCon(expr2)});
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
  return new_Inst(fr,
		  vector<const PtConnection*>{new_OrderedCon(expr1),
					      new_OrderedCon(expr2),
 					      new_OrderedCon(expr3)});
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
  return new_Inst(fr,
		  vector<const PtConnection*>{new_OrderedCon(expr1),
					      new_OrderedCon(expr2),
					      new_OrderedCon(expr3),
 					      new_OrderedCon(expr4)});
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
  return new_InstN(fr, name,
		   vector<const PtConnection*>{new_OrderedCon(expr1)});

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
  return new_InstN(fr, name,
		   vector<const PtConnection*>{new_OrderedCon(expr1),
						new_OrderedCon(expr2)});
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
  return new_InstN(fr, name,
		   vector<const PtConnection*>{new_OrderedCon(expr1),
						 new_OrderedCon(expr2),
						 new_OrderedCon(expr3)});
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
  return new_InstN(fr, name,
		  vector<const PtConnection*>{new_OrderedCon(expr1),
					      new_OrderedCon(expr2),
					      new_OrderedCon(expr3),
 					      new_OrderedCon(expr4)});
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
  return new_InstV(fr, name, left, right,
		   vector<const PtConnection*>{new_OrderedCon(expr1)});
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
  return new_InstV(fr, name, left, right,
		  vector<const PtConnection*>{new_OrderedCon(expr1),
 					      new_OrderedCon(expr2)});
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
  return new_InstV(fr, name, left, right,
		  vector<const PtConnection*>{new_OrderedCon(expr1),
					      new_OrderedCon(expr2),
 					      new_OrderedCon(expr3)});

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
		  vector<const PtConnection*>{new_OrderedCon(expr1),
					      new_OrderedCon(expr2),
					      new_OrderedCon(expr3),
 					      new_OrderedCon(expr4)});
}

// @brief 階層名の生成
// @param[in] head_name 階層の上位部分
// @param[in] index インデックス
// @param[in] name 階層の最下位部分
PuHierName*
PtiFactory::new_HierName(const PtNameBranch* nb,
			 const char* name)
{
  void* p{mAlloc.get_memory(sizeof(PuHierName))};
  auto hname{new (p) PuHierName(nb, name)};
  return hname;
}

END_NAMESPACE_YM_VERILOG
