--TEST--
Check for timeout presence
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
function called_func(){
    echo "called_func without params and return\n";
}

$ret = call_func_with_timeout('called_func', 1000);
echo $ret,"\n";
?>
--EXPECT--
called_func without params and return
0
