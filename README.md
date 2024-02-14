Compile Instruction:

apt install libgtk-3-dev

g++ -o esim_manager esim_manager.cpp 'pkg-config --cflags --libs gtk+-3.0'
