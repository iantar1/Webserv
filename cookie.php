<?php
// Set a cookie named "user" with the value "ismail antar" that expires in 1 hour
setcookie("user", "ismail antar", time() + 3600, "/");

// Display a message indicating that the cookie has been set
echo "Cookie 'user' is set!";

?>