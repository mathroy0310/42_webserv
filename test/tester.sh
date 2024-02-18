#!/bin/bash

for conf_file in test/conf/*.conf
do
    echo "Testing $conf_file"
    ./webserv "$conf_file"
done
