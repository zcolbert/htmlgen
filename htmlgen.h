#ifndef HTMLGEN_H
#define HTMLGEN_H

#define MAX_ATTRS 16
#define MAX_CHILDREN 255
#define MAX_LINES 255

#include <stdbool.h>
#include <stdio.h>


typedef struct HtmlAttribute {
    char *name;
    char *value;
} HtmlAttribute;


typedef struct  {
    char *name;
    bool closes;
    HtmlAttribute attrs[MAX_ATTRS];
    size_t num_attrs;
} HtmlTag;


typedef struct HtmlNode {
    HtmlTag tag;
    struct HtmlNode *parent;
    struct HtmlNode *children[MAX_CHILDREN];
    char *text[MAX_LINES];
    size_t num_children;
    size_t num_lines;
} HtmlNode;


typedef struct {
    HtmlNode *root;
    HtmlNode *html;
    HtmlNode *head;
    HtmlNode *body;
    HtmlNode *current;
} HtmlDoc;


void add_attribute(HtmlNode *node, char *name, char *value);
void add_child(HtmlNode *node, HtmlNode *child);
int append_node_text(HtmlNode *node, char *text);
int trim_node_text(HtmlNode *node);
void clear_node_text(HtmlNode *node);
HtmlNode *make_node(char *name, bool must_close, HtmlNode *parent);
HtmlDoc make_doc();
void destroy_node(HtmlNode *node);
void destroy_doc(HtmlDoc *doc);
void print_node(HtmlNode *node, int depth);
void write_node(HtmlNode *node, FILE *fp, int depth);
int write_doc(HtmlDoc *doc, FILE *fp);


#endif

