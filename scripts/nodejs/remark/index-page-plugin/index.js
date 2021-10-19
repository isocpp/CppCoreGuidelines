'use strict';

var toString = require('mdast-util-to-string');
var visit = require('unist-util-visit');

module.exports = attacher;

function attacher() {
  return transformer;
}

class IndexEntry {
  constructor() {
    this.refs = [];
  }

  static addTags(indexEntries, tags, ref, refNameParts) {
    for (let tag of tags) {
      if (!indexEntries.has(tag)) {
        indexEntries.set(tag, new IndexEntry());
      }

      indexEntries.get(tag).add(ref, refNameParts);
    }
  }

  static genPage(indexEntries) {
    /* build cross reference map */
    let crossMap = new Map();

    for (let [key, entry] of indexEntries) {
      for (let ref of entry.refs) {
        if (!crossMap.has(ref.anchor)) {
          crossMap.set(ref.anchor, new Set());
        }

        crossMap.get(ref.anchor).add(key);
      }
    }

    for (let [key, tags] of crossMap) {
      tags = Array.from(tags);
      tags.sort((a, b) => a.localeCompare(b));
      crossMap.set(key, tags);
    }

    function makeNiceName(key) {
      return key.split(" ").map(key => key[0].toLocaleUpperCase() + key.slice(1, key.length)).join(" ");
    }

    /* sort tags */
    let keys = Array.from(indexEntries.keys());
    keys.sort((a, b) => a.localeCompare(b));

    /* build index */
    let node = {type: "paragraph", children: []};
    for (let key of keys) {
      let entry = indexEntries.get(key);
      let value = makeNiceName(key);

      let subtree = [];
      let crossrefs = [];
      let crossvisit = new Set([key]);

      for (let ref of entry.refs) {
        subtree.push({
          type    : "listItem",
          children: [
            {
              type    : "link",
              url     : "#" + ref.anchor,
              children: ref.nameParts
            }
          ]
        })

        /* build cross references */
        for (let crosskey of crossMap.get(ref.anchor)) {
          if (crossvisit.has(crosskey)) {
            continue;
          }

          crossvisit.add(crosskey);

          crossrefs.push({
            type    : "listItem",
            children: [
              {
                type: "link", url: `#index.${crosskey.replace(/ /g, "-")}`,
                children: [
                  {type : "text", value : makeNiceName(crosskey)}
                ]
              }
            ]
          });
        }
      }

      subtree.push({
        type    : "listItem",
        children: [
          {type: "text", value: "See Also:"},
          {
            type    : "list",
            start   : null,
            loose   : false,
            ordered : false,
            children: crossrefs
          }
        ]
      });

      /* put cross references in at the end */
      node.children.push({
        "type"  : "heading",
        depth   : 2,
        children: [
          {
            type: "html",
            value: `<a name="index.${key.replace(/ /g, '-')}"></a>`
          },
          {
            type: "text", value
          }
        ]
      });
      node.children.push({type : "break"})
      node.children.push({
        "type"  : "paragraph",
        children: [
          {
            type    : "list",
            start   : null,
            loose   : false,
            ordered : false,
            children: subtree
          }
        ]
      });

      /*for some reason marked requires two breaks here*/
      node.children.push({type : "break"})
      node.children.push({type : "break"})
    }

    return node;
  }

  add(ref, refName) {
    this.refs.push({
      anchor   : ref,
      nameParts: refName
    });
  }
}

let pat = /tags[ \t]*=[ \t]*"((:?[\-a-zA-Z_ ]+[\-a-zA-Z_ *0-9]*([, ])?)*)"/;
let aname = /name[ \t]*=[ \t]*"([a-zA-Z0-9_*.\- ]+)"/

/* Patch slugs on heading nodes. */
function transformer(ast) {
  let indexEntries = new Map();
  let indexPageNode = undefined;

  let newNode = {
    type: "paragraph", children: [
      {type: "heading", depth: 1, children: [{type: "text", value: "Index Page"}]},
      {type: "break"}, /*avoid remark bug of failing to put newline in generated header*/
      {type: "list", ordered: false, children: []}
    ]
  };

  visit(ast, 'heading', function (node, index, parent) {
    let title = [];

    /* build title as nodes, so we can preserve styling */
    for (let c of node.children) {
      if (c.type === "text" || c.type === "inlineCode") {
        title.push(c)
      }
    }

    /* find anchor element with tags */
    for (let c of node.children) {
      if (c.type === "text" && c.value === "put-index-page-here") {
        parent.children[index] = newNode
      }

      if (c.type === 'html') {
        let htmltag = c.value;

        if (htmltag.startsWith("</")) {
          continue;
        }

        let match = htmltag.match(pat);

        if (match && match[1]) {
          let tags = match[1].split(",").map(tag => tag.trim());

          let anchor = htmltag.match(aname)
          if (!anchor) {
            throw new Error(`${node.position.start.line}: Invalid anchor name attribute: ${htmltag}`)
          }

          anchor = anchor[1].trim();

          IndexEntry.addTags(indexEntries, tags, anchor, title);
          newNode.children[2] = IndexEntry.genPage(indexEntries)
        }
      }
    }
  });
}
