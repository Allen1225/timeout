--TEST--
Check for call user class method and execute timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
class Test {
    public function called_func(){
        sleep(1);
        echo "called_func without params and return\n";
    }
}

$ret = call_func_with_timeout(array('Test', 'called_func'), 1000);
echo $ret,"\n";
?>
--EXPECT--
104
