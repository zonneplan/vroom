#ifndef JOB_H
#define JOB_H

/*

This file is part of VROOM.

Copyright (c) 2015-2024, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include <numeric>
#include <string>

#include "structures/typedefs.h"
#include "structures/vroom/amount.h"
#include "structures/vroom/location.h"
#include "structures/vroom/time_window.h"
#include "structures/vroom/vehicle.h"

namespace vroom {

struct Job {
  Location location;
  const Id id;
  const JOB_TYPE type;
  const Duration setup;
  const Duration service;
  const DurationMap service_per_vehicle_type;
  const Amount delivery;
  const Amount pickup;
  const Skills skills;
  const Priority priority;
  const std::vector<TimeWindow> tws;
  const std::string description;

  // Constructor for regular one-stop job (JOB_TYPE::SINGLE).
  Job(Id id,
      const Location& location,
      UserDuration setup = 0,
      UserDuration service = 0,
      UserDurationMap service_per_vehicle_type = {},
      Amount delivery = Amount(0),
      Amount pickup = Amount(0),
      Skills skills = Skills(),
      Priority priority = 0,
      const std::vector<TimeWindow>& tws =
        std::vector<TimeWindow>(1, TimeWindow()),
      std::string description = "");

  // Constructor for pickup and delivery jobs (JOB_TYPE::PICKUP or
  // JOB_TYPE::DELIVERY).
  Job(Id id,
      JOB_TYPE type,
      const Location& location,
      UserDuration setup = 0,
      UserDuration service = 0,
      UserDurationMap service_per_vehicle_type = {},
      const Amount& amount = Amount(0),
      Skills skills = Skills(),
      Priority priority = 0,
      const std::vector<TimeWindow>& tws =
        std::vector<TimeWindow>(1, TimeWindow()),
      std::string description = "");

  Index index() const {
    return location.index();
  }

  Duration service_for_vehicle(const Vehicle& vehicle) const {
    if (vehicle.service_type.has_value()) {
      auto it = service_per_vehicle_type.find(vehicle.service_type.value());
      return (it != service_per_vehicle_type.end()) ? it->second : service;
    }

    return service;
  }

  bool is_valid_start(Duration time) const;
};

} // namespace vroom

#endif
