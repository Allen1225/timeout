--TEST--
Check for call user function with reference param and execute timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
function called_func(&$id){
    sleep(1);
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
104
512
