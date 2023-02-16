#ifdef __cplusplus
extern "C" {
#endif
#include "fabgen.h"

typedef void* matplot_line_handle;
typedef void* matplot_vector_1d;
typedef void* matplot_figure_handle;
extern void matplot_line_handle_free(matplot_line_handle);
extern void matplot_vector_1d_free(matplot_vector_1d);
extern void matplot_figure_handle_free(matplot_figure_handle);
extern matplot_line_handle  matplot_plot(matplot_vector_1d  x ,matplot_vector_1d  y);
extern void matplot_show(matplot_figure_handle  f);
#ifdef __cplusplus
}
#endif
