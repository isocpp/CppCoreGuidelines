//
// file_op.js
//
// perform basic file operations: to make the 
// build process more portable 
//


'use strict';

var fs = require('fs');
var path = require('path');

var command = process.argv[2];

switch( command ) 
{
    case 'cp':  var sourceFile = process.argv[3];
                var destPath = process.argv[4];

                var basename = path.basename( sourceFile );

                if( fs.existsSync( destPath ) ) {
                    if( lastCharOf( destPath ) !== path.sep )
                        destPath = destPath + path.sep;
                    destPath = destPath + basename;
                }
                fs.copyFile( sourceFile, destPath, 
                    (err) => {
                        if( err ) 
                            throw new Error("error in copy"); 
                    });
                break;
    case 'mkdir': var destPath = process.argv[3]; 
                if( !fs.existsSync( destPath ) ) 
                    fs.mkdirSync( destPath );
                break;
    default: throw new Error("command not recognized: " + command);
                break;
}


function lastCharOf( str ) {
    return str.substr( str.length - 1 );

}