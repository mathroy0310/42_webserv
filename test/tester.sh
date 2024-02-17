#!/bin/bash

for conf_file in conf/*.conf
do
    echo "Testing $conf_file"
    ./webserv "$conf_file"
done