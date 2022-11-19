#include "parser/parser.h"

extern double scale_x;
extern double scale_y;
extern double origin_x;
extern double origin_y;
extern double rot;

extern double get_expr_val(struct expr_node *tree);
extern void draw_loop(struct token *param, double start, double end, double step, struct expr_node *exp_x, struct expr_node *exp_y);
extern void close_drawer(void);