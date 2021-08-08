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
    char *text;
    struct HtmlNode *parent;
    struct HtmlNode *children[MAX_CHILDREN];
    size_t num_children;
    HtmlAttribute attrs[MAX_ATTRS];
    size_t num_attrs;
    bool must_close;
} HtmlNode;


typedef struct {
    HtmlNode *root;
    HtmlNode *current;
} HtmlDoc;


void add_attribute(HtmlNode *node, char *name, char *value);
void add_child(HtmlNode *node, HtmlNode *child);
HtmlNode *make_node(char *name, bool must_close, HtmlNode *parent);
void destroy_node(HtmlNode *node);
void destroy_doc(HtmlDoc *doc);
void print_node(HtmlNode *node, int depth);
HtmlNode *push_node(HtmlDoc *root, char *name, bool must_close, HtmlNode *parent);
HtmlNode *pop_node(HtmlDoc *root);
void write_node(HtmlNode *node, FILE *fp, int depth);
int write_doc(HtmlDoc *doc, FILE *fp);


#endif

