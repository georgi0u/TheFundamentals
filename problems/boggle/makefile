all: libboggle.a main.o
	clang boggle_main.o -o boggle -L./ -L/usr/lib/x86_64-linux-gnu   -I./include -L/usr/lib/x86_64-linux-gnu -lboost_system -lstdc++ -lm -lboost_program_options -lboggle

boggle.o:
	clang -c src/boggle/Board.cxx -I./include

libboggle.a: boggle.o
	ar r libboggle.a Board.o

main.o:
	clang -c src/boggle_main.cxx -I./include

clean:
	rm ./*.a ./*.o boggle
