--TEST--
Check for call php internal function
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
$ret = null;
$r = call_func_with_timeout('array_keys', 1000, array(array('id' => 1,'cid'=>2)), $ret);
foreach($ret as $v){
    echo $v,"\n";
}
?>
--EXPECT--
id
cid
