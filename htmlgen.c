#include <assert.h>
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
    node->tag.attrs[node->tag.num_attrs++] = attr;
}

void add_child(HtmlNode *node, HtmlNode *child)
{
    if (node->num_children >= MAX_CHILDREN) {
        return;
    }
    node->children[node->num_children++] = child;
}

int append_node_text(HtmlNode *node, char* text) 
{
    if (node->num_lines < MAX_LINES-1) {
        char *buf = (char*)malloc((strlen(text)*sizeof(char))+1);
        strcpy(buf, text);
        node->text[node->num_lines++] = buf;
        return 0;
    }
    return 1;
}

int trim_node_text(HtmlNode *node)
{
    if (node->num_lines > 0)
    {
        free(node->text[node->num_lines - 1]);
        node->num_lines--;
        return 0;
    }
    return 1;
}
void clear_node_text(HtmlNode *node)
{
    while (node->num_lines > 0) {
        free(node->text[node->num_lines - 1]);
        node->num_lines--;
    }
    assert(node->num_lines == 0);
}

HtmlNode *make_node(char *name, bool must_close, HtmlNode *parent)
{
    HtmlNode *node = malloc(sizeof(HtmlNode));

    HtmlTag tag = { name: name, closes: must_close, num_attrs: 0 };
    node->tag = tag;

    node->parent = parent;
    node->num_children = 0;
    node->num_lines = 0;

    if (parent != NULL) {
        add_child(parent, node);
    }
    return node;
}

HtmlDoc make_doc()
{
    HtmlDoc doc;
    doc.root = make_node("!DOCTYPE html", false, NULL);
    doc.html = make_node("html", true, doc.root);
    doc.head = make_node("head", true, doc.html);
    doc.body = make_node("body", true, doc.html);
    return doc;
}

void destroy_node(HtmlNode *node)
{
    for (size_t i=0; i<node->num_children; ++i) {
        destroy_node(node->children[i]);
    }
    for (size_t i=0; i<node->tag.num_attrs; ++i) {
        free(node->tag.attrs[i].name);
        free(node->tag.attrs[i].value);
    }
    clear_node_text(node);
    free(node);
}

void destroy_doc(HtmlDoc *doc)
{
    destroy_node(doc->root);
}

void print_node(HtmlNode *node, int depth) 
{
    for (int i=0; i<depth-1; ++i) {
        printf("\t");
    }
    printf("<%s", node->tag.name);
    for (size_t i=0; i<node->tag.num_attrs; ++i)
    {
        HtmlAttribute attr = node->tag.attrs[i];
        printf(" %s=%s", attr.name, attr.value);
    }
    printf(">\n");
    for (size_t i=0; i<node->num_lines; ++i)
    {
        for (int i=0; i<depth; ++i) {
            printf("\t");
        }
        printf("%s\n", node->text[i]);

    }

    for (size_t i=0; i<node->num_children; ++i) {
        print_node(node->children[i], depth+1);
    }
    if (node->tag.closes) {
        for (int i=0; i<depth-1; ++i) {
            printf("\t");
        }
        printf("</%s>\n", node->tag.name);
    }
}

void write_node(HtmlNode *node, FILE *fp, int depth)
{
    for (int i=0; i<depth-1; ++i) {
        fputc('\t', fp);
    }
    fputc('<', fp);
    fputs(node->tag.name, fp);
    for (size_t i=0; i<node->tag.num_attrs; ++i)
    {
        HtmlAttribute attr = node->tag.attrs[i];
        fputc(' ', fp);
        fputs(attr.name, fp);
        fputc('=', fp);
        fputs(attr.value, fp);
    }
    fputs(">\n", fp);
    for (size_t i=0; i<node->num_lines; ++i) 
    {
        for (int i=0; i<depth; ++i) {
            fputc('\t', fp);
        }
        fputs(node->text[i], fp);
        fputc('\n', fp);
    }

    for (size_t i=0; i<node->num_children; ++i) {
        write_node(node->children[i], fp, depth+1);
    }
    if (node->tag.closes) {
        for (int i=0; i<depth-1; ++i) {
            fputc('\t', fp);
        }
        fputs("</", fp);
        fputs(node->tag.name, fp);
        fputs(">\n", fp);
    }
}

int write_doc(HtmlDoc *doc, FILE *fp)
{
    if (doc->root == NULL || fp == NULL) {
        return 1;
    }
    write_node(doc->root, fp, 0);
}
