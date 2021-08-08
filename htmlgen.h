#ifndef HTMLGEN_H
#define HTMLGEN_H

#define MAX_CHILDREN 255
#define MAX_ATTRS 16


typedef struct HtmlAttribute {
    char *name;
    char *value;
} HtmlAttribute;


typedef struct HtmlNode {
    char *name;
    struct HtmlNode *parent;
    struct HtmlNode *children[MAX_CHILDREN];
    size_t num_children;
    HtmlAttribute attrs[MAX_ATTRS];
    size_t num_attrs;
    bool must_close;
} HtmlNode;


void add_attribute(HtmlNode *node, char *name, char *value);
void add_child(HtmlNode *node, HtmlNode *child);
HtmlNode *make_node(char *name, bool must_close, HtmlNode *parent);
void destroy_node(HtmlNode *node);
void print_node(HtmlNode *node, int depth);
void write_node(HtmlNode *node, FILE *fp, int depth);


#endif

