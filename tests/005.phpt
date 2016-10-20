--TEST--
Check for call user function with reference param without timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
function called_func(&$id){
    
    $id = 1024;
    return "called_func retval";
}


$id = 512;

echo $id,"\n";
$ret = call_func_with_timeout('called_func', 1000, array(&$id), $retval);
echo $ret,"\n";

echo $id;
?>
--EXPECT--
512
0
1024
