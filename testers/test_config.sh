#!/bin/bash

# This is a minimal set of ANSI/VT100 color codes
_END='\e[0m'
_BOLD='\e[1m'
_UNDER='\e[4m'
_REV='\e[7m'

# Colors
_GREY='\e[30m'
_RED='\e[31m'
_GREEN='\e[32m'
_YELLOW='\e[33m'
_BLUE='\e[34m'
_PURPLE='\e[35m'
_CYAN='\e[36m'
_WHITE='\e[37m'

# Inverted, i.e. colored backgrounds
_IGREY='\e[40m'
_IRED='\e[41m'
_IGREEN='\e[42m'
_IYELLOW='\e[43m'
_IBLUE='\e[44m'
_IPURPLE='\e[45m'
_ICYAN='\e[46m'
_IWHITE='\e[47m'

# **************************************************************************** #

dir_conf=./testers/bad_conf/

# list of all config files
broken_conf=(brackets.conf comments.conf empty.conf expected_equal.conf expected_semi.conf invalid_directive.conf invalid_error_pages.conf no_perms.conf no_server.conf noip.conf unexpected_token_serv_block.conf)

which siege
if [[ $? != 0 ]]; then
	apt-get install siege
fi

if [ ! -f "webserv" ]; then
	make
fi

function check_return_not
{
	if [ $? == $1 ]; then
		printf "Wrong return"
	fi
}

printf "${_ICYAN}${_BOLD}${_GREY}  ARGS TESTS  ${_END}\n"
# printf "${_GREEN}./webserv${_END}\n"
# ./webserv
# check_return_not 0

printf "${_GREEN}./webserv ${_YELLOW}1 2${_END}\n"
./webserv 1 2
check_return_not 0

printf "${_GREEN}./webserv ${_YELLOW}aaaaa${_END}\n"
./webserv aaaaa
check_return_not 0

printf "${_ICYAN}${_BOLD}${_GREY}  SYNTAX CONFIG TESTS  ${_END}\n"

i=0;
size=${#broken_conf[@]}
while (($i < $size)); do
	printf "${_GREEN}./webserv ${_YELLOW}${dir_conf}${broken_conf[$i]}${_END}\n"
	./webserv ${dir_conf}${broken_conf[$i]}
	check_return_not 0
	i=${i}+1
done
