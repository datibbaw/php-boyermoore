<?php

function test_strpos($haystack, $needle)
{
    return strpos($haystack, $needle);
}

function test_bm($haystack, $needle)
{
	return boyermoore_indexof($haystack, $needle);
}

function testall($haystack, $needle, $n, $desc)
{
        echo "Test case: $desc ($n iterations)\n";

        $results = array();

        $start = microtime(true);
        for ($i = 0; $i != $n; ++$i) {
                test_strpos($haystack, $needle);
        }
        $results['strpos'] = microtime(true) - $start;

        $start = microtime(true);
        for ($i = 0; $i != $n; ++$i) {
                test_bm($haystack, $needle);
        }
        $results['boyermoore'] = microtime(true) - $start;

	$boyermoore_needle = boyermoore_study($needle);

        $start = microtime(true);
        for ($i = 0; $i != $n; ++$i) {
                test_bm($haystack, $boyermoore_needle);
        }
        $results['boyermoore (studied)'] = microtime(true) - $start;

        showresults($results);
}

function showresults(array $results)
{
        $min = PHP_INT_MAX;
        $minkey = -1;

        foreach ($results as $key => $time) {
                if ($time < $min) {
                        $min = $time;
                        $minkey = $key;
                }
        }

        foreach ($results as $key => $time) {
                echo sprintf("%-25s%-10.5f %s\n", $key, $time, $key === $minkey ? '(fastest)' : '-' . number_format(100 * ($time - $min) / $min, 2) . '%');
        }
}

$c_twokay = str_repeat('c', 2048);
$c_tenkay = str_repeat('c', 10 * 1024);
$c_128 = str_repeat('c', 128);
$d_128 = str_repeat('d', 128);
$cd_128 = str_repeat('c', 64) . str_repeat('d', 64);

$cd_twokay = str_repeat('c', 2048) . str_repeat('d', 2048);

//testall($c_twokay, 'd', 200000, 'Single char needle, 2k haystack, no match');
//testall($c_twokay, 'c', 200000, 'Single char needle, 2k haystack, match');
testall($c_twokay, $d_128, 200000, '128 byte needle, 2k haystack, no match');
testall($c_twokay, $c_128, 200000, '128 byte needle, 2k haystack, match');
testall($c_twokay, $cd_128, 200000, '64+64 byte needle, 2k haystack, no match');
testall($c_tenkay, $cd_128, 100000, '64+64 byte needle, 10k haystack, no match');
testall($cd_twokay, $cd_128, 100000, '64+64 byte needle, 2k+2k haystack, match');

