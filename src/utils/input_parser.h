#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

/*

This file is part of VROOM.

Copyright (c) 2015-2016, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include <array>
#include <vector>
#include "../../include/rapidjson/document.h"
#include "../../include/rapidjson/error/en.h"
#include "../structures/typedefs.h"
#include "../structures/abstract/matrix.h"
#include "../structures/vroom/job.h"
#include "../structures/vroom/vehicle.h"
#include "../structures/vroom/input.h"
#include "./exceptions.h"

input parse(const std::string& input_string);

#endif
