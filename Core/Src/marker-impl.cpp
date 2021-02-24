/* This file is needed due to some quirks with templates in C++
 * Without this file, all functions in the EPaperScreen would have to be
 * inlined, and we don't want that.
 * Instead, put a list of all screens that will be instantiated here.
 * In most cases, that should only be one.
 * For more info, please see
 * https://isocpp.org/wiki/faq/templates#separate-template-class-defn-from-decl
 * */

#include "marker.cpp"
template class emarker::EPaperScreen<128u, 296u>;
