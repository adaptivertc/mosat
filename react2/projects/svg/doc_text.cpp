
#include <stdio.h>
#include "gen_display.h"

void doc_text_object_t::start(const char *name, const char *short_desc)
{
  this->param_number = 0;
  fprintf(this->fp, "-------------\n\n");
  fprintf(this->fp, "%s - %s\n\n", name, short_desc);
}

void doc_text_object_t::param(const char *short_desc)
{
  this->param_number++;
  if (this->param_number == 1)
  {
    fprintf(this->fp, "Parameters:\n\n");
  }
  fprintf(this->fp, "  %d: %s\n", this->param_number, short_desc);
}

void doc_text_object_t::notes(const char *notes)
{
  fprintf(this->fp, "\n  Notes:\n\n");
  fprintf(this->fp, "%s\n", notes);
}

void doc_text_object_t::header(void)
{
  fprintf(this->fp, "SVG Widget Documentation\n\n");
}

void doc_text_object_t::footer(void)
{
  fprintf(this->fp, "\n-------------\n");
}

void doc_text_object_t::end(void)
{
  fprintf(this->fp, "\n");
}

