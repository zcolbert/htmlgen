#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htmlgen.h"


void add_attribute(HtmlNode *node, char *name, char *value)
{
    HtmlAttribute attr;
    attr.name = malloc((sizeof(char)*strlen(name)+1));
    attr.value = malloc((sizeof(char)*strlen(value)+1));
    strcpy(attr.name, name);
    strcpy(attr.value, value);
    node->attrs[node->num_attrs++] = attr;
}

void add_child(HtmlNode *node, HtmlNode *child)
{
    if (node->num_children >= MAX_CHILDREN) {
        return;
    }
    node->children[node->num_children++] = child;
}

HtmlNode *make_node(char *name, bool must_close, HtmlNode *parent)
{
    HtmlNode *node = malloc(sizeof(HtmlNode));
    node->name = name;
    node->parent = parent;
    node->num_children = 0;
    node->num_attrs = 0;
    node->must_close = must_close;

    if (parent != NULL) {
        add_child(parent, node);
    }
    return node;
}

void destroy_node(HtmlNode *node)
{
    for (size_t i=0; i<node->num_children; ++i) {
        destroy_node(node->children[i]);
    }
    for (size_t i=0; i<node->num_attrs; ++i) {
        free(node->attrs[i].name);
        free(node->attrs[i].value);
    }
    free(node);
}

void print_node(HtmlNode *node, int depth) 
{
    for (int i=0; i<depth-1; ++i) {
        printf("\t");
    }
    printf("<%s", node->name);
    for (size_t i=0; i<node->num_attrs; ++i)
    {
        HtmlAttribute attr = node->attrs[i];
        printf(" %s=%s", attr.name, attr.value);
    }
    printf(">\n");

    for (size_t i=0; i<node->num_children; ++i) {
        print_node(node->children[i], depth+1);
    }
    if (node->must_close) {
        for (int i=0; i<depth-1; ++i) {
            printf("\t");
        }
        printf("</%s>\n", node->name);
    }
}

void write_node(HtmlNode *node, FILE *fp, int depth)
{
    for (int i=0; i<depth-1; ++i) {
        fputc('\t', fp);
    }
    fputc('<', fp);
    fputs(node->name, fp);
    for (size_t i=0; i<node->num_attrs; ++i)
    {
        HtmlAttribute attr = node->attrs[i];
        fputc(' ', fp);
        fputs(attr.name, fp);
        fputc('=', fp);
        fputs(attr.value, fp);
    }
    fputs(">\n", fp);

    for (size_t i=0; i<node->num_children; ++i) {
        write_node(node->children[i], fp, depth+1);
    }
    if (node->must_close) {
        for (int i=0; i<depth-1; ++i) {
            fputc('\t', fp);
        }
        fputs("</", fp);
        fputs(node->name, fp);
        fputs(">\n", fp);
    }
}
