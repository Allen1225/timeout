--TEST--
Check for call user class static method
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
class Test {
    public static function called_func(){
        return "Test::called_func";
    }
}

$ret = call_func_with_timeout(array('Test', 'called_func'), 1000);
echo $ret,"\n";
?>
--EXPECT--
0
