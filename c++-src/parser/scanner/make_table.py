#! /usr/bin/env python3

### @file make_table.py
### @brief InputFile.cc 内部で使うテーブルを生成するスクリプト
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2019 Yusuke Matsunaga
### All rights reserved.

# 述語の内容のテーブルを作る．
def make_table(pred) :

    header = '  static char table[] = {'
    tailer = '  };'

    print (header)
    for y in range(0, 16) :
        line = '    '
        for x in range(0, 16) :
            code = x + (y * 16)
            ch = chr(code)
            #print ('chr(%d) = %c' % (code, ch))
            if pred(ch) :
                line += '1'
            else :
                line += '0'

            if x < 15 :
                line += ', '
            elif y < 15 :
                line += ','

        print (line)

    print (tailer)


# 識別子の先頭で使える文字
def is_strchar1(ch) :
    if 'a' <= ch and ch <= 'z' :
        return True

    if 'A' <= ch and ch <= 'Z' :
        return True

    if ch == '_' :
        return True

    if ch == '$' :
        return True

    return False

# 識別子で使える文字
def is_strchar(ch) :
    if is_strchar1(ch) :
        return True

    if '0' <= ch and ch <= '9' :
        return True

    return False


# 不定値
def is_dcchar(ch) :
    if ch == 'x' :
        return True

    if ch == 'X' :
        return True

    if ch == 'z' :
        return True

    if ch == 'Z' :
        return True

    if ch == '?' :
        return True

    return False


# 2進数モードで使える文字
def is_binchar(ch) :
    if ch == '0' :
        return True

    if ch == '1' :
        return True

    if is_dcchar(ch) :
        return True

    return False


# 8進数モードで使える文字
def is_octchar(ch) :
    if '0' <= ch and ch <= '7' :
        return True

    if is_dcchar(ch) :
        return True

    return False


# 10進数モードで使える文字
def is_decchar(ch) :
    if '0' <= ch and ch <= '9' :
        return True

    return False


# 16進数モードで使える文字
def is_hexchar(ch) :
    if '0' <= ch and ch <= '9' :
        return True

    if 'a' <= ch and ch <= 'f' :
        return True

    if 'A' <= ch and ch <= 'F' :
        return True

    if is_dcchar(ch) :
        return True

    return False


if __name__ == '__main__' :
    import sys
    import os

    def usage() :
        print('USAGE: {} command'.format(os.path.basename(sys.argv[0])))
        print(' command| strchar1: 識別子の先頭文字')
        print('        | strchar:  識別子の文字')
        print('        | binchar:  2進数モードの文字')
        print('        | octchar:  8進数モードの文字')
        print('        | decchar:  10進数モードの文字')
        print('        | hexchar:  16進数モードの文字')
        exit(1)

    if len(sys.argv) != 2 :
        usage()

    if sys.argv[1] == 'strchar1' :
        make_table(is_strchar1)
    elif sys.argv[1] == 'strchar' :
        make_table(is_strchar)
    elif sys.argv[1] == 'binchar' :
        make_table(is_binchar)
    elif sys.argv[1] == 'octchar' :
        make_table(is_octchar)
    elif sys.argv[1] == 'decchar' :
        make_table(is_decchar)
    elif sys.argv[1] == 'hexchar' :
        make_table(is_hexchar)
    else:
        usage()

    exit(0)
