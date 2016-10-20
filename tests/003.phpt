--TEST--
Check for call user function with reture value
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
function called_func(){
    return "called_func retval";
}


$retval = null;
$ret = call_func_with_timeout('called_func', 1000, array(), $retval);
echo $ret,"\n";

echo $retval;
?>
--EXPECT--
0
called_func retval
