##READ ME

1. Run ./compileall.sh to compile all source code files into their binaries. The script will produce ./ldes, ./mdes1, ./mdes2

For manual compilation, using the commands below:

g++ -o ldes ldes.cpp
g++ -o mdes1 mdes-v1.cpp
g++ -o mdes2 mdes.cpp

./mdes2 is the version with support for CBC

2. Run ./ldes and ./mdes1 without any command line arguments

3. Run ./mdes2 as shown below

./mdes2 -key 10 -mode ECB -encrypt f4a5a32
./mdes2 -key 01 -mode ECB -decrypt b6f7a11
./mdes2 -key 11 -mode CBC -iv a -encrypt 2a45def
./mdes2 -key 00 -mode CBC -iv 4 -decrypt b412ab2