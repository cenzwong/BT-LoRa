//=============
	develop by Cenz on 14/8/2018
	version 1.0
//============

This Library rely on the config.h to config E32 module, therefore, the tranditional way to include arduino library is not work
You may have a look on the E32_Lib.ino on how to start with

Step 1: 
Open your sketch

Step 2:
Drag config.h , E32.cpp, E32.h to the directory

Step 3:
#include "E32.h" only

Step 4
create E32 object, E32Device and E32Data.
Some setting need to be change in config.h
(you may refer to the example: E32_Lib.ino)


ref: http://www.ebyte.com/en/product-view-news.aspx?id=130
