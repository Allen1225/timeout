--TEST--
Check for call user class method normaly
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
class Test {
    public function called_func(){
        echo "called_func without params and return\n";
    }
}

$ret = call_func_with_timeout(array('Test', 'called_func'), 1000);
echo $ret,"\n";
?>
--EXPECT--
called_func without params and return
0
