/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_geo.h"

#include "geohash.h"
#include <string>

#define PI 3.14159265359
#define deg2rad(deg) ((PI*deg)/180)
#define rad2deg(rad) ((180*rad)/PI)
#define EARTH_RADIUS 6378137

static double get_distance(double lat1,double lng1,double lat2,double lng2);

/* If you declare any globals in php_geo.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(geo)
*/

/* True global resources - no need for thread safety here */
static int le_geo;

/* {{{ geo_functions[]
 *
 * Every user visible function must have an entry in geo_functions[].
 */
const zend_function_entry geo_functions[] = {
	PHP_FE(geohash_decode,	NULL)
	PHP_FE(geohash_encode,	NULL)
	PHP_FE(geo_distance,	NULL)
	PHP_FE(geo_info,	NULL)
	PHP_FE_END	/* Must be the last line in geo_functions[] */
};
/* }}} */

/* {{{ geo_module_entry
 */
zend_module_entry geo_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"geo",
	geo_functions,
	PHP_MINIT(geo),
	PHP_MSHUTDOWN(geo),
	PHP_RINIT(geo),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(geo),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(geo),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_GEO_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GEO
ZEND_GET_MODULE(geo)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("geo.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_geo_globals, geo_globals)
    STD_PHP_INI_ENTRY("geo.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_geo_globals, geo_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_geo_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_geo_init_globals(zend_geo_globals *geo_globals)
{
	geo_globals->global_value = 0;
	geo_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(geo)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(geo)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(geo)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(geo)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(geo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "geo support", "enabled");
	php_info_print_table_row(2, "version", PHP_GEO_VERSION);
	php_info_print_table_row(2, "author", "wuyong");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* Every user-visible function in PHP should document itself in the source */

/*geohash_encode(double lat, double lng, int precision)*/
PHP_FUNCTION(geohash_encode)
{
    double lat,lng;
    int precision = 12;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &lat,&lng,&precision) == FAILURE) {
        return;
    }

    if(lat>90.0 || lat<-90.0){
        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Argument #1 range from -90.0 to 90.0");
        return;
    }
    if(lng>180.0 || lng<-180.0){
        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Argument #2 range from -180.0 to 180.0");
        return;
    }
    if(precision!=0){
        if(precision<0){
            php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Argument #3 should be a positive number");
            return;
        }
    }else{
        precision=12;
    }
    
    char* hash = geohash_encode(lat,lng,precision);
    int hash_len = strlen(hash);

    RETURN_STRINGL(hash,hash_len,1);
}

/*geohash_decode(char* hash)*/
PHP_FUNCTION(geohash_decode)
{
    char* hash;
    int hash_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hash,&hash_len) == FAILURE) {
        return;
    }

    GeoCoord coord = geohash_decode(hash);

    array_init(return_value);
    add_assoc_double(return_value,"lat",coord.latitude);
    add_assoc_double(return_value,"lng",coord.longitude);

    add_assoc_double(return_value,"north",coord.north);
    add_assoc_double(return_value,"east",coord.east);
    add_assoc_double(return_value,"south",coord.longitude);
    add_assoc_double(return_value,"west",coord.west);


    //zend_printf('geohash_decode');
}


/* {{{ proto string geo_distance(double lat1,double lng1,double lat2,double lng2)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(geo_distance)
{
    double lat1,lng1,lat2,lng2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &lat1,&lng1,&lat2,&lng2) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(get_distance(lat1,lng1,lat2,lng2));
}
/* }}} */

/*geo_info() 输出geo扩展相关信息*/
PHP_FUNCTION(geo_info)
{
    std::string info = "this is geo php extension~!";

	RETURN_STRINGL(info.c_str(), info.length(), 1);
}


static double get_distance(double lat1,double lng1,double lat2,double lng2)
{
    double lat,lng;
    double distance = 0.0;
    lat = deg2rad((lat2-lat1));
    lng = deg2rad((lng2-lng1));
    distance = 2 * EARTH_RADIUS * asin( sqrt( sin(lat/2) * sin(lat/2) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * sin(lng/2) * sin(lng/2)));

    return distance;
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
