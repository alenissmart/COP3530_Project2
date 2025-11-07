build:
	g++ -o project2 main.cpp trie.cpp hashtable.cpp

run:
	./project2

clean:
	rm -f project2
	rm -f benchmark_results.csv
	rm -f performance_report.txt