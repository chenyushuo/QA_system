objects = main.o search_engine.o index.o document.o answer_extract.o search_result.o query.o replacer.o kbhit.o filter.o
pattern = --std=c++11 -g -Wall -O3 -pg
ltp_path = /home/cys/ltp/ltp-3.4.0
ltp_pattern = -I $(ltp_path)/include/ltp/ -I $(ltp_path)/thirdparty/boost/include
ltp_link = -Wl,-dn -L $(ltp_path)/lib/ -lsegmentor -lpostagger -lboost_regex -Wl,-dy

run : main
	reset && ./main

main : $(objects)
	g++ -o main $(objects) $(ltp_link) $(pattern)

main.o : main.cpp search_engine.h
	g++ -c main.cpp $(pattern)

search_engine.o : search_engine.cpp search_engine.h document.h index.h search_result.h query.h kbhit.h filter.h answer_extract.h replacer.h
	g++ -c search_engine.cpp $(pattern) $(ltp_pattern)

index.o : index.cpp index.h document.h
	g++ -c index.cpp $(pattern)

document.o : document.cpp document.h
	g++ -c document.cpp $(pattern)

answer_extract.o : answer_extract.cpp answer_extract.h search_result.h
	g++ -c answer_extract.cpp $(pattern) $(ltp_pattern)

search_result.o : search_result.cpp search_result.h index.h document.h query.h set_color.h
	g++ -c search_result.cpp $(pattern)

query.o : query.cpp query.h set_color.h filter.h replacer.h
	g++ -c query.cpp $(pattern) $(ltp_pattern)

replacer.o : replacer.cpp replacer.h
	g++ -c replacer.cpp $(pattern)

kbhit.o : kbhit.cpp kbhit.h set_color.h
	g++ -c kbhit.cpp $(pattern)

filter.o : filter.cpp filter.h
	g++ -c filter.cpp $(pattern)


clean :
	rm -rf main $(objects)
