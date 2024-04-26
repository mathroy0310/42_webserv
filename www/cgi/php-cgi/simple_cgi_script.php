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
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            font-family: Arial, sans-serif;
        }
        div {
            text-align: center;
        }
    </style>
</head>
<body>
    <div>
        <h1>Hello, {$name}!</h1>
    </div>
</body>
</html>
HTML;