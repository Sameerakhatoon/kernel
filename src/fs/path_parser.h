#ifndef PATH_PARSE_H
#define PATH_PARSE_H

typedef struct PathPart PathPart;

struct PathPart{
    const char* name;
    PathPart* next;
};

typedef struct PathRoot{
    int drive_num;
    PathPart* first;
} PathRoot;

void path_parser_free_path(PathRoot* root);
PathRoot* path_parser_parse_path(const char* path, const char* current_dir);

#endif