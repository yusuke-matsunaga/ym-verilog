#ifndef ATTRGEN_H
#define ATTRGEN_H

/// @file AttrGen.h
/// @brief AttrGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "parser/PtiAttrInfo.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AttrGen AttrGen.h "AttrGen.h"
/// @brief attribute instance を生成するクラス
///
/// attribute instance は構文木の要素に対して設定されており，
/// 例えばモジュールを複数インスタンス化した時には当然複数回
/// この関数が呼ばれることになる．
/// 通常のエラボレーションと異なり attribute instance は毎回同じなので
/// おなじオブジェクトを使い回す．
///
/// 実際には attribute instance のリストであり，
/// さらに attribute instance 自体が attribute spec のリストなので
/// ややこしい
//////////////////////////////////////////////////////////////////////
class AttrGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  AttrGen(Elaborator& elab,
	  ElbMgr& elb_mgr);

  /// @brief デストラクタ
  ~AttrGen();


public:
  //////////////////////////////////////////////////////////////////////
  // AttrGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtAttr から VlAttribute を生成する．
  /// @param[in] attr_info 属性リストの情報
  ///
  /// 結果は mHash に登録される．
  void
  instantiate_attribute(const PtiAttrInfo& attr_info);

  /// @brief 構文木要素に対応する属性リストを返す．
  /// @param[in] pt_obj 元となる構文木要素
  const vector<const VlAttribute*>&
  attribute_list(const PtBase* pt_obj);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  unordered_map<const PtBase*, vector<const VlAttribute*>> mHash;

  // 空のリスト
  vector<const VlAttribute*> mEmptyList{};

};

END_NAMESPACE_YM_VERILOG

#endif // ATTRGEN_H
