--TEST--
Check for call user function and execute timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
function called_func(){
    sleep(1);
    echo "called_func without params and return\n";
}

$ret = call_func_with_timeout('called_func', 1000);
echo $ret,"\n";
?>
--EXPECT--
104
