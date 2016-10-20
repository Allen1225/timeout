--TEST--
Check for call user function with reture value and execute timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
function called_func(){
    sleep(1);
    return "called_func retval";
}


$retval = null;
$ret = call_func_with_timeout('called_func', 1000, array(), $retval);
echo $ret,"\n";

echo $retval;
?>
--EXPECT--
104
