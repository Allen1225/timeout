--TEST--
Check for call user class static method and execute timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
class Test {
    public static function called_func(){
        sleep(1);
        return "Test::called_func";
    }
}

$ret = call_func_with_timeout(array('Test', 'called_func'), 1000);
echo $ret,"\n";
?>
--EXPECT--
104
