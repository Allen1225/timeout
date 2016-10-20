--TEST--
Check for call user class undefined method
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
$r1 = call_func_with_timeout(array($obj, 'called_func_no'), 1000, array(1024), $retval);
$r2 = call_func_with_timeout(array('test_no', 'called_func'), 1000, array(1024), $retval);
$r3 = call_func_with_timeout(array('test', 'called_func_no'), 1000, array(1024), $retval);
echo $r1,"\n";
echo $r2,"\n";
echo $r3,"\n";
?>
--EXPECT--
101
100
101

