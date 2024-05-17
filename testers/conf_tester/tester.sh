#!/bin/bash
chmod 000 test/conf/no_perms.conf

for conf_file in test/conf/*.conf
do
    echo "Testing $conf_file"
    ./webserv "$conf_file"
done

chmod 644 test/conf/no_perms.conf