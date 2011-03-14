

int main(int argc, char *argv[])
{
  // Link text lines
  on_new_progress_label("Linking text lines");
  line_links<L> llinks = scribo::text::link_lines(lines);

  // Filter line links.
  on_new_progress_label("Filter line links");
  llinks = scribo::filter::line_links_x_height(llinks);

  // Construct paragraphs
  on_new_progress_label("Constructing paragraphs");
  scribo::paragraph_set<L> parset = scribo::make::paragraph(llinks);
  doc.set_paragraphs(parset);

  on_progress();


  // Extract other Elements
  on_new_progress_label("Extracting Elements");
  component_set<L>
    elements = scribo::primitive::extract::non_text_fast(doc);

}
