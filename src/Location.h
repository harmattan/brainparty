#ifndef BRAINPARTY_LOCATION_H
#define BRAINPARTY_LOCATION_H

#include <string>

void bp_init_location(int argc, char **argv);
std::string bp_get_save_file_name();
std::string bp_get_data_file(const std::string &filename);

#endif /* BRAINPARTY_LOCATION_H */
