NAME = webserv

CXXFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g3

CXX = c++

CFILES =  main.cpp Server.cpp Location.cpp Config/Confi_Pars.cpp Config/fun.cpp delete.cpp\

OBJ = ${CFILES:.cpp=.o}

all: ${NAME}

${NAME} : ${OBJ}
	@${CXX} ${CXXFLAGS} -o $@ $^

%.o:%.cpp webserv.hpp
	@${CXX} ${CXXFLAGS} -c $<

clean :
	@rm -rf ${OBJ}
fclean :clean
	@rm -rf ${NAME}

re : fclean all

run : all clean
	@ ./webserv
