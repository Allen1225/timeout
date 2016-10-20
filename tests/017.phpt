--TEST--
Check for call user class method with param by object and execute timeout
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
class Test {
    public function called_func($id){
        echo $id,"\n";
        sleep(1);
        return "Test::called_func";
    }
}

$obj = new Test;

$retval = null;
$ret = call_func_with_timeout(array($obj, 'called_func'), 1000, array(1024), $retval);
echo $ret,"\n";
?>
--EXPECT--
1024
104
