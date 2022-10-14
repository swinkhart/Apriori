all:
	g++ apriori.cpp -o apriori.exe
	./apriori.exe input.txt 10
	./apriori.exe input.txt 20
	./apriori.exe input.txt 30
	./apriori.exe input.txt 40
	./apriori.exe input.txt 50
clean:
	rm apriori.exe