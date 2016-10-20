--TEST--
Check for call user class static method with return value and execute timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
class Test {
    public static function called_func($id){
        echo $id,"\n";
        sleep(1);
        return "Test::called_func";
    }
}

$retval = null;
$ret = call_func_with_timeout(array('Test', 'called_func'), 1000,array(1024), $retval);
echo $ret,"\n";

echo $retval;
?>
--EXPECT--
1024
104
