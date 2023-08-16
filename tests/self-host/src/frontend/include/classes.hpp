#ifndef CLASSES_HPP
#define CLASSES_HPP

/* identifier is split for the sake of bootstrapping */

size_t has_class_identifier(std::string buf, const char *file, size_t line_no);
void locate_class_name(std::string buf, std::ifstream& fp, const char *file, size_t line_no);

#endif
