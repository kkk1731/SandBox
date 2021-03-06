# SandBox
## 実行環境
Windows10

VirtualBox　バージョン 6.1.18 r142142

CentOS Linux release 8.3.2011

カーネルバージョン 5.10.25

# 結果
簡単に結果を示す。サンドボックス内で実行されたプログラムでは、禁止したシステムコールを呼び出した場合-1が返されるようにした。今回は演習で自作した、"1"を返すシステムコール(上)とpidを返すシステムコール(下)を禁止した。

なおpidを返すもの(下)については、もともと存在するシステムコール"getpid()"も同時に行い、また子プロセスでも同じことを行うプログラムによってサンドボックスの動作を確認した。

確認していないが子プロセス以下も大丈夫そう。

```
$ ./return_constant
return 1
$ ./sandbox ./return_constant
return -1
```

```
$ ./fork_and_neogetpid
neo pid number = 2457, pid number = 2457
neo pid number = 2458, pid number = 2458
$ ./sandbox ./fork_and_neogetpid
neo pid number = -1, pid number = 2476
neo pid number = -1, pid number = 2477
$ 
```


## サンドボックスの実装
サンドボックス内で実行されたプログラムにおいて、指定したシステムコールを実行すると-1が返されるようにする。

systemcallが呼び出されると arch/x86/entry/common.c にてsystemcallが実行されるため、この部分を変更する。

### common.c
システムコール実行部の"regs->ax = sys_call_table[nr](regs);" (regs-ax にはシステムコールの返り値が格納される)の前に、プロセスがサンドボックス内にあるか/システムコールが許されているかを判断する。

regs->axはもともと-1となっている（entry_64.S内で確認)ので、許されないシステムコールの場合はスルーする。

```c
(省略)
#ifdef CONFIG_X86_64
__visible noinstr void do_syscall_64(unsigned long nr, struct pt_regs *regs)
{
        nr = syscall_enter_from_user_mode(regs, nr);

        instrumentation_begin();
        if (likely(nr < NR_syscalls)) {
                nr = array_index_nospec(nr, NR_syscalls);

                if(!is_in_proc(task_tgid_vnr(current)) ||  !is_in_syscall(nr)){
                        regs->ax = sys_call_table[nr](regs);
                }
#ifdef CONFIG_X86_X32_ABI
(以下略)
```

用いる関数等（is_in_proc等）はinclude/linux/sandbox.hに記し、common.cで<linux/sandbox.h>をインクルードして利用している。
実際の内容はarch/x86/kernel/sandbox_test.c内に記した。

### 子プロセスへの対応
kernel/exit.c と kernel/fork.cに変更を加えることで対応した。

- exitするときに、pidが下で示す構造体のpidメンバに含まれていればそれを消去
- forkするときに、pidが下に示す構造体のpidメンバに含まれていれば、次に生成するpidを追加
した。

なお簡易プロセス制御ブロックは次のように定義した。

```c
struct simple_proc_struct{
        pid_t pid;
        struct list_head proc_list;
};
```

### list.hの利用
ヘッダファイル include/linux/sandbx.h で、上記の簡易プロセス制御ブロックのrootノードになるもの(以下headと呼ぶ)を定義し、複数のプログラムでそれを共有した。

カーネル空間では、構造体が共有される？ため、一方のプログラムでheadに新たな要素を追加し、他方のプログラムからそれを参照できる。

今回の場合、list_head型のrootノード:simple_proc_struct_head(以下head)をsandbox.hで静的に宣言し、他プログラムでheadに追加・削除・参照
した。

# 問題点・疑問点
- どのファイルまでアクセスを許可するか指定する機能は未実装
- 名前空間の異なるプロセスへの対応
- おそらくexit.c とfork.cをいじったせいで、動作が重くなった
- task_struct構造体であるcurrentの実装はどうなっているのか
- 
 
