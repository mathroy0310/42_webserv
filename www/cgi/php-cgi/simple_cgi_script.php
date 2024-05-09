#!/usr/bin/php
<?php
// Read the value of 'name' parameter from the query string
parse_str($_SERVER['QUERY_STRING'], $query);
$name = isset($query['name']) ? urldecode($query['name']) : '';

// Output HTML response with the formatted name
echo "Content-type: text/html\r\n\r\n";
?>
<!DOCTYPE html>
<html>
<head>
<title>Hello, <?php echo htmlspecialchars($name); ?>!</title>
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
<h1>Hello, <?php echo htmlspecialchars($name); ?>!</h1>
</div>
</body>
</html>
