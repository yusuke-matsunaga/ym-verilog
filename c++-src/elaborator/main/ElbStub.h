#ifndef ELBSTUB_H
#define ELBSTUB_H

/// @file ElbStub.h
/// @brief ElbStub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbStub ElbStub.h "ElbStub.h"
/// @brief インスタンス化で用いる stub クラス
//////////////////////////////////////////////////////////////////////
class ElbStub
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~ElbStub() = default;

  /// @brief 処理を行う．
  virtual
  void
  eval() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @name 個々のクラスごとの ElbStub クラスを作るためのテンプレートクラス
//////////////////////////////////////////////////////////////////////
//@{

/// @brief 引数が1つのタイプ
template<typename T,
	 typename A>
class ElbStubT1 :
  public ElbStub
{
public:

  /// @brief 引数が1つの ElbMgrImpl のメンバ関数
  using ElbFunc = void (T::*)(A);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a memfunc を起動するときの引数
  ElbStubT1(T* obj,
	    ElbFunc memfunc,
	    A a);

  /// @brief デストラクタ
  ~ElbStubT1();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a) を実行する．
  void
  eval() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // T のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;

};


/// @brief 引数が2つのタイプ
template<typename T,
	 typename A,
	 typename B>
class ElbStubT2 :
  public ElbStub
{
public:

  /// @brief 引数が2つの ElbMgrImpl のメンバ関数
  using ElbFunc = void(T::*)(A, B);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a, b memfunc を起動するときの引数
  ElbStubT2(T* obj,
	    ElbFunc memfunc,
	    A a,
	    B b);

  /// @brief デストラクタ
  ~ElbStubT2();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b) を実行する．
  void
  eval() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;
  B mB;
};


/// @brief 引数が3つのタイプ
template<typename T,
	 typename A,
	 typename B,
	 typename C>
class ElbStubT3 :
  public ElbStub
{
public:

  /// @brief 引数が3つの ElbMgrImpl のメンバ関数
  using ElbFunc = void (T::*)(A, B, C);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a, b, c memfunc を起動するときの引数
  ElbStubT3(T* obj,
	    ElbFunc memfunc,
	    A a,
	    B b,
	    C c);

  /// @brief デストラクタ
  ~ElbStubT3();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b, c) を実行する．
  void
  eval() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;
  B mB;
  C mC;
};


/// @brief 引数が4つのタイプ
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
class ElbStubT4 :
  public ElbStub
{
public:

  /// @brief 引数が4つの ElbMgrImpl のメンバ関数
  using ElbFunc = void (T::*)(A, B, C, D);

public:

  /// @brief コンストラクタ
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] memfunc メンバ関数へのポインタ
  /// @param[in] a, b, c, d memfunc を起動するときの引数
  ElbStubT4(T* obj,
	    ElbFunc memfunc,
	    A a,
	    B b,
	    C c,
	    D d);

  /// @brief デストラクタ
  ~ElbStubT4();

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b, c, d) を実行する．
  void
  eval() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;
  B mB;
  C mC;
  D mD;
};

//@}


//////////////////////////////////////////////////////////////////////
// インライン関数の実装
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] obj 対象のオブジェクト
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] a memfunc を起動するときの引数
template<typename T,
	 typename A>
ElbStubT1<T, A>::ElbStubT1(T* obj,
			   ElbFunc memfunc,
			   A a) :
  mObj{obj},
  mMemFunc{memfunc},
  mA{a}
{
}

// @brief デストラクタ
template<typename T,
	 typename A>
ElbStubT1<T, A>::~ElbStubT1()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A>
void
ElbStubT1<T, A>::eval()
{
  (mObj->*(mMemFunc))(mA);
}

// @brief コンストラクタ
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] obj 対象のオブジェクト
// @param[in] a, b memfunc を起動するときの引数
template<typename T,
	 typename A,
	 typename B>
ElbStubT2<T, A, B>::ElbStubT2(T* obj,
			      ElbFunc memfunc,
			      A a,
			      B b) :
  mObj{obj},
  mMemFunc{memfunc},
  mA{a},
  mB{b}
{
}

// @brief デストラクタ
template<typename T,
	 typename A,
	 typename B>
ElbStubT2<T, A, B>::~ElbStubT2()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A,
	 typename B>
void
ElbStubT2<T, A, B>::eval()
{
  (mObj->*(mMemFunc))(mA, mB);
}

// @brief コンストラクタ
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] obj 対象のオブジェクト
// @param[in] a, b, c memfunc を起動するときの引数
template<typename T,
	 typename A,
	 typename B,
	 typename C>
ElbStubT3<T, A, B, C>::ElbStubT3(T* obj,
				 ElbFunc memfunc,
				 A a,
				 B b,
				 C c) :
  mObj{obj},
  mMemFunc{memfunc},
  mA{a},
  mB{b},
  mC{c}
{
}

// @brief デストラクタ
template<typename T,
	 typename A,
	 typename B,
	 typename C>
ElbStubT3<T, A, B, C>::~ElbStubT3()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A,
	 typename B,
	 typename C>
void
ElbStubT3<T, A, B, C>::eval()
{
  (mObj->*(mMemFunc))(mA, mB, mC);
}

// @brief コンストラクタ
// @param[in] memfunc メンバ関数へのポインタ
// @param[in] obj 対象のオブジェクト
// @param[in] a, b, c, d memfunc を起動するときの引数
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
ElbStubT4<T, A, B, C, D>::ElbStubT4(T* obj,
				    ElbFunc memfunc,
				    A a,
				    B b,
				    C c,
				    D d) :
  mObj{obj},
  mMemFunc{memfunc},
  mA{a},
  mB{b},
  mC{c},
  mD{d}
{
}

// @brief デストラクタ
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
ElbStubT4<T, A, B, C, D>::~ElbStubT4()
{
}

// @brief 処理を行う．
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
void
ElbStubT4<T, A, B, C, D>::eval()
{
  (mObj->*(mMemFunc))(mA, mB, mC, mD);
}

END_NAMESPACE_YM_VERILOG

#endif // ELBSTUB_H
