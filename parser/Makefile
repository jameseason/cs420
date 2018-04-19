main_options = -Wall -g

truc: truc.o addoptoken.o buffer.o eoftoken.o idtoken.o keywordtoken.o muloptoken.o numtoken.o parser.o punctoken.o reloptoken.o scanner.o symbol_table.o token.o
	g++ $(main_options) -o truc truc.o addoptoken.o buffer.o eoftoken.o idtoken.o keywordtoken.o muloptoken.o numtoken.o parser.o punctoken.o reloptoken.o scanner.o symbol_table.o token.o

truc.o: truc.cc parser.h scanner.h
	g++ -c truc.cc

addoptoken.o: addoptoken.cc addoptoken.h token.h
	g++ -c addoptoken.cc

buffer.o: buffer.cc buffer.h 
	g++ -c buffer.cc

eoftoken.o: eoftoken.cc eoftoken.h
	g++ -c eoftoken.cc

idtoken.o: idtoken.cc idtoken.h token.h
	g++ -c idtoken.cc

keywordtoken.o: keywordtoken.cc keywordtoken.h token.h
	g++ -c keywordtoken.cc

muloptoken.o: muloptoken.cc muloptoken.h token.h
	g++ -c muloptoken.cc

numtoken.o: numtoken.cc numtoken.h token.h
	g++ -c numtoken.cc

parser.o: parser.cc parser.h addoptoken.h eoftoken.h idtoken.h keywordtoken.h muloptoken.h numtoken.h punctoken.h reloptoken.h scanner.h symbol_table.h token.h
	g++ -c parser.cc

punctoken.o: punctoken.cc punctoken.h token.h
	g++ -c punctoken.cc

reloptoken.o: reloptoken.cc reloptoken.h token.h
	g++ -c reloptoken.cc

scanner.o: scanner.cc scanner.h addoptoken.h buffer.h eoftoken.h idtoken.h keywordtoken.h muloptoken.h numtoken.h punctoken.h reloptoken.h token.h
	g++ -c scanner.cc

symbol_table.o: symbol_table.h symbol_table.cc
	g++ -c symbol_table.cc

token.o: token.cc token.h
	g++ -c token.cc

clean: 
	rm *.o