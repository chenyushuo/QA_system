objects = main.o queryjr.o chat.o querytype.o ask.o multiqa.o search_engine.o index.o document.o answer_extract.o search_result.o query.o ltp.o replacer.o filter.o restorer.o kbhit.o my_define.o
pattern = --std=c++11 -g -Wall -O3 -DCYS_COMPUTER -DSET_COLOR
ltp_path = /home/cys/ltp/ltp-3.4.0
ltp_pattern = -I $(ltp_path)/include/ltp/ -I $(ltp_path)/thirdparty/boost/include
ltp_link = -Wl,-dn -L $(ltp_path)/lib/ -lsegmentor -lpostagger -lboost_regex -Wl,-dy

run : main
	reset && ./main -input 院长是谁？

main : $(objects)
	g++ -o main $(objects) $(ltp_link)

main.o : main.cpp main.cpp ask.h multiqa.h queryjr.h
	g++ -c main.cpp $(pattern)

queryjr.o : queryjr.cpp queryjr.h ask.h multiqa.h querytype.h search_engine.h ltp.h chat.h
	g++ -c queryjr.cpp $(pattern)

chat.o : chat.cpp chat.h
	g++ -c chat.cpp $(pattern)

querytype.o : querytype.cpp querytype.h ltp.h
	g++ -c querytype.cpp $(pattern)

ask.o : ask.cpp ask.h
	g++ -c ask.cpp $(pattern)

multiqa.o : multiqa.cpp multiqa.h ltp.h
	g++ -c multiqa.cpp $(pattern)

search_engine.o : search_engine.cpp search_engine.h document.h index.h query.h search_result.h answer_extract.h ltp.h kbhit.h
	g++ -c search_engine.cpp $(pattern)

index.o : index.cpp index.h document.h
	g++ -c index.cpp $(pattern)

document.o : document.cpp document.h
	g++ -c document.cpp $(pattern)

answer_extract.o : answer_extract.cpp answer_extract.h search_result.h ltp.h
	g++ -c answer_extract.cpp $(pattern)

search_result.o : search_result.cpp search_result.h index.h document.h query.h
	g++ -c search_result.cpp $(pattern)

query.o : query.cpp query.h ltp.h
	g++ -c query.cpp $(pattern)

ltp.o : ltp.cpp ltp.h filter.h replacer.h restorer.h
	g++ -c ltp.cpp $(pattern) $(ltp_pattern)

replacer.o : replacer.cpp replacer.h
	g++ -c replacer.cpp $(pattern)

filter.o : filter.cpp filter.h
	g++ -c filter.cpp $(pattern)

restorer.o : restorer.cpp restorer.h
	g++ -c restorer.cpp $(pattern)

kbhit.o : kbhit.cpp kbhit.h
	g++ -c kbhit.cpp $(pattern)

my_define.o : my_define.cpp my_define.h
	g++ -c my_define.cpp $(pattern)


clean :
	rm -rf main $(objects)
