//
// convert_md_html.js
//
// converts a md file into html using showdown 
//



'use strict';

var fs = require('fs');

var inputFile = getInputFile();
var outputFile = getOutputFile();


convertMdFileHtml( fs, inputFile, outputFile );


function getInputFile(){
  return process.argv[2];
}

function getOutputFile(){
  return process.argv[3];
}

function convertMdFileHtml( fs, inputFile, outputFile ){


  var showdown = require('showdown');

  var ignoreBreaks = {
    type    : 'output',
    regex   : /<br \/>/g, 
    replace : ''
  };

  var personalizeDoctype = {
    type    : 'output',
    regex   : /<!DOCTYPE HTML>/g,
    replace : "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
       + "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
  }; 

  var personalizeAnchor = {
    type    : 'output', 
    regex   : '<a name=\"([^\"]+)\">',
    replace : '<a id=\"$1\">'
  };

  var personalizeHtml = {
    type    : 'output',
    regex   : /<html>/g,
    replace : "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">"
  }; 

  var correctInclude = {
    type    : 'output', 
    regex   : /<iostream>/g, 
    replace : "&lt;iostream&gt;"
  };

  var personalizeHead = {
    type    : 'output',
    regex   : /<head>/g, 
    replace : '<head>\n'
     +'<title>C++ Core Guidelines</title>\n'
     +'<link rel="stylesheet" type="text/css" href="poole.css"/>\n'
     +'<link rel="stylesheet" type="text/css" href="custom.css"/>\n'
  };

  var personalizeMeta = {
    type    : 'output',
    regex   : /<meta charset=\"utf-8\">/g, 
    replace : ''
  };

  var personalizeBody = {
    type    : 'output',
    regex   : /<body>/g, 
    replace : '<body><div class="content container">'
  }; // personalizeBody

  var personalizeCloseBody = {
    type    : 'output',
    regex   : /<\/body>/g, 
    replace : '</div></body>'
  };

  var converterOptions = 
    {
      completeHTMLDocument : true,
      excludeTrailingPunctuationFromURLs : true,
      simpleLineBreaks : false,
      extensions: [personalizeDoctype, 
                   personalizeHtml, 
                   personalizeAnchor,
                   correctInclude,
                   ignoreBreaks, 
                   personalizeHead, 
                   personalizeMeta, 
                   personalizeBody, 
                   personalizeCloseBody]
    };

  var converter = new showdown.Converter( converterOptions );
  converter.setFlavor('github');

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


} // convertMdFileHtml



