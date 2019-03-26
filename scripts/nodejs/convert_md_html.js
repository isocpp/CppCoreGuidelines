/**
 * Created by tivie
 */
'use strict';

var showdown = require('showdown');
var converterOptions = 
  {
    'completeHTMLDocument' : true
  };

var converter = new showdown.Converter( converterOptions );
converter.setFlavor('github');

var inputFile = getInputFile();
var outputFile = getOutputFile();

var fs = require('fs');

fs.readFile( inputFile, 'utf8',
        ( err, data ) => {
          var html = converter.makeHtml( data );
          fs.writeFile(outputFile, html, { 'flags' : 'w' },
            (err) => { 
                  if( err ){
                    console.log(err);
                    throw err;
                  } // if 
                });
          });


function getInputFile(){
  return process.argv[2];
}

function getOutputFile(){
  return process.argv[3];
}

