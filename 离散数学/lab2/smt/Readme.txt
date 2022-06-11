1、在虚拟机中输入下面的命令，安装Z3。Z3是微软开发的SMT solver，被广泛使用。
git clone https://github.com/Z3Prover/z3.git
cd z3
python scripts/mk_make.py
cd build
make
sudo make install
make examples

2、SMT solver的示例程序在smt.cpp中，请把smt.cpp和Makefile放在同一个文件夹下面，然后输入make就可以编译执行smt.cpp。

3、安装好Z3后，在下载的z3文件夹下面的examples/c++/example.cpp里有更多C++示例程序。
   示例教程请看http://www.cs.utah.edu/~vinu/research/formal/tools/notes/z3-notes.html