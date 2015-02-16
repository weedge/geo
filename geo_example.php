<?php
$distance = geo_distance(39.4269170000,100.92224000000,39.4169170000,100.92224000000);
$hash = geohash_encode(39.4169170000,100.92224000000);
$coord = geohash_decode($hash);
var_dump($hash,$coord);
