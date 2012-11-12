
#include <stdio.h>
#include "gen_display.h"

void doc_html_object_t::start(const char *name, const char *short_desc)
{
  this->param_number = 0;
  fprintf(this->fp, "<hr>\n");
  fprintf(this->fp, "<h3>%s - %s</h3>\n", name, short_desc);
}

void doc_html_object_t::param(const char *short_desc)
{
  this->param_number++;
  if (this->param_number == 1)
  {
    fprintf(this->fp, "<bold>Parameters:</bold><br><br>\n");
  }
  fprintf(this->fp, "<ul><bold>%d:</bold> %s</ul>\n", this->param_number, short_desc);
}

void doc_html_object_t::notes(const char *notes)
{
  fprintf(this->fp, "\n  Notes:<br><br>\n\n");
  fprintf(this->fp, "%s<br>\n", notes);
}

void doc_html_object_t::header(void)
{
  fprintf(this->fp, "<html>\n");
  fprintf(this->fp, "<h1>SVG Widget Documentation<h1>\n");
}

void doc_html_object_t::footer(void)
{
  fprintf(this->fp, "<hr>\n");
  fprintf(this->fp, "</html>\n");
}
void doc_html_object_t::end(void)
{
  fprintf(this->fp, "<br>\n");
}

