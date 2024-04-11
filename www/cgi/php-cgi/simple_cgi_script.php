#!/usr/bin/php-cgi
<?php

// Read the 'name' parameter from the query string
parse_str($_SERVER['QUERY_STRING'], $query_params);
$name = $query_params['name'] ?? 'World';

// Output HTML response with the formatted name

echo <<<HTML
<!DOCTYPE html>
<html>
<head>
    <title>Hello, {$name}!</title>
</head>
<body>
    <h1>Hello, {$name}!</h1>
</body>
</html>
HTML;
